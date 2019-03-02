#include "ofApp.h"


ofxZmqReply server;

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetFrameRate(60);
	ofBackground(22);
	// start server
	//client.bind("tcp://*:9999");

	// start client
	server.bind("tcp://*:9999");
	ofSetWindowTitle("server");

}


//--------------------------------------------------------------
void ofApp::update()
{

	while (server.hasWaitingMessage()){

		ofBuffer data;
		server.getNextMessage(data);

		ofLog() << "SERVER RECEIVED: \"" << data << "\"";

		if(server.isConnected()){
			string response  = "Hello back: " + data.getText();
			if (!server.send(response)){
				ofLogError() << "SEND FAILED message: " << response;
			}else{
				ofLog() << "SENT message: \"" << response  << "\"";
			}
		}else{
			ofLogError() << "SERVER NOT CONNECTED: " << data;
		}
	}

}

//--------------------------------------------------------------
void ofApp::draw(){

	ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
	ofSetColor(255);
	ofPushMatrix();
	ofRotateDeg(360 * ofGetElapsedTimef(), 0, 0, 1);
	ofDrawRectangle(0, -2, 100, 4);
	ofPopMatrix();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{

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
