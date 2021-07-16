SYSTEM_THREAD(ENABLED);
#include "MQTT.h"
#include <blynk.h>
MQTT client("lab.thewcl.com", 1883, callback);
#include "oled-wing-adafruit.h"
OledWingAdafruit display;
double longitude = 0.0;
double latitude = 0.0;
WidgetMap testMap(V1);
// setup() runs once, when the device is first turned on.
void setup() {
  // Put initialization like pinMode and begin functions here.
  display.setup();
  display.clearDisplay();
  display.display();
  Serial.begin(9600); 
  Blynk.begin("0cifaHaIgzv-R9t0zphjlaO62iNwdPcx", IPAddress(167, 172, 234, 162), 9090); 
  
}
void callback(char* topic, byte* payload, unsigned int length)
{
  //checks which topic and sets Latitude an longitude accordingly, getting the value from mqtt
  char p[length + 1];
  memcpy(p, payload, length);
  p[length] = NULL;
  Serial.println(topic);
  String s = p;
  if((String)topic == "noderedlongitude"){
    longitude = s.toFloat();
    Serial.printf("%s",p);
    Serial.println();
    Serial.println(longitude);
    delay(1000);
  }
  
  Serial.println();
  if((String)topic == "noderedLatitude"){
    latitude = s.toFloat();
    Serial.printf("%s",p);
    Serial.println();
    Serial.println(latitude);
    delay(1000);
  }  

  


}  
// loop() runs over and over again, as quickly as it can execute.
void loop() {
  if (client.isConnected()) {
    client.loop();
    
  }
  else {
    client.connect(System.deviceID());
    client.subscribe("noderedLatitude");
    client.subscribe("noderedlongitude");
    
  } 
  // displays the lat and long on oled and then on blynk shows it on the map
  display.loop();
  Blynk.run();
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("Longitude: ");
  display.println(longitude);
  display.print("Latitude: ");
  display.println(latitude);
  display.display();

  testMap.location(0, latitude, longitude, "value");
  delay(1000);
}