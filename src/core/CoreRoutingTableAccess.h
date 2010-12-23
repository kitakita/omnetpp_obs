/*
 * CoreRoutingTableAccess.h
 *
 *  Created on: 2010/12/19
 *      Author: kitakita
 */

#ifndef COREROUTINGTABLEACCESS_H_
#define COREROUTINGTABLEACCESS_H_

#include <omnetpp.h>
#include "ModuleAccess.h"
#include "CoreRoutingTable.h"

class CoreRoutingTableAccess : public ModuleAccess<CoreRoutingTable>
{
  public:
	CoreRoutingTableAccess() : ModuleAccess<CoreRoutingTable>("coreRoutingTable") {}
};

#endif /* COREROUTINGTABLEACCESS_H_ */
