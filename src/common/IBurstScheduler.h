#ifndef IBURSTSCHEDULER_H_
#define IBURSTSCHEDULER_H_

#include <omnetpp.h>

class IBurstScheduler
{
  public:
	virtual int schedule(int port, simtime_t arrivalTime, simtime_t length) = 0;
	virtual int schedule(int port, int channel, simtime_t arrivalTime, simtime_t length) = 0;
};

#endif /* IBURSTSCHEDULER_H_ */
