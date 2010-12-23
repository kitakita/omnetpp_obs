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
#include "Schedule.h"

class HorizonScheduler : public cSimpleModule, public IBurstScheduler
{
  protected:
	bool droppable;
	bool waveConversion;

	typedef std::vector<Schedule *> ScheduleTable;
	typedef std::vector<ScheduleTable> ScheduleTables;
	ScheduleTables scheduleTables;

	virtual void initialize();
	virtual void handleMessage(cMessage *msg);
	virtual void finish();

	virtual void printSchedule(int outPort);
	virtual void updateDisplayString();
	virtual ScheduleResult getScheduleResult(int port, simtime_t arrivalTime);

  public:
	virtual ScheduleResult schedule(int port, cMessage *msg);
};

#endif
