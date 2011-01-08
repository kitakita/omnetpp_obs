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

#ifndef __OMNETPP_OBS_PACKETSINK_H_
#define __OMNETPP_OBS_PACKETSINK_H_

#include <omnetpp.h>
#include "IPvXAddress.h"

/**
 * TODO - Generated class
 */
class PacketSink : public cSimpleModule
{
  protected:
	IPvXAddress myAddress;

	int numPackets;
	int64 sumBytes;
	simtime_t sumDelay;
	double packetPerSec;
	double bitPerSec;

    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void finish();
};

#endif
