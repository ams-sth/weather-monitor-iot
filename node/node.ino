#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"



#include "Adafruit_MQTT_Client.h"


#include <DHT.h>



int DHTPIN = D3;
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);


int dhtIn = D3;
float dhtTemp;
float dhtHumidity;



#define WLAN_SSID "Rita Shrestha"  // replace with your wifi ssid and wpa2 key
#define WLAN_PASS "M9841775176a"



#define AIO_SERVER "io.adafruit.com"
#define AIO_SERVERPORT 1883


#define AIO_USERNAME "amshshrestha"
#define AIO_KEY "aio_TXSs74IlgMGKtfm7O0XeSatpWKK9"


WiFiClient client;




// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);



Adafruit_MQTT_Publish Temperature = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Temperature");
Adafruit_MQTT_Publish Humidity = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Humidity");
Adafruit_MQTT_Publish Pressure = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Pressure");






void setup() {



  Serial.begin(9600);



  Serial.println("Connecting to ");
  Serial.println(WLAN_SSID);



  WiFi.begin(WLAN_SSID, WLAN_PASS);



  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}




void loop() {



  MQTT_connect();


  dhtHumidity = dht.readHumidity();
  dhtTemp = dht.readTemperature();

  Temperature.publish(dhtTemp);
  Humidity.publish(dhtHumidity);

  Serial.println((String) " DHT Temp:  " + dhtTemp + " ,DHT Humidity:  " + dhtHumidity);


  delay(5000);
}








void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;

  while ((ret = mqtt.connect()) != 0) {  // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      while (1)
        ;
    }
  }
  Serial.println("MQTT Connected!");
}