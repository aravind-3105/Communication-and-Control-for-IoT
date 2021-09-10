#include<ESP8266WiFi.h>
#include<BlynkSimpleEsp8266.h>
#include<DHT.h>

DHT dht(0, DHT11);
BlynkTimer timer;

void read_from_DHT(){
  //First we check if value can be read or not to ensure before sending
  if(isnan(dht.readHumidity()))
  {
    if(isnan(dht.readTemperature()))
    {
      Serial.println("Error!");
      return;
    }
  }
  //Virtual pin associated for Temperature Reading
  Blynk.virtualWrite(V5,dht.readTemperature());
  //Virtual pin associated for Humidity Reading
  Blynk.virtualWrite(V6,dht.readHumidity());
  //Print Temperature and Humidity
  Serial.print("Temperature:");
  Serial.println(dht.readTemperature());
  Serial.print("Humidity:");
  Serial.println(dht.readHumidity());
  Serial.println("Next Reading....");
}

void setup(){
  Serial.begin(9600);
  Blynk.begin("PDy7PNaI9FdTQpstdOTRaoX9MTCZ6B4I", "Y","X");
  dht.begin();
  timer.setInterval(1000L,read_from_DHT);
}

void loop(){
  Blynk.run();  //Initialise the blynk 
  timer.run();  //Initialise the timer for Blynk
}
