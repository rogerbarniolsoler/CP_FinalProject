#pragma once

#include "ofMain.h"
#include "ofxNetwork.h"
#include "ofxSvg.h"

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

// Calcula distancia entre dos puntos
float getDist(ofPoint p1, ofPoint p2) {
	return p1.distance(p2);
}

// Verifica si un dedo está estirado (comparando la punta con el nudillo inferior)
// Esto asume que la mano apunta hacia arriba. 
bool isFingerExtended(const vector<ofPoint> & hand, int fingerTipIdx) {
	// El nudillo base del dedo es tipIdx - 2 (ej: Indice punta 8, nudillo 6)
	// Si la distancia de la punta a la muñeca (0) es mayor que la del nudillo a la muñeca, está estirado.
	return getDist(hand[fingerTipIdx], hand[0]) > getDist(hand[fingerTipIdx - 2], hand[0]);
}

// Devuelve TRUE si la mano está en horizontal
// Compara la posición de la muñeca (0) con la base del dedo índice (5)
bool isHandHorizontal(const vector<ofPoint> & hand) {
	float diffX = abs(hand[5].x - hand[0].x);
	float diffY = abs(hand[5].y - hand[0].y);
	// Si la diferencia en X es mayor que en Y, la mano está tumbada
	return diffX > diffY;
}

// Verifica si el pulgar está "abierto" (lejos del índice)
bool isThumbOpen(const vector<ofPoint> & hand) {
	return getDist(hand[4], hand[8]) > 50; // Ajustar umbral según resolución
}

//struct for the shapes that appears on the screen
struct FiguraViva {
	ofxSVG * imagenRef; // Puntero al molde original (para no cargar el archivo 100 veces)
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

		void lanzarFigura(string nombreGesto);

		ofxTCPServer server;
		vector<ofPoint> handPoints;

		vector<TimedShape> shapes;
		ofVideoGrabber cam; //PC camera

		std::map<string, ofxSVG> galeriaIconos; //si buscamos "corazón" nos da el svg del cora
		vector<FiguraViva> figurasActivas; //lista de figuras qeu se estan dibujando en elmomento

		//control del tiempo para no dibujar 50 formas por seg
		float lastSpawnTime = 0;
};
