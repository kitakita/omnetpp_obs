//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef __OMNETPP_OBS_ANSYNCSLOTTABLE_H_
#define __OMNETPP_OBS_ANSYNCSLOTTABLE_H_

#include <omnetpp.h>
#include "IPAddress.h"

/**
 * TODO - Generated class
 */
class AnsyncSlotTable : public cSimpleModule
{
  protected:
	typedef std::map<IPAddress, int> EnsureLengthTable;
	typedef std::pair<IPAddress, int> EnsureLength;
	EnsureLengthTable ensureTable;

	typedef std::map<IPAddress, int> OffsetTable;
	typedef std::pair<IPAddress, int> Offset;
	OffsetTable offsetTable;

    virtual void initialize();
    virtual void handleMessage(cMessage *msg);

  public:
    virtual int getBitOffset(const IPAddress& dest);
    virtual int getEnsureBitLength(const IPAddress& dest);
};

#endif
