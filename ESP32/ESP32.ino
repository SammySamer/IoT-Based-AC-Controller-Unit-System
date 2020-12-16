#define RXD2 16
#define TXD2 17
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include <AsyncTCP.h>
#include <String>

// Replace with your network credentials
const char* ssid = "Villa 41";
const char* password = "Coolbad_7";

const char* PARAM_INPUT_1 = "input1";
const char* PARAM_INPUT_2 = "input2";
const char* PARAM_INPUT_3 = "input3";

String temp = "TEMP";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String processor(const String& var){
  //Serial.println(var);
  //Serial2.flush();
  //return (Serial2.readString());
  String test = Serial2.readStringUntil('`');
  if(test == "" || test == NULL)
  {
    return temp;
  }
  else
  {
    temp = test;
    return test;
  }
}

void setup() {
  // Note the format for setting a serial port is as follows: Serial2.begin(baud-rate, protocol, RX pin, TX pin);
  Serial.begin(38400);
  //Serial1.begin(38400, SERIAL_8N1, RXD, TXD);
  Serial2.begin(38400, SERIAL_8N1, RXD2, TXD2);
  //Serial.println("Serial Txd is on pin: "+String(TX));
  //Serial.println("Serial Rxd is on pin: "+String(RX));

  // Initialize SPIFFS
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });

  // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/index.html", String(), false, processor);
    String inputMessage;
    String inputParam;
    char tempMsg[7];
    // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
      inputMessage.toCharArray(tempMsg, 7);
      Serial2.println(tempMsg);
    }
    // GET input2 value on <ESP_IP>/get?input2=<inputMessage>
    else if (request->hasParam(PARAM_INPUT_2)) {
      inputMessage = request->getParam(PARAM_INPUT_2)->value();
      inputParam = PARAM_INPUT_2;
      inputMessage.toCharArray(tempMsg, 7);
      Serial2.println(tempMsg);
    }
    // GET input3 value on <ESP_IP>/get?input3=<inputMessage>
    else if (request->hasParam(PARAM_INPUT_3)) {
      inputMessage = request->getParam(PARAM_INPUT_3)->value();
      inputParam = PARAM_INPUT_3;
      inputMessage.toCharArray(tempMsg, 7);
      Serial2.println(tempMsg);
    }
    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    Serial.println(tempMsg);
    //request->send(200, "text/html", "HTTP GET request sent to your ESP on input field (" 
    //                                 + inputParam + ") with value: " + inputMessage +
    //                                 "<br><a href=\"/\">Return to Home Page</a>");
  });
  server.begin();
}

void loop() {
}
