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

#include "BurstControlPacket.h"

BurstControlPacket::BurstControlPacket(const char *name = NULL) : BurstControlPacket_Base(name)
{
	initialize();
}

BurstControlPacket::BurstControlPacket(const BurstControlPacket& other) : BurstControlPacket_Base(other)
{
	initialize();
}

BurstControlPacket::~BurstControlPacket()
{
	bst = NULL;
}

void BurstControlPacket::initialize()
{
	bst = NULL;
}

void BurstControlPacket::setBurst(cMessage *msg)
{
	bst = msg;
}

cMessage *BurstControlPacket::getBurst()
{
	return bst;
}
