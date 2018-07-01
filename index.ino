#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "Adafruit_Thermal.h"
#include "SoftwareSerial.h"
#define TX_PIN 12
#define RX_PIN 14

SoftwareSerial mySerial(RX_PIN, TX_PIN);
Adafruit_Thermal printer(&mySerial);

MDNSResponder mdns;

ESP8266WebServer server(80);
String webPage;
boolean hasPrintedFirstLine = false;

const char* ssid     = "";
const char* password = "";

void setup() {
  webPage += "<h1 style=\"text-align: center;font-size: 64px;font-family: arial;color: #263238;\">notes</h1>";

  mySerial.begin(9600);
  printer.begin();
  printer.setSize('S');
  printer.justify('L');

  Serial.begin(74880);
  delay(100);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  if (mdns.begin("esp8266", WiFi.localIP()))
    Serial.println("MDNS responder started");

  server.on("/", [](){
    server.send(200, "text/html", webPage);
  });
  server.on("/print", [](){
    String task = server.arg("task");
    printer.begin();
    buildTaskString(task);
    server.send(200, "text/html", webPage + "<p style=\"text-align: center;font-size: 16px;font-family: arial;color: #263238;\">" + task + "</p>");
    delay(1000);
  });

  server.begin();
  Serial.println("HTTP server started");
}

void buildTaskString (String task) {
  String line = "";
  for (int i = 0; i < task.length(); i++) {
    char letter = task[i];
    if (i == task.length() - 1) {
      printTask(line + letter);
      hasPrintedFirstLine = true;
    }
    if (line.length() >= 26) {
      printTask(line);
      hasPrintedFirstLine = true;
      line = letter;
    } else {
      line += letter;
    }
  }

  hasPrintedFirstLine = false;
}

void printTask (String taskString) {
  if (hasPrintedFirstLine) {
    String t = "    | " + taskString;
    printer.println(t);
  } else {
    String t = "(  ): " + taskString;
    printer.println(t);
  }
}

void loop() {
  server.handleClient();
}
