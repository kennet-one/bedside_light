//************************************************************
// nodeId = 635035530
//
//************************************************************
#include "painlessMesh.h"

#define   MESH_PREFIX     "kennet"
#define   MESH_PASSWORD   "kennet123"
#define   MESH_PORT       5555

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;


char buttonState = 0;

unsigned long pMillis = 0;
const long interval = 100;
int lButState = LOW; // Зберігаємо останній стан кнопки

void power (){
  if (buttonState == 1) {
      buttonState = 0;
      mesh.sendSingle(624409705,"bedside_off");
    } else {
      buttonState++;
      mesh.sendSingle(624409705,"bedside_on");
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

void receivedCallback( uint32_t from, String &msg ) {

  String str1 = msg.c_str();
  String str2 = "bedside";
  String str3 = "bedside_echo";

  if (str1.equals(str2)) {
    power();
  }

  if (str1.equals(str3)) {
    if (buttonState == 0) {
      mesh.sendSingle(624409705,"bedside_off");
    } else {
      mesh.sendSingle(624409705,"bedside_on");
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

  powerBut();

  mesh.update();

  if (buttonState == 0) {
    pinMode(5,INPUT);
  } else {
    pinMode(5,OUTPUT);
  }
}
