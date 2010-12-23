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

#ifndef __TSOBS_CONTROLUNIT_H_
#define __TSOBS_CONTROLUNIT_H_

#include <omnetpp.h>
#include "CoreRoutingTable.h"
#include "IBurstScheduler.h"
#include "OpticalSwitchFabric.h"

/**
 * TODO - Generated class
 */
class ControlUnit : public cSimpleModule
{
  protected:
	CoreRoutingTable *crt;
	IBurstScheduler *scd;
	OpticalSwitchFabric *osf;
	int numDataChannels;
	int numLinkedNodes;
	simtime_t processDelay;
	simtime_t guardtime;
	cModule *parent;
	int failedCounter;

    virtual void initialize();
    virtual void handleMessage(cMessage *msg);

    virtual void handleSelfEvent(cMessage *msg);
    virtual void handleBurstControlPacket(cMessage *msg);
};

#endif
