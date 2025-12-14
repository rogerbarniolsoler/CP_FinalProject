#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(0);
	server.setup(12345); //the same port as in Python
	server.setMessageDelimiter("\n");
}

//--------------------------------------------------------------
void ofApp::update(){

	// if there are any connected clients, try to receive messages from them
	if (server.getNumClients() > 0) {
		int lastID = server.getLastID();
		for (int clientID = 0; clientID <= lastID; ++clientID) {
			// skip invalid / empty client slots
			if (server.getClientIP(clientID).empty()) continue;

			std::string msg = server.receive(clientID);
			if (!msg.empty()) {
				vector<ofPoint> points;
				stringstream ss(msg);
				string token;
				while (std::getline(ss, token, ';')) {
					stringstream coord(token);
					float x, y;
					char comma;
					if (!(coord >> x >> comma >> y)) continue;
					// scale to the window's setup
					points.push_back(ofPoint(x * ofGetWidth(), y * ofGetHeight()));
				}
				handPoints = points;

			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	// Right window: hand in real time
	ofPushMatrix();
	ofTranslate(ofGetWidth() / 2, 0);
	ofSetColor(0, 255, 0);
	for (auto & p : handPoints) {
		ofDrawCircle(p.x - ofGetWidth() / 2, p.y, 5);
	}
	ofPopMatrix();

	// Left window: generated shapes
	ofSetColor(255, 0, 0);
	for (auto & s : shapes) {
		ofDrawRectangle(s);
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
