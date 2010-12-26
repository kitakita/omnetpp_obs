#ifndef IBURSTSCHEDULER_H_
#define IBURSTSCHEDULER_H_

#include <omnetpp.h>

class IBurstScheduler
{
  public:
	virtual int schedule(cMessage *msg, int port) = 0;
};

#endif /* IBURSTSCHEDULER_H_ */
