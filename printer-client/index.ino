#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "Adafruit_Thermal.h"
#include "SoftwareSerial.h"

#include "sun.h"
#include "rain.h"
#include "sunClouds.h"
#include "lightning.h"
#include "clouds.h"

#define TX_PIN 12
#define RX_PIN 14

SoftwareSerial mySerial(RX_PIN, TX_PIN);
Adafruit_Thermal printer(&mySerial);

MDNSResponder mdns;

ESP8266WebServer server(80);
boolean hasPrintedFirstLine = false;

const char* ssid     = "bergenrot";
const char* password = "IzabellaRichardHarry";

void setup() {

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
    server.send(200, "text/html", "<p>ok</p>");
  });
  server.on("/print/task-item", [](){
    String task = server.arg("task");
    printer.begin();
    buildTaskString(task);
    server.send(200, "text/html", "<p>ok</p>");
  });

  server.on("/print/title", [](){
    String title = server.arg("title");
    printer.begin();
    printTitle(title);
    server.send(200, "text/html", "<p>ok</p>");
  });

  server.on("/print/line", [](){
    String text = server.arg("text");
    printer.begin();
    printLine(text);
    server.send(200, "text/html", "<p>ok</p>");
  });

  server.on("/print/feed", [](){
    String title = server.arg("amount");
    printer.begin();
    feed(title.toInt());
    server.send(200, "text/html", "<p>ok</p>");
  });

  server.on("/print/weather", [](){
    String type = server.arg("type");
    printer.wake();
    printer.begin();
    printer.setDefault();
    printWeather(type);
    server.send(200, "text/html", "<p>ok</p>");
    printer.sleep();
  });

  server.begin();
  Serial.println("HTTP server started");
}

void printWeather (String type) {
  if (type == "rain") {
    printer.printBitmap(200, 200, rain_data);
  }

  if (type == "sun") {
    printer.printBitmap(75, 75, sun_data);
  }

  if (type == "lightning") {
    printer.printBitmap(75, 75, lightning_data);
  }

  if (type == "sunClouds") {
    printer.printBitmap(75, 75, sunClouds_data);
  }

  if (type == "clouds") {
    printer.printBitmap(75, 75, clouds_data);
  }
}

void feed (int feedValue) {
  printer.feed(feedValue);
}

void printLine (String text) {
  printer.println(text);
  printer.setDefault();
}

void printTitle (String text) {
  printer.boldOn();
  printer.underlineOn();
  printer.setSize('L');
  printer.println(text);
  printer.feed(1);
  printer.setDefault();
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
