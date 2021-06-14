#include <DHT.h> 
#include <ESP8266WiFi.h>

String API = "AEIJL417Z4JYRWBF";     //API key
DHT dht(0, DHT11);  //GPIO0 refers to D3

WiFiClient client;  //The client for contacting server is initialised

#define NUM_FIELDS 3       //Array Size(3 taken for convenience)                       
#define TEMPERATURE 1      //element for temperature             
#define HUMIDITY 2         // element for humidity
void setup() 
{
       Serial.begin(9600);  //Serial monitor with baud rate of 9600
       dht.begin();    
       WiFi.begin("NANI", "arvi1308");  //Connect to WiFi
       while(WiFi.status() != WL_CONNECTED) {
         delay(500);
         Serial.print(".");
        }
        Serial.println("");
        Serial.print("Connected to WiFi network with IP Address: ");
        Serial.println(WiFi.localIP());
}
 
void loop() 
{
    String Sensor_Values[ NUM_FIELDS ]; //Create a string array of size 3 so that Array[1] and Array[2] correspond to Temperature and Humidity repsectively
    
    Sensor_Values[ TEMPERATURE ] = String( dht.readTemperature() );  //Read temperatrue and assign to string array
    Serial.print( "Temperature = " );
    Serial.println( Sensor_Values[ TEMPERATURE ] );
    
    Sensor_Values[ HUMIDITY ] = String( dht.readHumidity() );   //Read humidity and assign to string array
    Serial.print( "Humidity = " );
    Serial.println( Sensor_Values[ HUMIDITY ] ); 
    
    Send_to_Thingspeak( Sensor_Values );  //Function implementing HTTP POST Request
    delay( 15000 ); //Thingspeak takes 15 seconds per transmission so we take value every 15 seconds
}

int Send_to_Thingspeak( String Sensor_Values[] ){
  
    if (client.connect( "api.thingspeak.com" , 80 ))  //Connect to the server
    {
       String Send_Sensor_Value= "api_key=" + API ;
       for ( int sensor_number = 1; sensor_number < NUM_FIELDS; sensor_number++ )
          {
            String fieldName = "field" + String( sensor_number );
            Send_Sensor_Value += "&" + fieldName + "=" + Sensor_Values[ sensor_number ]; 
          }

        // POST data via HTTP.
        Serial.println( "Connecting to ThingSpeak" );
        Serial.println();
        int len = Send_Sensor_Value.length();
        //Standard Protocols for making a HTTP Request
        client.println( "POST /update HTTP/1.1" );  //Here the given URL will be used to process the passed data
        client.println( "Host: api.thingspeak.com" );  //
        client.println( "Connection: close" );
        client.println( "Content-Type: application/x-www-form-urlencoded" );  //Content-type tells the server that the passed data is a simple web form data
        client.println( "Content-Length: " + String( len ) ); //length will be the actual length of the data put in the message body.
        client.println();
        client.println( Send_Sensor_Value );  
        Serial.println( Send_Sensor_Value );  //Print the feeds value
        //https://thingspeak.com/channels/1140953/feeds   List of POST DATA
        //https://thingspeak.com/channels/1140953/feeds/last.txt Recent POST DATA
    }
    else
    {
      Serial.println ( "Connection Failed" );  //If user cannot connect to server
    }
    
}
