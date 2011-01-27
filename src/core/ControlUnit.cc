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

#include "ControlUnit.h"
#include "Burst.h"
#include "BurstControlPacket.h"
#include "CoreRoutingTableAccess.h"
#include "BurstSchedulerAccess.h"
#include "OpticalSwitchFabricAccess.h"
#include "WDMTableAccess.h"

Define_Module(ControlUnit);

void ControlUnit::initialize()
{
	crt = CoreRoutingTableAccess().get();
	scd = BurstSchedulerAccess().get();
	osf = OpticalSwitchFabricAccess().get();
	wdm = WDMTableAccess().get();

	simtime_t bcpProcessDelay = par("bcpProcessDelay");
	simtime_t oeConvertDelay = par("oeConvertDelay");
	processDelay = bcpProcessDelay + (oeConvertDelay * 2);
	guardtime = SimTime::parse("0.00000000001");

	failedCounter = 0;

	WATCH(failedCounter);
}

void ControlUnit::handleMessage(cMessage *msg)
{
	if (msg->isSelfMessage())
		handleSelfEvent(msg);
	else
		handleBurstControlPacket(msg);
}

void ControlUnit::finish()
{
	recordScalar("ControlUnit-failedCounter", failedCounter);
}

void ControlUnit::handleSelfEvent(cMessage *msg)
{
	ConnectionEvent *cue = check_and_cast<ConnectionEvent *>(msg);

	ev << "connect port " << cue->getIn() << " to port " << cue->getOut() << endl;

	osf->connect(cue->getIn(), cue->getOut());

	delete msg;
}

void ControlUnit::handleBurstControlPacket(cMessage *msg)
{
	printBCP(msg);

	BurstControlPacket *bcp = check_and_cast<BurstControlPacket *>(msg);

	int inPort = bcp->getBurstPort();
	int inChannel = bcp->getBurstChannel();
	int outPort = crt->getSendPort(bcp->getDestAddress());
	int outChannel = scd->schedule(bcp, outPort);

	if (outChannel < 0)
	{
		failedCounter++;

		ev << bcp << " schedule failed." << endl;

		delete msg;
	}
	else
	{
		ev << bcp << " schedule success." << endl;

		int inGateIndex = wdm->getGateIndex(inPort, inChannel);
		int outGateIndex = wdm->getGateIndex(outPort, outChannel);

		char msgName[48];
		sprintf(msgName, "%s-connect", bcp->getName());
		ConnectionEvent *connect = new ConnectionEvent(msgName);
		connect->setIn(inGateIndex);
		connect->setOut(outGateIndex);
		scheduleAt(bcp->getBurstArrivalTime() - guardtime, connect);

		ev << "Burst sending schedule at " << bcp->getBurstArrivalTime() << " to " << bcp->getBurstArrivalTime() + bcp->getBurstlength() << endl;

		bcp->setBurstArrivalTime(bcp->getBurstArrivalTime() + wdm->getTransmissionDelay(outPort));
		bcp->setBurstPort(crt->getReceivePort(bcp->getDestAddress()));
		bcp->setBurstChannel(outChannel);

		sendDelayed(bcp, processDelay, "bcpg$o", outPort);
	}

	if (ev.isGUI())
	{
		char buf[32];
		sprintf(buf, "drop: %d bursts", failedCounter);
		getDisplayString().setTagArg("t", 0, buf);
	}
}

void ControlUnit::printBCP(cMessage *msg)
{
	BurstControlPacket *bcp = check_and_cast<BurstControlPacket *>(msg);

	ev << bcp->getName()
	<< " | Src: " << bcp->getSrcAddress().str().c_str()
	<< " Dest: " << bcp->getDestAddress().str().c_str()
	<< " BurstArrivalTime: " << bcp->getBurstArrivalTime()
	<< " Burstlength: " << bcp->getBurstlength()
	<< " BurstPort: " << bcp->getBurstPort()
	<< " BurstChannel: " << bcp->getBurstChannel()
	<< " Bursthead: " << bcp->getBursthead()
	<< " Bursttail: " << bcp->getBursttail() << endl;
}
