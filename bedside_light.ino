//************************************************************
// nodeId = 635035530
//
//************************************************************
#include "painlessMesh.h"
#include "mash_parameter.h"
#include "CRC.h"

Scheduler userScheduler; // to control your personal task
painlessMesh mesh; 

char buttonState = 0;

unsigned long pMillis = 0;
const long interval = 100;
int lButState = LOW; // Зберігаємо останній стан кнопки

void power (){
  if (buttonState == 1) {
      buttonState = 0;
      sendB("bedsi_off");
      sendB("powled0");
      sendB("bdsdl0");
    } else {
      buttonState++;
      sendB("bedsi_on");
      sendB("powled1");
      sendB("bdsdl1");
    }
}

void powerBatt(){
    unsigned long cMillis = millis();

  if (cMillis - pMillis >= interval) {
    pMillis = cMillis;

    int cButState = digitalRead(4);

    // Перевіряємо, чи стан кнопки змінився
    if (cButState != lButState) {
      lButState = cButState; // Оновлюємо останній стан кнопки
      power();
    }
  }
}

void handleBody(const String &msg ) {

  if (msg.equals("bedside")) {
    power();
  }

  if (msg.equals("bedside_echo")) {
    if (buttonState == 0) {
      sendB("bedsi_off");
      sendB("powled0");
      sendB("bdsdl0");
    } else {
      sendB("bedsi_on");
      sendB("powled1");
      sendB("bdsdl1");
    }
  }
}

void setup() {
  Serial.begin(115200);

  WiFi.setSleep(false);

  pinMode(5, INPUT);
  pinMode(4, INPUT_PULLUP);

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
}

void loop() {
  powerBatt();

  for (uint8_t _i=0; _i<4; ++_i){ String _b; if (!qPop(_b)) break; handleBody(_b); }
  mesh.update();

  if (buttonState == 0) {
    pinMode(5,INPUT);
  } else {
    pinMode(5,OUTPUT);
  }
}