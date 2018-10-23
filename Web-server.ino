#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid = "DESKTOP-C0SM8HK 1763";
const char* password = ";R363a16";
const char* host = "127.0.0.1";
const int port = 5000;


const int Temperature = 18;
const int Humidity = 56;

ESP8266WebServer server(80);

const int led = 13;

void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/plain", "hello from esp8266!");
  digitalWrite(led, 0);
}

void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}


void setup(void) {
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.on("/register", handleRegister);
  server.on("/availableData", availableData);
  server.on("/getData", getData);

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();

    
}

void handleRegister(){
  //Device Register, Post and get method
  String json = "{\"Location\":\"Mures\",\"DeviceName\":\"NodeMcu v3\"}";
  if(server.hasArg("plain")==false){
    server.send(200, "application/json", json);
    return;
  }

  String message = "Body recived:\n";
  message += server.arg("plain");
  message += "\n";

  server.send(200, "text/plain", message);
  Serial.println(message);
}

void availableData(){
  String json = "{\"Temperature\":\"1\",\"Humidity\":\"2\"}";
  if(server.hasArg("plain") == false){
    server.send(200, "application/json", json);
    return;
  }
}

void getData(){
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["Temperature"] = Temperature;
  root["Humidity"] = Humidity;
  String json;
  root.prettyPrintTo(json);
  if(server.hasArg("plain") == false){
    server.send(200, "application/json", json);
    return;
  }
}
