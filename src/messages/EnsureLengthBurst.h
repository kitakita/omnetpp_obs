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

#ifndef ENSURELENGTHBURST_H_
#define ENSURELENGTHBURST_H_

#include <omnetpp.h>
#include "Burst_m.h"
#include "IBurst.h"

class EnsureLengthBurst : public Burst_Base, public IBurst
{
  protected:
	int maxFrontByteLength;
	int maxEnsureByteLength;
	int maxBackByteLength;

	cPacketQueue *frontPackets;
	cPacketQueue *ensurePackets;
	cPacketQueue *backPackets;

	virtual void initialize();
	virtual void finish();

  public:
	EnsureLengthBurst(const char *name);
	EnsureLengthBurst(const EnsureLengthBurst& other);
	virtual ~EnsureLengthBurst();
	EnsureLengthBurst& operator=(const EnsureLengthBurst& other) { Burst_Base::operator=(other); return *this; }
	virtual EnsureLengthBurst *dup() { return new EnsureLengthBurst(*this); }

	virtual cPacketQueue *getPacketQueue();
	virtual void setPacketQueue(cPacketQueue *queue);

	virtual int dropPacketsFromFront(int byteLength);
	virtual int dropPacketsFromBack(int byteLength);

	virtual int getNumPackets() const;
	virtual int64 getBitLength() const;
	virtual int64 getByteLength() const;

	void setFrontPackets(cPacketQueue *queue) { frontPackets = queue; }
	cPacketQueue *getFrontPackets() { return frontPackets; }
	void setEnsurePackets(cPacketQueue *queue) { ensurePackets = queue; }
	cPacketQueue *getEnsurePackets() { return ensurePackets; }
	void setBackPackets(cPacketQueue *queue) { backPackets = queue; }
	cPacketQueue *getBackPackets() { return backPackets; }

	int getNumFrontPackets() { return frontPackets->length(); }
	int64 getFrontPacketsBitLength() { return frontPackets->getBitLength(); }
	int64 getFrontPacketsByteLength() { return frontPackets->getByteLength(); }

	int getNumEnsurePackets() { return ensurePackets->length(); }
	int64 getEnsurePacketsBitLength() { return ensurePackets->getBitLength(); }
	int64 getEnsurePacketsByteLength() { return ensurePackets->getByteLength(); }

	int getNumBackPackets() { return backPackets->length(); }
	int64 getBackPacketsBitLength() { return backPackets->getBitLength(); }
	int64 getBackPacketsByteLength() { return backPackets->getByteLength(); }
};

#endif /* ENSURELENGTHBURST_H_ */
