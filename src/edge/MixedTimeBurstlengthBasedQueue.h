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

#ifndef __OMNETPP_OBS_MIXEDTIMEBURSTLENGTHBASEDQUEUE_H_
#define __OMNETPP_OBS_MIXEDTIMEBURSTLENGTHBASEDQUEUE_H_

#include <omnetpp.h>

/**
 * TODO - Generated class
 */
class MixedTimeBurstlengthBasedQueue : public cSimpleModule
{
  protected:
	cPacketQueue *queue;
	cMessage *timeoutEvent;

	int maxByteBurstlength;
	double timeout;

	static int counter;

	virtual void initialize();
	virtual void handleMessage(cMessage *msg);

	virtual void handleTimeout();
	virtual void handlePacket(cMessage *msg);
	virtual void assembleBurst();

  public:
	MixedTimeBurstlengthBasedQueue();
	virtual ~MixedTimeBurstlengthBasedQueue();
};

#endif
