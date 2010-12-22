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

#ifndef BURST_H_
#define BURST_H_

#include <omnetpp.h>
#include "Burst_m.h"

class Burst : public Burst_Base
{
  protected:
	cPacketQueue *packets;

	virtual void initialize();

  public:
	Burst(const char *name);
	Burst(const Burst& other);
	virtual ~Burst();
	Burst& operator=(const Burst& other) { Burst_Base::operator=(other); return *this; }
	virtual Burst *dup() { return new Burst(*this); }

	cPacketQueue *getPacketQueue() { return packets; }
	virtual void setPacketQueue(cPacketQueue *queue);

	virtual int dropPacketsFromFront(int byteLength);
	virtual int dropPacketsFromBack(int byteLength);

	int getNumPackets() { return packets->length(); }
	int64 getBitLength() { return packets->getByteLength(); }
	int64 getByteLength() { return packets->getBitLength(); }
};

#endif /* BURST_H_ */
