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

#include "PacketGenerator.h"
#include "IPControlInfo.h"
#include "IPAddressResolver.h"

Define_Module(PacketGenerator);

int PacketGenerator::counter;

void PacketGenerator::initialize()
{
	myAddress = IPAddressResolver().resolve(par("myAddress"));
	pktByteLength = par("packetLength");
	numPackets = par("numPackets");
	simtime_t startTime = par("startTime");

	const char *destAddrs = par("destAddresses");
	cStringTokenizer tokenizer(destAddrs);
	const char *token;
	while ((token = tokenizer.nextToken())!=NULL)
		destAddresses.push_back(IPAddressResolver().resolve(token));

	counter = 0;

	numSent = 0;

	WATCH(numSent);

	if (destAddresses.empty())
		return;

	if (numPackets > 0) {
		cMessage *timer = new cMessage("sendTimer");
		scheduleAt(startTime, timer);
	}
}

void PacketGenerator::handleMessage(cMessage *msg)
{
	if (msg->isSelfMessage())
	{
		sendPacket();

		if (!numPackets || numSent < numPackets)
			scheduleAt(simTime() + (double)par("packetInterval"), msg);
		else
			delete msg;
	}

	if (ev.isGUI())
	{
		char buf[40];
		sprintf(buf, "sent: %d packets", numSent);
		getDisplayString().setTagArg("t", 0, buf);
	}
}

IPvXAddress PacketGenerator::chooseDestAddr()
{
    int k = intuniform(0, (double)(destAddresses.size() - 1));
    return destAddresses[k];
}

void PacketGenerator::sendPacket()
{
    char pktName[32];
    sprintf(pktName,"packet-%d", counter++);

    cPacket *pkt = new cPacket(pktName);
    pkt->setByteLength(pktByteLength);

    IPvXAddress destAddr = chooseDestAddr();
    IPControlInfo *controlInfo = new IPControlInfo();
    controlInfo->setSrcAddr(myAddress.get4());
    controlInfo->setDestAddr(destAddr.get4());
    pkt->setControlInfo(controlInfo);

    send(pkt, "out");
    numSent++;
}
