/*
 * BurstSchedulerAccess.h
 *
 *  Created on: 2010/12/22
 *      Author: kitakita
 */

#ifndef BURSTSCHEDULERACCESS_H_
#define BURSTSCHEDULERACCESS_H_

#include <omnetpp.h>
#include "ModuleAccess.h"
#include "IBurstScheduler.h"

class BurstSchedulerAccess : public ModuleAccess<IBurstScheduler>
{
  public:
	BurstSchedulerAccess() : ModuleAccess<IBurstScheduler>("scheduler") {}
};

#endif /* BURSTSCHEDULERACCESS_H_ */
