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

package omnetpp_obs.simulations.test.twoedges;

import ned.DelayChannel;
import omnetpp_obs.edge.EdgeNode;

//
// TODO Auto-generated network
//
network TwoEdges
{
    parameters:
        int numDataChannels;
        @display("bgl=2");
    types:
        channel C extends DelayChannel
        {
            delay = 0.5ms;
        }
    submodules:
        edge0: EdgeNode {
            parameters:
                numDataChannels = numDataChannels;
                @display("p=70,90");
        }
        edge1: EdgeNode {
            parameters:
                numDataChannels = numDataChannels;
                @display("p=250,90");
        }
    connections:
        edge0.bcpg <--> C <--> edge1.bcpg;
        for i=0..numDataChannels-1 {
            edge0.burstg[i] <--> C <--> edge1.burstg[i];
        }
}
