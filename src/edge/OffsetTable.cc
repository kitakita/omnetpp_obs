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
#include "OffsetTable.h"
#include "IPAddressResolver.h"

Define_Module(OffsetTable);

void OffsetTable::initialize()
{
	const char *destAddrs = par("offsetTableFile");

	std::ifstream ifs(destAddrs);
	std::string line;
	while (getline(ifs, line) != NULL)
	{
		cStringTokenizer tokenizer(line.c_str());
		IPAddress dest = IPAddressResolver().resolve(tokenizer.nextToken()).get4();
		int hops = atoi(tokenizer.nextToken());
		delayTable.insert(ProcessingDelay(dest, calcProcessingDelay(hops)));
		hopsTable.insert(Hops(dest, hops));
	}
	ifs.close();
}

void OffsetTable::handleMessage(cMessage *msg)
{
	opp_error("This module cannot to receive any messages.");
	delete msg;
}

simtime_t OffsetTable::calcProcessingDelay(int hops)
{
	simtime_t bcpProcessDelay = par("bcpProcessDelay");
	simtime_t oeConvertDelay = par("oeConvertDelay");

	return (bcpProcessDelay + (oeConvertDelay * 2)) * hops;
}

simtime_t OffsetTable::getOffset(const IPAddress& dest)
{
	ProcessingDelayTable::iterator it = delayTable.find(dest);
	if (it != delayTable.end())
		return it->second;
	else
	{
		opp_error("%s not found in this offset table.", dest.str().c_str());
		return -1;
	}
}

int OffsetTable::getHops(const IPAddress& dest)
{
	HopsTable::iterator it = hopsTable.find(dest);
	if (it != hopsTable.end())
		return it->second;
	else
	{
		opp_error("%s not found  in this hops table.", dest.str().c_str());
		return -1;
	}
}
