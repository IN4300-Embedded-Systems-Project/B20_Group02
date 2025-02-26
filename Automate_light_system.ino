//------------------------------------------------------------------------------
// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID "TMPLivZv"
#define BLYNK_TEMPLATE_NAME "ESP32 Home Automation"
#define BLYNK_AUTH_TOKEN "oejt-3SJKSKRXj49vhL"
//------------------------------------------------------------------------------
// Comment this out to disable prints and save space
// #define BLYNK_PRINT Serial
//------------------------------------------------
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;
//------------------------------------------------
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "wifi"; 
char pass[] = "password"; 

//------------------------------------------------

BlynkTimer timer;

#define PIN_BUTTON1 26
#define PIN_BUTTON2 25
#define PIN_BUTTON3 33
#define PIN_BUTTON4 32

#define PIN_RELAY1 13
#define PIN_RELAY2 12
#define PIN_RELAY3 14
#define PIN_RELAY4 27

int state_relay1 = 0;
int state_relay2 = 0;
int state_relay3 = 0;
int state_relay4 = 0;

// Change the virtual pins according to the rooms
#define VPIN_BUTTON1    V1
#define VPIN_BUTTON2    V2
#define VPIN_BUTTON3    V3 
#define VPIN_BUTTON4    V4

BLYNK_CONNECTED() {
  Blynk.syncVirtual(VPIN_BUTTON1, VPIN_BUTTON2, VPIN_BUTTON3, VPIN_BUTTON4);
}

BLYNK_WRITE(VPIN_BUTTON1) { 
  state_relay1 = param.asInt(); 
  digitalWrite(PIN_RELAY1, state_relay1); 
}

BLYNK_WRITE(VPIN_BUTTON2) { 
  state_relay2 = param.asInt(); 
  digitalWrite(PIN_RELAY2, state_relay2); 
}

BLYNK_WRITE(VPIN_BUTTON3) { 
  state_relay3 = param.asInt(); 
  digitalWrite(PIN_RELAY3, state_relay3); 
}

BLYNK_WRITE(VPIN_BUTTON4) { 
  state_relay4 = param.asInt(); 
  digitalWrite(PIN_RELAY4, state_relay4); 
}

void setup() {
  pinMode(PIN_BUTTON1, INPUT_PULLUP);
  pinMode(PIN_BUTTON2, INPUT_PULLUP);
  pinMode(PIN_BUTTON3, INPUT_PULLUP);
  pinMode(PIN_BUTTON4, INPUT_PULLUP);  
  
  pinMode(PIN_RELAY1, OUTPUT);
  pinMode(PIN_RELAY2, OUTPUT);
  pinMode(PIN_RELAY3, OUTPUT);
  pinMode(PIN_RELAY4, OUTPUT);

  digitalWrite(PIN_RELAY1, HIGH);
  digitalWrite(PIN_RELAY2, HIGH);
  digitalWrite(PIN_RELAY3, HIGH);
  digitalWrite(PIN_RELAY4, HIGH);

  Blynk.begin(auth, ssid, pass);
}

void loop() {
  Blynk.run();
  listen_push_buttons();
}

void listen_push_buttons() {
  static unsigned long lastDebounceTime[4] = {0, 0, 0, 0};
  static int lastButtonState[4] = {HIGH, HIGH, HIGH, HIGH};
  unsigned long debounceDelay = 200; // Debounce time

  // Button 1
  if (digitalRead(PIN_BUTTON1) == LOW && (millis() - lastDebounceTime[0] > debounceDelay)) {
    lastDebounceTime[0] = millis();
    if (lastButtonState[0] == HIGH) {
      toggle_relay(PIN_RELAY1, state_relay1);
      Blynk.virtualWrite(VPIN_BUTTON1, state_relay1);
    }
    lastButtonState[0] = LOW;
  } else if (digitalRead(PIN_BUTTON1) == HIGH) {
    lastButtonState[0] = HIGH;
  }

  // Button 2
  if (digitalRead(PIN_BUTTON2) == LOW && (millis() - lastDebounceTime[1] > debounceDelay)) {
    lastDebounceTime[1] = millis();
    if (lastButtonState[1] == HIGH) {
      toggle_relay(PIN_RELAY2, state_relay2);
      Blynk.virtualWrite(VPIN_BUTTON2, state_relay2);
    }
    lastButtonState[1] = LOW;
  } else if (digitalRead(PIN_BUTTON2) == HIGH) {
    lastButtonState[1] = HIGH;
  }

  // Button 3
  if (digitalRead(PIN_BUTTON3) == LOW && (millis() - lastDebounceTime[2] > debounceDelay)) {
    lastDebounceTime[2] = millis();
    if (lastButtonState[2] == HIGH) {
      toggle_relay(PIN_RELAY3, state_relay3);
      Blynk.virtualWrite(VPIN_BUTTON3, state_relay3);
    }
    lastButtonState[2] = LOW;
  } else if (digitalRead(PIN_BUTTON3) == HIGH) {
    lastButtonState[2] = HIGH;
  }

  // Button 4
  if (digitalRead(PIN_BUTTON4) == LOW && (millis() - lastDebounceTime[3] > debounceDelay)) {
    lastDebounceTime[3] = millis();
    if (lastButtonState[3] == HIGH) {
      toggle_relay(PIN_RELAY4, state_relay4);
      Blynk.virtualWrite(VPIN_BUTTON4, state_relay4);
    }
    lastButtonState[3] = LOW;
  } else if (digitalRead(PIN_BUTTON4) == HIGH) {
    lastButtonState[3] = HIGH;
  }
}

void toggle_relay(int pin, int &state) {
  state = !state;
  digitalWrite(pin, state ? LOW : HIGH);
  delay(50);
}
