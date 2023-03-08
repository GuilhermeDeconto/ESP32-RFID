//Libraries
#include <SPI.h> //https://www.arduino.cc/en/reference/SPI
#include <MFRC522.h> //https://github.com/miguelbalboa/rfid
#include <iostream>
#include <string>
#include <vector>

//Constants
#define SS_PIN 5
#define RST_PIN 0
#define LED_BLUE 16
#define LED_RED 17

using namespace std;

// function declaration
vector<int> getRFIDCode(byte *buffer, byte bufferSize);

//Variables
MFRC522 rfid = MFRC522(SS_PIN, RST_PIN);
vector<vector<int>> authorizedKeys {{204, 207, 8 , 57}, {179, 64, 12, 170}};

void setup() {
  //Init Serial USB
  Serial.begin(115200);
  Serial.println("Initialize System");

  SPI.begin();
  rfid.PCD_Init();

  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_RED, OUTPUT);

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
  bool isAuthorized = false;
  for (vector<int> key : authorizedKeys) {
    if (currentRFID == key) {
      isAuthorized = true;
      break;
    }
  }
  Serial.println(isAuthorized ? "RIFD is authorized!": "RIFD is not authorized!");
  letUserIn(isAuthorized);
}

void letUserIn(bool isAuthorized) {
  if (isAuthorized) {
      digitalWrite(LED_BLUE, HIGH);
      delay(2000);
      digitalWrite(LED_BLUE, LOW);
  } else {
      digitalWrite(LED_RED, HIGH); 
      delay(2000);
      digitalWrite(LED_RED, LOW);
  }
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