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

#ifndef IBURST_H_
#define IBURST_H_

#include <omnetpp.h>
#include "Burst_m.h"

class IBurst : public Burst_Base
{
  public:
	IBurst(const char *name=NULL) : Burst_Base(name) {}
	IBurst(const IBurst& other) : Burst_Base(other) {}
	IBurst& operator=(const IBurst& other) { Burst_Base::operator=(other); return *this; }
	virtual IBurst *dup() { return new IBurst(*this); }

	virtual cPacketQueue *getPacketQueue();
	virtual void setPacketQueue(cPacketQueue *queue);

	virtual int dropPacketsFromFront(int byteLength);
	virtual int dropPacketsFromBack(int byteLength);

	virtual int getNumPackets();
	virtual int64 getBitLength();
	virtual int64 getByteLength();
};

#endif /* IBURST_H_ */
