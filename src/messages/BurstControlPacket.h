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

#ifndef BURSTCONTROLPACKET_H_
#define BURSTCONTROLPACKET_H_

#include <omnetpp.h>
#include "BurstControlPacket_m.h"

class BurstControlPacket : public BurstControlPacket_Base
{
  protected:
	cMessage *burst;
	int droppableByteLength;

  public:
	BurstControlPacket(const char *name = NULL) : BurstControlPacket_Base(name) {}
	BurstControlPacket(const BurstControlPacket& other) : BurstControlPacket_Base(other) {}
	BurstControlPacket& operator=(const BurstControlPacket& other) { BurstControlPacket_Base::operator=(other); return *this; }
	virtual BurstControlPacket *dup() { return new BurstControlPacket(*this); }

	cMessage *getBurst() { return burst; };
	void setBurst(cMessage *msg) { burst = msg; }

	int getDroppableBitLength() { return droppableByteLength * 8; }
	void setDroppableBitLength(int bits) { droppableByteLength = bits;  }
	int getDroppableByteLength() { return droppableByteLength; }
	void setDroppableByteLength(int bytes) { droppableByteLength = bytes; }
};

#endif /* BURSTCONTROLPACKET_H_ */
