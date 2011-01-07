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

#include "Burst.h"

Burst::Burst(const char *name = NULL) : Burst_Base(name)
{
	initialize();
}

Burst::Burst(const Burst& other) : Burst_Base(other)
{
	initialize();
}

Burst::~Burst()
{
	while (!packets->empty())
		delete packets->pop();
	delete packets;
}

void Burst::initialize()
{
	packets = NULL;
}

int Burst::dropPacketsFromFront(int dropByteLength)
{
	int droppedByteLength = 0;
	while (droppedByteLength < dropByteLength)
	{
		if (packets->empty())
			break;

		cPacket *pkt = packets->pop();
		droppedByteLength += pkt->getByteLength();
		delete pkt;
	}

	return droppedByteLength;
}

int Burst::dropPacketsFromBack(int dropByteLength)
{
	int droppedByteLength = 0;
	while (droppedByteLength < dropByteLength)
	{
		if (packets->empty())
			break;

		cPacket *pkt = packets->back();
		packets->remove(pkt);
		droppedByteLength += pkt->getByteLength();
		delete pkt;
	}

	return droppedByteLength;
}
