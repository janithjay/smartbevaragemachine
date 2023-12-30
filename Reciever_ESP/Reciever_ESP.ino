#include <ESP8266WiFi.h>

const char* ssid = "Flavor_Flow";     // Access Point SSID
const char* password = "flavorflow123"; // Access Point password
const char* serverAddress = "192.168.4.1"; // IP address of the AP (Sender NodeMCU)

WiFiClient client;

void setup() {
  Serial.begin(9600);
  delay(10);

  // Connect to the Access Point
  Serial.println();
  Serial.println("Connecting to AP");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (!client.connected()) {
    Serial.println("Connecting to sender NodeMCU");
    if (client.connect(serverAddress, 80)) {
      Serial.println("Connected to sender NodeMCU");
    }
  }

  if (client.connected()) {
    while (client.available()) {
      String receivedData = client.readStringUntil('\n'); // Read the received string until newline character

      Serial.print("Received Order Number: ");
      Serial.println(receivedData);
      // Process or use the received order number as needed
    }
  }
}
