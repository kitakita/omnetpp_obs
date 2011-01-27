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

void Burst::dropHead(int dropByteLength)
{
	if (getHead() > dropByteLength)
		opp_error("%s try to drop head. But dropByteLength (%d [byte]) lager than head (%d [byte]).", getName(), dropByteLength, getHead());
	else
	{
		int needDrop = dropByteLength - getHead() + getRestHead();
		if (needDrop > 0)
		{
			int dropped = 0;
			while (dropped < needDrop)
			{
				cPacket *pkt = packets->pop();

				if (pkt == NULL)
					opp_error("%s try to drop head. But cPacketQueue was empty.", getName());

				dropped += pkt->getByteLength();
				delete pkt;
			}
			setRestHead(getRestHead() - dropped);
		}
	}
}

void Burst::dropTail(int dropByteLength)
{
	if (getTail() > dropByteLength)
		opp_error("%s try to drop tail. But dropByteLength (%d [byte]) lager than tail (%d [byte]).", getName(), dropByteLength, getTail());
	else
	{
		int needDrop = dropByteLength - getTail() + getRestTail();
		if (needDrop > 0)
		{
			int dropped = 0;
			while (dropped < needDrop)
			{
				cPacket *pkt = packets->back();

				if (pkt == NULL)
					opp_error("%s try to drop tail. But cPacketQueue was empty.", getName());

				pkt = packets->remove(pkt);

				if (pkt == NULL)
					opp_error("%s try to remove a packet. But this packet dose not exist.", getName());

				dropped += pkt->getByteLength();
				delete pkt;
			}
			setRestTail(getRestTail() - dropped);
		}
	}
}
