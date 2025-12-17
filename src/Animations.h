//#pragma once
//#include "ofMain.h"
//
//// =====================================================================
//// CLASSE BASE: Defineix qu� tenen en com� totes les animacions.
//// Aix� �s el que et permetr� tractar-les de forma gen�rica al projecte gran.
//// =====================================================================
//class BaseAnimation {
//public:
//	ofVec2f position;
//	ofVec2f velocity;
//	float rotationDeg;
//	float scale;
//	float radius;
//
//	float timer;
//	float duration;
//	bool bIsFinished;
//
//	BaseAnimation() {
//		// Valors per defecte
//		radius = 50.0f;
//		rotationDeg = 0.0f;
//		scale = 1.0f;
//		timer = 0.0f;
//		duration = 4.0f; // Durada mitjana de 4 segons
//		bIsFinished = false;
//		// Punt d'inici: Centre de la pantalla
//		position.set(ofGetWidth() / 2, ofGetHeight() / 2);
//		velocity.set(0, 0);
//	}
//
//	// Destructor virtual essencial per a l'her�ncia
//	virtual ~BaseAnimation() { }
//
//	// M�todes que les subclasses sobreescriuran (virtual)
//	virtual void setup() { }
//
//	virtual void update(float dt) {
//		timer += dt;
//		if (timer >= duration) {
//			bIsFinished = true;
//			onFinished();
//		}
//	}
//
//	// M�tode opcional per fer alguna cosa just al final
//	virtual void onFinished() { }
//
//	// LA FUNCI� DE DIBUIX COMPARTIDA
//	// Dibuixa el cercle vermell amb la ratlla negra
//	void draw() {
//		if (bIsFinished) return;
//
//		ofPushStyle();
//		ofPushMatrix();
//		ofTranslate(position.x, position.y);
//		ofRotateDeg(rotationDeg);
//		ofScale(scale, scale);
//
//		// 1. Cercle vermell
//		ofSetColor(ofColor::red);
//		ofFill();
//		ofDrawCircle(0, 0, radius);
//
//		// 2. Ratlla negra central
//		ofSetColor(ofColor::black);
//		ofSetLineWidth(5.0f);
//		// La l�nia va d'esquerra a dreta del cercle
//		ofDrawLine(-radius, 0, radius, 0);
//
//		ofPopMatrix();
//		ofPopStyle();
//	}
//};
//
//// =====================================================================
//// LES 10 ANIMACIONS ESPEC�FIQUES (Subclasses)
//// =====================================================================
//
//// --- 1. Batec de cor (Heartbeat) ---
//class Anim1_Heartbeat : public BaseAnimation {
//public:
//	void setup() override {
//		duration = 4.0f;
//	}
//	void update(float dt) override {
//		BaseAnimation::update(dt);
//		if (bIsFinished) return;
//
//		// Utilitzem una combinaci� de sinus per simular el "lub-dub" del cor
//		float phase = timer * 3.0f; // Velocitat del batec
//		// Un sinus principal + un sinus secundari m�s r�pid
//		scale = 1.0f + sin(phase) * 0.2f + sin(phase * 2.5f) * 0.1f;
//	}
//};
//
//// --- 2. Rotaci� accelerada i frena de cop ---
//class Anim2_SpinStop : public BaseAnimation {
//	float rotationSpeed;
//
//public:
//	void setup() override {
//		duration = 3.5f;
//		rotationSpeed = 0;
//	}
//	void update(float dt) override {
//		BaseAnimation::update(dt);
//		if (bIsFinished) return;
//
//		// Acceleraci� constant de la velocitat de rotaci�
//		rotationSpeed += 400.0f * dt;
//		rotationDeg += rotationSpeed * dt;
//	}
//};
//
//// --- 3. Salts pilota basquet (esquerra a dreta) ---
//class Anim3_Basketball : public BaseAnimation {
//	float gravity;
//	float floorY;
//
//public:
//	void setup() override {
//		duration = 5.0f;
//		position.x = radius; // Comen�a a l'esquerra
//		position.y = ofGetHeight() / 2;
//		velocity.set(200, -400); // Impuls inicial cap amunt i dreta
//		gravity = 980.0f;
//		floorY = ofGetHeight() - radius - 20; // Un terra virtual
//	}
//	void update(float dt) override {
//		BaseAnimation::update(dt);
//		if (bIsFinished) return;
//
//		// F�sica simple
//		velocity.y += gravity * dt;
//		position += velocity * dt;
//
//		// Rebot a terra
//		if (position.y > floorY) {
//			position.y = floorY;
//			velocity.y *= -0.8f; // Coeficient de restituci� (p�rdua d'energia)
//		}
//
//		// Rotaci� segons la velocitat X per efecte realista
//		rotationDeg += velocity.x * dt * 0.5f;
//	}
//};
//
//// --- 4. Salts verticals pujant per la pantalla ---
//class Anim4_VerticalClimb : public BaseAnimation {
//	float gravity;
//	float currentFloorY;
//
//public:
//	void setup() override {
//		duration = 4.5f;
//		currentFloorY = ofGetHeight() / 2 + 100;
//		position.y = currentFloorY;
//		velocity.y = -500; // Primer salt fort
//		gravity = 1200.0f;
//	}
//	void update(float dt) override {
//		BaseAnimation::update(dt);
//		if (bIsFinished) return;
//
//		velocity.y += gravity * dt;
//		position.y += velocity.y * dt;
//
//		// El terra puja constantment
//		currentFloorY -= 100.0f * dt;
//
//		// Rebot quan toca el terra "m�bil"
//		if (position.y > currentFloorY) {
//			position.y = currentFloorY;
//			velocity.y = -500; // Sempre salta amb la mateixa for�a cap amunt
//		}
//	}
//};
//
//// --- 5. Moviments circulars amb pampallugues ---
//class Anim5_CircleFlicker : public BaseAnimation {
//	ofVec2f centerPos;
//	float orbitRadius;
//	bool bIsVisible;
//
//public:
//	void setup() override {
//		duration = 4.0f;
//		centerPos = position; // Guardem el centre original
//		orbitRadius = 150.0f;
//		bIsVisible = true;
//	}
//	void update(float dt) override {
//		BaseAnimation::update(dt);
//		if (bIsFinished) return;
//
//		// Moviment circular orbital
//		float speed = 3.0f;
//		position.x = centerPos.x + cos(timer * speed) * orbitRadius;
//		position.y = centerPos.y + sin(timer * speed) * orbitRadius;
//		// Rotaci� pr�pia tamb�
//		rotationDeg -= 180 * dt;
//
//		// Pampallugues r�pides (flicker)
//		// Utilitzem fmod sobre el temps per crear un interruptor r�pid
//		float flickerRate = 0.1f; // canvia cada 0.1s
//		bIsVisible = fmod(timer, flickerRate * 2) < flickerRate;
//
//		// Hack visual: si no �s visible, fem l'escala 0 temporalment
//		scale = bIsVisible ? 1.0f : 0.0f;
//	}
//};
//
//// --- 6. Moviment parab�lic i fora pantalla ---
//class Anim6_ParabolicOut : public BaseAnimation {
//	float gravity;
//
//public:
//	void setup() override {
//		duration = 3.0f;
//		// Llan�ament fort cap amunt i a la dreta
//		velocity.set(300, -700);
//		gravity = 1500.0f;
//	}
//	void update(float dt) override {
//		BaseAnimation::update(dt);
//		// No comprovar 'bIsFinished' aqu� per deixar que surti de la pantalla
//		// encara que el temps s'hagi acabat.
//
//		velocity.y += gravity * dt;
//		position += velocity * dt;
//
//		// Gira segons la traject�ria
//		rotationDeg = ofRadToDeg(atan2(velocity.y, velocity.x));
//	}
//};
//
//// --- 7. Dibuixa una creu (diagonals) ---
//class Anim7_CrossMoves : public BaseAnimation {
//	int state;
//	ofVec2f startPos, endPos;
//	float moveTimer;
//
//public:
//	void setup() override {
//		duration = 4.0f;
//		state = 0;
//		moveTimer = 0;
//		// Definim els punts de la creu al voltant del centre
//		float offset = 200;
//		startPos.set(ofGetWidth() / 2 - offset, ofGetHeight() / 2 - offset); // Top-Left
//		endPos.set(ofGetWidth() / 2 + offset, ofGetHeight() / 2 + offset); // Bottom-Right
//		position = startPos;
//	}
//	void update(float dt) override {
//		BaseAnimation::update(dt);
//		if (bIsFinished) return;
//
//		moveTimer += dt;
//		float durationPerLeg = 1.0f;
//		float t = ofClamp(moveTimer / durationPerLeg, 0, 1);
//
//		// Interpolaci� lineal (lerp) entre punts
//		position = startPos.getInterpolated(endPos, t);
//
//		// M�quina d'estats simple per canviar de diagonal
//		if (t >= 1.0f) {
//			moveTimer = 0;
//			state++;
//			float offset = 200;
//			if (state == 1) {
//				// Preparar segona diagonal: Top-Right -> Bottom-Left
//				startPos.set(ofGetWidth() / 2 + offset, ofGetHeight() / 2 - offset);
//				endPos.set(ofGetWidth() / 2 - offset, ofGetHeight() / 2 + offset);
//			} else if (state >= 2) {
//				// Acabar una mica abans si s'han fet les dues l�nies
//				timer = duration;
//			}
//		}
//	}
//};
//
//// --- 8. (Elecci�) Espiral hipn�tica cap enfora ---
//class Anim8_SpiralOut : public BaseAnimation {
//	ofVec2f centerPos;
//	float spiralRadius;
//
//public:
//	void setup() override {
//		duration = 5.0f;
//		centerPos = position;
//		spiralRadius = 0;
//		scale = 0.1f; // Comen�a petit
//	}
//	void update(float dt) override {
//		BaseAnimation::update(dt);
//		if (bIsFinished) return;
//
//		// El radi creix amb el temps
//		spiralRadius += 100 * dt;
//		// L'angle gira r�pid
//		float angle = timer * 5.0f;
//
//		position.x = centerPos.x + cos(angle) * spiralRadius;
//		position.y = centerPos.y + sin(angle) * spiralRadius;
//
//		rotationDeg = ofRadToDeg(angle);
//		// Es fa gran mentre surt
//		scale = ofMap(timer, 0, duration, 0.2f, 1.5f);
//	}
//};
//
//// --- 9. (Elecci�) Efecte "Gelatina" (Wobble el�stic) al centre ---
//class Anim9_Jelly : public BaseAnimation {
//	float elasticTimer;
//
//public:
//	void setup() override {
//		duration = 3.0f;
//		elasticTimer = 0;
//		// Comencem una mica despla�ats i grans per donar impacte
//		position.y -= 200;
//		scale = 2.0f;
//	}
//	void update(float dt) override {
//		BaseAnimation::update(dt);
//		if (bIsFinished) return;
//
//		elasticTimer += dt;
//
//		// Utilitzem una funci� d'amortiment el�stic (damped spring)
//		// Una f�rmula comuna per a "elastic out"
//		float t = elasticTimer / 2.0f; // Normalitzem una mica el temps
//		float elasticScale = pow(2, -10 * t) * sin((t - 0.1f / 4) * (2 * PI) / 0.1f) + 1;
//
//		// Apliquem l'efecte a l'escala Y i inversament a la X per fer "squash & stretch"
//		scale = 1.0f; // Escala base
//		float stretch = ofMap(elasticScale, 0, 2, 0.6, 1.4);
//
//		// Modifiquem manualment la matriu de dibuix a BaseAnimation no tenim acc�s a
//		// escala X i Y per separat f�cilment.
//		// Simplificaci�: fem servir l'escala global per simular vibraci�.
//
//		// Alternativa m�s simple de gelatina: vibraci� r�pida d'escala
//		scale = 1.0f + sin(timer * 20.0f) * (1.0f - timer / duration) * 0.3f;
//	}
//};
//
//// --- 10. (Elecci� 'q') Implosi� (desapareix fent-se petit) ---
//class Anim10_Implode : public BaseAnimation {
//public:
//	void setup() override {
//		duration = 2.0f; // M�s r�pida
//		rotationDeg = 0;
//	}
//	void update(float dt) override {
//		BaseAnimation::update(dt);
//		if (bIsFinished) return;
//
//		// Escala de 1 a 0 linealment
//		scale = ofMap(timer, 0, duration, 1.0f, 0.0f, true);
//		// Gira molt r�pid mentre desapareix
//		rotationDeg += 720 * dt;
//	}
//};


#pragma once
#include "ofMain.h"

// =====================================================================
// CLASSE BASE (No canvia)
// =====================================================================
class BaseAnimation {
public:
	ofVec2f position;
	ofVec2f velocity;
	float rotationDeg;
	float scale;
	float radius;

	float timer;
	float duration;
	bool bIsFinished;

	BaseAnimation() {
		radius = 50.0f;
		rotationDeg = 0.0f;
		scale = 1.0f;
		timer = 0.0f;
		duration = 4.0f;
		bIsFinished = false;
		position.set(ofGetWidth() / 2, ofGetHeight() / 2);
		velocity.set(0, 0);
	}

	virtual ~BaseAnimation() { }

	virtual void setup() { }

	virtual void update(float dt) {
		timer += dt;
		if (timer >= duration) {
			bIsFinished = true;
		}
	}

	void draw() {
		if (bIsFinished) return;
		ofPushStyle();
		ofPushMatrix();
		ofTranslate(position.x, position.y);
		ofRotateDeg(rotationDeg);
		ofScale(scale, scale);

		// Dibuix provisional (Cercle vermell amb ratlla)
		ofSetColor(ofColor::red);
		ofFill();
		ofDrawCircle(0, 0, radius);
		ofSetColor(ofColor::black);
		ofSetLineWidth(5.0f);
		ofDrawLine(-radius, 0, radius, 0);

		ofPopMatrix();
		ofPopStyle();
	}
};

// =====================================================================
// NOVES ANIMACIONS
// =====================================================================

// --- 1. ANGEL: Vol, moviment parabòlic suau, zoom subtil ---
class Anim_Angel : public BaseAnimation {
	ofVec2f startPos;
	float noiseOffset;

public:
	void setup() override {
		duration = 5.0f;
		// Posició inicial random dins de la pantalla (amb marge)
		position.set(ofRandom(100, ofGetWidth() - 100), ofRandom(100, ofGetHeight() - 100));
		startPos = position;
		noiseOffset = ofRandom(1000);
	}
	void update(float dt) override {
		BaseAnimation::update(dt);

		// Moviment flotant (Floating) usant Perlin Noise per suavitat
		float time = timer + noiseOffset;
		float floatX = ofSignedNoise(time * 0.5) * 100; // Moviment lateral suau
		float floatY = sin(time * 2.0) * 50; // Moviment vertical oscil·lant (parabòlic suau)

		position.x = startPos.x + floatX;
		position.y = startPos.y + floatY;

		// Zoom subtil (respiració)
		scale = 1.0f + sin(time * 3.0) * 0.1f;
	}
};

// --- 2. BURBUJAS: De baix a dalt, moviment lateral sinuós ---
class Anim_Bubbles : public BaseAnimation {
	float xFreq;
	float xAmp;

public:
	void setup() override {
		duration = 4.0f;
		// Comença a baix de tot, posició X random
		position.set(ofRandom(radius, ofGetWidth() - radius), ofGetHeight() + radius);
		velocity.y = -(ofGetHeight() + radius * 2) / duration; // Velocitat per creuar la pantalla exacte
		xFreq = ofRandom(2.0, 5.0);
		xAmp = ofRandom(20, 50);
	}
	void update(float dt) override {
		BaseAnimation::update(dt);

		// Puja constantment
		position.y += velocity.y * dt;

		// Moviment lateral (Sinuós)
		// Sumem un offset al moviment vertical
		float xOffset = sin(timer * xFreq) * xAmp;

		// Nota: Modifiquem la posició X relativa al seu centre original
		// Per fer-ho bé sense perdre el centre, podríem guardar el centreX,
		// però com que només puja, podem sumar directament a X si controlem la velocitat.
		// Farem un petit truc: velocity.x és el canvi del sinus.
		position.x += cos(timer * xFreq) * xAmp * dt * 2.0;

		// Les bombolles solen tremolar una mica
		scale = 1.0 + sin(timer * 10) * 0.05;
	}
};

// --- 3. CONEJO: Salts tipus bàsquet però sense perdre altura (constant) ---
class Anim_Rabbit : public BaseAnimation {
	float startY;
	float speedX;

public:
	void setup() override {
		duration = 4.0f;
		startY = ofGetHeight() / 2 + 100; // Un terra imaginari
		position.y = startY;
		position.x = radius;
		speedX = (ofGetWidth() - radius * 2) / duration; // Per creuar la pantalla
	}
	void update(float dt) override {
		BaseAnimation::update(dt);

		// Moviment horitzontal constant
		position.x += speedX * dt;

		// Salts: Utilitzem valor absolut del Sinus per fer arcs
		// |sin(t)| crea la forma de "m" "m" "m"
		float jumpHeight = 150.0f;
		float jumpSpeed = 6.0f; // Velocitat del salt
		float bounce = abs(sin(timer * jumpSpeed));

		position.y = startY - (bounce * jumpHeight);
	}
};

// --- 4. GATO: Gira al terra -> Salta -> Para -> Gira... ---
class Anim_Cat : public BaseAnimation {
	float floorY;
	int state; // 0: Spin, 1: Jump, 2: Wait
	float stateTimer;

public:
	void setup() override {
		duration = 5.0f;
		floorY = ofGetHeight() / 2 + 100;
		position.set(ofGetWidth() / 2, floorY);
		state = 0;
		stateTimer = 0;
	}
	void update(float dt) override {
		BaseAnimation::update(dt);

		stateTimer += dt;

		if (state == 0) { // GIRAR (al terra)
			rotationDeg += 360 * dt; // Gira ràpid
			if (stateTimer > 1.0f) { // Després d'1 segon, salta
				state = 1;
				stateTimer = 0;
				rotationDeg = 0; // Reset rotació
			}
		} else if (state == 1) { // SALTAR
			// Salt parabòlic simple hardcoded per durar 0.8s aprox
			float t = stateTimer;
			float jumpDur = 0.8f;
			// Formula paràbola: 4 * H * t * (1-t)  (normalitzat)
			if (t < jumpDur) {
				float normT = t / jumpDur;
				float height = 200.0f;
				position.y = floorY - (4 * height * normT * (1.0 - normT));
			} else {
				position.y = floorY;
				state = 2; // Passa a esperar
				stateTimer = 0;
			}
		} else if (state == 2) { // QUIET (Wait)
			scale = 1.0 + sin(timer * 10) * 0.02; // Respira una mica
			if (stateTimer > 1.0f) {
				state = 0; // Torna a girar
				stateTimer = 0;
			}
		}
	}
};

// --- 5. DEMONIO: Com Angel però agressiu, ràpid i fort ---
class Anim_Demon : public BaseAnimation {
	ofVec2f startPos;

public:
	void setup() override {
		duration = 3.5f; // Més curt, més ràpid
		position.set(ofRandom(100, ofGetWidth() - 100), ofRandom(100, ofGetHeight() - 100));
		startPos = position;
	}
	void update(float dt) override {
		BaseAnimation::update(dt);

		float time = timer * 3.0f; // Temps x3 de velocitat

		// Moviments més bruscos (ZigZag agressiu sumat a moviment ampli)
		float aggressiveX = sin(time) * 150 + sin(time * 5.3) * 30;
		float aggressiveY = cos(time * 0.8) * 100 + cos(time * 8.0) * 20;

		position.x = startPos.x + aggressiveX;
		position.y = startPos.y + aggressiveY;

		// Zoom agressiu
		scale = 1.0f + sin(timer * 15.0) * 0.3f;

		// Rotació petita i ràpida (vibració de fúria)
		rotationDeg = ofRandom(-15, 15);
	}
};

// --- 6. FUEGO: Creix fins cobrir pantalla, tremolor Parkinson ---
class Anim_Fire : public BaseAnimation {
public:
	void setup() override {
		duration = 4.0f;
		position.set(ofGetWidth() / 2, ofGetHeight() / 2);
		scale = 0.1f;
	}
	void update(float dt) override {
		BaseAnimation::update(dt);

		// 1. Es fa gegant (exponencial o lineal)
		// Volem que al final del temps cobreixi tot (scale aprox 20)
		float targetScale = 25.0f;
		scale = ofMap(timer, 0, duration, 0.1f, targetScale);

		// 2. Moviment "Parkinson" (Tremolor caòtic d'alta freqüència)
		float shakeAmount = 10.0f + (timer * 2.0f); // El tremolor augmenta
		position.x = (ofGetWidth() / 2) + ofRandom(-shakeAmount, shakeAmount);
		position.y = (ofGetHeight() / 2) + ofRandom(-shakeAmount, shakeAmount);

		// Opcional: Rotació vibratòria
		rotationDeg = ofRandom(-5, 5);
	}
};

// --- 7. GAFAS: Moviment horitzontal + Zoom In/Out ---
class Anim_Glasses : public BaseAnimation {
	float startX;

public:
	void setup() override {
		duration = 4.0f;
		startX = ofGetWidth() * 0.2;
		position.set(startX, ofGetHeight() / 2);
	}
	void update(float dt) override {
		BaseAnimation::update(dt);

		// Escombrat horitzontal d'esquerra a dreta i tornada
		position.x = startX + (sin(timer) + 1.0) * 300;

		// Zoom constant In/Out
		scale = 1.0f + sin(timer * 4.0) * 0.5f; // Oscil·la entre 0.5 i 1.5
	}
};

// --- 8. PEZ: Ràpids Zoom In/Out + Nedar ---
class Anim_Fish : public BaseAnimation {
	ofVec2f startPos;

public:
	void setup() override {
		duration = 5.0f;
		startPos.set(ofGetWidth() / 2, ofGetHeight() / 2);
		position = startPos;
	}
	void update(float dt) override {
		BaseAnimation::update(dt);

		// Moviment de nedar (avançar una mica i oscil·lar)
		// Fem un 8 (Lissajous)
		position.x = startPos.x + sin(timer * 2.0) * 200;
		position.y = startPos.y + sin(timer * 4.0) * 50;

		// Zoom molt ràpid (boca de peix o moviment d'aleta)
		scale = 1.0f + sin(timer * 15.0f) * 0.2f;

		// Orientem el peix cap on va (calculant la derivada del moviment X)
		float velX = cos(timer * 2.0);
		if (velX < 0)
			rotationDeg = 180; // Mira esquerra
		else
			rotationDeg = 0; // Mira dreta
	}
};

// --- 9. GUSANO: Diagonal (top-L -> bot-R) + Ondulació ---
class Anim_Worm : public BaseAnimation {
	ofVec2f start, end;

public:
	void setup() override {
		duration = 5.0f;
		start.set(100, 100);
		end.set(ofGetWidth() - 100, ofGetHeight() - 100);
	}
	void update(float dt) override {
		BaseAnimation::update(dt);

		float pct = timer / duration; // Percentatge del camí (0 a 1)
		if (pct > 1) pct = 1;

		// Posició base a la línia diagonal
		ofVec2f basePos = start.getInterpolated(end, pct);

		// Vector direcció
		ofVec2f dir = end - start;
		dir.normalize();

		// Vector perpendicular per a l'ondulació (-y, x)
		ofVec2f perp(-dir.y, dir.x);

		// Ondulació sinusoïdal
		float wave = sin(timer * 10.0f) * 40.0f; // Freq 10, Amp 40

		position = basePos + (perp * wave);

		// Rotació segueix l'ondulació (opcional, però queda millor)
		rotationDeg = 45 + cos(timer * 10.0f) * 30.0f;
	}
};

// --- 10. CORAZON: El batec que ja teniem ---
class Anim_Heart : public BaseAnimation {
public:
	void setup() override {
		duration = 4.0f;
	}
	void update(float dt) override {
		BaseAnimation::update(dt);
		// Lub-dub
		float phase = timer * 3.0f;
		scale = 1.0f + sin(phase) * 0.2f + sin(phase * 2.5f) * 0.1f;
	}
};

// --- 11. MARIPOSA: Apareix volant (caòtic però elegant) ---
class Anim_Butterfly : public BaseAnimation {
	float seed;

public:
	void setup() override {
		duration = 5.0f;
		seed = ofRandom(1000);
		position.set(ofGetWidth() / 2, ofGetHeight() / 2);
	}
	void update(float dt) override {
		BaseAnimation::update(dt);

		float t = timer + seed;

		// Moviment basat en soroll de Perlin per semblar orgànic
		float x = ofSignedNoise(t * 1.0f) * 300;
		float y = ofSignedNoise(t * 1.0f + 100) * 200;

		position.set(ofGetWidth() / 2 + x, ofGetHeight() / 2 + y);

		// Aleteig (scale X oscil·la ràpidament per simular ales tancant-se)
		// Modifiquem l'escala global, visualment semblarà que es fa petit i gran
		float wingFlap = sin(timer * 20.0f);
		scale = 1.0f + wingFlap * 0.3f;
	}
};

// --- 12. RAYO: Extrems zoom in/out molt ràpids ---
class Anim_Lightning : public BaseAnimation {
public:
	void setup() override {
		duration = 1.0f; // Molt ràpid
		position.set(ofGetWidth() / 2, ofGetHeight() / 2);
	}
	void update(float dt) override {
		BaseAnimation::update(dt);

		// Zoom extrem i aleatori (strobe effect)
		if (ofRandom(1.0) > 0.5) {
			scale = ofRandom(0.2, 0.5); // Molt petit
		} else {
			scale = ofRandom(3.0, 6.0); // Molt gran
		}

		// Canvi de posició brusc (jitter)
		position.x = ofGetWidth() / 2 + ofRandom(-100, 100);
		position.y = ofGetHeight() / 2 + ofRandom(-100, 100);

		// Rotació random
		rotationDeg = ofRandom(0, 360);
	}
};

// --- 13. ESTRELLA: Creua la pantalla amb caiguda (estrella fugaç) ---
class Anim_Star : public BaseAnimation {
	ofVec2f velocity;

public:
	void setup() override {
		duration = 2.5f;
		// Surt de l'esquerra, una mica amunt
		position.set(-radius, ofGetHeight() * 0.3);
		// Velocitat: Dreta ràpid, Baix lent
		velocity.set(600, 100);
	}
	void update(float dt) override {
		BaseAnimation::update(dt);

		position += velocity * dt;

		// Gira uniformement
		rotationDeg += 200 * dt;

		// Cua (opcional): Es fa petita mentre s'allunya
		scale = ofMap(timer, 0, duration, 1.2, 0.5);
	}
};
