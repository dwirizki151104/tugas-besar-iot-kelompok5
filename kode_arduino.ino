#include <WiFi.h>
  #include <PubSubClient.h>

  // Pin untuk sensor warna
  #define S0 5
  #define S1 18
  #define S2 2
  #define S3 4
  #define OUT 15

  // Pin untuk LED
  #define LED_PIN 19

  // Variabel untuk menyimpan nilai RGB
  int red = 0, green = 0, blue = 0;

  // WiFi credentials
  const char* ssid = "Sandinya 87654321";  // Ganti dengan SSID WiFi Anda
  const char* password = "87654321";       // Ganti dengan password WiFi Anda

  // MQTT broker settings
  const char* mqtt_server = "mqtt.my.id";  // Ganti dengan alamat broker MQTT Anda
  const int mqtt_port = 1883;              // Port MQTT (1883 untuk non-SSL)
  const char* mqtt_topic = "kotakamalpintar";  // Topik untuk mengirim data

  WiFiClient espClient;
  PubSubClient client(espClient);

  void setup() {
    // Inisialisasi serial monitor
    Serial.begin(115200);

    // Konfigurasi pin sensor
    pinMode(S0, OUTPUT);
    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);
    pinMode(OUT, INPUT);

    // Konfigurasi pin LED
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW); // Matikan LED saat mulai

    // Set frequency scaling ke 20%
    digitalWrite(S0, HIGH);
    digitalWrite(S1, LOW);

    // Koneksi ke WiFi
    connectWiFi();

    // Konfigurasi MQTT client
    client.setServer(mqtt_server, mqtt_port);
  }

  void loop() {
    // Pastikan koneksi MQTT tetap aktif
    if (!client.connected()) {
      reconnectMQTT();
    }
    client.loop();

    // Baca komponen warna
    digitalWrite(S2, LOW);
    digitalWrite(S3, LOW);
    red = pulseIn(OUT, LOW);

    digitalWrite(S2, HIGH);
    digitalWrite(S3, HIGH);
    green = pulseIn(OUT, LOW);

    digitalWrite(S2, LOW);
    digitalWrite(S3, HIGH);
    blue = pulseIn(OUT, LOW);

    // Print RGB values untuk debugging
    Serial.print("Red: ");
    Serial.print(red);
    Serial.print(" Green: ");
    Serial.print(green);
    Serial.print(" Blue: ");
    Serial.println(blue);

    // Deteksi warna dan kirim data ke MQTT
    String color = detectColor();
    int nominal = uang();
    if (nominal != 0) {
      String message = String(nominal);
      if (client.publish(mqtt_topic, message.c_str())) {
        Serial.println("Message sent to MQTT: " + message);
      } else {
        Serial.println("Failed to send message to MQTT");
      }

      Serial.println("Detected: " + color);

      // Nyalakan LED
      digitalWrite(LED_PIN, HIGH);
      delay(1000); // LED menyala selama 1 detik
      digitalWrite(LED_PIN, LOW);
    }

    delay(6000);  // Delay untuk membaca data sensor setiap detik
  }

  // Fungsi untuk koneksi WiFi
  void connectWiFi() {
    Serial.print("Connecting to WiFi...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("\nWiFi connected");
  }

  // Fungsi untuk koneksi ulang MQTT
  void reconnectMQTT() {
    while (!client.connected()) {
      Serial.print("Connecting to MQTT...");
      if (client.connect("ESP32Client")) {
        Serial.println("connected");
      } else {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
        delay(5000);
      }
    }
  }

  // Fungsi untuk mendeteksi warna berdasarkan nilai RGB
  int uang() {
    if (blue > red && blue > green && blue > 200) { 
      return 100000;
    } else if (red > blue && green > blue && red > 200 && green > 150) { 
      return 50000;
    } else if (green > red && green > blue && green > 200) { 
      return 5000;
    } else if (blue > green && red > blue && green > 200) { 
      return 10000;
    } else if (red < 50 && green < 50 && blue < 50) { 
      return 20000;
    } else if (red > 100 && green > 100 && blue > 100) {
      return 2000;
    } else {
      return 0;  // Tidak ada warna yang dikenali
    }
  }
  // Fungsi untuk mendeteksi warna berdasarkan nilai RGB
  String detectColor() {
    if (blue > red && blue > green && blue > 200) { 
      return "Uang yang masuk 100,000";
    } else if (red > blue && green > blue && red > 200 && green > 150) { 
      return "Uang yang masuk 50,000";
    } else if (green > red && green > blue && green > 200) { 
      return "Uang yang masuk 5,000";
    } else if (blue > green && red > blue && green > 200) { 
      return "Uang yang masuk 10,000";
    } else if (red < 50 && green < 50 && blue < 50) { 
      return "Uang yang masuk 20,000";
    } else if (red > 100 && green > 100 && blue > 100) {
      return "Uang yang masuk 2,000";
    } else {
      return "";  // Tidak ada warna yang dikenali
    }
  }