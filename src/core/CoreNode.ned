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

package omnetpp_obs.core;

import omnetpp_obs.common.HorizonScheduler;

module CoreNode
{
    parameters:
        int numDataChannels;
        int numLinkedNodes;
        string coreRoutingTableFile;
        @display("i=abstract/opticalrouter;bgl=4");
    gates:
        inout bcpg[numLinkedNodes+1];
        inout burstg[numDataChannels*(numLinkedNodes+1)];
    submodules:
        coreRoutingTable: CoreRoutingTable {
            parameters:
                coreRoutingTableFile = coreRoutingTableFile;
                @display("p=260,40");
        }
        scheduler: HorizonScheduler {
            parameters:
                @display("p=60,40");
        }
        opticalSwitchFabric: OpticalSwitchFabric {
            parameters:
                numDataChannels = numDataChannels;
                @display("p=165,170");
            gates:
                burstg[sizeof(burstg)];
        }
        controlUnit: ControlUnit {
            parameters:
                @display("p=165,90");
            gates:
                bcpg[sizeof(bcpg)];
        }
    connections:
        for i=0..sizeof(bcpg)-1 {
            bcpg[i] <--> controlUnit.bcpg[i];
        }
        for i=0..sizeof(burstg)-1 {
            burstg[i] <--> opticalSwitchFabric.burstg[i];
        }
}
