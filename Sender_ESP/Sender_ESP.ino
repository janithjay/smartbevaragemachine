#include<ESP8266WiFi.h>


const char* ssid = "Flavor_Flow";     // Your WiFi network SSID
const char* password = "flavorflow123"; // Your WiFi network password

WiFiServer server(80); // Create a server on port 80

String orderNumberString; // Initialize an empty String

void setup() {
  Serial.begin(9600);
  delay(10);

  // Set up Access Point
  WiFi.softAP(ssid, password);
  Serial.println();
  Serial.print("Access Point IP address: ");
  Serial.println(WiFi.softAPIP());

  server.begin();
}

void loop() {

  WiFiClient client = server.available(); // Check for incoming clients

  if (Serial.available() > 0) {
        char uniqueOrderNum = Serial.read();
        orderNumberString += String(uniqueOrderNum); // Append the character to the orderNumberString
        Serial.print("Sending Order Number: ");
        Serial.println(orderNumberString);
  }

  if (client) {
    while (client.connected()) {
      if (Serial.available() > 0) {
        char uniqueOrderNum = Serial.read();
        Serial.print("Sending Order Number: ");
        Serial.println(uniqueOrderNum);
        client.write(uniqueOrderNum); // Send the order number to the client
      }
    }
    client.stop();
    Serial.println("Client disconnected");
  }
  }
