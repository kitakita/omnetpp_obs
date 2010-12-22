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

void Burst::setPacketQueue(cPacketQueue *queue)
{
	packets = queue;
	if (packets->getByteLength() > getMaxByteLength())
		dropPacketsFromBack(getMaxByteLength());
}

int Burst::dropPacketsFromFront(int dropByteLength)
{
	int dropedByteLength = 0;
	while (dropedByteLength <= dropByteLength)
	{
		if (packets->empty())
			break;

		cPacket *pkt = packets->pop();
		dropedByteLength += pkt->getByteLength();
		delete pkt;
	}

	return dropedByteLength - dropByteLength;
}

int Burst::dropPacketsFromBack(int byteLength)
{

}
