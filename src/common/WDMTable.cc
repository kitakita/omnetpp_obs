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
	int gatesize = parent->gateSize("burstg$o");

	for (int i = 0, prev = -1; i < gatesize; i++)
	{
		cGate *g = parent->gate("burstg$o", i)->getNextGate();
		int id = g->getOwnerModule()->getId();
		if (id != prev)
		{
			prev = id;
			numChannelTable.push_back(0);
			cDatarateChannel *c = check_and_cast<cDatarateChannel *>(g->getChannel());
			transmissionDelayTable.push_back(c->getDelay());
		}
		numChannelTable.back() += 1;
	}

	numLinkedNodes = numChannelTable.size();
}

void WDMTable::handleMessage(cMessage *msg)
{
    opp_error("WDMTable cannot receive any messages.");
}

int WDMTable::getGateIndex(int port, int channel)
{
	return (port * numLinkedNodes) + channel;
}
