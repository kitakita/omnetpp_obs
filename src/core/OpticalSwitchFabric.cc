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
	numDataChannels = par("numDataChannels");

	if (gateSize("burstg$i") % numDataChannels != 0)
		opp_error("Number of data channels is wrong.");
	numLinkedNodes = gateSize("burstg$i") / numDataChannels;

	for (int i = 0; i < numDataChannels * numLinkedNodes; i++)
		connectionTable.push_back(-1);

	for (int i = 0; i < numLinkedNodes; i++)
	{
		cGate *g = gate("burstg$o", i * numDataChannels);
		cDatarateChannel *c = check_and_cast<cDatarateChannel *>(g->getTransmissionChannel());
		transmissionDelay.push_back(c->getDelay());
	}
	std::vector<simtime_t>::iterator it = transmissionDelay.begin();

	dropCounter = 0;

	WATCH(dropCounter);
	printSwitch();
}

void OpticalSwitchFabric::handleMessage(cMessage *msg)
{
	int port = msg->getArrivalGate()->getIndex();

	if (connectionTable[port] >= 0)
	{
		cGate *g = gate("burstg$o", connectionTable[port]);
		ev << "port " << port << " to " << connectionTable[port] << endl;
		send(msg, "burstg$o", connectionTable[port]);
		printSwitch();
	}
	else
	{
		ev << "Optical switch: port " << port << " disconnected. Burst was dropped." << endl;
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

void OpticalSwitchFabric::connect(int inPort, int inChannel, int outPort, int outChannel)
{
	Enter_Method("Switching port: %d channel: %d to port: %d channel: %d.", inPort, inChannel, outPort, outChannel);

	if ((inPort > numLinkedNodes - 1) || (outPort > numLinkedNodes - 1))
		opp_error("Wrong node (in:%d, out:%d).", inPort, outPort);

	int inIndex = inPort * numDataChannels + inChannel;
	int outIndex = outPort * numDataChannels + outChannel;
	connectionTable[inIndex] = outIndex;

	printSwitch();
}

void OpticalSwitchFabric::disconnect(int inPort, int inChannel)
{
	Enter_Method("port: %d channel: %d disconnected.", inPort, inChannel);

	connectionTable[inPort * numDataChannels + inChannel] = -1;

	printSwitch();
}

simtime_t OpticalSwitchFabric::getTransmissionDelay(int receiverNodeId)
{
	return transmissionDelay[receiverNodeId];
}

void OpticalSwitchFabric::printSwitch()
{
	ev << getFullPath() << " in " << simTime() << endl;
	for (int i = 0; i < numDataChannels * numLinkedNodes; i++)
		ev << "index " << i << " connect to " << connectionTable[i] << endl;
}
