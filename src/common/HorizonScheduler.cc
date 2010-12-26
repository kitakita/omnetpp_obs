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
#include "ConnectionEvent_m.h"
#include "Burst.h"
#include "BurstControlPacket.h"
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
		scheduleTables.back().push_back(new Schedule());
	}

	updateDisplayString();
}

void HorizonScheduler::handleMessage(cMessage *msg)
{
	opp_error("%s cannot receive any messages (receive: %s).", getFullName(), msg);
}

void HorizonScheduler::finish()
{
	for (unsigned int i = 0; i < scheduleTables.size(); i++)
		for (unsigned int j = 0; j < scheduleTables[i].size(); j++)
			delete scheduleTables[i][j];
}

void HorizonScheduler::printSchedule(int port)
{
	for (unsigned int i = 0; i < scheduleTables[port].size(); i++)
	{
		char schedule[32];
		sprintf(schedule, "%16.12f", scheduleTables[port][i]->getHorizon().dbl());
		ev << " | " << schedule;
	}
	ev << endl;
}

void HorizonScheduler::updateDisplayString()
{
    if (!ev.isGUI())
        return;

    char tag[128];
    sprintf(tag, "%d ports\nsize:", scheduleTables.size());

    ScheduleTables::iterator stit = scheduleTables.begin();
    while (stit != scheduleTables.end())
	{
    	char buf[16];
    	sprintf(buf, " %d", stit->size());
    	strcat(tag, buf);
    	stit++;
    }

    getDisplayString().setTagArg("t", 0, tag);
}

ScheduleResult HorizonScheduler::trySchedule(simtime_t arrivalTime, int port, int channel = -1)
{
	Schedule *sc = channel < 0 ? scheduleTables[port][0] : scheduleTables[port][channel];
	ScheduleResult res = { arrivalTime - sc->getHorizon(), channel < 0 ? 0 : channel, false};

	if (sc->getHorizon() == 0)
		return res;

	if (droppable && res.offset < 0 && res.offset + sc->getDroppableBitLength() / wdm->getDatarate(port) >= 0)
		res.dropped = true;

	if (channel >= 0)
		return res;

	simtime_t offset;

	for (unsigned int i = 1; i < scheduleTables[port].size(); i++)
	{
		sc = scheduleTables[port][i];

		if (sc->getHorizon() == 0)
		{
			res.offset = arrivalTime;
			res.channel = i;
			return res;
		}

		offset = arrivalTime - sc->getHorizon();

		if (offset > 0)
		{
			if (res.offset < 0 || (res.offset > 0 && res.offset > offset))
			{
				res.offset = offset;
				res.channel = i;
			}
		}
		else
		{
			if (res.offset < 0 && res.offset < offset)
			{
				if (droppable)
				{
					if (offset + sc->getDroppableBitLength() / wdm->getDatarate(port) >= 0)
					{
						res.offset = offset;
						res.channel = i;
						res.dropped = true;
					}
				}
				else
				{
					res.offset = offset;
					res.channel = i;
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

	ScheduleResult res;

	if (waveConversion)
		res = trySchedule(bcp->getBurstArrivalTime(), port);
	else
		res = trySchedule(bcp->getBurstArrivalTime(), port, bcp->getBurstChannel());

	Schedule *sc = scheduleTables[port][res.channel];

	if (res.offset < 0)
	{
		if (res.dropped)
		{
			Burst *bst = check_and_cast<Burst *>(sc->getBurst());
			int dropByteLength = 8 * (- res.offset.dbl()) * wdm->getDatarate(port);
			int droppedByteLength = bst->dropPacketsFromBack(dropByteLength);
			simtime_t burstlength = bst->getBitLength() / wdm->getDatarate(port);
			int restDroppableByteLength = bcp->getDroppableByteLength() - droppedByteLength;

			bcp->setBurstlength(burstlength);
			bcp->setDroppableByteLength(restDroppableByteLength);
		}
		else
		{
			res.channel = -1;
		}
	}

	ev << "bef";
	printSchedule(port);

	sc->setHorizon(bcp->getBurstArrivalTime() + bcp->getBurstlength());
	sc->setBurst(bcp->getBurst());
	sc->setDroppableByteLength(bcp->getDroppableByteLength());

	ev << "aft";
	if (res.channel < 0)
	{
		ev << " | failed." << endl;
		if (ev.isGUI()) bubble("failed");
	}
	else
	{
		printSchedule(port);
		if (ev.isGUI()) bubble("success");
	}

	return res.channel;
}
