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
		sendBurst(msg);
	else
		receiveBurst(msg);
}

void SlottedDispatcher::sendBurst(cMessage *msg)
{
	Burst *bst = check_and_cast<Burst *>(msg);

	char bcpName[32];
	sprintf(bcpName, "%s-bcp", bst->getName());
	BurstControlPacket *bcp = new BurstControlPacket(bcpName, 1);

	IPControlInfo *ctrl = check_and_cast<IPControlInfo *>(bst->getControlInfo());
	IPAddress src = ctrl->getSrcAddr();
	IPAddress dest = ctrl->getDestAddr();

	simtime_t offset = oft->getOffset(dest);
	simtime_t sendTime;
	simtime_t burstlength = bst->getBitLength() / wdm->getDatarate(0);

	if ((simTime() + offset).raw() % timeslot.raw() == 0)
		sendTime = timeslot * (int)((simTime() + offset) / timeslot);
	else
		sendTime = timeslot * ((int)((simTime() + offset) / timeslot) + 1);

	if (burstlength > timeslot)
	{
		opp_error("%s length (%f [s]) larger than timeslot (%f [s]).", bst, burstlength.dbl(), timeslot.dbl());
	}

	ev << "Dispatcher send burst." << endl
	   << "offset: " << offset << " | "
	   << "sendtime: " << sendTime << " | "
	   << "burstlength: " << burstlength << " | " << endl;

	bcp->setSrcAddress(src);
	bcp->setDestAddress(dest);
	bcp->setBurstArrivalTime(sendTime);
	bcp->setBurstlength(burstlength);
	bcp->setBurstPort(0);
	bcp->setBurstChannel(0);
	bcp->setBurst(bst);
	bcp->setBursthead(bst->getByteLength() / 3);
	bcp->setBursttail(bst->getByteLength() / 3);

	int channel = bsc->schedule(bcp, 0);
	while (channel < 0)
	{
		channel = bsc->schedule(bcp, 0);
		sendTime += timeslot;
		bcp->setBurstArrivalTime(sendTime);
	}

    bcp->setBurstChannel(channel);

    send(bcp, "bcpg$o");
    sendDelayed(bst, sendTime - simTime(), "burstg$o", channel);
}

void SlottedDispatcher::receiveBurst(cMessage *msg)
{
	send(msg, "out");
}

