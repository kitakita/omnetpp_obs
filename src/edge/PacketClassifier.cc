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

#include "PacketClassifier.h"
#include "IPControlInfo.h"

Define_Module(PacketClassifier);

void PacketClassifier::initialize()
{
	// nothing
}

void PacketClassifier::handleMessage(cMessage *msg)
{
	send(msg, "out", getQueueIndex(msg));
}

int PacketClassifier::getQueueIndex(cMessage *msg)
{
	cPacket *pkt = check_and_cast<cPacket*>(msg);

	IPControlInfo *ctrl = (IPControlInfo *)pkt->getControlInfo();
    IPAddress destAddr = ctrl->getDestAddr();

	std::vector<IPAddress>::iterator it = destAddresses.begin();
	for(int i = 0; it != destAddresses.end(); i++, it++)
		if (*it == destAddr) return i;

	destAddresses.push_back(destAddr);

//	if ((int)destAddresses.size() > gate("out")->getVectorSize())
//		opp_error("Queue index is out of range. (Destinations: %d, Gates: %d)", destAddresses.size(), gate("out")->getVectorSize());

	return destAddresses.size() - 1;
}
