/*
 * OpticalSwitchFabricAccess.h
 *
 *  Created on: 2010/12/19
 *      Author: kitakita
 */

#ifndef OPTICALSWITCHFABRICACCESS_H_
#define OPTICALSWITCHFABRICACCESS_H_

#include <omnetpp.h>
#include "ModuleAccess.h"
#include "OpticalSwitchFabric.h"

class OpticalSwitchFabricAccess : public ModuleAccess<OpticalSwitchFabric>
{
  public:
	OpticalSwitchFabricAccess() : ModuleAccess<OpticalSwitchFabric>("opticalSwitchFabric") {}
};

#endif /* OPTICALSWITCHFABRICACCESS_H_ */
