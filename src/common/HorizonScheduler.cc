//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "HorizonScheduler.h"

#include "Burst.h"
#include "BurstControlPacket.h"
#include "ConnectionEvent_m.h"
#include "WDMTableAccess.h"

Define_Module(HorizonScheduler);

void HorizonScheduler::initialize()
{
	wdm = WDMTableAccess().get();

	droppable = par("droppable");
	waveConversion = par("waveConversion");

	cModule *parent = getParentModule();
	int size = parent->gateSize("burstg$o");

	for (int i = 0, prev = -1; i < size; i++)
	{
		cGate *g = parent->gate("burstg$o", i)->getNextGate();
		int id = g->getOwnerModule()->getId();
		if (id != prev)
		{
			prev = id;
			ScheduleTable table;
			scheduleTables.push_back(table);
		}
		Schedule *sch = new Schedule;
		sch->horizon = 0;
		sch->tail = 0;
		sch->burst = NULL;
		scheduleTables.back().push_back(sch);
	}

	updateDisplayString();
}

void HorizonScheduler::handleMessage(cMessage *msg)
{
	opp_error("%s cannot receive any messages (receive: %s).", getFullName(), msg);
}

void HorizonScheduler::finish()
{
	ScheduleTables::iterator it = scheduleTables.begin();
	while (it != scheduleTables.end())
	{
		ScheduleTable::iterator jt = it->begin();
		while (jt != it->end())
		{
			delete *jt;
			jt++;
		}
		it++;
	}
}

void HorizonScheduler::printSchedule(int port)
{
	for (unsigned int i = 0; i < scheduleTables[port].size(); i++)
	{
		char str[48];
		sprintf(str, " | ch%2d:%16.12f", i, scheduleTables[port][i]->horizon.dbl());
		ev << str;
	}
	ev << endl;
}

void HorizonScheduler::updateDisplayString()
{
    if (!ev.isGUI())
        return;

    char tag[128];
    sprintf(tag, "%d ports\nsize:", scheduleTables.size());

    ScheduleTables::iterator it = scheduleTables.begin();
    while (it != scheduleTables.end())
	{
    	char buf[16];
    	sprintf(buf, " %d", it->size());
    	strcat(tag, buf);
    	it++;
    }

    getDisplayString().setTagArg("t", 0, tag);
}

ScheduleResult HorizonScheduler::trySchedule(cMessage *msg, int port, int channel = -1)
{
	BurstControlPacket *bcp = check_and_cast<BurstControlPacket *>(msg);

	Schedule *sch = channel < 0 ? scheduleTables[port][0] : scheduleTables[port][channel];
	ScheduleResult res = { channel < 0 ? 0 : channel, bcp->getBurstArrivalTime() - sch->horizon, 0, 0 };

	if (sch->horizon == 0)
		return res;

	if (res.offset < 0)
	{
		if (droppable)
		{
			if (res.offset + bcp->getBursthead() >= 0)
				res.droppedHead = -res.offset;
			else if (res.offset + bcp->getBursthead() + sch->tail >= 0)
			{
				res.droppedHead = bcp->getBursthead();
				res.droppedTail = -res.offset - bcp->getBursthead();
			}
			else
				res.channel = -1;
		}
		else
			res.channel = -1;
	}

	// waveConversion OFF
	if (channel >= 0)
		return res;

	// waveConversion ON
	simtime_t offset;

	for (unsigned int i = 1; i < scheduleTables[port].size(); i++)
	{
		sch = scheduleTables[port][i];

		if (sch->horizon == 0)
		{
			res.offset = bcp->getBurstArrivalTime();
			res.channel = i;
			res.droppedHead = 0;
			res.droppedTail = 0;
			return res;
		}

		offset = bcp->getBurstArrivalTime() - sch->horizon;

		if (offset >= 0)
		{
			if (res.offset < 0 || (res.offset >= 0 && res.offset > offset))
			{
				res.offset = offset;
				res.channel = i;
				res.droppedHead = 0;
				res.droppedTail = 0;
			}
		}
		else
		{
			if (droppable && res.offset < 0 && res.offset < offset)
			{
				if (offset + bcp->getBursthead() >= 0)
				{
					res.channel = i;
					res.offset = offset;
					res.droppedHead = -res.offset;
					res.droppedTail = 0;
				}
				else if (offset + bcp->getBursthead() + sch->tail >= 0)
				{
					res.channel = i;
					res.offset = offset;
					res.droppedHead = bcp->getBursthead();
					res.droppedTail = -res.offset - bcp->getBursthead();
				}
			}
		}
	}

	return res;
}

int HorizonScheduler::schedule(cMessage *msg, int port)
{
	BurstControlPacket *bcp = check_and_cast<BurstControlPacket *>(msg);

	Enter_Method("Schedule %s to port %d (wave conversion %s).", bcp->getName(), port, waveConversion ? "enable" : "disable");

	ev << "before";
	printSchedule(port);

	ScheduleResult res;
	if (waveConversion)
		res = trySchedule(msg, port);
	else
		res = trySchedule(msg, port, bcp->getBurstChannel());

	// schedule failed
	if (res.channel < 0)
	{
		ev << "after  | failed." << endl;
		if (ev.isGUI()) bubble("failed");
		return res.channel;
	}

	// schedule success
	Schedule *sch = scheduleTables[port][res.channel];
	if (res.droppedHead > 0)
	{
		Burst *bst = check_and_cast<Burst *>(bcp->getBurst());
		int dropByteLength = ((int)(res.droppedHead.dbl() * wdm->getDatarate(port))+7)>>3;
		bst->dropHead(dropByteLength);
	}

	if (res.droppedTail > 0)
	{
		Burst *bst = check_and_cast<Burst *>(sch->burst);
		int dropByteLength = ((int)(res.droppedTail.dbl() * wdm->getDatarate(port))+7)>>3;
		bst->dropTail(dropByteLength);
	}

	sch->horizon = bcp->getBurstArrivalTime() + bcp->getBurstlength();
	sch->tail = bcp->getBursttail();
	sch->burst = bcp->getBurst();

	ev << "after ";
	printSchedule(port);
	if (ev.isGUI()) bubble("success");

	return res.channel;
}
