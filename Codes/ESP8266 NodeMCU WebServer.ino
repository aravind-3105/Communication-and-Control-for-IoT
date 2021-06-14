#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>
Servo servo;

ESP8266WebServer server(80); // Create a webserver which searchesfor HTTP request on port 80
char webpage[] = R"=(
<html>
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
      body{
        background-color: pink;
        text-align:center;
        font-family: verdana;
       
        }
      .slidecontainer {
        width: 50%;
      }
      input[type=text] {
        background-color: #3CBC8D;
        color: white;
        text-align:center;
      }
      
      .slider {
        -webkit-appearance: none;
        width: 100%;
        height: 25px;
        background: #000000;
        outline: none;
        opacity: 0.7;
        -webkit-transition: .2s;
        transition: opacity .2s;
      }
  
      .slider:hover {
        opacity: 1;
      }
  
      .slider::-webkit-slider-thumb {
        -webkit-appearance: none;
        appearance: none;
        width: 25px;
        height: 25px;
        background: #0C36F4;
        cursor: pointer;
      }
      
      .slider::-moz-range-thumb {
        width: 25px;
        height: 25px;
        background: #0C36F4;
        cursor: pointer;
      }
    </style>
  </head>
  <body>
    <center><h1>Servo Motor Angle Control</h1></center>
    <center><h3>By: Aravind Narayanan(2019102014)</h3></center>
    <center><div class="slidecontainer">
      <center><input type="range" min="0" max="180" value="0" class="slider" id="analogval"> 
      </center>
      Value:<center><b><input type="text" id="textInput" value="" /></b></center>
    </div>,</center>
    
    <script>
      var slider = document.getElementById("analogval");
      var output = document.getElementById("textInput");
      output.innerHTML = slider.value;
      var xhr = new XMLHttpRequest();
      function updateTextInput() {
          document.getElementById('textInput').value=slider.value; 
        }
      slider.oninput = function() {
        output.innerHTML = this.value;
        updateTextInput();
        xhr.open("GET","sliderchange?sliderval="+slider.value);
        xhr.send();
      }
    </script>
  </body>
</html>
)=";
/*
 * slider and output variables are objects that are linked to slider and textInput respectively
 * output.innerHTML  = A String, representing the HTML content of an element
 * XMLHttpRequest = built-in browser object that allows to make HTTP requests in JavaScript.
 * xhr.open(method, URL)  here method is GET, URL is the for the slider 
 * xhr.send() opens the connection and sends the request to server 
 */
String value = "0";
void sliderchange(){ 
  if(server.hasArg("sliderval"))
  {                                                 //Loooks for the value in the incoming data from the HTML part where slider is
    value = server.arg("sliderval");                //Assigns the angle obtained for display
  }
  Serial.print("Servo Motor Angle: ");
  Serial.println(value);
}

void handleRoot(){   //HTTP handler
  server.send(200, "text/html", webpage);   // Send HTTP status 
}

void setup() {
  Serial.begin(9600);
  servo.attach(2);  //GPIO2
  WiFi.begin("NANI", "arvi1308");  //Wifi Connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("IP Address: ");
  Serial.print(WiFi.localIP());
  Serial.println("");
  server.begin();   //start the server
  server.on("/", handleRoot);  // Go to HTTP Handler  
  server.on("/sliderchange", sliderchange);    // First Variable is path, second is subroutine_name
}

void loop() {
  server.handleClient();  //Listen for HTTP Requests
  servo.write(value.toInt()); 
}
