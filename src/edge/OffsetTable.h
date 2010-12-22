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

#ifndef __OMNETPP_OBS_OFFSETTABLE_H_
#define __OMNETPP_OBS_OFFSETTABLE_H_

#include <omnetpp.h>
#include "IPAddress.h"

/**
 * TODO - Generated class
 */
class OffsetTable : public cSimpleModule
{
  protected:
	typedef std::map<IPAddress, simtime_t> ProcessingDelayTable;
	typedef std::pair<IPAddress, simtime_t> ProcessingDelay;
	ProcessingDelayTable delayTable;

	typedef std::map<IPAddress, int> HopsTable;
	typedef std::pair<IPAddress, int> Hops;
	HopsTable hopsTable;

    virtual void initialize();
    virtual void handleMessage(cMessage *msg);

    virtual simtime_t calcProcessingDelay(int hops);

  public:
    virtual simtime_t getOffset(const IPAddress& dest);
    virtual int getHops(const IPAddress& dest);
};

#endif
