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

#include "MixedTimeBurstlengthBasedQueue.h"
#include "Burst.h"
#include "IPControlInfo.h"

Define_Module(MixedTimeBurstlengthBasedQueue);

int MixedTimeBurstlengthBasedQueue::counter;

MixedTimeBurstlengthBasedQueue::MixedTimeBurstlengthBasedQueue()
{
	queue = NULL;
	timeoutEvent = NULL;
}

MixedTimeBurstlengthBasedQueue::~MixedTimeBurstlengthBasedQueue()
{
	while (!queue->empty())
		delete queue->pop();
	delete queue;
	cancelAndDelete(timeoutEvent);
}

void MixedTimeBurstlengthBasedQueue::initialize()
{
	queue = new cPacketQueue("queue");
	maxByteBurstlength = par("maxBurstlength");
	timeout = par("timeout");
	timeoutEvent = new cMessage("BurstAssembleTimeout");

	counter = 0;
}

void MixedTimeBurstlengthBasedQueue::handleMessage(cMessage *msg)
{
	if (!msg->isSelfMessage())
		handlePacket(msg);
	else if (msg == timeoutEvent)
		handleTimeout();
	else
	{
		opp_error("Receive unknown message (%s).", msg);
		delete msg;
	}
}

void MixedTimeBurstlengthBasedQueue::handleTimeout()
{
	ev << "Queue was timeout." << endl;
	assembleBurst();
}

void MixedTimeBurstlengthBasedQueue::assembleBurst()
{
	char msgName[32];
	sprintf(msgName, "burst-%d", counter++);
	Burst *bst = new Burst(msgName);

	cPacket *pkt = queue->get(0);

	if (pkt == NULL)
		opp_error("%s assembled burst with timeout. But queue is empty.", getFullName());

    IPControlInfo *bstCtrl = new IPControlInfo();
	IPControlInfo *pktCtrl = (IPControlInfo *)pkt->getControlInfo();
	bstCtrl->setSrcAddr(pktCtrl->getSrcAddr());
	bstCtrl->setDestAddr(pktCtrl->getDestAddr());
	bst->setControlInfo(bstCtrl);

    ev << "Queue assemble burst (" << msgName << ": " << queue->info() << ")" << endl;

    bst->setPacketQueue(queue);
    queue = new cPacketQueue("queue");

	send(bst, "out");
}

void MixedTimeBurstlengthBasedQueue::handlePacket(cMessage *msg)
{
	cPacket *pkt = check_and_cast<cPacket *>(msg);

    if (queue->isEmpty())
    {
    	queue->insert(pkt);
    	scheduleAt(simTime() + timeout, timeoutEvent);
    }
    else
    {
    	if (queue->getByteLength() + pkt->getByteLength() <= maxByteBurstlength)
    		queue->insert(pkt);
    	else
    	{
    		assembleBurst();
    		cancelEvent(timeoutEvent);
    		handlePacket(msg);
    	}
    }
}

