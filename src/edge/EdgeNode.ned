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

package omnetpp_obs.edge;

import omnetpp_obs.common.BurstScheduler;

//
// TODO auto-generated type
//
module EdgeNode
{
    parameters:
        string myAddress;
        string destAddresses;
        string offsetTableFile;
        string dispatcherType;
        string schedulerType;
        int numDestinations;
        int numDataChannels;
        @display("i=abstract/opticalswitch;bgl=4");
    gates:
        inout bcpg;
        inout burstg[numDataChannels];
    submodules:
        generator: PacketGenerator {
            parameters:
                myAddress = myAddress;
                destAddresses = destAddresses;
                @display("p=50,190");
        }
        classifier: PacketClassifier {
            @display("p=135,190");
        }
        queue[numDestinations]: MixedTimerBurstlengthBasedQueue {
            @display("p=250,190,column,80;q=queue");
        }
        dispatcher: <dispatcherType> like BurstDispatcher {
            @display("p=365,190");
            gates:
                burstg[numDataChannels];
        }
        offsetTable: OffsetTable {
            parameters:
                offsetTableFile = offsetTableFile;
                @display("p=365,50");
        }
        disassembler: BurstDisassembler {
            parameters:
                myAddress = myAddress;
                @display("p=250,120");
        }
        sink: PacketSink {
            parameters:
                myAddress = myAddress;
                @display("p=50,120");
        }
        scheduler: <schedulerType> like BurstScheduler {
            @display("p=250,50");
        }
        ansyncSlotTable: AnsyncSlotTable {
            @display("p=135,50");
        }
    connections:
        generator.out --> classifier.in;
        for i=0..numDestinations-1 {
            queue[i].in <-- classifier.out++;
            queue[i].out --> dispatcher.in++;
        }

        dispatcher.bcpg <--> bcpg;
        for i=0..numDataChannels-1 {
            dispatcher.burstg[i] <--> burstg[i];
        }

        disassembler.in <-- dispatcher.out;
        disassembler.out --> sink.in++;
}
