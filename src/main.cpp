#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>


// Variabel Wi-Fi
#define ssid  "Wokwi-GUEST"
#define pass  ""

// Firebase
#define DATABASE_URL "https://latihan-kirim-data-esp-e9c34-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define API_KEY "AIzaSyCyJ2Lv-8sFr--k588Y1-1bI3LRN7zwVpA"

FirebaseData firebaseData;
FirebaseConfig firebaseConfig;
FirebaseAuth firebaseAuth;

// Deklarasi LCD
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Konfigurasi PIR dan LED
const int pir1RuangTamu = 14;  // PIR sensor ruang tamu
const int pir2RuangKerja = 27; // PIR sensor ruang kerja
const int led1RuangTamu = 23;  // LED ruang tamu
const int led2RuangKerja = 19; // LED ruang kerja


// Fungsi untuk mengirim data ke Firebase
void sendToFirebase(bool a, bool b) {

  bool led1State = digitalRead(led1RuangTamu);
  bool led2State = digitalRead(led2RuangKerja);

  // Kirim data ke Firebase
  Firebase.RTDB.setBool(&firebaseData, "sensor/pir1", a);
  Firebase.RTDB.setBool(&firebaseData, "sensor/pir2", b);
  Firebase.RTDB.setBool(&firebaseData, "akuator/led1", led1State);
  Firebase.RTDB.setBool(&firebaseData, "akuator/led2", led2State);

  // Debug
  Serial.println("Data dikirim ke Firebase!");
}

// Fungsi untuk mengambil data dari Firebase
void getDataFirebase() {
  // Ambil status PIR1 dari Firebase
  if (Firebase.RTDB.getBool(&firebaseData, "sensor/pir1")) {
    Serial.print("PIR1 Firebase: ");
    Serial.println(firebaseData.boolData() ? "Aktif" : "Tidak aktif");
  } else {
    Serial.println("Gagal mendapatkan data PIR1!");
  }

  // Ambil status PIR2 dari Firebase
  if (Firebase.RTDB.getBool(&firebaseData, "sensor/pir2")) {
    Serial.print("PIR2 Firebase: ");
    Serial.println(firebaseData.boolData() ? "Aktif" : "Tidak aktif");
  } else {
    Serial.println("Gagal mendapatkan data PIR2!");
  }

  // Ambil status LED1 dari Firebase
  if (Firebase.RTDB.getBool(&firebaseData, "akuator/led1")) {
    Serial.print("LED1 Firebase: ");
    Serial.println(firebaseData.boolData() ? "ON" : "OFF");
  } else {
    Serial.println("Gagal mendapatkan data LED1!");
  }

  // Ambil status LED2 dari Firebase
  if (Firebase.RTDB.getBool(&firebaseData, "akuator/led2")) {
    Serial.print("LED2 Firebase: ");
    Serial.println(firebaseData.boolData() ? "ON" : "OFF");
  } else {
    Serial.println("Gagal mendapatkan data LED2!");
  }
}


void setup() {
  // Inisialisasi Serial dan LCD
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  // Inisialisasi pin
  pinMode(pir1RuangTamu, INPUT);
  pinMode(pir2RuangKerja, INPUT);
  pinMode(led1RuangTamu, OUTPUT);
  pinMode(led2RuangKerja, OUTPUT);

  // Tampilkan pesan awal di LCD
  lcd.setCursor(0, 0);
  lcd.print("Menghubungkan WiFi");
  Serial.println("Menghubungkan ke Wi-Fi...");

  // Hubungkan ke Wi-Fi
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Konfigurasi Firebase
  firebaseConfig.api_key = API_KEY;
  firebaseConfig.database_url = DATABASE_URL;
  Firebase.signUp(&firebaseConfig, &firebaseAuth, "", "");
  firebaseConfig.token_status_callback = tokenStatusCallback;

  Firebase.begin(&firebaseConfig, &firebaseAuth);
  Firebase.reconnectWiFi(true);

  // Tampilkan koneksi berhasil di LCD
  lcd.setCursor(0, 1);
  lcd.print("WiFi Terhubung!");
  Serial.println("\nWiFi dan Firebase terhubung!");
}

void loop() {
  // Membaca status dari PIR sensor
  bool pir1State = digitalRead(pir1RuangTamu);
  bool pir2State = digitalRead(pir2RuangKerja);

  // Logika untuk PIR ruang tamu
  if (pir1State == HIGH) {
    digitalWrite(led1RuangTamu, HIGH);
    lcd.setCursor(0, 0);
    lcd.print("R.Tamu: Gerakan! ");
    lcd.setCursor(0, 1);
    lcd.print("LED1: ON         ");
    Serial.println("Ruang Tamu: Gerakan terdeteksi, LED1 ON");
  } else {
    digitalWrite(led1RuangTamu, LOW);
    lcd.setCursor(0, 0);
    lcd.print("R.Tamu: Tidak!   ");
    lcd.setCursor(0, 1);
    lcd.print("LED1: OFF        ");
    Serial.println("Ruang Tamu: Tidak ada gerakan, LED1 OFF");
  }

  // Logika untuk PIR ruang kerja
  if (pir2State == HIGH) {
    digitalWrite(led2RuangKerja, HIGH);
    lcd.setCursor(0, 2);
    lcd.print("R.Kerja: Gerakan!");
    lcd.setCursor(0, 3);
    lcd.print("LED2: ON         ");
    Serial.println("Ruang Kerja: Gerakan terdeteksi, LED2 ON");
  } else {
    digitalWrite(led2RuangKerja, LOW);
    lcd.setCursor(0, 2);
    lcd.print("R.Kerja: Tidak!  ");
    lcd.setCursor(0, 3);
    lcd.print("LED2: OFF        ");
    Serial.println("Ruang Kerja: Tidak ada gerakan, LED2 OFF");
  }

  // Kirim data ke Firebase 
  sendToFirebase(pir1State, pir2State);

  delay(1000);  // Delay 1 detik
}

