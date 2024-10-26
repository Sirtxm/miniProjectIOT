
#define BLYNK_TEMPLATE_ID "TMPL6qa0kR4oO"
#define BLYNK_TEMPLATE_NAME "IOTminiproject"
#define BLYNK_AUTH_TOKEN "LbmxOw3Yu722fPNGpERrMoc7rtDot6Wh"  // New Blynk token

#include <Arduino.h>
#include <ESP32Servo.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

int RAIN_SENSOR_PIN= 34;    //D19
int LDR_PIN=32;             //D32
int SERVO_PIN=4;            //D4
int LIGHT_THRESHOLD = 300;

Servo myservo;

const int IDLE = 0;
const int RAINING = 1;
const int SUNSHINE = 2;
int state = IDLE;
int servoPosition = 0;  // 0 = closed, 1 = open



// WiFi credentials
char ssid[] = "TP-Link_A287";      // Replace with your WiFi SSID
char pass[] = "66845355";  // Replace with your WiFi password

void setup() {
    pinMode(RAIN_SENSOR_PIN, INPUT);
    pinMode(LDR_PIN, INPUT);
    myservo.attach(SERVO_PIN);
    myservo.write(0);
    servoPosition = 0;
    Serial.begin(115200);

    // Connect to WiFi
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("WiFi connected");

    // Connect to Blynk
    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
    Blynk.run();  // Run Blynk process

    int rainDetected = analogRead(RAIN_SENSOR_PIN);
    int lightValue = analogRead(LDR_PIN);

    if (state == IDLE) {
        Serial.println("IDLE MODE");
        Serial.print("lightValue = ");
        Serial.println(lightValue);
        Serial.print("Rain = ");
        Serial.println(rainDetected);
        delay(1000);

        // if (servoPosition == 0) {
        //     Serial.println("Servo is CLOSED");
        //     delay(1000);
        // } else {
        //     Serial.println("Servo is OPEN");
        //     delay(1000);
        // }

        if (rainDetected > 0) {
            state = RAINING;
            delay(1000);
        } 
        else if (lightValue > LIGHT_THRESHOLD && rainDetected == 0) {
            state = SUNSHINE;
           delay(1000);
        }
    }

    else if (state == RAINING) {
        Blynk.virtualWrite(V1, 0);
        Blynk.virtualWrite(V2, 255);
        Serial.println("RAINING MODE");
        
        if (servoPosition != 0) {
            myservo.write(0);
            servoPosition = 0;
            Serial.println("Servo is now CLOSED due to rain");
            delay(1000);
        }

        if (rainDetected == 0) {
            state = IDLE;
            delay(1000);
        }else{
            state = IDLE;
        }
    }

    else if (state == SUNSHINE) {
        Serial.println("SUNSHINE MODE");
        Blynk.virtualWrite(V1, 255);
        Blynk.virtualWrite(V2, 0);

        if (servoPosition != 1) {
            myservo.write(90);
            servoPosition = 1;
            Serial.println("Servo is now OPEN due to sunshine");
            delay(1000);
        }

        if (rainDetected == 0) {
            state = IDLE;
            delay(1000);
        }else{
            state = RAINING;
        }
    }
}
