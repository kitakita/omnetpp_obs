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
#include "BurstControlPacket.h"
#include "CoreRoutingTableAccess.h"
#include "BurstSchedulerAccess.h"
#include "OpticalSwitchFabricAccess.h"
#include "ConnectionEvent_m.h"

Define_Module(ControlUnit);

void ControlUnit::initialize()
{
	crt = CoreRoutingTableAccess().get();
	scd = BurstSchedulerAccess().get();
	osf = OpticalSwitchFabricAccess().get();

	simtime_t bcpProcessDelay = par("bcpProcessDelay");
	simtime_t oeConvertDelay = par("oeConvertDelay");
	processDelay = bcpProcessDelay + (oeConvertDelay * 2);
	guardtime = SimTime::parse("0.00000000001");
	failedCounter = 0;
	parent = getParentModule();
	WATCH(failedCounter);
}

void ControlUnit::handleMessage(cMessage *msg)
{
	if (msg->isSelfMessage())
		handleSelfEvent(msg);
	else
		handleBurstControlPacket(msg);
}

void ControlUnit::handleSelfEvent(cMessage *msg)
{
	ConnectionEvent *cue = check_and_cast<ConnectionEvent *>(msg);

	if (cue->getKind() == 0)
		osf->connect(cue->getInPort(), cue->getInChannel(), cue->getOutPort(), cue->getOutChannel());
	else
		osf->disconnect(cue->getInPort(), cue->getInChannel());

	delete msg;
}

void ControlUnit::handleBurstControlPacket(cMessage *msg)
{
	BurstControlPacket *bcp = check_and_cast<BurstControlPacket *>(msg);

	int inPort = bcp->getBurstIngressPort();
	int inChannel = bcp->getBurstIngressChannel();
	int outPort = crt->getSendPort(bcp->getDestAddresss());
	int outChannel = scd->schedule(outPort, bcp->getBurstArrivalTime(), bcp->getBurstlength());

	if (outChannel < 0)
	{
		ev << bcp << " schedule failed." << endl;
		delete msg;
		failedCounter++;
		if (ev.isGUI())
		{
			char buf[32];
		    sprintf(buf, "drop: %d bursts", failedCounter);
			getDisplayString().setTagArg("t", 0, buf);
		}
	}
	else
	{
		ev << bcp << " schedule success." << endl;

		ConnectionEvent *connect = new ConnectionEvent("ControlUnitEvent");
		connect->setKind(0);
		connect->setInPort(inPort);
		connect->setInChannel(inChannel);
		connect->setOutPort(outPort);
		connect->setOutChannel(outChannel);
		scheduleAt(bcp->getBurstArrivalTime() - guardtime, connect);

		ConnectionEvent *disconnect = new ConnectionEvent("ControlUnitEvent");
		disconnect->setKind(1);
		disconnect->setInPort(inPort);
		disconnect->setInChannel(inChannel);
		scheduleAt(bcp->getBurstArrivalTime() + bcp->getBurstlength(), disconnect);

		ev << "Burst sending schedule at " << bcp->getBurstArrivalTime() << " to " << bcp->getBurstArrivalTime() + bcp->getBurstlength() << endl;

		bcp->setBurstArrivalTime(bcp->getBurstArrivalTime() + osf->getTransmissionDelay(outPort));
		bcp->setBurstIngressPort(crt->getReceivePort(bcp->getDestAddresss()));
		bcp->setBurstIngressChannel(outChannel);

		sendDelayed(bcp, processDelay, "bcpg$o", outPort);
	}
}
