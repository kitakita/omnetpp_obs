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

Define_Module(HorizonScheduler);

void HorizonScheduler::initialize()
{
	cModule *parent = getParentModule();
	int gatesize = parent->gateSize("burstg$o");

	for (int i = 0, prev = -1; i < gatesize; i++)
	{
		cGate *g = parent->gate("burstg$o", i)->getNextGate();
		int id = g->getOwnerModule()->getId();
		if (id != prev)
		{
			prev = id;
			HorizonSchedules schedule;
			scheduleTables.push_back(schedule);
		}
		scheduleTables.back().push_back(0);
	}

	updateDisplayString();
}

void HorizonScheduler::handleMessage(cMessage *msg)
{
	opp_error("%s cannot receive any messages (receive: %s).", getFullName(), msg);
}

OffsetAndChannel HorizonScheduler::getMinFrontOffsetAndChannel(int outPort, simtime_t arrivalTime)
{
	OffsetAndChannel oac;
	oac.offset = arrivalTime - scheduleTables.at(outPort).at(0);
	oac.channel = 0;

	int size = scheduleTables.at(outPort).size();
	for (int i = 1; i < size; i++)
	{
		simtime_t offset = arrivalTime - scheduleTables.at(outPort).at(i);
		if (offset > 0)
		{
			if (((oac.offset > 0) && (oac.offset > offset)) || (oac.offset <= 0))
			{
				oac.offset = offset;
				oac.channel = i;
			}
		}
		else
		{
			if ((oac.offset <= 0) && (oac.offset < offset))
			{
				oac.offset = offset;
				oac.channel = i;
			}
		}
	}
	return oac;
}

void HorizonScheduler::printSchedule(int outPort)
{
	int size = scheduleTables.at(outPort).size();
	for (int i = 0; i < size; i++)
	{
		char schedule[32];
		sprintf(schedule, "%16.12f", scheduleTables.at(outPort).at(i).dbl());
		ev << " | " << schedule;
	}
}

void HorizonScheduler::updateDisplayString()
{
    if (!ev.isGUI())
        return;

    char tag[128];
    sprintf(tag, "%d ports\ntable:", scheduleTables.size());

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

int HorizonScheduler::schedule(int outPort, simtime_t arrivalTime, simtime_t length)
{
	Enter_Method("Schedule at %f (length %f [s]) to port %d.", arrivalTime.dbl(), length.dbl(), outPort);

	ev << getFullPath() << " (id=" << getId() << "): " << "port " << outPort << " schedule start." << endl
	   << "Burst Arrival Time: " << arrivalTime << " Burstlength: " << length << endl
	   << "before";
	printSchedule(outPort);
	ev << endl << "   after";

	OffsetAndChannel oac = getMinFrontOffsetAndChannel(outPort, arrivalTime);

	if (oac.offset < 0)
	{
		ev << " | failed." << endl;
		return -1;
	}

	scheduleTables.at(outPort).at(oac.channel) += (oac.offset + length);

	printSchedule(outPort);
	ev << endl;

	return oac.channel;
}
