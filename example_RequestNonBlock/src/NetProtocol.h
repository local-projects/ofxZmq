//
//  NetProtocol.h
//  zmqRequestNonBlock
//
//  Created by oriol on 02/03/2019.
//
//

#pragma once
#include "ofMain.h"

namespace NetProtocol{

	struct NetMessage{
		string data;
		string srcIP;
		string dstIP;
		int srcPort = 0;
		int dstPort = 0;
	};
}
