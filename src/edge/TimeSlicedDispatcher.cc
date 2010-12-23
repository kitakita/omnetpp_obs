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

#include "TimeSlicedDispatcher.h"
#include "IPAddress.h"
#include "IPControlInfo.h"
#include "Burst.h"
#include "BurstControlPacket.h"
#include "BurstSchedulerAccess.h"
#include "OffsetTableAccess.h"

Define_Module(TimeSlicedDispatcher);

void TimeSlicedDispatcher::initialize()
{
	oft = OffsetTableAccess().get();
	bsc = BurstSchedulerAccess().get();
	timeslot = par("timeslot");
	cDatarateChannel *c = check_and_cast<cDatarateChannel *>(getParentModule()->gate("burstg$o", 0)->getChannel());
	datarate = c->getDatarate();
}

void TimeSlicedDispatcher::handleMessage(cMessage *msg)
{
	std::string name = msg->getArrivalGate()->getName();
	if (name == "in")
		sendBurst(msg);
	else
		receiveBurst(msg);
}

void TimeSlicedDispatcher::sendBurst(cMessage *msg)
{
	Burst *bst = check_and_cast<Burst *>(msg);

	char bcpName[32];
	sprintf(bcpName, "%s-bcp", bst->getName());
	BurstControlPacket *bcp = new BurstControlPacket(bcpName);

	IPControlInfo *ctrl = check_and_cast<IPControlInfo *>(bst->getControlInfo());
	IPAddress src = ctrl->getSrcAddr();
	IPAddress dest = ctrl->getDestAddr();

	simtime_t offset = oft->getOffset(dest);
	simtime_t burstSendingTime = simTime() + offset;
	simtime_t nextTimeslot = timeslot * ((int)(burstSendingTime / timeslot) + 1);
	simtime_t burstlength = bst->getBitLength() / datarate;

	ev << "Dispatcher send burst." << endl
	   << "bcpoffset: " << offset << " | "
	   << "sendingtime: " << burstSendingTime << " | "
	   << "burstlength" << burstlength << " | "
	   << "nexttimeslot: " << nextTimeslot << " | " << endl;

	if (burstlength > timeslot)
		opp_error("%s length (%f [s]) larger than timeslot (%f [s]).", bst, burstlength.dbl(), timeslot.dbl());

	if (nextTimeslot < burstSendingTime + burstlength)
		burstSendingTime = nextTimeslot;

	bcp->setSrcAddress(src);
	bcp->setDestAddresss(dest);
	bcp->setBurstArrivalTime(burstSendingTime);
	bcp->setBurstlength(burstlength);
	bcp->setBurstIngressPort(0);
	bcp->setBurstIngressChannel(-1);

	ScheduleResult res;
	while ((res = bsc->schedule(0, bcp)).channel < 0)
	{
		if (burstSendingTime < nextTimeslot)
			burstSendingTime = nextTimeslot;
		else
			burstSendingTime += timeslot;

		bcp->setBurstArrivalTime(burstSendingTime);
	}

    bcp->setBurstIngressChannel(res.channel);
    bcp->setBurstDropableLength(bst->getBitLength() / 2);

    send(bcp, "bcpg$o");
    sendDelayed(bst, burstSendingTime - simTime(), "burstg$o", res.channel);
}

void TimeSlicedDispatcher::receiveBurst(cMessage *msg)
{
	send(msg, "out");
}
