#include "Adafruit_Fingerprint.h"
#include "SoftwareSerial.h"
SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup() {
  Serial.begin(9600);
  finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println("Sidik jari Sensor Ditemukan!");
  } else {
    Serial.println("Sidik jari Sensor Tidak Ditemukan! :(");
    while (1) {
      delay(1);
    }
  }

}

void loop() {
  FINGERPRINT();
}

//----------------------------------------------------FINGERPRINt----------------------------------------------//
void FINGERPRINT() {
  //getFingerprintIDez();
  getFingerprintID();
  delay(50);
}

//----------------------------------------------------PROSES FINGERPRINT--------------------------------------//

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Gambar sidik jari diambil");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Komunikasi error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Pengambilan sidik jari error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Gambar Sidik jari diconvert");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Gambar sidik jari tidak beraturan");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Komunikasi error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Feature sidik jari tidak ditemukan");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Gambar sidik jari tidak valid");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Sidik jari yang cocok ditemukan!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Komunikasi error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Sidik jari yang cocok tidak ditemukan");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  // found a match!
  Serial.print("Ditemukan ID #"); Serial.print(finger.fingerID);
  Serial.print(" Dengan ketepatan : "); Serial.println(finger.confidence);

  return finger.fingerID;
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  // found a match!
  Serial.print("Ditemukan ID #"); Serial.print(finger.fingerID);
  Serial.print(" Dengan ketepatan : "); Serial.println(finger.confidence);
  return finger.fingerID;
}
