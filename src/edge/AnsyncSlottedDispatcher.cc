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

#include <fstream>

#include "AnsyncSlottedDispatcher.h"

#include "IPAddress.h"
#include "IPAddressResolver.h"
#include "IPControlInfo.h"

#include "Burst.h"
#include "BurstControlPacket.h"
#include "BurstSchedulerAccess.h"
#include "OffsetTableAccess.h"
#include "WDMTableAccess.h"

Define_Module(AnsyncSlottedDispatcher);

void AnsyncSlottedDispatcher::initialize()
{
	bsc = BurstSchedulerAccess().get();
	oft = OffsetTableAccess().get();
	wdm = WDMTableAccess().get();

	parse();

	timeslot = par("timeslot");
}

void AnsyncSlottedDispatcher::handleMessage(cMessage *msg)
{
	std::string name = msg->getArrivalGate()->getName();
	if (name == "in")
		handleSendingBurst(msg);
	else if (dynamic_cast<Burst *>(msg) != NULL)
		handleReceivedBurst(msg);
	else
		send(msg, "out");
}

void AnsyncSlottedDispatcher::parse()
{
	const char *destAddrs = par("droppableLengthFile");

	std::ifstream ifs(destAddrs);
	std::string line;
	while (getline(ifs, line) != NULL)
	{
		cStringTokenizer tokenizer(line.c_str());
		IPAddress dest = IPAddressResolver().resolve(tokenizer.nextToken()).get4();
		DroppableLength data = { atof(tokenizer.nextToken()), atof(tokenizer.nextToken()) };
		droppableLengthTable.insert(DroppableLengthPair(dest, data));
	}
	ifs.close();

	ev << getFullName() << " setting" << endl
	   << "IPAddress\tBursthead\tBursttail" << endl;
	DroppableLengthTable::iterator it = droppableLengthTable.begin();
	char buf[32];
	while (it != droppableLengthTable.end())
	{
		sprintf(buf, "%f\t%f", (it->second).bursthead.dbl(), (it->second).bursttail.dbl());
		ev << it->first << "\t" << buf << endl;
		it++;
	}
}

void AnsyncSlottedDispatcher::handleSendingBurst(cMessage *msg)
{
	Burst *bst = check_and_cast<Burst *>(msg);

	char bcpName[32];
	sprintf(bcpName, "%s-bcp", bst->getName());
	BurstControlPacket *bcp = new BurstControlPacket(bcpName, 2);

	IPControlInfo *ctrl = check_and_cast<IPControlInfo *>(bst->getControlInfo());
	IPAddress src = ctrl->getSrcAddr();
	IPAddress dest = ctrl->getDestAddr();

	simtime_t offset = oft->getOffset(dest);
	simtime_t burstlength = bst->getBitLength() / wdm->getDatarate(0);
	simtime_t maxBursthead = getMaxBursthead(dest);
	simtime_t maxBursttail = getMaxBursttail(dest);

	if (burstlength > timeslot)
		opp_error("%s length (%f [s]) larger than timeslot (%f [s]).", bst, burstlength.dbl(), timeslot.dbl());

	simtime_t slothead = timeslot * (int)(simTime() / timeslot);
	simtime_t slottail = slothead + timeslot;
	simtime_t fixedSlothead = slothead + maxBursthead;
	simtime_t fixedSlottail = slottail - maxBursttail;
	simtime_t fixedTimeslot = fixedSlottail - fixedSlothead;

	if (fixedTimeslot <= 0)
		opp_error("In %s, fixed timeslot (%f [s]) larger than timeslot (%f [s]).", getFullName(), fixedTimeslot.dbl(), timeslot.dbl());

	simtime_t bursthead = 0;
	simtime_t bursttail = 0;
	simtime_t sendtime = 0;

	if (burstlength <= fixedTimeslot)
		sendtime = fixedSlothead;
	else
	{
		sendtime = fixedSlottail - burstlength;
		if (sendtime < slothead)
		{
			sendtime = slottail - burstlength;
			if (sendtime > fixedSlothead)
			{
				sendtime = fixedSlothead;
				bursttail = sendtime + burstlength - fixedSlottail;
			}
			else
			{
				bursthead = fixedSlothead - sendtime;
				bursttail = maxBursttail;
			}
		}
		else
			bursthead = fixedSlothead - sendtime;
	}

	while (sendtime < simTime() + offset)
		sendtime += timeslot;



	bcp->setSrcAddress(src);
	bcp->setDestAddress(dest);
	bcp->setBurstArrivalTime(sendtime);
	bcp->setBurstlength(burstlength);
	bcp->setBurstPort(0);
	bcp->setBurst(bst);
	bcp->setBursthead(bursthead);
	bcp->setBursttail(bursttail);

	int headByteLength = ((int)(bursthead * wdm->getDatarate(0)).dbl()+7)>>3;
	int tailByteLength = ((int)(bursttail * wdm->getDatarate(0)).dbl()+7)>>3;
	bst->setHead(headByteLength);
	bst->setRestHead(headByteLength);
	bst->setTail(tailByteLength);
	bst->setRestTail(tailByteLength);

	int channel = bsc->schedule(bcp, 0);
	while (channel < 0)
	{
		sendtime += timeslot;
		bcp->setBurstArrivalTime(sendtime);
		channel = bsc->schedule(bcp, 0);
	}

    bcp->setBurstChannel(channel);

    send(bcp, "bcpg$o");

    ev << "Dispatcher send burst." << endl
	   << "offset: " << offset << " | "
	   << "sendTime: " << sendtime << " | "
	   << "burstlength: " << burstlength << " | "
	   << "bursthead: " << bursthead << " | "
	   << "bursttail: " << bursttail << endl;

    sendDelayed(bst, sendtime - simTime(), "burstg$o", channel);
}

void AnsyncSlottedDispatcher::handleReceivedBurst(cMessage *msg)
{
	Burst *bst = check_and_cast<Burst *>(msg);
	simtime_t txFinishTime = (bst->getByteLength() - bst->getTail() + bst->getRestTail()) * 8 / wdm->getDatarate(0);

	sendDelayed(msg, txFinishTime, "out");
}

simtime_t AnsyncSlottedDispatcher::getMaxBursthead(const IPAddress& address)
{
	DroppableLength data = getDroppableLength(address);
	return data.bursthead;
}

simtime_t AnsyncSlottedDispatcher::getMaxBursttail(const IPAddress& address)
{
	DroppableLength data = getDroppableLength(address);
	return data.bursttail;
}

DroppableLength AnsyncSlottedDispatcher::getDroppableLength(const IPAddress& address)
{
	DroppableLengthTable::iterator it = droppableLengthTable.find(address);
	if (it == droppableLengthTable.end())
		opp_error("IP %s is not found in %s.", address.str().c_str(), getFullName());
	return it->second;
}

