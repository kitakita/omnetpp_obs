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

package omnetpp_obs.simulations.test.twocores;

import ned.DelayChannel;
import omnetpp_obs.core.CoreNode;
import omnetpp_obs.edge.EdgeNode;

//
// TODO Auto-generated network
//
network Twocores
{
    parameters:
        int numDataChannels;
        int numLinkedNodes = 2;
    	@display("bgl=2");
    types:
        channel IC extends DelayChannel {
            delay = 0;
        }
        channel C extends DelayChannel {
            delay = 0.5ms;
        }
    submodules:
        edge0: EdgeNode {
            parameters:
                numDataChannels = numDataChannels;
            	@display("p=50,50");
        }
        edge1: EdgeNode {
            parameters:
                numDataChannels = numDataChannels;
            	@display("p=400,50");
        }
        core0: CoreNode {
            parameters:
                numDataChannels = numDataChannels;
                numLinkedNodes = numLinkedNodes;
            	@display("p=150,50");
            gates:
                bcpg[2];
                burstg[numDataChannels*2];
        }
        core1: CoreNode {
            parameters:
                numDataChannels = numDataChannels;
                numLinkedNodes = numLinkedNodes;
            	@display("p=300,50");
            gates:
                bcpg[2];
                burstg[numDataChannels*2];
        }
    connections:
        edge0.bcpg <--> IC <--> core0.bcpg[0];
        for i=0..numDataChannels-1 {
            edge0.burstg[i] <--> IC <--> core0.burstg[i];
        }
        
        edge1.bcpg <--> IC <--> core1.bcpg[0];
        for i=0..numDataChannels-1 {
            edge1.burstg[i] <--> IC <--> core1.burstg[i];
        }
        
        core0.bcpg[1] <--> C <--> core1.bcpg[1];
        for i=0..numDataChannels-1 {
            core0.burstg[numDataChannels+i] <--> C <--> core1.burstg[numDataChannels+i];
        }
}
