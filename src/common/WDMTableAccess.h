/*
 * WDMTableAccess.h
 *
 *  Created on: 2010/12/23
 *      Author: kitakita
 */

#ifndef WDMTABLEACCESS_H_
#define WDMTABLEACCESS_H_

#include <omnetpp.h>
#include "ModuleAccess.h"
#include "WDMTable.h"

class WDMTableAccess : public ModuleAccess<WDMTable>
{
  public:
	WDMTableAccess() : ModuleAccess<WDMTable>("wdmTable") {}
};

#endif /* WDMTABLEACCESS_H_ */
