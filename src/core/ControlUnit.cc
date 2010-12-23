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
	else if (dynamic_cast<BurstControlPacket *>(msg) != NULL)
		handleBurstControlPacket(msg);
	else if (dynamic_cast<ConnectionEvent *>(msg) != NULL)
		handleSelfEvent(msg);
	else
		opp_error("ControlUnit receive unknown message.");
}

void ControlUnit::handleSelfEvent(cMessage *msg)
{
	ConnectionEvent *cue = check_and_cast<ConnectionEvent *>(msg);

	if (cue->getKind() == 0)
		osf->connect(cue->getIn(), cue->getOut());
	else
		osf->disconnect(cue->getIn());

	delete msg;
}

void ControlUnit::handleBurstControlPacket(cMessage *msg)
{
	BurstControlPacket *bcp = check_and_cast<BurstControlPacket *>(msg);

	int inPort = bcp->getBurstIngressPort();
	int inChannel = bcp->getBurstIngressChannel();
	int outPort = crt->getSendPort(bcp->getDestAddress());
	ScheduleResult res = scd->schedule(outPort, bcp);
	int outChannel = res.channel;

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

		ConnectionEvent *connect = new ConnectionEvent("ConnectionEvent");
		connect->setIn(inGateIndex);
		connect->setOut(outGateIndex);
		scheduleAt(bcp->getBurstArrivalTime() - guardtime, connect);

		ev << "Burst sending schedule at " << bcp->getBurstArrivalTime() << " to " << bcp->getBurstArrivalTime() + bcp->getBurstlength() << endl;

		bcp->setBurstArrivalTime(bcp->getBurstArrivalTime() + wdm->getTransmissionDelay(outPort));
		bcp->setBurstIngressPort(crt->getReceivePort(bcp->getDestAddress()));
		bcp->setBurstIngressChannel(outChannel);

		sendDelayed(bcp, processDelay, "bcpg$o", outPort);
	}

	if (ev.isGUI())
	{
		char buf[32];
		sprintf(buf, "drop: %d bursts", failedCounter);
		getDisplayString().setTagArg("t", 0, buf);
	}
}
