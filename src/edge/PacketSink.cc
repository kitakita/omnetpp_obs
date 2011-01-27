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
    sumBytes = 0;
    sumDelay = 0;
    packetPerSec = 0;
    bitPerSec = 0;

    WATCH(numPackets);
    WATCH(sumBytes);
    WATCH(sumDelay);
    WATCH(packetPerSec);
    WATCH(bitPerSec);
}

void PacketSink::handleMessage(cMessage *msg)
{
	cPacket *pkt = check_and_cast<cPacket *>(msg);
	IPControlInfo *ctrl = (IPControlInfo *)pkt->getControlInfo();

	if (myAddress == ctrl->getDestAddr())
	{
		numPackets++;
		sumBytes += pkt->getByteLength();
		sumDelay += simTime() - pkt->getCreationTime();
		packetPerSec = numPackets / simTime();
		bitPerSec = (sumBytes * 8) / simTime();
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
	recordScalar("Sink-numPackets", numPackets);
	recordScalar("Sink-sumBytes", sumBytes);
	recordScalar("Sink-packetPerSec", packetPerSec);
    recordScalar("Sink-bitPerSec", bitPerSec);
	recordScalar("Sink-avrDelay", sumDelay / numPackets);
}
