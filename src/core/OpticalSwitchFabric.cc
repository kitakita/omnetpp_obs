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

#include "OpticalSwitchFabric.h"

Define_Module(OpticalSwitchFabric);

void OpticalSwitchFabric::initialize()
{
	int gatesize = gateSize("burstg$i");

	for (int i = 0; i < gatesize; i++)
		connectionTable.push_back(-1); // disconnect = -1

	dropCounter = 0;

	WATCH(dropCounter);

	printSwitch();
}

void OpticalSwitchFabric::handleMessage(cMessage *msg)
{
	int index = msg->getArrivalGate()->getIndex();
	int port = connectionTable[index];

	if (port >= 0)
	{
		ev << getFullName() << ": " << msg->getName() << " switched port " << index << " to port " << port << "." << endl;
		send(msg, "burstg$o", port);
		disconnect(index);
	}
	else
	{
		ev << getFullName() << ": port " << port << " disconnected. Burst was dropped." << endl;
		dropCounter++;
		delete msg;
	}

	if (ev.isGUI())
	{
		char buf[32];
		sprintf(buf, "drop: %d bursts", dropCounter);
		getDisplayString().setTagArg("t", 0, buf);
	}
}

void OpticalSwitchFabric::finish()
{
	recordScalar("OpticalSwitchFabric-dropCounter", dropCounter);
}

void OpticalSwitchFabric::connect(unsigned int in, unsigned int out)
{
	Enter_Method("Switching port: in-port %d to out-port %d.", in, out);

	if ((in >= connectionTable.size()) || (out >= connectionTable.size()))
		opp_error("Switch index out of range (in: %d, out: %d).", in, out);

	ConnectionTable::iterator it = connectionTable.begin();
	while(it != connectionTable.end())
	{
		if (*it == (int)out)
			*it = -1;
		it++;
	}

	connectionTable[in] = out;

	printSwitch();
}

void OpticalSwitchFabric::disconnect(unsigned int in)
{
	Enter_Method("Switching port: in-port %d is disconnected.", in);

	if (in >= connectionTable.size())
		opp_error("Switch index out of range (in: %d).", in);

	connectionTable[in] = -1;

	printSwitch();
}

void OpticalSwitchFabric::printSwitch()
{
	ev << getFullName() << ": Connection Table" << endl
	   << "input\toutput" << endl;
	ConnectionTable::iterator it = connectionTable.begin();
	int i = 0;
	char buf[16];
	while(it != connectionTable.end())
	{
		sprintf(buf, "%2d\t%2d", i++, *it++);
		ev << buf << endl;
	}
}
