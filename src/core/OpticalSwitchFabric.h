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

#ifndef __TSOBS_OPTICALSWITCHFABRIC_H_
#define __TSOBS_OPTICALSWITCHFABRIC_H_

#include <omnetpp.h>

/**
 * TODO - Generated class
 */
class OpticalSwitchFabric : public cSimpleModule
{
  protected:
	int numDataChannels;
	int numLinkedNodes;
	std::vector<int> connectionTable;
	std::vector<simtime_t> transmissionDelay;
	int dropCounter;

    virtual void initialize();
    virtual void handleMessage(cMessage *msg);

    virtual void printSwitch();

  public:
    virtual void connect(int inNode, int in, int outNode, int out);
    virtual void disconnect(int inNode, int in);
    simtime_t getTransmissionDelay(int receiverNodeId);
};

#endif
