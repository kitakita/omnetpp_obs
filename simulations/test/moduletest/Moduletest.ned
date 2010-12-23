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

package omnetpp_obs.simulations.test.moduletest;

import inet.base.Sink;
import omnetpp_obs.edge.EdgeNode;
import omnetpp_obs.common.BurstScheduler;
import omnetpp_obs.edge.PacketGenerator;
import omnetpp_obs.edge.TimeSlicedDispatcher;
import omnetpp_obs.edge.PacketSink;
import omnetpp_obs.edge.PacketClassifier;
import omnetpp_obs.edge.OffsetTable;
import omnetpp_obs.edge.MixedTimerBurstlengthBasedQueue;
import omnetpp_obs.common.HorizonScheduler;
import omnetpp_obs.edge.BurstDisassembler;


//
// TODO Auto-generated network
//
network Moduletest
{
    submodules:
        edgeNode: EdgeNode {
            @display("p=149,101");
        }
    connections allowunconnected:
}
