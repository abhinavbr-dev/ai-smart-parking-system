#include <WiFi.h>
#include <Firebase_ESP_Client.h>

// WiFi credentials
#define WIFI_SSID "YOUR_WIFI_NAME"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

// Firebase credentials
#define API_KEY"AIzaSyDTAas1droAkeShhrnvoJhDCc74uJQhEA"
"
#define DATABASE_URL "https://esp32-parking-8feef-default-rtdb.firebaseio.com/"

// IR sensor pins
#define IR1 25
#define IR2 26
#define IR3 27

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

void setup() {
  Serial.begin(115200);

  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  pinMode(IR3, INPUT);

  // WiFi connection
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nWiFi Connected");

  // Firebase configuration
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  int slot1 = digitalRead(IR1);
  int slot2 = digitalRead(IR2);
  int slot3 = digitalRead(IR3);

  // Convert to parking logic
  // 1 = Occupied, 0 = Free
  int status1 = (slot1 == LOW) ? 1 : 0;
  int status2 = (slot2 == LOW) ? 1 : 0;
  int status3 = (slot3 == LOW) ? 1 : 0;

  Firebase.RTDB.setInt(&fbdo, "/parking/slot1", status1);
  Firebase.RTDB.setInt(&fbdo, "/parking/slot2", status2);
  Firebase.RTDB.setInt(&fbdo, "/parking/slot3", status3);

  Serial.println("Data Sent:");
  Serial.print("Slot1: "); Serial.println(status1);
  Serial.print("Slot2: "); Serial.println(status2);
  Serial.print("Slot3: "); Serial.println(status3);

  delay(2000);
}
