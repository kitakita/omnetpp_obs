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

#include "MixedTimerBurstlengthBasedQueue.h"
#include "Burst.h"
#include "IPControlInfo.h"

Define_Module(MixedTimerBurstlengthBasedQueue);

int MixedTimerBurstlengthBasedQueue::counter;

MixedTimerBurstlengthBasedQueue::MixedTimerBurstlengthBasedQueue()
{
	queue = NULL;
	timeoutEvent = NULL;
}

MixedTimerBurstlengthBasedQueue::~MixedTimerBurstlengthBasedQueue()
{
	while (!queue->empty())
		delete queue->pop();
	delete queue;
	cancelAndDelete(timeoutEvent);
}

void MixedTimerBurstlengthBasedQueue::initialize()
{
	queue = new cPacketQueue("queue");
	maxByteBurstlength = par("maxBurstlength");
	timeoutPeriod = par("timeout");
	timeoutEvent = new cMessage("BurstAssembleTimeout");

	counter = 0;
}

void MixedTimerBurstlengthBasedQueue::handleMessage(cMessage *msg)
{
	if (!msg->isSelfMessage())
        processPacket(msg);
    else if (msg == timeoutEvent)
    	handleTimeout();
    else
    	opp_error("Receive unknown message (%s).", msg);
}

void MixedTimerBurstlengthBasedQueue::handleTimeout()
{
	ev << "Assemble timeout. ";
	sentCurrentQueueAsBurst();
}

void MixedTimerBurstlengthBasedQueue::sentCurrentQueueAsBurst()
{
	char msgName[32];
	sprintf(msgName, "burst-%d", counter++);
	Burst *bst = new Burst(msgName);

    cPacket *pkt = queue->get(0);
    IPControlInfo *bstCtrl = new IPControlInfo();
	IPControlInfo *pktCtrl = (IPControlInfo *)pkt->getControlInfo();
	bstCtrl->setSrcAddr(pktCtrl->getSrcAddr());
	bstCtrl->setDestAddr(pktCtrl->getDestAddr());
	bst->setControlInfo(bstCtrl);

    ev << "Burst created (" << msgName << ": " << queue->info() << ")" << endl;

    bst->setPacketQueue(queue);
    queue = new cPacketQueue("queue");

	send(bst, "out");
}

void MixedTimerBurstlengthBasedQueue::processPacket(cMessage *msg)
{
	cPacket *pkt = check_and_cast<cPacket *>(msg);

    if (queue->isEmpty())
    {
    	queue->insert(pkt);
    	scheduleAt(simTime() + timeoutPeriod, timeoutEvent);
    }
    else
    {
    	if ((queue->getByteLength() + pkt->getByteLength()) <= maxByteBurstlength)
    		queue->insert(pkt);
    	else
    	{
    		sentCurrentQueueAsBurst();
    		cancelEvent(timeoutEvent);
    		processPacket(msg);
    	}
    }
}
