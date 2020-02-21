#if defined(ESP8266)
#include <ESP8266WiFi.h>  //ESP8266 Core WiFi Library         
#else
#include <WiFi.h>      //ESP32 Core WiFi Library    
#endif
 
#if defined(ESP8266)
#include <ESP8266WebServer.h> //Local WebServer used to serve the configuration portal
#else
#include <WebServer.h> //Local WebServer used to serve the configuration portal ( https://github.com/zhouhan0126/WebServer-esp32 )
#endif
 
#include <DNSServer.h> //Local DNS Server used for redirecting all requests to the configuration portal ( https://github.com/zhouhan0126/DNSServer---esp32 )
#include <WiFiManager.h>   // WiFi Configuration Magic ( https://github.com/zhouhan0126/WIFIMANAGER-ESP32 ) >> https://github.com/tzapu/WiFiManager (ORIGINAL)

#include <WiFiClient.h> 

#include <FS.h>

const int PIN_AP = 2;
int NIVEL = 321;
int TEMPERATURA = 33;
String VALOR_RECEBIDO_TELA = "";

ESP8266WebServer server(80);

void handleRoot() {
String html ="<!DOCTYPE html> <html> <head> <title>Tela de Estudo</title> <meta charset=\"utf-8\" name=\"viewport\" content=\"width=device-width, minimumscale=1.0, maximum-scale=1.0, initial-scale=1\" /> </head> <body> <h1>Recebe Valor do Arduino:</h1> <h3>Nível: <span id=\"nivel\"></span></h3> <h3>Temperatura: <span id=\"temp\"></span></h3> <h1>Enviar valor para Arduino</h1> <input placeholder=\"Digite um Texto\" id=\"input1\"> <button onClick=\"setValor(1)\">Enviar</button> <p>Varlos Enviado: <span id=\"valorEnviado\"></span></p> <script> function getAllStatus() { console.log(\"Entrou no getAll\"); var xhttp = new XMLHttpRequest(); xhttp.onreadystatechange = function() { if (this.readyState == 4 && this.status == 200) { var obj = JSON.parse(this.responseText); document.getElementById(\"nivel\").innerHTML = obj.data[0].nivel; document.getElementById(\"temp\").innerHTML = obj.data[1].temperatura; console.log(obj) }; }; xhttp.open(\"GET\", \"data\", true); xhttp.send(); }; function setValor(valor) { console.log(\"Set Valor\"); var xhttp = new XMLHttpRequest(); xhttp.onreadystatechange = function() { if (this.readyState == 4 && this.status == 200) { document.getElementById(\"valorEnviado\").innerHTML = this.responseText; } }; var valorInput = document.getElementById(\"input1\").value; console.log(valorInput); xhttp.open(\"GET\", \"setValor?valorInput=\"+valorInput, true); xhttp.send(); }; getAllStatus(); </script> </body> </html>";

  server.send(200, "text/html", html);
}


void handleWebRequests(){
  //if(loadFromSpiffs(server.uri())) return;
  String message = "File Not Detected\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " NAME:"+server.argName(i) + "\n VALUE:" + server.arg(i) + "\n";
  }
  server.send(404, "text/html", message);
  Serial.println(message);
}

void getData() {   
   //This is a JSON formatted string that will be served. You can change the values to whatever like.
   // {"data":[{"dataValue":"1024"},{"dataValue":"23"}]} This is essentially what is will output you can add more if you like
  String text2 = "{\"data\":[";
  text2 += "{\"nivel\":\"";
  text2 += NIVEL;
  text2 += "\"},";
  text2 += "{\"temperatura\":\"";
  text2 += TEMPERATURA;
  text2 += "\"}";
  text2 += "]}";  
  Serial.println("Realizado GETALL: " + text2);
  server.send(200, "text/html", text2);
 
}


void setValor(){
  String valor = server.arg("valorInput");
  VALOR_RECEBIDO_TELA = valor;
   Serial.print("Valor Recebido da tela: ");
   Serial.print(VALOR_RECEBIDO_TELA);
    server.send(200, "text/plane", "Resposta do Arduino: " + valor); //Send web page    
}

 
void setup() {
  Serial.begin(9600);
  pinMode(PIN_AP, INPUT);
  //declaração do objeto wifiManager
  WiFiManager wifiManager;
 
  //utilizando esse comando, as configurações são apagadas da memória
  //caso tiver salvo alguma rede para conectar automaticamente, ela é apagada.
//  wifiManager.resetSettings();
 
//callback para quando entra em modo de configuração AP
  wifiManager.setAPCallback(configModeCallback); 
//callback para quando se conecta em uma rede, ou seja, quando passa a trabalhar em modo estação
  wifiManager.setSaveConfigCallback(saveConfigCallback); 
 
//cria uma rede de nome ESP_AP com senha 12345678
  wifiManager.autoConnect("ESP_AP", "12345678"); 

if(wifiManager.autoConnect()){
  delay(1000);
  Serial.print("Configuring access point...");
  
  SPIFFS.begin();
  
  /* You can remove the password parameter if you want the AP to be open. */
  
  //WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  
  server.on("/", handleRoot);
  //server.onNotFound(handleWebRequests);
  server.on("/data", getData);
  server.on("/setValor", setValor);

  server.begin();
  Serial.println("HTTP server started");
  
}}
}

void loop() 
{
   if ( digitalRead(PIN_AP) == HIGH ) 
   {
      WiFiManager wifiManager;
      if(!wifiManager.startConfigPortal("ESP_AP", "12345678") )
      {
        Serial.println("Falha ao conectar");
        delay(2000);
        ESP.restart();
      }
   }

   server.handleClient();
 
}

//callback que indica que o ESP entrou no modo AP
void configModeCallback (WiFiManager *myWiFiManager) {  
//  Serial.println("Entered config mode");
  Serial.println("Entrou no modo de configuração");
  Serial.println(WiFi.softAPIP()); //imprime o IP do AP
  Serial.println(myWiFiManager->getConfigPortalSSID()); //imprime o SSID criado da rede
 
}

//callback que indica que salvamos uma nova rede para se conectar (modo estação)
void saveConfigCallback () {
  Serial.println("Configuração salva");
}

bool loadFromSpiffs(String path){
  String dataType = "text/html";
  if(path.endsWith("/")) path += "index.htm";

  if(path.endsWith(".src")) path = path.substring(0, path.lastIndexOf("."));
  else if(path.endsWith(".html")) dataType = "text/html";
  else if(path.endsWith(".htm")) dataType = "text/html";
  else if(path.endsWith(".css")) dataType = "text/css";
  else if(path.endsWith(".js")) dataType = "application/javascript";
  else if(path.endsWith(".png")) dataType = "image/png";
  else if(path.endsWith(".gif")) dataType = "image/gif";
  else if(path.endsWith(".jpg")) dataType = "image/jpeg";
  else if(path.endsWith(".ico")) dataType = "image/x-icon";
  else if(path.endsWith(".xml")) dataType = "text/xml";
  else if(path.endsWith(".pdf")) dataType = "application/pdf";
  else if(path.endsWith(".zip")) dataType = "application/zip";
  File dataFile = SPIFFS.open(path.c_str(), "r");
  if (server.hasArg("download")) dataType = "application/octet-stream";
  if (server.streamFile(dataFile, dataType) != dataFile.size()) {
  }

  dataFile.close();
  return true;
}
