// Based on https://arduinogetstarted.com/tutorials/arduino-uno-r4-wifi-controls-relay-via-web
#include <WiFiS3.h>
#include "./secrets.h"

// TODO: pin definition map
#define REGION_1_PIN 2

const char ssid[] = SECRET_SSID;
const char pass[] = SECRET_PASS;

int status = WL_IDLE_STATUS;

WiFiServer server(80);

void setup() {
  Serial.begin(9600);
  setPins();
  setupWiFi();
}

// Define pins for each watering region, etc.
void setPins() {
  pinMode(REGION_1_PIN, OUTPUT);
}

// Connects to the wifi network.
void setupWiFi() {
  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);

    delay(10000);
  }
  server.begin();
  printWifiStatus();
}

void loop() {
  // Listen
  WiFiClient client = server.available();
  if (client) {
    String HTTP_req = "";
    while (client.connected()) {
      if (client.available()) {
        Serial.println("New HTTP Request");
        HTTP_req = client.readStringUntil('\n');  // read the first line of HTTP request
        Serial.print("<< ");
        Serial.println(HTTP_req);  // print HTTP request to Serial Monitor
        break;
      }
    }

    // read the remaining lines of HTTP request header
    while (client.connected()) {
      if (client.available()) {
        String HTTP_header = client.readStringUntil('\n');  // read the header line of HTTP request

        if (HTTP_header.equals("\r"))  // the end of HTTP request
          break;

        Serial.print("<< ");
        Serial.println(HTTP_header);  // print HTTP request to Serial Monitor
      }
    }

    // Read requests for regions
    // Expected values: 
    // - POST region/1 to turn on
    // - DELETE region/1 to turn off
    if (HTTP_req.indexOf("region") > -1) {
      writeRegion(&HTTP_req);
    }

    // Send HTTP Response
    sendResponse(&client);
  }
}

// Routes a region request and turns it on or off.
void writeRegion(String *req) {
  // Decide whether region will be on or off. Other types of request do not affect it. Saving those for future features.
  PinStatus output = LOW;
  if (req->indexOf("POST") > -1) {
    output = HIGH;
  } else if (req->indexOf("DELETE") == -1) {
    // Don't do anything if the request is not a DELETE either.
    return;
  }

  // Check the first region.
  if (req->indexOf("region/1") > -1) {
    digitalWrite(REGION_1_PIN, output);
  }
}

// Send a 200 response and close the connection.
void sendResponse(WiFiClient *client) {
    client->println("HTTP/1.1 200 OK");
    client->println("Content-Type: text/html");
    client->println("Connection: close");  // the connection will be closed after completion of the response
    client->println();
    client->println("");                     // the separator between HTTP header and body
    client->flush();

    delay(10);
    client->stop();
}

void printWifiStatus() {
  // print your board's IP address:
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // print the received signal strength:
  Serial.print("signal strength (RSSI):");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");
}