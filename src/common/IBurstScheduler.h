#ifndef IBURSTSCHEDULER_H_
#define IBURSTSCHEDULER_H_

#include <omnetpp.h>

class IBurstScheduler
{
  public:
	virtual int schedule(int outPort, simtime_t arrivalTime, simtime_t length) = 0;
};

#endif /* IBURSTSCHEDULER_H_ */
