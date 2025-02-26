


#define BLYNK_TEMPLATE_ID "TMPL650"
#define BLYNK_TEMPLATE_NAME "ESP 32 Home system"
#define BLYNK_AUTH_TOKEN "axXdSsSHcFn3z73H4upgIxQ"

#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "wifi";
char pass[] = "jjj864kllc3";

BlynkTimer timer;

// Button and Relay Pin Definitions
#define button1_pin 26
#define button2_pin 25
#define button3_pin 33
#define button4_pin 32

#define relay1_pin 13
#define relay2_pin 12
#define relay3_pin 14
#define relay4_pin 27

// Virtual Pins for Blynk App
#define button1_vpin V1
#define button2_vpin V2
#define button3_vpin V3
#define button4_vpin V4

// Relay States
bool relay1_state = LOW;
bool relay2_state = LOW;
bool relay3_state = LOW;
bool relay4_state = LOW;

// Previous button states to avoid rapid toggling due to button noise
bool lastButton1State = HIGH;
bool lastButton2State = HIGH;
bool lastButton3State = HIGH;
bool lastButton4State = HIGH;

void checkButtons() {
    bool currentButton1State = digitalRead(button1_pin);
    bool currentButton2State = digitalRead(button2_pin);
    bool currentButton3State = digitalRead(button3_pin);
    bool currentButton4State = digitalRead(button4_pin);

    if (currentButton1State == LOW && lastButton1State == HIGH) {
        relay1_state = !relay1_state;
        digitalWrite(relay1_pin, relay1_state);
        Blynk.virtualWrite(button1_vpin, relay1_state); // Update Blynk UI
    }
    if (currentButton2State == LOW && lastButton2State == HIGH) {
        relay2_state = !relay2_state;
        digitalWrite(relay2_pin, relay2_state);
        Blynk.virtualWrite(button2_vpin, relay2_state);
    }
    if (currentButton3State == LOW && lastButton3State == HIGH) {
        relay3_state = !relay3_state;
        digitalWrite(relay3_pin, relay3_state);
        Blynk.virtualWrite(button3_vpin, relay3_state);
    }
    if (currentButton4State == LOW && lastButton4State == HIGH) {
        relay4_state = !relay4_state;
        digitalWrite(relay4_pin, relay4_state);
        Blynk.virtualWrite(button4_vpin, relay4_state);
    }

    // Update the last button state for debounce
    lastButton1State = currentButton1State;
    lastButton2State = currentButton2State;
    lastButton3State = currentButton3State;
    lastButton4State = currentButton4State;
}

// Sync Blynk when ESP32 connects
BLYNK_CONNECTED() {
    Blynk.syncVirtual(button1_vpin, button2_vpin, button3_vpin, button4_vpin);
}

// Blynk Button Handlers
BLYNK_WRITE(button1_vpin) {
    relay1_state = param.asInt();
    digitalWrite(relay1_pin, relay1_state);
}

BLYNK_WRITE(button2_vpin) {
    relay2_state = param.asInt();
    digitalWrite(relay2_pin, relay2_state);
}

BLYNK_WRITE(button3_vpin) {
    relay3_state = param.asInt();
    digitalWrite(relay3_pin, relay3_state);
}

BLYNK_WRITE(button4_vpin) {
    relay4_state = param.asInt();
    digitalWrite(relay4_pin, relay4_state);
}

void setup() {
    Serial.begin(115200);
    Blynk.begin(auth, ssid, pass);

    pinMode(button1_pin, INPUT_PULLUP);
    pinMode(button2_pin, INPUT_PULLUP);
    pinMode(button3_pin, INPUT_PULLUP);
    pinMode(button4_pin, INPUT_PULLUP);
    
    pinMode(relay1_pin, OUTPUT);
    pinMode(relay2_pin, OUTPUT);
    pinMode(relay3_pin, OUTPUT);
    pinMode(relay4_pin, OUTPUT);

    digitalWrite(relay1_pin, relay1_state);
    digitalWrite(relay2_pin, relay2_state);
    digitalWrite(relay3_pin, relay3_state);
    digitalWrite(relay4_pin, relay4_state);

    // Ensure Blynk gets the latest state at startup
    Blynk.syncVirtual(button1_vpin, button2_vpin, button3_vpin, button4_vpin);

    // Check buttons every 200ms
    timer.setInterval(200L, checkButtons);
}

void loop() {
    Blynk.run();
    timer.run();
}

