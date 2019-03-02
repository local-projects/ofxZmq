#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup()
{
	TIME_SAMPLE_ENABLE();
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetFrameRate(60);
	ofBackground(22);
	ofSetWindowTitle("client");

	queue.setup("127.0.0.1", 9999);
	queue.setTxTimeout(2000); //ms
	queue.setMaxRetries(3);
	queue.setMaxQueueLen(10);
}

//--------------------------------------------------------------
void ofApp::update(){

	queue.update();
}

//--------------------------------------------------------------
void ofApp::draw(){


	ofSetColor(255);
	ofPushMatrix();
		ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
		ofRotateDeg(360 * ofGetElapsedTimef(), 0, 0, 1);
		ofDrawRectangle(0, -2, 100, 4);
	ofPopMatrix();

	ofDrawBitmapStringHighlight("Queue Len: " + ofToString(queue.getQueueLen()), 20, 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{

	NetProtocol::NetMessage msg;
	msg.dstIP = "127.0.0.1"; //those are sort of irrelevent - we are always connected
	msg.dstPort = 9999; //those are sort of irrelevent - we are always connected
	msg.data = "This is msg " + ofToString(count);
	queue.enqueueMessage(msg);
	count++;
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
