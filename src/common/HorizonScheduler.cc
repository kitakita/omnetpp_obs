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
	Schedule *sc;
	ScheduleResult st;

	int i = 0;
	int size = scheduleTables.at(port).size();
	do
	{
		sc = scheduleTables.at(port).at(i);
		st.offset = arrivalTime - sc->getTime();
		st.channel = 0;
		st.dropped = false;

		if (droppable)
		{
			if (!((st.offset < 0) && (st.offset + sc->getDroppableTimelength() < 0)))
				st.channel = -1;
		}

		simtime_t offset = arrivalTime - sc->getTime();
		if (offset > 0)
		{
			if (((st.offset > 0) && (st.offset > offset)) || (st.offset < 0))
			{
				st.offset = offset;
				st.channel = i;
			}
		}
		else
		{
			if (droppable)
			{
				if ((st.offset < offset) && (offset + sc->getDroppableTimelength() < 0))
				{
					st.offset = offset;
					st.channel = i;
				}
			}
			else
			{
				if ((st.offset < 0) && (st.offset < offset))
				{
					st.offset = offset;
					st.channel = i;
				}
			}
		}

		i++;
	} while (i < size);

	return st;
}

ScheduleResult HorizonScheduler::schedule(int port, cMessage *msg)
{
	BurstControlPacket *bcp = check_and_cast<BurstControlPacket *>(msg);

	Enter_Method("Schedule %s to port %d (wave conversion %s).", bcp->getName(), port, waveConversion ? "enable" : "disable");

	ev << getFullPath() << " (id=" << getId() << "): " << "port " << port << " schedule start." << endl
	   << "Burst Arrival Time: " << bcp->getBurstArrivalTime() << " Burstlength: " << bcp->getBurstlength() << endl
	   << "before";
	printSchedule(port);

	ScheduleResult res = getScheduleResult(port, bcp->getArrivalTime());

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
		sc->getBurst()->dropPacketsFromBack(droppedByte);
	}

	sc->setTime(bcp->getBurstArrivalTime() + bcp->getBurstlength());
	cMessage *message = bcp->getBurst();
	Burst *bst = check_and_cast<Burst *>(message);
	sc->setBurst(bst);

	ev << endl << "   after";
	printSchedule(port);
	ev << endl;

	return res;
}
