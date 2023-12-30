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

    if (client) {
      while (client.connected()) {
        if (Serial.available() > 0) {
          String orderNumberString = Serial.readStringUntil('\n'); // Read the entire order number from Serial until '\n'
          
          if (orderNumberString.length() > 0) {
            Serial.print("Sending Order Number: ");
            Serial.println(orderNumberString);

            // Send the order number to the client
            client.println(orderNumberString);

            delay(1000); // Optionally, add a delay or logic to avoid spamming the client
          }
        }
      }
      client.stop();
      Serial.println("Client disconnected");
    }
  }
