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

#include <fstream>
#include "AnsyncSlotTable.h"
#include "IPAddressResolver.h"

Define_Module(AnsyncSlotTable);

void AnsyncSlotTable::initialize()
{
//	const char *destAddrs = par("ansyncSlotTableFile");
//
//	std::ifstream ifs(destAddrs);
//	std::string line;
//	while (getline(ifs, line) != NULL)
//	{
//		cStringTokenizer tokenizer(line.c_str());
//		IPAddress dest = IPAddressResolver().resolve(tokenizer.nextToken()).get4();
//		int ensure = atoi(tokenizer.nextToken());
//		ensureTable.insert(EnsureLength(dest, ensure * 8));
//		int offset = atoi(tokenizer.nextToken());
//		offsetTable.insert(Offset(dest, offset * 8));
//	}
//	ifs.close();
}

void AnsyncSlotTable::handleMessage(cMessage *msg)
{
//	opp_error("This module cannot to receive any messages.");
//	delete msg;
}

//int AnsyncSlotTable::getEnsureBitLength(const IPAddress& dest)
//{
//	EnsureLengthTable::iterator it = ensureTable.find(dest);
//	if (it != ensureTable.end())
//		return it->second;
//	else
//	{
//		opp_error("%s not found in this ensure length table.", dest.str().c_str());
//		return -1;
//	}
//}
//
//int AnsyncSlotTable::getEnsureByteLength(const IPAddress& dest)
//{
//	EnsureLengthTable::iterator it = ensureTable.find(dest);
//	if (it != ensureTable.end())
//		return it->second * 8;
//	else
//	{
//		opp_error("%s not found in this ensure length table.", dest.str().c_str());
//		return -1;
//	}
//}
//
//int AnsyncSlotTable::getBitOffset(const IPAddress& dest)
//{
//	OffsetTable::iterator it = offsetTable.find(dest);
//	if (it != offsetTable.end())
//		return it->second;
//	else
//	{
//		opp_error("%s not found in this slot offset table.", dest.str().c_str());
//		return -1;
//	}
//}
