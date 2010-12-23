/*
 * AnsyncSlotTableAccess.h
 *
 *  Created on: 2010/12/23
 *      Author: kitakita
 */

#ifndef ANSYNCSLOTTABLEACCESS_H_
#define ANSYNCSLOTTABLEACCESS_H_

#include <omnetpp.h>
#include "ModuleAccess.h"
#include "AnsyncSlotTable.h"

class AnsyncSlotTableAccess : public ModuleAccess<AnsyncSlotTable>
{
  public:
	AnsyncSlotTableAccess() : ModuleAccess<AnsyncSlotTable>("ansyncSlotTable") {}
};


#endif /* ANSYNCSLOTTABLEACCESS_H_ */
