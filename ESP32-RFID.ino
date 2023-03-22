//Libraries
#include <SPI.h> //https://www.arduino.cc/en/reference/SPI
#include <MFRC522.h> //https://github.com/miguelbalboa/rfid
#include <WiFi.h>
#include <HTTPClient.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include "WiFiCredentials.h"

//Constants
#define SS_PIN 5
#define RST_PIN 0
#define LED_BLUE 16
#define LED_RED 17
#define BUZZER_PIN 25

using namespace std;

// function declaration
vector<int> getRFIDCode(byte *buffer, byte bufferSize);
bool postClockInOut(string serverName, string tagid);

//Variables
MFRC522 rfid = MFRC522(SS_PIN, RST_PIN);
string serverUrl = "http://192.168.0.228:3000/users/working/";

void setup() {
    //Init Serial USB
    Serial.begin(115200);
    Serial.println("Initialize System");

    // Init WiFi lib and connect to network
    WiFi.begin(SSID, PASSWORD);
    Serial.println("Connecting to wifi");
    while(WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println("");
    Serial.println("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());
    
    SPI.begin();
    rfid.PCD_Init();

    pinMode(LED_BLUE, OUTPUT);
    pinMode(LED_RED, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);

    Serial.print("Reader :");
    rfid.PCD_DumpVersionToSerial();
}

void loop() {
    readRFID();
}

//Read RFID card function
void readRFID() {

    // Look for new card and verify if the NUID has been readed
    if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
        return;
    }

    vector<int> currentRFID = getRFIDCode(rfid.uid.uidByte, rfid.uid.size);

    // Check if RFID has access
    checkRFIDCode(currentRFID);

    // Stop reading PICC
    rfid.PICC_HaltA();

    // Stop encryption on PCD
    rfid.PCD_StopCrypto1();

}

void checkRFIDCode(vector<int> currentRFID) {
    stringstream ss;
    for (auto it = currentRFID.begin(); it != currentRFID.end(); it++)    {
        ss << *it;
    }
    bool isAuthorized = postClockInOut(ss.str());
    letUserIn(isAuthorized);
}

void letUserIn(bool isAuthorized) {
    if (isAuthorized) {
        digitalWrite(LED_BLUE, HIGH);
        digitalWrite(BUZZER_PIN, HIGH);
        delay(2000);
        digitalWrite(LED_BLUE, LOW);
        digitalWrite(BUZZER_PIN, LOW);
    } else {
        digitalWrite(LED_RED, HIGH);
        digitalWrite(BUZZER_PIN, HIGH);
        delay(500);
        digitalWrite(BUZZER_PIN, LOW);
        delay(500);
        digitalWrite(BUZZER_PIN, HIGH);
        delay(500);
        digitalWrite(BUZZER_PIN, LOW);
        delay(500);
        digitalWrite(BUZZER_PIN, HIGH);
        delay(500);
        digitalWrite(BUZZER_PIN, LOW);
        delay(500);
        digitalWrite(LED_RED, LOW);
    }
}

bool postClockInOut(string tagid) {
    if(WiFi.status() == WL_CONNECTED) {
        WiFiClient client;
        HTTPClient http;

        string url = serverUrl + tagid;
        http.begin(client, url.c_str());
        Serial.println(url.c_str());
        http.addHeader("Content-Type", "application/json");

        int httpResponseCode = http.POST("");

        // Free resources
        http.end();

        Serial.println(httpResponseCode);

        if (httpResponseCode >= 200 && httpResponseCode <= 302) {
            return true;
        }
    } else {
        Serial.println("WiFi not connected!");
    }
    return false;
}

vector<int> getRFIDCode(byte *buffer, byte bufferSize) {
    vector<int> currentRFID;
    Serial.print("RFID code: ");
    for (byte i = 0; i < bufferSize; i++) {
        currentRFID.push_back(buffer[i]);
        Serial.print(buffer[i]);
    }
    Serial.println("");
    return currentRFID;
}