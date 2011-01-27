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
		NodeOffset nodeOffset;
		nodeOffset.head = atof(tokenizer.nextToken());
		nodeOffset.tail = atof(tokenizer.nextToken());
		ansyncOffsetTable.insert(AnsyncOffset(dest, nodeOffset));
	}
	ifs.close();

	AnsyncOffsetTable::iterator it = ansyncOffsetTable.begin();
	while (it != ansyncOffsetTable.end())
	{
		ev << "IPAddress: " << it->first << " HeadOffset: " << (it->second).head << endl;
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
	NodeOffset nodeOffset = getAnsyncOffset(dest);
	simtime_t burstlength = bst->getBitLength() / wdm->getDatarate(0);

	if (burstlength > timeslot)
	{
		opp_error("%s length (%f [s]) larger than timeslot (%f [s]).", bst, burstlength.dbl(), timeslot.dbl());
	}

	simtime_t sHead = timeslot * (int)(simTime() / timeslot);
	simtime_t sTail = sHead + timeslot;
	simtime_t fsHead = sHead + nodeOffset.head;
	simtime_t fsTail = sTail - nodeOffset.tail;
	simtime_t fs = fsTail - fsHead;

	if (fs <= 0)
	{
		opp_error("fixed slot length (%f [s]) larger than timeslot (%f [s]).", fs.dbl(), timeslot.dbl());
	}

	simtime_t dTop = 0;
	simtime_t dBtm = 0;
	simtime_t sendTime = 0;

	if (burstlength <= fs)
		sendTime = fsHead;
	else
	{
		sendTime = fsTail - burstlength;
		if (sendTime < sHead)
		{
			sendTime = sTail - burstlength;
			if (sendTime > fsHead)
			{
				sendTime = fsHead;
				dBtm = sendTime + burstlength - fsTail;
			}
			else
			{
				dTop = fsHead - sendTime;
				dBtm = nodeOffset.tail;
			}
		}
		else
		{
			dTop = fsHead - sendTime;
		}
	}

	while (sendTime < simTime() + offset)
	{
		sendTime += timeslot;
	}

	ev << "Dispatcher send burst." << endl
	   << "offset: " << offset << " | "
	   << "burstlength: " << burstlength << " | "
	   << "sendTime: " << sendTime << " | "
	   << "droppableTop: " << dTop << " | "
	   << "droppableBtm: " << dBtm << endl;

	bcp->setSrcAddress(src);
	bcp->setDestAddress(dest);
	bcp->setBurstArrivalTime(sendTime);
	bcp->setBurstlength(burstlength);
	bcp->setBurstPort(0);
	bcp->setBurst(bst);
	bcp->setBursthead(dTop);
	bcp->setBursttail(dBtm);

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

void AnsyncSlottedDispatcher::receiveBurst(cMessage *msg)
{
	if (dynamic_cast<Burst *>(msg) != NULL)
	{
		Burst *bst = check_and_cast<Burst *>(msg);
		sendDelayed(msg, bst->getBitLength() / wdm->getDatarate(0), "out");
	}
	else if (dynamic_cast<BurstControlPacket *>(msg) != NULL)
		send(msg, "out");
}

NodeOffset AnsyncSlottedDispatcher::getAnsyncOffset(const IPAddress& address)
{
	AnsyncOffsetTable::iterator it = ansyncOffsetTable.find(address);
	if (it != ansyncOffsetTable.end())
		return it->second;
	else
	{
		opp_error("%s not found in this ansyncOffsetTable.", address.str().c_str());
		NodeOffset nodeOffset = {0, 0};
		return nodeOffset;
	}
}
