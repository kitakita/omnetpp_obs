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

#ifndef __OMNETPP_OBS_HORIZONSCHEDULER_H_
#define __OMNETPP_OBS_HORIZONSCHEDULER_H_

#include <omnetpp.h>
#include "IBurstScheduler.h"

struct OffsetAndChannel
{
    simtime_t offset;
    int channel;
};

class HorizonScheduler : public cSimpleModule, public IBurstScheduler
{
  protected:
	typedef std::vector<simtime_t> HorizonSchedules;
	typedef std::vector<HorizonSchedules> ScheduleTables;
	ScheduleTables scheduleTables;

	virtual void initialize();
	virtual void handleMessage(cMessage *msg);

	virtual OffsetAndChannel getMinFrontOffsetAndChannel(int port, simtime_t arrivalTime);
	virtual void printSchedule(int outPort);
	virtual void updateDisplayString();

  public:
	virtual int schedule(int port, simtime_t arrivalTime, simtime_t length);
	virtual int schedule(int port, int channel, simtime_t arrivalTime, simtime_t length);
};

#endif
