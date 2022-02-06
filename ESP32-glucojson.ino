#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <AS726X.h>

AS726X sensor;

byte GAIN = 2;
byte MEASUREMENT_MODE = 2;

int CANT = 7;

// JSON data buffer
StaticJsonDocument<850> jsonDocument;
char buffer[850];

//const char *ssid = "Fibertel WiFi960 2.4GHz";
//const char *password = "00439895925";

const char *ssid = "FliaMendoza";
const char *password = "bernarda2020";

ESP8266WebServer  server(80);


//Funcion para conectarse a wifi
void conectarWiFi() {
  Serial.print("Conectando a ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
 
  Serial.print("Conectado a IP: ");
  Serial.println(WiFi.localIP());
}

void add_json_object(float R, float S, float T, float U, float V, float W) {
  JsonObject obj = jsonDocument.createNestedObject();
  obj["R"] = R;
  obj["S"] = S;
  obj["T"] = T; 
  obj["U"] = U; 
  obj["V"] = V; 
  obj["W"] = W; 
}


void medirNir() {
  jsonDocument.clear();
  for(int i=0; i<CANT; i++){
    sensor.takeMeasurementsWithBulb();
    float R = sensor.getCalibratedR();
    float S = sensor.getCalibratedS();
    float T = sensor.getCalibratedT();
    float U = sensor.getCalibratedU();
    float V = sensor.getCalibratedV();
    float W = sensor.getCalibratedW();
    add_json_object(R, S, T, U, V, W);
    delay(2000);
  }
  serializeJson(jsonDocument, buffer);
  server.send(200, "application/json", buffer);
}



void setup(){
  Wire.begin();
  Serial.begin(9600);
  sensor.begin(Wire, GAIN, MEASUREMENT_MODE);
  conectarWiFi();
  server.on("/medir", medirNir);
  // start server     
  server.begin();

}

void loop(){
  server.handleClient();
}
