#include <ESP8266WiFi.h> 
#include <ThingerESP8266.h>
#include <DHT.h> // Jika menggunakan sensor DHT

#define USERNAME "ujaipahcoi"
#define DEVICE_ID "Postest3_Kelompok5"
#define DEVICE_CREDENTIAL "6R4DO@1-TyoJIrn-"

#define SSID "amin" //Hotspot yang kita pakai
#define SSID_PASSWORD "12345678"

#define DHTPIN D4     // Pin yang terhubung ke output sensor DHT
#define DHTTYPE DHT11 // Tipe sensor DHT yang digunakan

#define LED_RED D2   // Pin untuk LED merah
#define LED_GREEN D1 // Pin untuk LED hijau
#define LED_YELLOW D0  // Pin untuk LED biru
#define BUZZER_PIN D7 // Pin untuk buzzer

#define TEMPERATURE_THRESHOLD 36
#define NORMAL_TEMP_LOWER 30

DHT dht(DHTPIN, DHTTYPE);
ThingerESP8266 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

void setup() {
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  thing.add_wifi(SSID, SSID_PASSWORD);
  dht.begin();
  
  thing["temperature"] >> [](pson &out) {
    out = dht.readTemperature();
  };
}

void loop() {
  thing.handle();

  float temperature = dht.readTemperature();

  if (!isnan(temperature)) {
    if (temperature > TEMPERATURE_THRESHOLD) {
      digitalWrite(LED_RED, HIGH);
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_YELLOW, LOW);
      tone(BUZZER_PIN, 1000); // Nyalakan buzzer
      // Sistem akan memberikan peringatan suara di sini
    } else if (temperature >= NORMAL_TEMP_LOWER) {
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_YELLOW, HIGH);
      noTone(BUZZER_PIN); // Matikan buzzer
    } else {
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_GREEN, HIGH);
      digitalWrite(LED_YELLOW, LOW);
      noTone(BUZZER_PIN); // Matikan buzzer
    }
  }

  delay(3000); // Perbarui setiap 5 detik
}