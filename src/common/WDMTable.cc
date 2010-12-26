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

#include "WDMTable.h"

Define_Module(WDMTable);

void WDMTable::initialize()
{
	cModule *parent = getParentModule();
	int size = parent->gateSize("burstg$o");

	for (int i = 0, prev = -1; i < size; i++)
	{
		cGate *nextg = parent->gate("burstg$o", i)->getNextGate();
		int id = nextg->getOwnerModule()->getId();
		if (id != prev)
		{
			prev = id;
			ChannelTable table;
			channelTables.push_back(table);
		}
		cGate *g = parent->gate("burstg$o", i);
		channelTables.back().push_back(g->getChannel());
	}

	numLinkedNodes = 0;

	ChannelTables::iterator it = channelTables.begin();
	while (it != channelTables.end())
	{
		numLinkedNodes++;
		it++;
	}

	datarate = par("datarate");

	printTables();
}

void WDMTable::handleMessage(cMessage *msg)
{
    opp_error("WDMTable cannot receive any messages.");
}

void WDMTable::printTables()
{
	ev << "Number of linked nodes is " << numLinkedNodes << endl;

	ev << "Number of Channel";

	for (int i = 0; i < numLinkedNodes; i++)
		ev << " | port:" << i << " - " << channelTables[i].size() << " ch";

	ev << endl;

	ev << "Transmisson Delay";

	for (int i = 0; i < numLinkedNodes; i++)
		ev << " | port:" << i << " - " << getTransmissionDelay(i, 0) << " sec";

	ev << endl;
}

int WDMTable::getGateIndex(int port, int channel)
{
	return (port * channelTables[port].size()) + channel;
}

simtime_t WDMTable::getTransmissionDelay(int port, int channel)
{
	cChannel *c = channelTables[port][channel];
	cDelayChannel *dc = check_and_cast<cDelayChannel *>(c);

	return dc->getDelay();
}
