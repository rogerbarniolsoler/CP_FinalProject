#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(0);
	server.setup(12345); //the same port as in Python
	server.setMessageDelimiter("\n");
}

//--------------------------------------------------------------
void ofApp::update(){
	if (server.isConnected())
	{
		string msg = server.getLastMessage();
		if (!msg.empty())
		{
			vector<ofPoint> points;
			stringstream ss(msg); //buscar para que es el ss!!
			string token;
			while (getline(ss, token, ';'))
			{
				stringstream coord(token);
				float x, y;
				char comma;
				coord >> x >> comma >> y;
				//scale to the window's setup
				points.push_back(ofPoint(x * ofGetWidth(), y * ofGetHeight()));
			}
			handPoints = points;
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetColor(0, 255, 0);
	for (auto & p : handPoints) {
		ofDrawCircle(p, 5);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
