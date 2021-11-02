// #include <WiFi.h>
// #include <WiFiClient.h>
// #include <WebServer.h>
// #include <ESPmDNS.h>

// #define DEFAULT_AP_SSID "Chawevan 2.4G"
// #define DEFAULT_AP_PASS "08768859"

// String apSsid = DEFAULT_AP_SSID;
// String apPass = DEFAULT_AP_PASS;

// WebServer server(80);

// /*
// #ifndef LED_BUILTIN
// #define LED_BUILTIN 16
// #else 
// #undef LED_BUILTIN
// #define LED_BUILTIN 16
// #endif
// */

// void handleRoot() {
//   digitalWrite(LED_BUILTIN, LOW);
//   server.send(200, "text/plain", "hello from esp32!");
//   digitalWrite(LED_BUILTIN, HIGH);
// }

// void handleNotFound() {
//   digitalWrite(LED_BUILTIN, LOW);
//   String message = "File Not Found\n\n";
//   message += "URI: ";
//   message += server.uri();
//   message += "\nMethod: ";
//   message += (server.method() == HTTP_GET) ? "GET" : "POST";
//   message += "\nArguments: ";
//   message += server.args();
//   message += "\n";
//   for (uint8_t i = 0; i < server.args(); i++) {
//     message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
//   }
//   server.send(404, "text/plain", message);
//   digitalWrite(LED_BUILTIN, HIGH);
// }

// void handleApSetup() {
//   digitalWrite(LED_BUILTIN, LOW);
//   server.send(200, "text/html", "<!DOCTYPE html><html lang='en'><head> <meta charset='UTF-8'> <title>Access Point Configuration</title> <style>body{font-family: 'Courier New', 'Courier', Courier, monospace;}.input-group{padding: 5px;}.button-group{padding: 5px;}input{font-family: inherit; padding: 5px; width: 200px; border: solid 1px gray; border-radius: 10px;}button{font-family: inherit; padding: 5px; width: 80px; border: solid 1px gray; border-radius: 10px;}</style></head><body align='center'> <div class='input-group'><label>SSID:</label><input></div><div class='input-group'><label>PASS:</label><input></div><div class='button=group'> <button id='reloadButton'>Reload</button> <button id='SubmitButton'>Submit</button> </div><script>document.getElementById('reloadButton').onclick=function(){console.log('reload button is clicked');}document.getElementById('submitButton').onclick=function(){console.log('submit button is clicked');}</script></body></html>");
//   digitalWrite(LED_BUILTIN, HIGH);
// }

// void setup(void) {
//   pinMode(LED_BUILTIN, OUTPUT);
//   digitalWrite(LED_BUILTIN, HIGH);

//   Serial.begin(115200);
//   WiFi.mode(WIFI_STA);
//   WiFi.begin(apSsid.c_str(), apPass.c_str());
//   Serial.println("");

//   // Wait for connection
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }
//   Serial.println("");
//   Serial.print("Connected to ");
//   Serial.println(apSsid.c_str());
//   Serial.print("IP address: ");
//   Serial.println(WiFi.localIP() );

//   if (MDNS.begin("esp32")) {
//     Serial.println("MDNS responder started");
//   }

//   server.on("/", handleRoot);

//   server.on("/inline", []() {
//     server.send(200, "text/plain", "this works as well");
//   });


//   server.on("/apSetup", handleApSetup);


//   server.onNotFound(handleNotFound);

//   server.begin();

//   Serial.println("HTTP server started");

//   digitalWrite(LED_BUILTIN, HIGH);
// }

// void loop(void) {

//   server.handleClient();
//   delay(1); 
// }




#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <ArduinoJson.h>
#include <EEPROM.h>

#define SPI_FLASH_SEC_SIZE 1024

#define DEFAULT_AP_SSID "Chawevan 2.4G"
#define DEFAULT_AP_PASS "08768859"

// const char* ssid = "........";
// const char* password = "........";

String apSsid = DEFAULT_AP_SSID;
String apPass = DEFAULT_AP_PASS;

String myId = "";

WebServer server(80);

/*
#ifndef LED_BUILTIN
#define LED_BUILTIN, 16
#else  
#undef LED_BUILTIN
#define LED_BUILTIN, 16
#endif
*/

const int led = 16;

void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/plain", "hello from esp32!");
  digitalWrite(led, 0);
}

void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void handleApSetup() {
  digitalWrite(led, 1);

  server.send(200, "<!DOCTYPE html><html lang='en'><head> <meta charset='UTF-8'> <title>Access Point Configuration</title> <style>body{font-family: 'Courier New', 'Courier', Courier, monospace;}.input-group{padding: 5px;}.button-group{padding: 5px;}input{font-family: inherit; padding: 5px; width: 200px; border: solid 1px gray; border-radius: 10px;}button{font-family: inherit; padding: 5px; width: 80px; border: solid 1px gray; border-radius: 10px;}</style></head><body align='center'> <div class='input-group'><label>SSID:</label><input></div><div class='input-group'><label>PASS:</label><input></div><div class='button=group'> <button id='reloadButton'>Reload</button> <button id='SubmitButton'>Submit</button> </div><script>document.getElementById('reloadButton').onclick=function (){console.log('reload button is clicked'); var xmlHttp=new XMLHttpRequest(); xmlHttp.onreadystatechange=function (){if (xmlHttp.readyState==XMLHttpRequest.DONE){if (xmlHttp.status==200){/* success */ var res=JSON.parse(xmlHttp.responseText); console.log(res); document.getElementById('ssid').value=res.ssid; document.getElementById('pass').value=res.pass;}else{/*fail*/ alert('loading fail');}}}; xmlHttp.open('GET', '/ap'); xmlHttp.send();}; document.getElementById('submitButton').onclick=function (){console.log('submit button is clicked...'); var xmlHttp=new XMLHttpRequest(); xmlHttp.onreadystatechange=function(){if(xmlHttp.readyState==XMLHttpRequest.Done){if(xmlHttp.status==200){/*success */ alert('success')}else{/*fail */ alert('fail');}}}; var data=JSON.stringify({ssid: document.getElementById('ssid').value, pass: document.getElementById('pass').value}); xmlHttp.open('POST', '/ap'); xmlHttp.send(data);}; document.getElementById('reloadButton').click(); </script></body></html>");
  digitalWrite(led, 0);
}

void handleApGet() {
  digitalWrite(led, 1);

  String str = "";
  str += "{";
  str += "\"ssid\":\"" + apSsid + "\",";  // pack ssid to json string
  str += "\"pass\":\"" + apPass + "\",";  // pack pass to json string 
  str += "}";

  server.send(200, "text/json", str);

  digitalWrite(led, 0);
}


void handleApPost() {
  // {ssid: <ssid>, pass: <pass>},  args is 1 at arg(0)
  digitalWrite(LED_BUILTIN, LOW);

  if (server.args() != 1) {
    server.send(400, "text/plain", "argument error");
}
  else {
    String str = server.arg(0);
    StaticJsonDocument<100> doc;
    DeserializationError err = deserializaJson(doc,str);
    if(err) {
      server.send(50, "text/plan", "server error");
    }
    else {
      apSsid = doc["ssid"].as<String>();
      apPass = doc["pass"].as<String>();
      server.send(200, "text/plain", "ok");
    }
  }
  digitalWrite(led, 0);

}

void eepromWrite() {
  // "@$" [n]<ssid>[m]<pass>
  char c;
  int addr = 0;
  unsigned char s, i;

  EEPROM.begin(SPI_FLASH_SEC_SIZE);

  // write "@$"
  c = "@";  EEPROM.put(addr, c);  addr ++;
  c = "$";  EEPROM.put(addr, c);  addr ++;

  // ssid 
  s = (unsigned char)apSsid.length(); EEPROM.put(addr, s); addr ++;
  for(i = 0; i < s; i++) {
    c = apSsid[i]; EEPROM.put(addr, c); addr ++;
  }

  // pass
  s = (unsigned char)apPass.length(); EEPROM.put(addr, s); addr ++;
  for(i = 0; i < s; i++) {
    c = apPass[i]; EEPROM.put(addr, c); addr ++;
  }

  EEPROM.end();
}

// get chipId
char temp[10];
uint64_t id = ESP.getEfuseMac();
sprintf(temp, "%04X", (uint16_t)(id >> 32));
myId += myId + "-" + String(temp);
sprintf(temp, "%08X", (uint32_t)id);
myId += myId + String(temp);
Serial.printIn("myId: " + myId);



void setup(void) {
  pinMode(led, OUTPUT);
  digitalWrite(led, 1);

  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  WiFi.begin(apSsid.c_str(), apPass.c_str());
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(apSsid.c_str());
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP() );

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  // front-end
  // display ap configuration page
  server.on("/apSetup", handleApSetup);

  // webservive (back-end)
  server.on("/ap", HTTP_GET, handleApGet);    // return ssid, pass in json format
  server.on("/ap", HTTP_POST, handleApPost);  // update ssid, pass from web browser


  server.onNotFound(handleNotFound);

  server.begin();

  Serial.println("HTTP server started");

  digitalWrite(led, 0);
}

void loop(void) {

  server.handleClient();
  delay(1); 

}