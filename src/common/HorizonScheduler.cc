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
#include "BurstControlPacket.h"
#include "Burst.h"
#include "ConnectionEvent_m.h"

Define_Module(HorizonScheduler);

void HorizonScheduler::initialize()
{
	droppable = par("droppable");
	waveConversion = par("waveConversion");
	datarate = par("datarate");

	cModule *parent = getParentModule();
	int gatesize = parent->gateSize("burstg$o");


	for (int i = 0, prev = -1; i < gatesize; i++)
	{
		cGate *g = parent->gate("burstg$o", i)->getNextGate();
		int id = g->getOwnerModule()->getId();
		if (id != prev)
		{
			prev = id;
			ScheduleTable table;
			scheduleTables.push_back(table);
		}
		scheduleTables.back().push_back(new Schedule(datarate));
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
		for (unsigned int j = 0; j < scheduleTables.at(i).size(); j++)
			delete scheduleTables[i][j];
}

void HorizonScheduler::printSchedule(int port)
{
	for (unsigned int i = 0; i < scheduleTables.at(port).size(); i++)
	{
		char schedule[32];
		sprintf(schedule, "%16.12f", scheduleTables.at(port).at(i)->getTime().dbl());
		ev << " | " << schedule;
	}
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

ScheduleResult HorizonScheduler::getScheduleResult(int port, simtime_t arrivalTime)
{
	Schedule *sc = scheduleTables.at(port).at(0);
	ScheduleResult res;
	res.offset = arrivalTime - sc->getTime();
	res.channel = 0;
	res.dropped = false;

	if (droppable && res.offset < 0 && res.offset + sc->getDroppableTimelength() < 0)
		res.channel = -1;

	for (unsigned int i = 1; i < scheduleTables.at(port).size(); i++)
	{
		simtime_t offset = arrivalTime - sc->getTime();
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
					if (offset + sc->getDroppableTimelength() >= 0)
					{
						res.offset = offset;
						res.channel = i;
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

ScheduleResult HorizonScheduler::schedule(int port, cMessage *msg)
{
	BurstControlPacket *bcp = check_and_cast<BurstControlPacket *>(msg);

	Enter_Method("Schedule %s to port %d (wave conversion %s).", bcp->getName(), port, waveConversion ? "enable" : "disable");

	ev << getFullPath() << " (id=" << getId() << "): " << "port " << port << " schedule start." << endl
	   << "Burst Arrival Time: " << bcp->getBurstArrivalTime() << " Burstlength: " << bcp->getBurstlength() << endl
	   << "before";
	printSchedule(port);

	ScheduleResult res = getScheduleResult(port, bcp->getBurstArrivalTime());

	if ((res.channel < 0) || (res.dropped && !droppable) || ((res.channel != bcp->getBurstIngressChannel()) && !waveConversion))
	{
		ev << endl << "   after" << " | failed." << endl;
		return res;
	}

	Schedule *sc = scheduleTables.at(port).at(res.channel);
	if (res.dropped)
	{
		int droppedByte = res.offset.dbl() * sc->getDatarate();
		ev << "Packet dropped in " << droppedByte << " byte." << endl;
		Burst *bst = check_and_cast<Burst *>(sc->getBurst());
		bst->dropPacketsFromBack(droppedByte);
	}

	sc->setTime(bcp->getBurstArrivalTime() + bcp->getBurstlength());

	Burst *bst = check_and_cast<Burst *>(bcp->getBurst());
	sc->setBurst(bst);

	ev << endl << "   after";
	printSchedule(port);
	ev << endl;

	return res;
}
