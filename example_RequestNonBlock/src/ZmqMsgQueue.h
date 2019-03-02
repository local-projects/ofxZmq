//
//  ZmqMsgQueue.h
//  zmqRequestNonBlock
//
//  Created by oriol on 02/03/2019.
//
//

#pragma once
#include "ofMain.h"
#include "ofxZmq.h"
#include "ofxHistoryPlot.h"
#include "NetProtocol.h"


class ZmqMsgQueue{

public:
	
	ZmqMsgQueue();

	void setup(string dstIP, int dstPort);
	void update();

	bool enqueueMessage(const NetProtocol::NetMessage & msg);

	//params
	void setTxTimeout(int ms);
	void setMaxQueueLen(int l){maxQueueLen = l;};
	int getQueueLen(){return queue.size();};
	void setMaxRetries(int t){ maxRetries = t;}

	//stats
	ofxHistoryPlot* getPlot(){return plot;};
	float getAvgTxDuration(){return avgTxDuration;};

protected:

	enum MSG_SEND_STATE{
		NO_PENDING_MSG,
		SHOULD_SEND_MESSAGE,
		SEND_MSG_FAILED,
		WAITING_REPLY,
		GOT_REPLY,
		TIMED_OUT_WAITING_FOR_REPLY_WILL_TRY_AGAIN,
		TIMED_OUT_WAITING_FOR_REPLY_NO_MORE_TRY
	};

	struct MessageInfo{
		NetProtocol::NetMessage currentMsg;
		string reply;
		MSG_SEND_STATE state;
		float sendTime = 0; //on the current try
		float accSendTime = 0; //across retries
		int numTriesLeft = 3;
	};

	vector<NetProtocol::NetMessage> queue;
	MessageInfo msg;

	string destinationIP;
	int destinationPort = 0;
	int txTimeout = 1000; //ms

	void setupSocket();
	ofxZmqRequest * client = nullptr;

	int maxQueueLen = 10;
	int maxRetries = 3;

	float avgTxDuration = 0.0f;

	ofxHistoryPlot * plot = nullptr;

};

