#include <HTTPFREEDOM.h>
#include "WiFiManager.h"  

HTTPFREEDOM httpFreedom;
WiFiManager wifiManager;

//################### Variaveis referentes aos pinos do Arduino ################### 
int NIVEL = 321;
int TEMPERATURA = 33;
String VALOR_RECEBIDO_TELA = "";

int auxD1 = 1, auxD2 = 2, auxD3 = 3, auxD4 = 4, auxD5 = 5, auxD6 = 6, auxD7 = 7;
int auxA0 = 8, auxA1 = 9, auxA2 = 10, auxA3 = 11, auxA4 = 12, auxA5 = 13;
int auxR1 = 14, auxR2 = 15, auxR3 = 16, auxR4 = 17;

//#################################################################################

String responseJson() {   
  String response = "{\"data\":[";
  response += "{\"d1\":\"";
  response += auxD1;
  response += "\"},";
  response += "{\"d2\":\"";
  response += auxD2;
  response += "\"},";
  response += "{\"d3\":\"";
  response += auxD3;
  response += "\"},";
  response += "{\"d4\":\"";
  response += auxD4;
  response += "\"},";
  response += "{\"d5\":\"";
  response += auxD5;
  response += "\"},";
  response += "{\"d6\":\"";
  response += auxD6;
  response += "\"},";
  response += "{\"d7\":\"";
  response += auxD7;
  response += "\"},";
  response += "{\"a0\":\"";
  response += auxA0;
  response += "\"},";
  response += "{\"a1\":\"";
  response += auxA1;
  response += "\"},";
  response += "{\"a2\":\"";
  response += auxA2;
  response += "\"},";
  response += "{\"a3\":\"";
  response += auxA3;
  response += "\"},";
  response += "{\"a4\":\"";
  response += auxA4;
  response += "\"},";
  response += "{\"a5\":\"";
  response += auxA5;
  response += "\"},";
  response += "{\"r1\":\"";
  response += auxR1;
  response += "\"},";
  response += "{\"r2\":\"";
  response += auxR2;
  response += "\"},";
  response += "{\"r3\":\"";
  response += auxR3;
  response += "\"},";
  response += "{\"r4\":\"";
  response += auxR4;
  response += "\"}";
  response += "]}";
  return response;
}

void setup() {

  Serial.begin(115200);
  
 // wifiManager.resetSettings();//reseta a eeprom para voltar pro modo AP, deixe comentando.

  wifiManager.autoConnect();

  httpFreedom.defaultSetting(responseJson(), "<!DOCTYPE html> <html> <head> <title>Tela de Estudo</title> <meta charset=\"utf-8\" name=\"viewport\" content=\"width=device-width, minimumscale=1.0, maximum-scale=1.0, initial-scale=1\" /> </head> <style> /* Estilo iOS */ .switch__container { margin: 15px auto; width: 60px; } .switch { visibility: hidden; position: absolute; margin-left: -9999px; } .switch + label { display: block; position: relative; cursor: pointer; outline: none; user-select: none; } .switch--shadow + label { padding: 2px; width: 60px; height: 30px; background-color: #dddddd; border-radius: 60px; } .switch--shadow + label:before, .switch--shadow + label:after { display: block; position: absolute; top: 1px; left: 1px; bottom: 1px; content: \"\"; } .switch--shadow + label:before { right: 1px; background-color: #f1f1f1; border-radius: 30px; transition: background 0.4s; } .switch--shadow + label:after { width: 30px; background-color: #fff; border-radius: 100%; box-shadow: 0 2px 5px rgba(0, 0, 0, 0.3); transition: all 0.4s; } .switch--shadow:checked + label:before { background-color: #8ce196; } .switch--shadow:checked + label:after { transform: translateX(30px); } </style> <body><!-- Corpo da pagina --> <h1>Freedom Board</h1> <table> <tr> <th>Pinos Digitais </th> <th>Pinos Analógicas </th> <th>Saídas a Relé</th> </tr> <tr> <td><h3>D1: <span id=\"d1\"></span></h3></td> <td><h3>A0: <span id=\"a0\"></span></h3></td> <td><h3>R1: <span id=\"r1\"></span></h3></td> </tr> <tr> <td><h3>D2: <span id=\"d2\"></span></h3></td> <td><h3>A0: <span id=\"a1\"></span></h3></td> <td><h3>R2: <span id=\"r2\"></span></h3></td> </tr> <tr> <td><h3>D3: <span id=\"d3\"></span></h3></td> <td><h3>A0: <span id=\"a2\"></span></h3></td> <td><h3>R3: <span id=\"r3\"></span></h3></td> </tr> <tr> <td><h3>D4: <span id=\"d4\"></span></h3></td> <td><h3>A0: <span id=\"a3\"></span></h3></td> <td><h3>R4: <span id=\"r4\"></span></h3></td> </tr> <tr> <td><h3>D5: <span id=\"d5\"></span></h3></td> <td><h3>A0: <span id=\"a4\"></span></h3></td> <td></td> </tr> <tr> <td><h3>D6: <span id=\"d6\"></span></h3></td> <td><h3>A0: <span id=\"a5\"></span></h3></td> <td></td> </tr> <tr> <td><h3>D7: <span id=\"d7\"></span></h3></td> <td></td> <td></td> </tr> </table> <div class=\"switch__container\" style=\"display: inline-flex;\"> <input onClick=\"FaroisOnOff(checked)\" id=\"switch-shadow-r1\" class=\"switch switch--shadow\" type=\"checkbox\"> <label for=\"switch-shadow-r1\"></label> </div> <div class=\"switch__container\" style=\"display: inline-flex;\"> <input onClick=\"FaroisOnOff(checked)\" id=\"switch-shadow-farois\" class=\"switch switch--shadow\" type=\"checkbox\"> <label for=\"switch-shadow-farois\"></label> </div> <div class=\"switch__container\" style=\"display: inline-flex;\"> <input onClick=\"FaroisOnOff(checked)\" id=\"switch-shadow-r3\" class=\"switch switch--shadow\" type=\"checkbox\"> <label for=\"switch-shadow-r3\"></label> </div> <div class=\"switch__container\" style=\"display: inline-flex;\"> <input onClick=\"FaroisOnOff(checked)\" id=\"switch-shadow-r4\" class=\"switch switch--shadow\" type=\"checkbox\"> <label for=\"switch-shadow-r4\"></label> </div> <div> <h1>Terminal</h1> <input placeholder=\"Digite um Texto\" id=\"input1\"> <button onClick=\"setValor(1)\">Enviar</button> <p>Varlos Enviado: <span id=\"valorEnviado\"></span></p> </div> <script> function getAllStatus() { console.log(\"Entrou no getAll\"); var xhttp = new XMLHttpRequest(); xhttp.onreadystatechange = function() { if (this.readyState == 4 && this.status == 200) { var obj = JSON.parse(this.responseText); document.getElementById(\"d1\").innerHTML = obj.data[0].d1; document.getElementById(\"d2\").innerHTML = obj.data[1].d2; document.getElementById(\"d3\").innerHTML = obj.data[2].d3; document.getElementById(\"d4\").innerHTML = obj.data[3].d4; document.getElementById(\"d5\").innerHTML = obj.data[4].d5; document.getElementById(\"d6\").innerHTML = obj.data[5].d6; document.getElementById(\"d7\").innerHTML = obj.data[6].d7; document.getElementById(\"a0\").innerHTML = obj.data[7].a0; document.getElementById(\"a1\").innerHTML = obj.data[8].a1; document.getElementById(\"a2\").innerHTML = obj.data[9].a2; document.getElementById(\"a3\").innerHTML = obj.data[10].a3; document.getElementById(\"a4\").innerHTML = obj.data[11].a4; document.getElementById(\"a5\").innerHTML = obj.data[12].a5; document.getElementById(\"r1\").innerHTML = obj.data[13].r1; document.getElementById(\"r2\").innerHTML = obj.data[14].r2; document.getElementById(\"r3\").innerHTML = obj.data[15].r3; document.getElementById(\"r4\").innerHTML = obj.data[16].r4; console.log(obj) }; }; xhttp.open(\"GET\", \"data\", true); xhttp.send(); }; function setValor(valor) { console.log(\"Set Valor\"); var xhttp = new XMLHttpRequest(); xhttp.onreadystatechange = function() { if (this.readyState == 4 && this.status == 200) { document.getElementById(\"valorEnviado\").innerHTML = this.responseText; } }; var valorInput = document.getElementById(\"input1\").value; console.log(valorInput); xhttp.open(\"GET\", \"setValor?valorInput=\"+valorInput, true); xhttp.send(); }; getAllStatus(); </script> </body> </html>");

}

void loop() {
  httpFreedom.handleClient();

}
