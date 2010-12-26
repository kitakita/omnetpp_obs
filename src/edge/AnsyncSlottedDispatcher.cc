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
		sendBurst(msg);
	else
		receiveBurst(msg);
}

void AnsyncSlottedDispatcher::parse()
{
	const char *destAddrs = par("ansyncOffsetFile");

	std::ifstream ifs(destAddrs);
	std::string line;
	while (getline(ifs, line) != NULL)
	{
		cStringTokenizer tokenizer(line.c_str());
		IPAddress dest = IPAddressResolver().resolve(tokenizer.nextToken()).get4();
		int offset = atoi(tokenizer.nextToken());
		ansyncOffsetTable.insert(AnsyncOffset(dest, offset));
	}
	ifs.close();

	AnsyncOffsetTable::iterator it = ansyncOffsetTable.begin();
	while (it != ansyncOffsetTable.end())
	{
		ev << "IPAddress: " << it->first << " Offset: " << it->second << endl;
		it++;
	}
}

void AnsyncSlottedDispatcher::sendBurst(cMessage *msg)
{
	Burst *bst = check_and_cast<Burst *>(msg);

	char bcpName[32];
	sprintf(bcpName, "%s-bcp", bst->getName());
	BurstControlPacket *bcp = new BurstControlPacket(bcpName, 2);

	IPControlInfo *ctrl = check_and_cast<IPControlInfo *>(bst->getControlInfo());
	IPAddress src = ctrl->getSrcAddr();
	IPAddress dest = ctrl->getDestAddr();

	simtime_t offset = oft->getOffset(dest);
	simtime_t ansyncOffset = getAnsyncOffset(dest);
	simtime_t burstlength = bst->getBitLength() / wdm->getDatarate(0);
	simtime_t sendTime = timeslot * (int)(simTime() / timeslot) + ansyncOffset;
	simtime_t nextSlot = timeslot * (int)(simTime() / timeslot) + timeslot;

	if (sendTime <= simTime() + offset)
	{
		sendTime += timeslot;
		nextSlot += timeslot;
	}

	if (burstlength > timeslot)
	{
		opp_error("%s length (%f [s]) larger than timeslot (%f [s]).", bst, burstlength.dbl(), timeslot.dbl());
	}

	int droppableByteLength = 0;
	simtime_t droppableTime = 0;

	if (sendTime + burstlength - nextSlot > 0)
	{
		droppableTime = sendTime + burstlength - nextSlot;
		droppableByteLength = (droppableTime * wdm->getDatarate(0) / 8).dbl();
	}

	ev << "Dispatcher send burst." << endl
	   << "offset: " << offset << " | "
	   << "ansyncOffset: " << ansyncOffset << " | "
	   << "burstlength: " << burstlength << " | "
	   << "sendTime: " << sendTime << " | "
	   << "nextSlot: " << nextSlot << " | "
	   << "droppableTime: " << droppableTime << " | "
	   << "droppableByteLength: " << droppableByteLength << " | "<< endl;

	bcp->setSrcAddress(src);
	bcp->setDestAddress(dest);
	bcp->setBurstArrivalTime(sendTime);
	bcp->setBurstlength(burstlength);
	bcp->setBurstPort(0);
	bcp->setBurstChannel(0);
	bcp->setBurst(bst);
	bcp->setDroppableByteLength(droppableByteLength);

	int channel = -1;
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

void AnsyncSlottedDispatcher::receiveBurst(cMessage *msg)
{
	send(msg, "out");
}

simtime_t AnsyncSlottedDispatcher::getAnsyncOffset(const IPAddress& address)
{
	AnsyncOffsetTable::iterator it = ansyncOffsetTable.find(address);
	if (it != ansyncOffsetTable.end())
		return (it->second * 8) / wdm->getDatarate(0);
	else
	{
		opp_error("%s not found in this ansyncOffsetTable.", address.str().c_str());
		return -1;
	}
}
