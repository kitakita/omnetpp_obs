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

#include "PacketSink.h"
#include "IPControlInfo.h"
#include "IPAddressResolver.h"

Define_Module(PacketSink);

void PacketSink::initialize()
{
	myAddress = IPAddressResolver().resolve(par("myAddress"));
    numPackets = 0;
    numBits = 0;
    throughput = 0;
    packetsPerSec = 0;

    WATCH(numPackets);
    WATCH(numBits);
    WATCH(throughput);
    WATCH(packetsPerSec);
}

void PacketSink::handleMessage(cMessage *msg)
{
	cPacket *pkt = check_and_cast<cPacket *>(msg);
	IPControlInfo *ctrl = (IPControlInfo *)pkt->getControlInfo();

	if (myAddress == ctrl->getDestAddr())
	{
		numPackets++;
		numBits += pkt->getBitLength();
		throughput = numBits / simTime();
		packetsPerSec = numPackets / simTime();
	}

    if (ev.isGUI())
    {
        char buf[32];
        sprintf(buf, "rcvd: %d packets", numPackets);
        getDisplayString().setTagArg("t", 0, buf);
    }

    delete msg;
}

void PacketSink::finish()
{
	recordScalar("numPackets", numPackets);
	recordScalar("numBits", numBits);
    recordScalar("throughput", throughput);
	recordScalar("packetsPerSec", packetsPerSec);
}
