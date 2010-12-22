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

class IBurst
{
  public:
	virtual ~IBurst() {};

	virtual cPacketQueue *getPacketQueue() = 0;
	virtual void setPacketQueue(cPacketQueue *queue) = 0;

	virtual int dropPacketsFromFront(int byteLength) = 0;
	virtual int dropPacketsFromBack(int byteLength) = 0;

	virtual int getNumPackets() const = 0;
	virtual int64 getBitLength() const = 0;
	virtual int64 getByteLength() const = 0;
};

#endif /* IBURST_H_ */
