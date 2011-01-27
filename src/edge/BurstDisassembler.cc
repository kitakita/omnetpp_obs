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

#include "BurstDisassembler.h"
#include "Burst.h"
#include "BurstControlPacket.h"
#include "IPControlInfo.h"
#include "IPAddressResolver.h"

Define_Module(BurstDisassembler);

void BurstDisassembler::initialize()
{
	myAddress = IPAddressResolver().resolve(par("myAddress"));
	numBursts = 0;
	numBCPs = 0;
	burstPerSec = 0;
	sumPackets =0;
	sumBytes = 0;
	sumDelay = 0;

	WATCH(numBursts);
	WATCH(numBCPs);
	WATCH(burstPerSec);
	WATCH(sumPackets);
	WATCH(sumBytes);
	WATCH(sumDelay);
}

void BurstDisassembler::handleMessage(cMessage *msg)
{
	if (dynamic_cast<Burst *>(msg) != NULL)
		handleBurst(msg);
	else if (dynamic_cast<BurstControlPacket *>(msg) != NULL)
		handleBurstControlPacket(msg);
	else
		delete msg;
}

void BurstDisassembler::finish()
{
	if (numBursts == 0)
		numBursts = 1;

    recordScalar("Disassembler-numBursts", numBursts);
    recordScalar("Disassembler-numBCPs", numBCPs);
    recordScalar("Disassembler-burstPerSec", burstPerSec);
    recordScalar("Disassembler-sumPackets", sumPackets);
    recordScalar("Disassembler-sumBytes", sumBytes);
    recordScalar("Disassembler-packetPreBurst", sumPackets / numBursts);
    recordScalar("Disassembler-avrByteBurstlength", sumBytes / numBursts);
    recordScalar("Disassembler-avrDelay", sumDelay / numBursts);
}

void BurstDisassembler::handleBurst(cMessage *msg)
{
	Burst *bst = check_and_cast<Burst *>(msg);
	IPControlInfo *ctrl = (IPControlInfo *)bst->getControlInfo();

	if (myAddress == ctrl->getDestAddr())
	{
		cPacketQueue *queue = bst->getPacketQueue();

		numBursts++;
		burstPerSec = numBursts / simTime();
		sumPackets += queue->length();
		sumBytes += bst->getByteLength();
		sumDelay += simTime() - bst->getCreationTime();

		while (!queue->empty())
			send(queue->pop(), "out");
	}

	if (ev.isGUI())
	{
		char buf[32];
		sprintf(buf, "rcvd: %d bursts", numBursts);
		getDisplayString().setTagArg("t", 0, buf);
	}

	delete msg;
}

void BurstDisassembler::handleBurstControlPacket(cMessage *msg)
{
	BurstControlPacket *bcp = check_and_cast<BurstControlPacket *>(msg);

	if (myAddress == bcp->getDestAddress())
		numBCPs++;

	delete msg;
}
