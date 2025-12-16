#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(0);
	server.setup(1588); //the same port as in Python
	server.setMessageDelimiter("\n");
	std::cout << "Servidor OF escuchando en puerto 1588" << std::endl;


}

//--------------------------------------------------------------
void ofApp::update(){



	// if there are any connected clients, try to receive messages from them
	if (server.getNumClients() > 0) {
		int lastID = server.getLastID();
		for (int clientID = 0; clientID <= lastID; clientID++) {
			if (server.getClientIP(clientID).empty()) continue;
			std::string msg = server.receive(clientID);
			if (!msg.empty()) {
				std::cout << " Mensaje recibido: " << msg << std::endl;
				std::vector<ofPoint> points;
				std::stringstream ss(msg);
				std::string token;
				while (std::getline(ss, token, ';')) {
					std::stringstream coord(token);
					float x, y;
					char comma;
					if (coord >> x >> comma >> y) {
						points.push_back(ofPoint(x * ofGetWidth(), y * ofGetHeight()));
					}
				}
				handPoints = points;

				// Ejemplo: si hay pinch (pulgar cerca del índice), crear figura
				if (handPoints.size() >= 9) {
					float dist = handPoints[4].distance(handPoints[8]);
					if (dist < 30) {
						TimedShape ts;
						ts.rect = ofRectangle(ofRandom(0, ofGetWidth() / 2), ofRandom(0, ofGetHeight()), 50, 50);
						ts.birthTime = ofGetElapsedTimef();
						shapes.push_back(ts);
						std::cout << "Figura creada" << std::endl;
					}
				}
			}
		}
	}

	// Eliminar figuras viejas
	float now = ofGetElapsedTimef();
	shapes.erase(remove_if(shapes.begin(), shapes.end(),
					 [now](TimedShape & s) { return now - s.birthTime > 3.0; }),
		shapes.end());
	
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
		ofDrawRectangle(s.rect);
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
