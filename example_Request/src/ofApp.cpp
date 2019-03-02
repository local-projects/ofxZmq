#include "ofApp.h"

int counter = 0;

ofxZmqRequest * client;
float timeout = 1000; //miliseconds
string destination = "tcp://127.0.0.1:9999";

ofxZmqRequest * ofApp::getNewSocket(){
	// start server
	ofxZmqRequest * req = new ofxZmqRequest();
	req->setSendHighWaterMark(1);
	req->setConflate(true);
	req->connect(destination);
	ofLogWarning() << "connected new socket! " << destination;
	//ofLogNotice() << "high watermark: " << req->getSendHighWaterMark();
	return req;
}


//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetFrameRate(60);
	ofSetWindowTitle("client");
	client = getNewSocket();
}

//--------------------------------------------------------------
void ofApp::update()
{

}

//--------------------------------------------------------------
void ofApp::draw()
{

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{

	if(client->isConnected()){

		string message  = "Hello From client: " + ofToString(counter);

		//see if there's a pending message from a previously lost connection
		bool pendingMsg = client->hasWaitingMessage(0);
		if(pendingMsg){
			ofBuffer data;
			client->getNextMessage(data);
			ofLog() << "got PENDING msg: \"" << data  << "\"";
		}

		if (!client->send(message, false)){

			ofLogError() << "SEND FAILED message: " << message;

		}else{

			ofLog() << "SENT message: \"" << message << "\"";
			counter++;

			bool gotReply = false;
			float sendTime = ofGetElapsedTimef();
			gotReply = client->hasWaitingMessage(timeout);

			if(!gotReply){ // we could not send!

				ofLogError() << "time out! Gave up sending msg!";
				client = getNewSocket();

			}else{//sent msg ok and got reply!

				ofBuffer data;
				client->getNextMessage(data);
				float travelTime = ofGetElapsedTimef() - sendTime;
				ofLog() << "client RECEIVED: \"" << data  << "\" - took " << travelTime * 1000 << " ms";

			}
		}
	}else{
		ofLogError() << "NOT CONNECTED";
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{

}
