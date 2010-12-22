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
	numBits = 0;
	packetsPreBurst = 0;
	burstsPerSec = 0;
	avrDelay = 0;

	WATCH(numBursts);
	WATCH(numBits);
	WATCH(packetsPreBurst);
	WATCH(burstsPerSec);
	WATCH(avrDelay);
}

void BurstDisassembler::handleMessage(cMessage *msg)
{
	if (dynamic_cast<Burst *>(msg) != NULL)
		handleBurst(msg);
	else if (dynamic_cast<BurstControlPacket *>(msg) != NULL)
		handleBurstControlPacket(msg);
}

void BurstDisassembler::finish()
{
    recordScalar("numBursts", numBursts);
    recordScalar("numBits", numBits);
    recordScalar("packetsPreBurst", packetsPreBurst);
    recordScalar("burstsPerSec", burstsPerSec);
}

void BurstDisassembler::handleBurst(cMessage *msg)
{
	Burst *bst = check_and_cast<Burst *>(msg);
	IPControlInfo *ctrl = (IPControlInfo *)bst->getControlInfo();

	if (myAddress == ctrl->getDestAddr())
	{
		numBursts++;
		numBits += bst->getBitLength();
		packetsPreBurst = (packetsPreBurst * (((double)numBursts - 1) / (double)numBursts))
						  + ((double)(bst->getNumPackets()) / (double)numBursts);
		burstsPerSec = numBursts / simTime();

		cPacketQueue *queue = bst->getPacketQueue();
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

	simtime_t creation = bcp->getCreationTime();
	simtime_t arrival = bcp->getBurstArrivalTime() + bcp->getBurstlength();
	simtime_t delay = arrival - creation;

	avrDelay = (avrDelay * (((double)numBursts - 1) / (double)numBursts)) + (delay / (double)numBursts);

	delete msg;
}
