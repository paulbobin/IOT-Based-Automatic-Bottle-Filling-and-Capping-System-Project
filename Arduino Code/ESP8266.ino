/*Automatic Water Bottle Filling  System
 * Bobin B Paul 
 * Noorul Islam Centre for Higher Education
 * Electronics and Instrumentation Engineering
 */


#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>
#include <WiFiClient.h>
//===================================================================================================================
// change these values to match your network
const char* ssid     = "SSID"; 
const char* password = "password";
int ircinput = D3;     //IR sensor pin for capping
int caprelayinput = D5;    // Analog PIN 5 for relay for Capping Motor
int servoinput = D4;
int counter;
int i = 0;

String capbdv = "Connecting";
String capmv = "Connecting";
String counterv = "0";
String fillbdv = "Connecting";
String fillwpv = "Connecting";
String ipaddressv = "---------";
String statev = "OFF";


//Static IP address configuration
IPAddress staticIP(192, 168, 1, 10);//ESP static ip
IPAddress gateway(192, 168, 1, 1);   //IP Address of your WiFi Router (Gateway)
IPAddress subnet(255, 255, 255, 0);  //Subnet mask
IPAddress dns(8, 8, 8, 8);  //DNS
ESP8266WebServer server(80);
const char* deviceName = "Arduino WiFi";
//===HTML CODE===============================================================================================
// This function returns an HTML formated page in the correct type for display
// It uses the Raw string macro 'R' to place commands in PROGMEM
const char Web_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
 <style>
.p1 {
font-family: 'Work Sans', sans-serif;
}
.p2 {
font-family: 'Roboto', sans-serif;
}
</style> 
<body>
<div class="container p-3 my-3 bg-dark text-white">
<div class="container p-3 rounded my-3 bg-dark text-white">
    <h1 class="display-4">Automatic Bottle Filling System</h1>
 <p  class="p2" ><button type="button" class="btn btn-success" onclick="send(1)" >START</button> <button type="button" class="btn btn-danger" onclick="send(0)">STOP</button>   Status :<span id="state"></span> </p>
  
<div class="container p-3 my-3 bg-secondary text-white">
      <h4 class="p2">Conveyor : ON </h4>
  <div class="row">
    <div class="col-sm-6 bg-info rounded">
      <h3 class="p1" >Filling System</h3>
      <p class="p2">Bottle Detector: <span id="fillbd">No Bottle</span></p>
      <p class="p2">Water Pump    : <span id="fillwp">OFF</span></p>
      <p class="p2">Filling Time   : <span id="fillt">7 secs</span></p>      
    </div>
    <div class="col-sm-6 bg-info rounded">
      <h3 class="p1">Capping System</h3>
      <p class="p2">Bottle Detector: <span id="capbd">No Bottle</span></p>
      <p class="p2">Capping Motor  : <span id="capm">OFF</span></p>
      <p class="p2">Time Capping   : <span id="captc">2 sec</span></p>
    </div>
    
  </div>
    <p  class="p2" >Counter: <span id="countr">0</span></p>
</div>
           <h  class="p2">IP:<span id="ipaddress">---------</span></h7> 
</div>
     <script>
       setInterval(function() {getSensorData();}, 1000); // Call the update function every set interval e.g. 1000mS or 1-sec
        function send(led_sts) 
      {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
            document.getElementById("state").innerHTML = this.responseText;
          }
        };
        xhttp.open("GET", "stater?state="+led_sts, true);
        xhttp.send();
      }
       function getSensorData() {

        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
            document.getElementById("capm").innerHTML = this.responseText;
          }
        };
        xhttp.open("GET", "capmr", true);
        xhttp.send();
        
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
            document.getElementById("capbd").innerHTML = this.responseText;
          }
        };
        xhttp.open("GET", "capbdr", true);
        xhttp.send();
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
            document.getElementById("countr").innerHTML = this.responseText;}
        };  
        xhttp.open("GET", "countrr", true);
        xhttp.send(); 
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
            document.getElementById("fillbd").innerHTML = this.responseText;}
        };  
        xhttp.open("GET", "fillbdr", true);
        xhttp.send(); 
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
            document.getElementById("fillwp").innerHTML = this.responseText;}
        };  
        xhttp.open("GET", "fillwpr", true);
        xhttp.send(); 
         //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
            document.getElementById("ipaddress").innerHTML = this.responseText;}
        };  
        xhttp.open("GET", "ipaddressr", true);
        xhttp.send(); 
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
            document.getElementById("state").innerHTML = this.responseText;}
        };  
        xhttp.open("GET", "stater", true);
        xhttp.send(); 
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
           
      }
    </script>
    
<head>
<link rel="preconnect" href="https://fonts.gstatic.com">
<link href="https://fonts.googleapis.com/css2?family=Roboto:wght@300;400&family=Work+Sans:wght@300&display=swap" rel="stylesheet">
  <title>Automatic Bottle Filling System</title>
  <meta charset="utf-8">
  <meta name="viewport"
 content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css">
  <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.5.1/jquery.min.js"></script>
  <script src="https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.16.0/umd/popper.min.js"></script>
  <script src='https://kit.fontawesome.com/a076d05399.js' crossorigin='anonymous'></script>
</head>
  </body>
</html>
)=====";

//===================================================================
// This routine is executed when you open a browser at the IP address
//===================================================================
void handleRoot() {
  //String s = Web_page;             //Display HTML contents
  server.send(200, "text/html", Web_page); //Send web page
}

void handlecapbdr() {
    server.send(200, "text/plain", capbdv); 
  } 


void handlecapmr() { 
    server.send(200, "text/plain", capmv); //Send sensor reading when there's a client ajax request
}

void handlecountrr() { 
    server.send(200, "text/plain", counterv); //Send sensor reading when there's a client ajax request
}
void handlefillbdr() {
    server.send(200, "text/plain", fillbdv); 
  } 


void handlefillwpr() { 
    server.send(200, "text/plain", fillwpv); //Send sensor reading when there's a client ajax request
}

void handleipaddressr() { 
    server.send(200, "text/plain", ipaddressv); //Send sensor reading when there's a client ajax request
}
void handlestater() { 
     String act_state = server.arg("state");
     if(act_state == "1")
     {
      statev = "ON";
     }
     if(act_state == "0")
     {
      statev = "OFF";
     }
     server.send(200, "text/plane", statev);
}
//=====================================================================================================================
void setup() 
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);        // Connect to your wifi
  WiFi.disconnect(); //Prevent connecting to wifi based on previous configuration
  WiFi.hostname(deviceName); // DHCP Hostname 
  WiFi.config(staticIP, subnet, gateway, dns);
  WiFi.begin(ssid, password); // Start the Wi-Fi services
  Serial.println("Connecting to : "+String(ssid));
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {Serial.print(".");} // Wait for WiFi to connect
  Serial.println(" Connected to : "+String(ssid));
  Serial.print("Use IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP

  //----------------------------------------------------------------
  server.on("/", handleRoot);           // This displays the main webpage, it is called when you open a client connection on the IP address using a browser
  server.on("/capbdr", handlecapbdr);   // To update Capping Bottle detector called by the function getSensorData
  server.on("/capmr", handlecapmr);     // To update Capping motor called by the function getSensorData
  server.on("/countrr", handlecountrr); // To update Counter called by the function getSensorData
  server.on("/fillbdr", handlefillbdr); // To update Pressure called by the function getSensorData
  server.on("/fillwpr", handlefillwpr); // To update Pressure called by the function getSensorData
  server.on("/ipaddressr", handleipaddressr); // To update Pressure called by the function getSensorData
  server.on("/stater", handlestater); // To update Pressure called by the function getSensorData
  //----------------------------------------------------------------
  server.begin();                     // Start the webserver
  pinMode(ircinput,INPUT);                              // Input of IR Sensor For Capping
  pinMode(caprelayinput,INPUT);                          //Output of Relay for Capping Motor

}
void increaseAndDisplay() {
  counter++;
  if(counter > 20) {
    counter = 1;
  }
String counter = counterv;
}
void loop() 
{

   if (digitalRead(ircinput) == LOW)
        {
          capbdv = "Bottle Detected";
        }
        else
        {
         capbdv = "No Bottle";    
        }
   if (digitalRead(caprelayinput) == LOW)
      {
      capmv = "ON"; 
      increaseAndDisplay();
      }
      else
      {
       capmv = "OFF"; 
      }
  server.handleClient();  // Keep checking for a client connection
}

 //Bobin
 //Credit me if you like
