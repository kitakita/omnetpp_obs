/*
 * OffsetTableAccess.h
 *
 *  Created on: 2010/12/22
 *      Author: kitakita
 */

#ifndef OFFSETTABLEACCESS_H_
#define OFFSETTABLEACCESS_H_

#include <omnetpp.h>
#include "ModuleAccess.h"
#include "OffsetTable.h"

class OffsetTableAccess : public ModuleAccess<OffsetTable>
{
  public:
	OffsetTableAccess() : ModuleAccess<OffsetTable>("offsetTable") {}
};

#endif /* OFFSETTABLEACCESS_H_ */
