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

#ifndef SCHEDULE_H_
#define SCHEDULE_H_

#include <omnetpp.h>
#include "Burst.h"

class Schedule {
  protected:
	simtime_t time;
	Burst *bst;
	int burstDroppableByteLength;
	double datarate;

  public:
	Schedule(double rate);
	virtual ~Schedule();

	simtime_t getTime() { return time; }
	void setTime(simtime_t t) { time = t; }

	Burst *getBurst() { return bst; }
	void setBurst(Burst *b) { bst = b; }

	int getBurstDroppableByteLength() { return burstDroppableByteLength; }
	int getBurstDroppableBitLength() { return burstDroppableByteLength * 8; }
	simtime_t getBurstDroppableTimelength() { return (burstDroppableByteLength * 8) / datarate; }
	void setBurstDroppableByteLength(int byte) { burstDroppableByteLength = byte; }
	void setBurstDroppableBitLength(int bit) { burstDroppableByteLength = bit / 8; }

	double getDatarate() { return datarate; }
	void setDatarate(double rate) { datarate = rate; }
};

#endif /* SCHEDULE_H_ */
