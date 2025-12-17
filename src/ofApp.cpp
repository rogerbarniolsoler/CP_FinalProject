#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(0);
	ofSetFrameRate(60);
	ofEnableAlphaBlending();

	server.setup(1588); //the same port as in Python
	server.setMessageDelimiter("\n");
	std::cout << "Servidor OF escuchando en puerto 1588" << std::endl;

	bool intentarUsarCamara = true;

	if (intentarUsarCamara) {
		cam.setVerbose(true);
		// Intenta abrir la cámara. Si Python la tiene ocupada, esto fallará pero no romperá todo.
		cam.setup(1280, 720);
	}

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
		if (entry.second.isAllocated())
		{
			entry.second.setAnchorPercent(0.5, 0.5);
		}
	}
}

//--------------------------------------------------------------
void ofApp::update(){

	if (cam.isInitialized()) {
		cam.update();
	}

	// if there are any connected clients, try to receive messages from them
	if (server.getNumClients() > 0) {
		int lastID = server.getLastID();
		for (int clientID = 0; clientID <= lastID; clientID++) {
			if (server.getClientIP(clientID).empty()) continue;

			string msg = server.receive(clientID);
			if (!msg.empty()) {
				vector<ofPoint> points;
				stringstream ss(msg);
				string token;
				while (getline(ss, token, ';')) {
					stringstream coord(token);
					float x, y;
					char comma;
					if (coord >> x >> comma >> y) {
						// Mapeamos a pantalla completa
						points.push_back(ofPoint(x * ofGetWidth(), y * ofGetHeight()));
					}
				}
				handPoints = points;
			}
		}
	}

	// LIMPIEZA DE FIGURAS VIEJAS (Viven 3 segundos)
	float now = ofGetElapsedTimef();
	for (int i = figurasActivas.size() - 1; i >= 0; i--) {
		if (now - figurasActivas[i].birthTime > 3.0) {
			figurasActivas.erase(figurasActivas.begin() + i);
		}
	}

	// =========================================================
	// LÓGICA DE GESTOS MEJORADA (Con Escala y Prioridad)
	// =========================================================

	bool gafasDetectadas = false; // Bandera de prioridad

	// --- 1. DOS MANOS DETECTADAS ---
	if (handPoints.size() >= 42) {
		// Separar puntos
		vector<ofPoint> rawH1(handPoints.begin(), handPoints.begin() + 21);
		vector<ofPoint> rawH2(handPoints.begin() + 21, handPoints.end());

		// Ordenar visualmente (h1 Izquierda, h2 Derecha)
		vector<ofPoint> h1, h2;
		if (rawH1[0].x < rawH2[0].x) {
			h1 = rawH1;
			h2 = rawH2;
		} else {
			h1 = rawH2;
			h2 = rawH1;
		}

		// Calcular ESCALA (tamaño de la mano en pantalla)
		float s = (getHandScale(h1) + getHandScale(h2)) / 2.0;

		// GESTO: GAFAS 👓 (Pinch cerrado en ambas manos + tocándose)
		bool h1Closed = getDist(h1[4], h1[8]) < (s * 0.5) && getDist(h1[4], h1[12]) < (s * 0.5);
		bool h2Closed = getDist(h2[4], h2[8]) < (s * 0.5) && getDist(h2[4], h2[12]) < (s * 0.5);

		if (h1Closed && h2Closed && getDist(h1[8], h2[8]) < (s * 1.5)) {
			ofPoint centro = (h1[8] + h2[8]) / 2;
			lanzarFigura("gafas", centro);
			gafasDetectadas = true; // Bloquear burbujas
		}

		// GESTO: ALAS / REZO 🙏 (Muñecas juntas + Dedos arriba)
		bool munecasJuntas = getDist(h1[0], h2[0]) < (s * 2.5);
		bool dedosArriba = h1[12].y < h1[0].y && h2[12].y < h2[0].y; // Dedos más arriba que muñeca

		if (munecasJuntas && dedosArriba && !gafasDetectadas) {
			// Si hay mucha diferencia de altura entre muñecas -> Dormir
			if (abs(h1[0].y - h2[0].y) > (s * 0.8)) {
				lanzarFigura("dormir", (h1[12] + h2[12]) / 2);
			} else {
				lanzarFigura("alas", (h1[12] + h2[12]) / 2);
			}
		}

		// GESTO: DIABLO 😈 (Indices arriba, resto abajo)
		bool h1IndexOnly = isFingerExtended(h1, 8) && !isFingerExtended(h1, 12) && !isFingerExtended(h1, 20);
		bool h2IndexOnly = isFingerExtended(h2, 8) && !isFingerExtended(h2, 12) && !isFingerExtended(h2, 20);

		if (h1IndexOnly && h2IndexOnly) {
			lanzarFigura("diablo", (h1[8] + h2[8]) / 2);
		}

		// GESTO: CORAZÓN 🫶 (Dedos gordos e índices tocándose)
		if (getDist(h1[4], h2[4]) < s && getDist(h1[8], h2[8]) < s && !gafasDetectadas) {
			lanzarFigura("corazon", (h1[8] + h2[8]) / 2);
		}

		// GESTO: FUEGO 🔥 (Manos abiertas cerca)
		// ... puedes añadirlo aquí ...
	}

	// --- 2. UNA MANO (Si no hay gafas) ---
	if (!gafasDetectadas) {
		int numManos = handPoints.size() / 21;

		for (int i = 0; i < numManos; i++) {
			auto start = handPoints.begin() + (i * 21);
			vector<ofPoint> hand(start, start + 21);

			float s = getHandScale(hand); // Escala individual
			ofPoint puntaIndice = hand[8];
			ofPoint centroMano = hand[9];

			// BURBUJA 🫧 (Mano cerrada 'pinch' de todos los dedos)
			bool isHandClosed = getDist(hand[4], hand[8]) < (s * 0.6) && getDist(hand[4], hand[12]) < (s * 0.6);

			if (isHandClosed) {
				lanzarFigura("burbuja", centroMano);
			}

			// PEZ 🐟 vs CONEJO 🐰
			else if (isFingerExtended(hand, 8) && isFingerExtended(hand, 12) && !isFingerExtended(hand, 16) && !isFingerExtended(hand, 20)) {
				if (isHandHorizontal(hand)) {
					lanzarFigura("pez", puntaIndice);
				} else {
					lanzarFigura("conejo", puntaIndice);
				}
			}

			// ESTRELLA ⭐ (Solo índice y pulgar, medio levantado)
			else if (getDist(hand[4], hand[8]) < (s * 0.5) && getDist(hand[4], hand[12]) > (s * 1.0)) {
				lanzarFigura("estrella", puntaIndice);
			}

			// ROCK 🤘
			else if (isFingerExtended(hand, 8) && !isFingerExtended(hand, 12) && isFingerExtended(hand, 20)) {
				lanzarFigura("rock", centroMano);
			}

			// GUSANO 🐛 (Solo meñique)
			else if (!isFingerExtended(hand, 8) && isFingerExtended(hand, 20)) {
				lanzarFigura("gusano", hand[20]); // En la punta del meñique
			}

			// INSULTO 🖕 (Cerrar app)
			// (¡Cuidado probando esto que se cierra de verdad!)
			else if (isFingerExtended(hand, 12) && !isFingerExtended(hand, 8) && !isFingerExtended(hand, 16)) {
				// std::exit(0);
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	float w = ofGetWidth();
	float h = ofGetHeight();

	// 1. DIBUJAR FONDO (Cámara o Negro)
	ofSetColor(255);
	if (cam.isInitialized()) {
		cam.draw(0, 0, w, h);
	} else {
		// Si no hay cámara (porque Python la tiene), pintamos gris oscuro
		ofSetColor(20);
		ofDrawRectangle(0, 0, w, h);

		ofSetColor(255);
		ofDrawBitmapString("CAMARA OCUPADA POR PYTHON - MODO VISUALIZACION", 20, 20);
	}

	// 2. DIBUJAR FIGURAS
	for (auto & f : figurasActivas) {
		if (f.imagenRef != nullptr && f.imagenRef->isAllocated()) {
			ofPushMatrix();
			ofTranslate(f.pos.x, f.pos.y);
			float scale = f.sizeScale * 0.3; // Ajusta este 0.3 si son muy grandes/pequeñas
			ofScale(scale, scale);

			ofSetColor(255); // Reset color
			f.imagenRef->draw(0, 0);
			ofPopMatrix();
		}
	}

	// 3. MINIATURA HAND TRACKING (Abajo Derecha)
	float pipW = w / 4.0;
	float pipH = h / 4.0;
	float pipX = w - pipW - 10;
	float pipY = h - pipH - 10;

	// Fondo miniatura
	ofSetColor(0, 0, 0, 200);
	ofDrawRectangle(pipX, pipY, pipW, pipH);
	ofNoFill();
	ofSetColor(255);
	ofSetLineWidth(2);
	ofDrawRectangle(pipX, pipY, pipW, pipH);
	ofFill();

	// Dibujar esqueletos en miniatura
	ofSetColor(0, 255, 0);
	for (auto & p : handPoints) {
		float mx = ofMap(p.x, 0, w, pipX, pipX + pipW);
		float my = ofMap(p.y, 0, h, pipY, pipY + pipH);
		ofDrawCircle(mx, my, 4);
	}
	// Mostrar información de texto
	ofDrawBitmapStringHighlight("Modo: Deteccion de Gestos", pipX + 10, pipY - 10);
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
			std::cout << "Gesto detectado: " << nombreGesto << std::endl;
		}
	}
}
