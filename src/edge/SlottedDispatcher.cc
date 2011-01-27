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

#include "SlottedDispatcher.h"

#include "IPAddress.h"
#include "IPControlInfo.h"

#include "Burst.h"
#include "BurstControlPacket.h"
#include "BurstSchedulerAccess.h"
#include "OffsetTableAccess.h"
#include "WDMTableAccess.h"

Define_Module(SlottedDispatcher);

void SlottedDispatcher::initialize()
{
	bsc = BurstSchedulerAccess().get();
	oft = OffsetTableAccess().get();
	wdm = WDMTableAccess().get();

	timeslot = par("timeslot");
}

void SlottedDispatcher::handleMessage(cMessage *msg)
{
	std::string name = msg->getArrivalGate()->getName();
	if (name == "in")
		handleSendingBurst(msg);
	else if (dynamic_cast<Burst *>(msg) != NULL)
		handleReceivedBurst(msg);
	else
		send(msg, "out");
}

void SlottedDispatcher::handleSendingBurst(cMessage *msg)
{
	Burst *bst = check_and_cast<Burst *>(msg);

	char bcpName[32];
	sprintf(bcpName, "%s-bcp", bst->getName());
	BurstControlPacket *bcp = new BurstControlPacket(bcpName, 1);

	IPControlInfo *ctrl = check_and_cast<IPControlInfo *>(bst->getControlInfo());
	IPAddress src = ctrl->getSrcAddr();
	IPAddress dest = ctrl->getDestAddr();

	simtime_t offset = oft->getOffset(dest);
	simtime_t burstlength = bst->getBitLength() / wdm->getDatarate(0);

	if (burstlength > timeslot)
	{
		opp_error("%s length (%f [s]) larger than timeslot (%f [s]).", bst, burstlength.dbl(), timeslot.dbl());
	}

	simtime_t sendTime = timeslot * (int)((simTime() + offset) / timeslot);
	while (sendTime < simTime() + offset)
		sendTime += timeslot;

	ev << "Dispatcher send burst." << endl
	   << "offset: " << offset << " | "
	   << "sendtime: " << sendTime << " | "
	   << "burstlength: " << burstlength << " | " << endl;

	bcp->setSrcAddress(src);
	bcp->setDestAddress(dest);
	bcp->setBurst(bst);
	bcp->setBurstArrivalTime(sendTime);
	bcp->setBurstlength(burstlength);
	bcp->setBurstPort(0);
	bcp->setBurstChannel(0);
	bcp->setBursthead(burstlength / 3);
	bcp->setBursttail(burstlength / 3);

	int headAndTail = (int)(burstlength / 3 * wdm->getDatarate(0) * 8).dbl();
	bst->setHead(headAndTail);
	bst->setRestHead(headAndTail);
	bst->setTail(headAndTail);
	bst->setRestTail(headAndTail);

	int channel = bsc->schedule(bcp, 0);

	while (channel < 0)
	{
		sendTime += timeslot;
		bcp->setBurstArrivalTime(sendTime);
		channel = bsc->schedule(bcp, 0);
	}

    bcp->setBurstChannel(channel);

    send(bcp, "bcpg$o");
    sendDelayed(bst, sendTime - simTime(), "burstg$o", channel);
}

void SlottedDispatcher::handleReceivedBurst(cMessage *msg)
{
	Burst *bst = check_and_cast<Burst *>(msg);
	simtime_t txFinishTime = (bst->getByteLength() - bst->getTail() + bst->getRestTail()) * 8 / wdm->getDatarate(0);

	sendDelayed(msg, txFinishTime, "out");
}

