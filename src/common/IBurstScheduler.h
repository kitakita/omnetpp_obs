#ifndef IBURSTSCHEDULER_H_
#define IBURSTSCHEDULER_H_

#include <omnetpp.h>

struct ScheduleResult
{
    simtime_t offset;
    int channel;
    bool dropped;
};

class IBurstScheduler
{
  public:
	virtual ScheduleResult schedule(int port, cMessage *msg) = 0;
};

#endif /* IBURSTSCHEDULER_H_ */
