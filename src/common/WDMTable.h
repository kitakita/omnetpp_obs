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

#ifndef __OMNETPP_OBS_WDMTABLE_H_
#define __OMNETPP_OBS_WDMTABLE_H_

#include <omnetpp.h>

/**
 * TODO - Generated class
 */
class WDMTable : public cSimpleModule
{
  protected:
	int numLinkedNodes;

	typedef std::vector<int> NumChannelTable;
	NumChannelTable numChannelTable;

	typedef std::vector<simtime_t> TransmissionDelayTable;
	TransmissionDelayTable transmissionDelayTable;

    virtual void initialize();
    virtual void handleMessage(cMessage *msg);

    virtual void printTables();

  public:
    int getNumLinkedNodes() { return numLinkedNodes; }
    virtual int getGateIndex(int port, int channel);
    simtime_t getTransmissionDelay(int port) { return transmissionDelayTable.at(port); }
};

#endif
