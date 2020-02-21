#ifndef HTTPFREEDOM_H
#define HTTPFREEDOM_H

#include "Arduino.h"
#include <ESP8266WebServer.h>

extern "C" {
  #include "user_interface.h"
}

class HTTPFREEDOM
{
    public:
        void handleRoot();
        void handleRoot2();        
        void handleNotFound();
        void getData();
        String setValue();
        void defaultSetting(String json, String html);
        void handleClient();      
        std::unique_ptr<ESP8266WebServer> server;
        String htmlText;
        String responseJson;
};

#endif