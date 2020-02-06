#include "Adafruit_Fingerprint.h"
#include <SoftwareSerial.h>

SoftwareSerial mySerial(2,3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
uint8_t id;

void setup() {
  Serial.begin(9600);
  finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println("FingerPrint Sensor Ditemukan!");
  } else {
    Serial.println("FingerPrint Sensor Tidak Ditemukan! :(");
    while (1) {
      delay(1);
    }
  }

}

void loop() {
  ENROLL();
}

//---------------------------------------ENROLL--------------------------------------------------------------//
void ENROLL() {
  Serial.println("Bersiap untuk mendaftarkan Sidik jari!");
  Serial.println("Silahkan tulis ID # (1 sampai 127) untuk menyimpan sidik jari...");
  id = readnumber();
  if (id == 0) {// ID #0 not allowed, try again!
    return;
  }
  Serial.print("Mendaftarkan sidik jari ID #");
  Serial.println(id);

  while (!  getFingerprintEnroll() );
}

//------------------------------------PROSES ENROLL----------------------------------------------------------//

uint8_t readnumber(void) {
  uint8_t num = 0;

  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}

//----RETURN NUM-------------------//

uint8_t getFingerprintEnroll() {
  int p = -1;
  Serial.print("Menunggu sidik jari yang valid untuk di daftarkan sebagai ID#"); Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Gambar Sidik jari Sudah Diambil");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.println(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Komunikasi error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Pengambilan gambar error");
        break;
      default:
        Serial.println("Unknown error");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Gambar sidik jari sudah diconvert");
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

  Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Silahkan letakkan jari lagi");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Gambar sidik jari diambil");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.print("Sidik Jari tidak ada");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("komunikasi error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Pengambilan gambar error");
        break;
      default:
        Serial.println("Unknown error");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Gambar sidik jari sudah diconvert");
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
  Serial.print("Creating model for #");  Serial.println(id);

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Sidik jari cocok!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Komunikasi error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Sidik jari tidak cocok");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Sidik jari tersimpan!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Komunikasi error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Sidik jadi tidak tersimpan pada lokasi");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error ketika menulis ke Flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }
}
