#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#define BLYNK_TEMPLATE_ID "TMPL6mfoqd_c2"
#define BLYNK_TEMPLATE_NAME "polusi udara"
#define BLYNK_FIRMWARE_VERSION "1.3.2"
#define APP_DEBUG
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

// Definisi pin
#define DHTPIN 4       // Pin DHT11 terhubung ke pin D2
#define DHTTYPE DHT11   // Tipe sensor DHT11
#define LEDPIN 2       // Pin LED terhubung ke pin D4
#define BUZZERPIN 15    // Pin Buzzer terhubung ke pin D8
#define MQ135PIN A0     // Pin MQ135 terhubung ke pin A0

// Inisialisasi DHT sensor
DHT dht(DHTPIN, DHTTYPE);

BlynkTimer timer;
bool isconnected = false;

// Masukkan Auth Token dari Blynk
char auth[] = "-t2LsxD0XL7zzMLyR8Us1yvPvct2rMGh";

// Masukkan SSID dan password WiFi
char ssid[] = "Victor15pro";
char pass[] = "victor123";

// Batasan untuk deteksi CO
const int safeThreshold = 10;   // Nilai batas aman CO (disesuaikan dengan kalibrasi)
const int dangerThreshold = 15; // Nilai batas berbahaya CO (disesuaikan dengan kalibrasi)

void setup() {
  // Inisialisasi serial komunikasi
  Serial.begin(115200);

  // Memulai koneksi ke Blynk
  Blynk.begin(auth, ssid, pass);

  timer.setInterval(2000L, checkBlynkStatus);
  timer.setInterval(2000L, sendSensor);

  // Inisialisasi sensor DHT11
  dht.begin();

  // Inisialisasi pin LED dan Buzzer sebagai output
  pinMode(LEDPIN, OUTPUT);
  pinMode(BUZZERPIN, OUTPUT);
}

void loop()
{
  // Menggunakan Blynk untuk menjalankan loop
  Blynk.run();
  timer.run();
}

void checkBlynkStatus() {
  isconnected = Blynk.connected();
  if (isconnected == true) {
    //digitalWrite(wifiled, HIGH);
    Serial.println("Blynk Connected");
  }
  else {
    //digitalWrite(wifiled, LOW);
    Serial.println("Blynk Not Connected");
  }
}

void sendSensor() {
  // Membaca data dari sensor DHT11
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Membaca nilai dari sensor MQ135 (kalibrasi untuk deteksi CO)
  int coLevel = analogRead(MQ135PIN);
  int humidity = analogRead(DHTPIN);
  
  // Print data to Serial Monitor
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %\t");
  Serial.print("Air: ");
  Serial.print(coLevel);
  Serial.println(" PPM");
  
  // Mengirim data ke Blynk
  Blynk.virtualWrite(V1, t);
  Blynk.virtualWrite(V2, h);
  Blynk.virtualWrite(V3, coLevel);

  // Mengecek kualitas udara terkait CO
  if (coLevel > dangerThreshold) {
    // Level CO berbahaya
    digitalWrite(LEDPIN, HIGH);
    digitalWrite(BUZZERPIN, HIGH);
  } else if (coLevel > safeThreshold) {
    // Level CO terancam
    digitalWrite(LEDPIN, HIGH);
    digitalWrite(BUZZERPIN, LOW);
  } else {
    // Level CO aman
    digitalWrite(LEDPIN, LOW);
    digitalWrite(BUZZERPIN,LOW);
	}
}