#pragma once

#include "ofMain.h"
#include "ofxNetwork.h"
#include "ofxOsc.h"

struct TimedShape {
	ofRectangle rect;
	float birthTime;
};

// Índices de los dedos en MediaPipe (para referencia)
// 0: Muñeca
// 4: Pulgar (Punta)
// 8: Índice (Punta)
// 12: Corazón (Punta)
// 16: Anular (Punta)
// 20: Meñique (Punta)

//// Verifica si el pulgar está "abierto" (lejos del índice)
//bool isThumbOpen(const vector<ofPoint> & hand) {
//	return getDist(hand[4], hand[8]) > 50; // Ajustar umbral según resolución
//}

// Calcula distancia entre dos puntos
float getDist(ofPoint p1, ofPoint p2) {
	return p1.distance(p2);
}

// Calcula una referencia de tamaño de la mano (distancia Muñeca -> Nudillo Medio)
// Esto nos ayuda a saber si la mano está cerca o lejos
float getHandScale(const vector<ofPoint> & hand) {
	float scale = hand[0].distance(hand[9]);
	return (scale > 0) ? scale : 1.0;
}

// Verifica si un dedo está estirado
bool isFingerExtended(const vector<ofPoint> & hand, int fingerTipIdx) {
	return getDist(hand[fingerTipIdx], hand[0]) > getDist(hand[fingerTipIdx - 2], hand[0]);
}

// Devuelve TRUE si la mano está en horizontal (tipo pez)
bool isHandHorizontal(const vector<ofPoint> & hand) {
	float diffX = abs(hand[5].x - hand[0].x);
	float diffY = abs(hand[5].y - hand[0].y);
	return diffX > diffY;
}

//struct for the shapes that appears on the screen
struct FiguraViva {
	// Puntero al molde original (para no cargar el archivo 100 veces)
	ofImage * imagenRef; 
	ofPoint pos;
	float birthTime;
	float sizeScale; // For making one bigger than others
};

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		void lanzarFigura(string nombreGesto, ofPoint position);

		ofxTCPServer server;
		vector<ofPoint> handPoints;

		map<string, ofImage> galeriaIconos; //si buscamos "corazón" nos da el svg del cora
		vector<FiguraViva> figurasActivas; //lista de figuras qeu se estan dibujando en elmomento

		//control del tiempo para no dibujar 50 formas por seg
		float lastSpawnTime = 0;

		ofxOscReceiver receiver;
		ofVideoGrabber cam;
};
