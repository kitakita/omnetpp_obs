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

#include "EnsureLengthBurst.h"

EnsureLengthBurst::EnsureLengthBurst(const char *name = NULL) : Burst_Base(name)
{
	initialize();
}

EnsureLengthBurst::EnsureLengthBurst(const EnsureLengthBurst& other) : Burst_Base(other)
{
	initialize();
}

EnsureLengthBurst::~EnsureLengthBurst()
{
	finish();
}

void EnsureLengthBurst::initialize()
{
	maxFrontByteLength = 0;
	maxEnsureByteLength = 0;
	maxBackByteLength = 0;

	frontPackets = NULL;
	ensurePackets = NULL;
	backPackets = NULL;
}

void EnsureLengthBurst::finish()
{
	if (frontPackets != NULL)
	{
		while (!frontPackets->empty())
			delete frontPackets->pop();
	}
	if (ensurePackets != NULL)
	{
		while (!ensurePackets->empty())
			delete ensurePackets->pop();
	}
	if (backPackets != NULL)
	{
		while (!backPackets->empty())
			delete backPackets->pop();
	}

	delete frontPackets;
	delete ensurePackets;
	delete backPackets;
}

cPacketQueue *EnsureLengthBurst::getPacketQueue()
{
	return frontPackets;
}

void EnsureLengthBurst::setPacketQueue(cPacketQueue *queue)
{
	finish(); // delete all contain queue and packets

	ensurePackets = queue;
	frontPackets = new cPacketQueue();
	backPackets = new cPacketQueue();

	if (ensurePackets->empty())
		return;

	cPacket *pkt = NULL;
	while (ensurePackets->getByteLength() > maxEnsureByteLength)
	{
		pkt = ensurePackets->pop();
		if (frontPackets->getByteLength() + pkt->getByteLength() > maxFrontByteLength)
			break;
		else
			frontPackets->insert(pkt);
	}
	while (ensurePackets->getByteLength() > maxEnsureByteLength)
	{
		pkt = ensurePackets->pop();
		if (backPackets->getByteLength() + pkt->getByteLength() > maxBackByteLength)
			break;
		else
			backPackets->insert(pkt);
	}
	delete pkt;

	while (ensurePackets->getByteLength() > maxEnsureByteLength)
		delete ensurePackets->pop();
}

int EnsureLengthBurst::dropPacketsFromFront(int dropByteLength)
{
	if ((dropByteLength > maxFrontByteLength) && (dropByteLength > frontPackets->getByteLength()))
		finish();
}

int EnsureLengthBurst::dropPacketsFromBack(int dropByteLength)
{

}

int EnsureLengthBurst::getNumPackets() const
{
	return 0;
}

int64 EnsureLengthBurst::getBitLength() const
{
	return 0;
}

int64 EnsureLengthBurst::getByteLength() const
{
	return 0;
}
