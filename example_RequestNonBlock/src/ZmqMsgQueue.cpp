//
//  ZmqMsgQueue.cpp
//  zmqRequestNonBlock
//
//  Created by oriol on 02/03/2019.
//
//

#include "ZmqMsgQueue.h"

ZmqMsgQueue::ZmqMsgQueue(){
}

ZmqMsgQueue::~ZmqMsgQueue(){
	if(client){
		delete client;
		client = nullptr;
	}
}


void ZmqMsgQueue::setup(string dstIP, int dstPort){
	destinationIP = dstIP;
	destinationPort = dstPort;
	setupSocket(); //connect on setup
}


void ZmqMsgQueue::setTxTimeout(int ms){
	txTimeout = ms;
}


void ZmqMsgQueue::setupSocket(){

	string destination = "tcp://" + destinationIP + ":" + ofToString(destinationPort);
	if(client){
		client->disconnect(destination);
		delete client;
	}
	client = new ofxZmqRequest();
	client->setSendHighWaterMark(1);
	client->setConflate(true);
	client->connect(destination);
	client->setSendHighWaterMark(1);
	client->setConflate(true);
	ofLogWarning("ZmqMsgQueue") << "connected new socket! " << destination;
	//ofLogNotice() << "high watermark: " << req->getSendHighWaterMark();
}


bool ZmqMsgQueue::enqueueMessage(const NetProtocol::NetMessage & msg){
	bool ret;
	if(queue.size() < maxQueueLen){
		queue.push_back(msg);
		ret = true;
	}else{
		ofLogError("ZmqMsgQueue") << "dropping msg bc the queue is too long!";
		ret = false;
	}
	return ret;
}


void ZmqMsgQueue::update(){

	switch(msg.state){

		case NO_PENDING_MSG:{
			if(queue.size() > 0){
				msg.currentMsg = queue.front();
				queue.erase(queue.begin());
				msg.state = SHOULD_SEND_MESSAGE;
				msg.numTriesLeft = maxRetries - 1;
				msg.accSendTime = ofGetElapsedTimef();

				update(); //note that we are recursing here to reduce latency (avoid waiting a whole new frame for this)
			}
			}break;

		case SHOULD_SEND_MESSAGE:{
			msg.sendTime = ofGetElapsedTimef();
			if (!client->send(msg.currentMsg.data, false)){
				ofLogError("ZmqMsgQueue") << "SEND FAILED: \"" << msg.currentMsg.data << "\"";
				msg.state = SEND_MSG_FAILED;
			}else{
				ofLogNotice("ZmqMsgQueue") << "SENT OK: \"" << msg.currentMsg.data << "\"";
				msg.state = WAITING_REPLY;

				update(); //note that we are recursing here to reduce latency (avoid waiting a whole new frame for this)
			}
		}break;

		case SEND_MSG_FAILED:{
			ofLogError("ZmqMsgQueue") << "SEND FAILED message: " << msg.currentMsg.data;
			setupSocket();
			msg.state = NO_PENDING_MSG;

		}break;

		case WAITING_REPLY:{
			bool pendingMsg = client->hasWaitingMessage(0); //just check if its there, spend 0 ms waiting
			if(pendingMsg){ //msg is ready to be retrieved
				client->getNextMessage(msg.reply);
				msg.state = GOT_REPLY;

				update(); //note that we are recursing here to reduce latency (avoid waiting a whole new frame for this)

			}else{ //msg is not here yet
				if(ofGetElapsedTimef() - msg.sendTime > txTimeout / 1000.0f){ //timed out waiting for reply!
					msg.state = TIMED_OUT_WAITING_FOR_REPLY_WILL_TRY_AGAIN;
				}else{
					//still waiting for reply....
				}
			}
		}break;

		case GOT_REPLY:{
			float travelTime = ofGetElapsedTimef() - msg.accSendTime;
			ofLogNotice("ZmqMsgQueue") << "client RECEIVED: \"" << msg.reply  << "\" - took " << travelTime * 1000 << " ms and " << maxRetries - msg.numTriesLeft<< " tries";
			//do something with reply ...
			msg.state = NO_PENDING_MSG; //back to original state
		}break;

		case TIMED_OUT_WAITING_FOR_REPLY_WILL_TRY_AGAIN:{
			ofLogError("ZmqMsgQueue") << "timed out waiting for a reply!";
			msg.numTriesLeft--;
			if(msg.numTriesLeft < 0){
				msg.state = TIMED_OUT_WAITING_FOR_REPLY_NO_MORE_TRY; //ran out of retries! game over!
				ofLogError("ZmqMsgQueue") << "Ran out of retries (" << maxRetries << ")!" ;
			}else{
				ofLogError("ZmqMsgQueue") << msg.numTriesLeft << " retries left out of " << maxRetries;
				msg.state = SHOULD_SEND_MESSAGE; //retry!
				setupSocket();
			}
		}break;

		case TIMED_OUT_WAITING_FOR_REPLY_NO_MORE_TRY:{
			ofLogError("ZmqMsgQueue") << "Gave up sending msg \"" << msg.currentMsg.data << "\"";
			setupSocket();
			msg.state = NO_PENDING_MSG;
		}break;

	}

}
