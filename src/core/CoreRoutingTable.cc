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
#include "CoreRoutingTable.h"
#include "IPAddressResolver.h"

Define_Module(CoreRoutingTable);

void CoreRoutingTable::initialize()
{
	const char *destAddrs = par("coreRoutingTableFile");
	std::ifstream ifs(destAddrs);
	std::string line;
	while (getline(ifs, line) != NULL)
	{
		cStringTokenizer tokenizer(line.c_str());
		IPAddress dest = IPAddressResolver().resolve(tokenizer.nextToken()).get4();
		int next = atoi(tokenizer.nextToken());
		sendPortTable.insert(PortPair(dest, next));
		next = atoi(tokenizer.nextToken());
		receivePortTable.insert(PortPair(dest, next));
	}
	ifs.close();

	PortTable::iterator sit = sendPortTable.begin();
	PortTable::iterator rit = receivePortTable.begin();
	while (sit != sendPortTable.end())
	{
		ev << sit->first << ": send port " << sit->second << ", receive port " << rit->second << endl;
		sit++;
		rit++;
	}
}

void CoreRoutingTable::handleMessage(cMessage *msg)
{
	opp_error("This module cannot receive any messages (receive: %s).", msg);
	delete msg;
}

int CoreRoutingTable::getSendPort(const IPAddress& address)
{
	PortTable::iterator it = sendPortTable.find(address);
	if (it != sendPortTable.end())
		return it->second;
	else
	{
		opp_error("Send port not found %s in this routing table.", address.str().c_str());
		return -1;
	}
}

int CoreRoutingTable::getReceivePort(const IPAddress& address)
{
	PortTable::iterator it = receivePortTable.find(address);
	if (it != receivePortTable.end())
		return it->second;
	else
	{
		opp_error("Receive port not found %s in this routing table.", address.str().c_str());
		return -1;
	}
}
