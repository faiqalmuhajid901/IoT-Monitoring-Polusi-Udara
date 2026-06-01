#include <ESP8266WiFi.h>

#define MQ135PIN A0   // Pin MQ135 terhubung ke pin A0

// Waktu stabilisasi (dalam milidetik)
const unsigned long stabilizeTime = 300000;  // 5 menit (300.000 ms)

// Waktu kalibrasi (dalam milidetik)
const unsigned long calibrationTime = 60000; // 1 menit (60.000 ms)

// Variabel untuk menyimpan hasil kalibrasi
float baseLineValue = 0;   // Nilai baseline untuk udara bersih
int sampleCount = 0;

void setup() {
  // Inisialisasi serial komunikasi
  Serial.begin(115200);

  // Stabilkan sensor dengan memberikan waktu pemanasan (burn-in time)
  Serial.println("Stabilizing sensor, please wait...");
  delay(stabilizeTime);
  Serial.println("Sensor stabilization complete.");
}

void loop() {
  // Reset variabel kalibrasi
  baseLineValue = 0;
  sampleCount = 0;

  // Proses kalibrasi
  Serial.println("Starting calibration process...");

  unsigned long startTime = millis();

  while (millis() - startTime < calibrationTime) {
    int sensorValue = analogRead(MQ135PIN);
    baseLineValue += sensorValue;
    sampleCount++;
    
    Serial.print("Sensor Value: ");
    Serial.println(sensorValue);

    delay(1000); // Delay 1 detik antara pembacaan
  }

  // Menghitung nilai baseline rata-rata
  baseLineValue = baseLineValue / sampleCount;

  Serial.print("Calibration complete. Baseline value: ");
  Serial.println(baseLineValue);

  // Anda bisa menyimpan nilai baseline ini untuk digunakan sebagai threshold pada program deteksi
  // atau gunakan untuk mendeteksi jika udara bersih.
  
  delay(10000); // Delay sebelum kalibrasi berikutnya
}