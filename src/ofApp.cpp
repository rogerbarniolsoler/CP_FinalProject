#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(0);
	ofSetFrameRate(60);
	ofEnableAlphaBlending();

	server.setup(1588); //the same port as in Python
	server.setMessageDelimiter("\n");
	std::cout << "Servidor OF escuchando en puerto 1588" << std::endl;

	//update the SVGs
	galeriaIconos["corazon"].load("Heart.png");
	galeriaIconos["conejo"].load("Bunny.png");
	galeriaIconos["gafas"].load("Gafas.png");
	galeriaIconos["estrella"].load("Star.png");
	galeriaIconos["gusano"].load("Gusi.png");
	galeriaIconos["rock"].load("Rayito.png");
	galeriaIconos["alas"].load("Angeeeeel.png"); // Para el rezo
	galeriaIconos["dormir"].load("dormir.svg");
	galeriaIconos["diablo"].load("Demon.png");
	galeriaIconos["fuego"].load("Fuego.png");
	galeriaIconos["pez"].load("Gluglu.png");
	galeriaIconos["burbuja"].load("Bubbles.png");
	galeriaIconos["pulgar"].load("Posa.png");

	// Hacemos que las imágenes se dibujen desde su centro
	for (auto & entry : galeriaIconos) {
		entry.second.setAnchorPercent(0.5, 0.5);
	}
}

//--------------------------------------------------------------
void ofApp::update(){

	// if there are any connected clients, try to receive messages from them
	if (server.getNumClients() > 0) {
		int lastID = server.getLastID();
		for (int clientID = 0; clientID <= lastID; clientID++) {
			if (server.getClientIP(clientID).empty()) continue;
			string msg = server.receive(clientID);
			if (!msg.empty()) {
				cout << " Mensaje recibido: " << msg << std::endl;
				vector<ofPoint> points;
				stringstream ss(msg);
				string token;
				while (getline(ss, token, ';')) {
					stringstream coord(token);
					float x, y;
					char comma;
					if (coord >> x >> comma >> y) {
						// Mapeamos X: si viene 0 será el centro de pantalla, si viene 1 será el borde derecho
						float mappedX = ofMap(x, 0, 1, ofGetWidth() / 2, ofGetWidth());
						float mappedY = y * ofGetHeight();

						// Invertimos X sise hace efecto espejo
						// float mappedX = ofMap(x, 0, 1, ofGetWidth(), ofGetWidth()/2);

						points.push_back(ofPoint(mappedX, mappedY));
					}
				}
				handPoints = points;

				// --- 1. GESTO SUPREMO: INSULTO 🖕 (Cerrar App) ---
				// Lo comprobamos primero por seguridad
				if (handPoints.size() >= 21) {
					// Si el dedo corazón (12) está estirado Y el índice (8) y anular (16) encogidos
					if (isFingerExtended(handPoints, 12) && !isFingerExtended(handPoints, 8) && !isFingerExtended(handPoints, 16)) {

						std::cout << "Tu madre! Adiós." << std::endl;
						std::exit(0); // Cierra la aplicación
					}
				}
				bool gafasDetectadas = false;

				// --- 2. GESTOS DE DOS MANOS (Requiere 42 puntos) ---
				if (handPoints.size() >= 42) {
					// Separamos las manos para que sea más legible
					vector<ofPoint> h1(handPoints.begin(), handPoints.begin() + 21);
					vector<ofPoint> h2(handPoints.begin() + 21, handPoints.end());

					//-------------BORRAR ESTO PARA HACER ALEATORIEDAD DE POSICIÓN -------------//
					// Calculamos el centro entre las dos manos para poner ahí la figura
					ofPoint centroManos = (h1[0] + h2[0]) / 2;
					//-------------BORRAR ESTO PARA HACER ALEATORIEDAD DE POSICIÓN -------------//

					// A. CORAZÓN
					// Puntas de los índices se tocan Y puntas de los pulgares se tocan
					if (getDist(h1[4], h2[4]) < 40 && getDist(h1[8], h2[8]) < 40) {
						std::cout << "Gesto: CORAZÓN <3" << std::endl;
						lanzarFigura("corazon", centroManos);
					}

					// B. GAFAS (🤌 + 🤌 tocándose)
					// Puntas de h1 juntas (Pinch total) Y Puntas de h2 juntas
					// Y h1 toca h2
					bool h1Closed = getDist(h1[4], h1[8]) < 30 && getDist(h1[4], h1[12]) < 30;
					bool h2Closed = getDist(h2[4], h2[8]) < 30 && getDist(h2[4], h2[12]) < 30;
					if (h1Closed && h2Closed && getDist(h1[8], h2[8]) < 50) {
						std::cout << "Gesto: GAFAS 8-)" << std::endl;
						ofPoint centro = (h1[8] + h2[8]) / 2;
						lanzarFigura("gafas", centro); //para que esté en los ojos (ajustar)
						gafasDetectadas = true;
					}

					// C. DIABLO (☝🏼 ☝🏼)
					// Índice de h1 estirado, Índice de h2 estirado. Resto cerrados.
					if (isFingerExtended(h1, 8) && !isFingerExtended(h1, 12) && isFingerExtended(h2, 8) && !isFingerExtended(h2, 12)) {
						std::cout << "Gesto: DIABLO >:)" << std::endl;
						lanzarFigura("diablo", centroManos);
					}

					// D. ARO / REZO (🙏)
					// Muñecas juntas y puntas de dedos medios juntas
					if (getDist(h1[0], h2[0]) < 60 && getDist(h1[12], h2[12]) < 60) {

						// Chequeo extra para "Zzz" (Inclinado)
						// Calculamos ángulo entre muñeca y dedo medio
						float angulo = atan2(h1[12].y - h1[0].y, h1[12].x - h1[0].x);
						// Si no es vertical (aprox -PI/2), es Zzz
						if (abs(angulo - (-1.57)) > 0.5) {
							std::cout << "Gesto: Zzzz (Dormir)" << std::endl;
							lanzarFigura("dormir", centroManos);
						} else {
							std::cout << "Gesto: REZO / ALAS" << std::endl;
							lanzarFigura("alas", centroManos);
						}
					}

					// E. FUEGO (👐 Uff hot)
					// Todos los dedos estirados y manos cerca
					// (Simplificado: si 4 dedos de cada mano están estirados y manos cerca)
					if (isFingerExtended(h1, 8) && isFingerExtended(h1, 12) && isFingerExtended(h1, 16) && isFingerExtended(h1, 20) && getDist(h1[0], h2[0]) < 150) { // 150 es "cerca pero no pegado"
						std::cout << "Gesto: FUEGO HOT" << std::endl;
						lanzarFigura("fuego", centroManos);
					}
				}

				// ---  GESTOS DE UNA MANO (Usamos la primera mano detectada) ---
				// Iteramos por las manos detectadas (sea 1 o 2)
				if (!gafasDetectadas) {
					int numManos = handPoints.size() / 21;

					for (int i = 0; i < numManos; i++) {
						// Extraer sub-vector de puntos para esta mano
						auto start = handPoints.begin() + (i * 21);
						vector<ofPoint> hand(start, start + 21);

						//-------------BORRAR ESTO PARA HACER ALEATORIEDAD DE POSICIÓN -------------//
						// Usamos el índice como referencia
						ofPoint puntaIndice = hand[8];
						//-------------BORRAR ESTO PARA HACER ALEATORIEDAD DE POSICIÓN -------------//

						// A. PUÑO 👊 (Rompe pantalla)
						// Ningún dedo estirado (excepto quizas pulgar que puede estar sobre los dedos)
						if (!isFingerExtended(hand, 8) && !isFingerExtended(hand, 12) && !isFingerExtended(hand, 16) && !isFingerExtended(hand, 20)) {
							std::cout << "Gesto: PUÑO (Romper)" << std::endl;
							figurasActivas.clear(); // Borrar todo violento
							// Activar sonido de romper pantalla
						}

						// B. ESTRELLA 🤏 (Pinch clásico)
						else if (getDist(hand[4], hand[8]) < 30 && isFingerExtended(hand, 12)) {
							// Nota: El isFingerExtended(12) es para diferenciarlo del puño o del "OK"
							std::cout << "Gesto: ESTRELLA" << std::endl;
							lanzarFigura("estrella", puntaIndice);
						}

						// 🐟 PEZ vs 🐰 CONEJO
						else if (isFingerExtended(hand, 8) && isFingerExtended(hand, 12) && !isFingerExtended(hand, 16) && !isFingerExtended(hand, 20)) {

							// Aquí viene la magia: ¿Está la mano tumbada o de pie?
							if (isHandHorizontal(hand)) {
								lanzarFigura("pez", puntaIndice); // Mano tumbada 🐟
							} else {
								lanzarFigura("conejo", puntaIndice); // Mano vertical 🐰
							}
						}

						// D. ROCK / RAYOS 🤘 (Índice y Meñique arriba)
						else if (isFingerExtended(hand, 8) && !isFingerExtended(hand, 12) && !isFingerExtended(hand, 16) && isFingerExtended(hand, 20)) {
							std::cout << "Gesto: ROCK / RAYOS" << std::endl;
							lanzarFigura("rock", hand[9]);
							//añasir sonido de guitarra electrica
						}

						// E. GUSANO ☝🏼 (Meñique solo)
						else if (!isFingerExtended(hand, 8) && !isFingerExtended(hand, 12) && isFingerExtended(hand, 20)) { // Solo meñique
							std::cout << "Gesto: GUSANO" << std::endl;
							lanzarFigura("gusano", puntaIndice);
						}

						// F. PULGAR ARRIBA 👍
						// Pulgar lejos del índice y resto de dedos cerrados
						else if (isThumbOpen(hand) && !isFingerExtended(hand, 8) && !isFingerExtended(hand, 12) && !isFingerExtended(hand, 20)) {
							// Comprobación extra: que la punta del pulgar esté más arriba (menor Y) que su base
							if (hand[4].y < hand[2].y) {
								std::cout << "Gesto: LIKE" << std::endl;
								lanzarFigura("pulgar", hand[9]);
							}
						}

						// G. BURBUJITA
						bool isHandClosed = getDist(hand[4], hand[8]) < 30 && getDist(hand[4], hand[12]) < 30;

						if (isHandClosed) {
							// Opcional: Si quieres que sea SOLO "de lado" (horizontal), descomenta esto:
							// if (isHandHorizontal(hand)) {
							lanzarFigura("burbuja", hand[9]);
							// }
						}
					}
				}
			}
		}
	}

	// Eliminar figuras viejas
	float now = ofGetElapsedTimef();
	figurasActivas.erase(remove_if(figurasActivas.begin(), figurasActivas.end(),
							 [now](FiguraViva & f) { return now - f.birthTime > 3.0; }),
		figurasActivas.end());


}

//--------------------------------------------------------------
void ofApp::draw(){
	// Dibujar línea separadora
	ofSetColor(100);
	ofDrawLine(ofGetWidth() / 2, 0, ofGetWidth() / 2, ofGetHeight());

	// Pantalla Derecha: La mano (ya ajustamos las coordenadas en update)
	ofSetColor(0, 255, 0);
	for (auto & p : handPoints) {
		ofDrawCircle(p.x, p.y, 5); // Quitamos la traslación anterior, ya está mapeado
	}

	// Pantalla Izquierda: Figuras generadas
	
	ofSetColor(255); // Resetear color a blanco para que los PNG no salgan teñidos
	for (auto & f : figurasActivas) {
		ofPushMatrix();
		ofTranslate(f.pos.x, f.pos.y);

		// Escalar
		float scale = f.sizeScale * 0.2; // 0.2 porque los PNG suelen ser muy grandes, ajusta si se ven enanos
		ofScale(scale, scale);

		// Dibujar (ya está centrado por el setAnchorPercent)
		f.imagenRef->draw(0, 0);

		ofPopMatrix();
	}
	ofPopMatrix();
	
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

void ofApp::lanzarFigura(string nombreGesto, ofPoint position) {
	float now = ofGetElapsedTimef();

	//Cooldown: només surt una figura cada 0.5 segons (de momet es pot tant treure com ajustar)
	if (now - lastSpawnTime > 0.5) {
		//verificar que la icona existeix
		if (galeriaIconos.count(nombreGesto)) {
			FiguraViva f;
			f.imagenRef = &galeriaIconos[nombreGesto];

			// ASIGNAMOS LA POSICIÓN (con un pequeño random para que no se apilen exactas)
			f.pos.x = position.x + ofRandom(-20, 20);
			f.pos.y = position.y + ofRandom(-20, 20);

			f.birthTime = now;
			f.sizeScale = ofRandom(0.5, 1.5); //random size scale between 0.5 and 1.5

			figurasActivas.push_back(f);

			lastSpawnTime = now; //update last spawn time
			std::cout << "Lanzando figura: " << nombreGesto << std::endl;
		}
	}
}
