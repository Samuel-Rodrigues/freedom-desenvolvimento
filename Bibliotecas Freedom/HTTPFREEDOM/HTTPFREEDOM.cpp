#include "Arduino.h"
#include "HTTPFREEDOM.h"

void HTTPFREEDOM::handleRoot(){

	 server->send(200, "text/html", htmlText);   
}

void HTTPFREEDOM::handleRoot2(){
	 server->send(200, "text/plain", "this works as well!");   
}


void HTTPFREEDOM::handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server->uri();
  message += "\nMethod: ";
  message += (server->method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server->args();
  message += "\n";
  for (uint8_t i = 0; i < server->args(); i++) {
    message += " " + server->argName(i) + ": " + server->arg(i) + "\n";
  }
  server->send(404, "text/plain", message);
}

void HTTPFREEDOM::getData() {   
  Serial.println("Realizado getData: " + responseJson);
  server->send(200, "text/html", responseJson);
}

String HTTPFREEDOM::setValue(){
  String value = server->arg("valueInput");
  Serial.print("Valor Recebido da tela: ");
  Serial.print(value);
  Serial.println();
  server->send(200, "text/plane", "Resposta do Arduino: " + value);  
  return value; 
}

void HTTPFREEDOM::defaultSetting(String json, String html){

  htmlText = html;

  responseJson = json;
  
  server.reset(new ESP8266WebServer(WiFi.localIP(), 80));

  server->on("/", std::bind(&HTTPFREEDOM::handleRoot, this));

  server->on("/inline", std::bind(&HTTPFREEDOM::handleRoot2, this));

  server->onNotFound(std::bind(&HTTPFREEDOM::handleNotFound, this));

  server->on("/data", std::bind(&HTTPFREEDOM::getData, this));

  server->on("/setValue", std::bind(&HTTPFREEDOM::setValue, this));

  server->begin();
  Serial.println("HTTP server started");
  Serial.println(WiFi.localIP());
}

void HTTPFREEDOM::handleClient(){
  server->handleClient();
}