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

#ifndef __OMNETPP_OBS_ANSYNCSLOTTEDDISPATCHER_H_
#define __OMNETPP_OBS_ANSYNCSLOTTEDDISPATCHER_H_

#include <omnetpp.h>

#include "IBurstScheduler.h"
#include "OffsetTable.h"
#include "WDMTable.h"

struct DroppableLength
{
    simtime_t bursthead;
    simtime_t bursttail;
};

/**
 * TODO - Generated class
 */
class AnsyncSlottedDispatcher : public cSimpleModule
{
  protected:
	IBurstScheduler *bsc;
	OffsetTable *oft;
	WDMTable *wdm;

	typedef std::map<IPAddress, DroppableLength> DroppableLengthTable;
	typedef std::pair<IPAddress, DroppableLength> DroppableLengthPair;
	DroppableLengthTable droppableLengthTable;

	simtime_t timeslot;

    virtual void initialize();
    virtual void handleMessage(cMessage *msg);

    virtual void parse();
	virtual void handleSendingBurst(cMessage *msg);
	virtual void handleReceivedBurst(cMessage *msg);
	virtual simtime_t getMaxBursthead(const IPAddress& address);
	virtual simtime_t getMaxBursttail(const IPAddress& address);
	virtual DroppableLength getDroppableLength(const IPAddress& address);
};

#endif
