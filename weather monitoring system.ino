#include <Adafruit_Fingerprint.h>
#include <Adafruit_BMP085.h>
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#define WLAN_SSID       "Realme"
#define WLAN_PASS       "012345688"
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "akshaypsmec"
#define AIO_KEY         "bb1f21fe02c74595b5572e7c728caa36"
#define relay D0

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Publish  temp= Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temp");
Adafruit_MQTT_Publish  pre= Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/preassure");
Adafruit_MQTT_Publish  amp= Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/altitude");
Adafruit_BMP085 bmp;
void setup() {
  Serial.begin(9600);
  if (!bmp.begin()) {
	Serial.println("Could not find a valid BMP085 sensor, check wiring!");
	while (1) {}
  }
}

void MQTT_connect();

void loop() {
    MQTT_connect();
    float t=bmp.readTemperature();
    temp.publish(t);
    Serial.print(t);
    Serial.println(" *C");
    delay(2000);
    float p=bmp.readPressure();
    pre.publish(p);
    Serial.print("Pressure = ");
    Serial.print(p);
    Serial.println(" Pa");
    delay(2000);
    float a=bmp.readAltitude();
    amp.publish(a);
    Serial.print("Altitude = ");
    Serial.print(a);
    Serial.println(" meters"); 
    delay(2000);
}
void MQTT_connect() {
  int8_t ret;
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}


