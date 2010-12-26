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

package omnetpp_obs.simulations.ansync;

import ned.DelayChannel;
import omnetpp_obs.core.CoreNode;
import omnetpp_obs.edge.EdgeNode;


network AnsyncNetwork
{
    parameters:
        int numDataChannels;
        @display("bgl=25");
    types:
        channel IC extends DelayChannel
        {
            delay = 0;
        }
        channel C01 extends DelayChannel
        {
            delay = 0.5ms;
        }
        channel C02 extends DelayChannel
        {
            delay = 0.4ms;
        }
        channel C12 extends DelayChannel
        {
            delay = 0.45ms;
        }
        channel C13 extends DelayChannel
        {
            delay = 0.425ms;
        }
        channel C23 extends DelayChannel
        {
            delay = 0.3ms;
        }
        channel C24 extends DelayChannel
        {
            delay = 0.35ms;
        }
        channel C34 extends DelayChannel
        {
            delay = 0.25ms;
        }
    submodules:
        edge0: EdgeNode {
            parameters:
                numDataChannels = numDataChannels;
                @display("p=167,440");
        }
        core0: CoreNode {
            parameters:
                numDataChannels = numDataChannels;
                numLinkedNodes = 3;
                @display("p=247,351");
            gates:
                bcpg[3];
                burstg[numDataChannels*3];
        }
        edge1: EdgeNode {
            parameters:
                numDataChannels = numDataChannels;
                @display("p=136,152");
        }
        core1: CoreNode {
            parameters:
                numDataChannels = numDataChannels;
                numLinkedNodes = 4;
                @display("p=247,191");
            gates:
                bcpg[4];
                burstg[numDataChannels*4];
        }
        edge2: EdgeNode {
            parameters:
                numDataChannels = numDataChannels;
                @display("p=475,351");
        }
        core2: CoreNode {
            parameters:
                numDataChannels = numDataChannels;
                numLinkedNodes = 5;
                @display("p=385,280");
            gates:
                bcpg[5];
                burstg[numDataChannels*5];
        }
        edge3: EdgeNode {
            parameters:
                numDataChannels = numDataChannels;
                @display("p=424,94");
        }
        core3: CoreNode {
            parameters:
                numDataChannels = numDataChannels;
                numLinkedNodes = 4;
                @display("p=403,191");
            gates:
                bcpg[4];
                burstg[numDataChannels*4];
        }
        edge4: EdgeNode {
            parameters:
                numDataChannels = numDataChannels;
                @display("p=610,191");
        }
        core4: CoreNode {
            parameters:
                numDataChannels = numDataChannels;
                numLinkedNodes = 3;
                @display("p=499,230");
            gates:
                bcpg[3];
                burstg[numDataChannels*3];
        }
    connections:
        // connect edges
        edge0.bcpg <--> IC <--> core0.bcpg[0];
        for i=0..numDataChannels-1 {
            edge0.burstg[i] <--> IC <--> core0.burstg[i];
        }

        edge1.bcpg <--> IC <--> core1.bcpg[0];
        for i=0..numDataChannels-1 {
            edge1.burstg[i] <--> IC <--> core1.burstg[i];
        }

        edge2.bcpg <--> IC <--> core2.bcpg[0];
        for i=0..numDataChannels-1 {
            edge2.burstg[i] <--> IC <--> core2.burstg[i];
        }

        edge3.bcpg <--> IC <--> core3.bcpg[0];
        for i=0..numDataChannels-1 {
            edge3.burstg[i] <--> IC <--> core3.burstg[i];
        }

        edge4.bcpg <--> IC <--> core4.bcpg[0];
        for i=0..numDataChannels-1 {
            edge4.burstg[i] <--> IC <--> core4.burstg[i];
        }

		// connect core0 <--> core1 (port 1 <--> 1, index 4-7 <--> 4-7)
        core0.bcpg[1] <--> C01 <--> core1.bcpg[1];
        for i=0..numDataChannels-1 {
            core0.burstg[numDataChannels+i] <--> C01 <--> core1.burstg[numDataChannels+i];
        }

        // connect core0 <--> core2 (port 2 <--> 1, index 8-11 <--> 4-6)
        core0.bcpg[2] <--> C02 <--> core2.bcpg[1];
        for i=0..numDataChannels-1 {
            core0.burstg[numDataChannels*2+i] <--> C02 <--> core2.burstg[numDataChannels+i];
        }

        // connect core1 <--> core2 (port 2 <--> 2, index 8-11 <--> 8-11)
        core1.bcpg[2] <--> C12 <--> core2.bcpg[2];
        for i=0..numDataChannels-1 {
            core1.burstg[numDataChannels*2+i] <--> C12 <--> core2.burstg[numDataChannels*2+i];
        }

        // connect core1 <--> core3 (port 3 <--> 1, index 12-15 <--> 4-7)
        core1.bcpg[3] <--> C13 <--> core3.bcpg[1];
        for i=0..numDataChannels-1 {
            core1.burstg[numDataChannels*3+i] <--> C13 <--> core3.burstg[numDataChannels+i];
        }

        // connect core2 <--> core3 (port 3 <--> 2, index 12-15 <--> 8-11)
        core2.bcpg[3] <--> C23 <--> core3.bcpg[2];
        for i=0..numDataChannels-1 {
            core2.burstg[numDataChannels*3+i] <--> C23 <--> core3.burstg[numDataChannels*2+i];
        }

        // connect core2 <--> core4 (port 4 <--> 1, index 16-19 <--> 4-7)
        core2.bcpg[4] <--> C24 <--> core4.bcpg[1];
        for i=0..numDataChannels-1 {
            core2.burstg[numDataChannels*4+i] <--> C24 <--> core4.burstg[numDataChannels+i];
        }

        // connect core3 <--> core4 (port 3 <--> 2, index 12-15 <--> 8-11)
        core3.bcpg[3] <--> C34 <--> core4.bcpg[2];
        for i=0..numDataChannels-1 {
            core3.burstg[numDataChannels*3+i] <--> C34 <--> core4.burstg[numDataChannels*2+i];
        }
}
