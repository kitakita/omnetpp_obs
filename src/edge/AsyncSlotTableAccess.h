/*
 * AsyncSlotTableAccess.h
 *
 *  Created on: 2010/12/23
 *      Author: kitakita
 */

#ifndef ASYNCSLOTTABLEACCESS_H_
#define ASYNCSLOTTABLEACCESS_H_

#include <omnetpp.h>
#include "ModuleAccess.h"
#include "AsyncSlotTable.h"

class AsyncSlotTableAccess : public ModuleAccess<AsyncSlotTable>
{
  public:
	AsyncSlotTableAccess() : ModuleAccess<AsyncSlotTable>("asyncSlotTable") {}
};


#endif /* ASYNCSLOTTABLEACCESS_H_ */
