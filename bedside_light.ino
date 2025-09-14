//************************************************************
// nodeId = 635035530
//
//************************************************************
#include "painlessMesh.h"
#include "mash_parameter.h"
#include "CRCMASH.h"

Scheduler userScheduler; // to control your personal task
// (disabled) painlessMesh mesh; // now provided by CRCMASH.h
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

void handleBodyFrom( uint32_t from, const String &body ) {

  String str1 = body.c_str();
  String str2 = "bedside";
  String str3 = "bedside_echo";

  if (str1.equals(str2)) {
    power();
  }

  if (str1.equals(str3)) {
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

  pinMode(5, INPUT);
  pinMode(4, INPUT_PULLUP);

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);

}

void loop() {

  powerBatt();

  // === CRCMASH Variant B: (from,body) queue ===
  for (uint8_t _i=0; _i<4; ++_i){ uint32_t _from; String _b; if (!qPop2(_from, _b)) break; handleBodyFrom(_from, _b); }
  mesh.update();

  if (buttonState == 0) {
    pinMode(5,INPUT);
  } else {
    pinMode(5,OUTPUT);
  }
}