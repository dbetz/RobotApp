#include <string.h>
#include "ELClient.h"
#include "ELClientHTTP.h"
#include "stream.h"
#include "robot.h"

// setup the SLIP connection
ELClient esp(&Serial, &Serial);

// object to handle HTTP requests
ELClientHTTP http(&esp);

// object to drive the ActivityBot
Robot myRobot;
          
const char *webpage1 = 
"<!DOCTYPE HTML>"
"<html>"
  "<head>"
    "<style>"
      "a{"
        "background-color:#637aad;"
        "border:1px solid navy;"
        "display:inline-block;"
        "color:white;"
        "font-size:18px;"
        "margin:20px;"
        "width:100px;"
        "height:55px;"
        "font-family:Arial;"
        "cursor:pointer;"
        "padding-top:37px"
      "}"
      "a:hover{background-color:white;color:navy;}"
    "</style>"
    "<script>"
      "function gt(ik){"
        "var http = new XMLHttpRequest();"
        "var url = \"/robot\";"
        "var params = \"gto=\"+ik;"
        "http.open(\"POST\", url, true);"
        "http.setRequestHeader(\"Content-type\",\"application/x-www-form-urlencoded\");"
        "http.send(params);"
      "}"
    "</script>"
  "</head>"
  "<body bgcolor=\"3b5898\">"
    "<div align=\"center\">"
      "<font face=\"Arial\" size=6 color=\"white\">"
        "Hi! I'm ActivityBot :)"
      "</font><br>"
      "<a onclick=\"gt('F');\">Forward</a><br>"
      "<a onclick=\"gt('L');\">Left</a><a onclick=\"gt('S');\">Stop</a><a onclick=\"gt('R');\">Right</a><br>"
      "<a onclick=\"gt('B');\">Reverse</a>"
    "</div>"
  "</body>"
"<html>";

void httpCbHandler(void *response)
{
  ELClientResponse *resp = (ELClientResponse *)response;
  char requestType[20]; // GET or POST
  char url[128];        // URL without the query string
  char vars[128];       // query string after the "?" in the url
  char post[128];       // post data
  int ret;
  
  Serial.print("Argc: ");
  Serial.println(resp->argc());
  
  if (resp->argc() != 4) {
    http.sendResponse(400, "Malformed request");
    return;
  }
  
  resp->popArg(requestType, sizeof(requestType));
  resp->popArg(url, sizeof(url));
  resp->popArg(vars, sizeof(vars));
  resp->popArg(post, sizeof(post));

  Serial.print("HTTP ");
  Serial.print(requestType);
  Serial.println(" request");

  Serial.print("  URL: ");
  Serial.println(url);

  Serial.print("  VARS: ");
  Serial.println(vars);
  
  Serial.print("  POST: ");
  Serial.println(post);
  
  if (strcmp(requestType, "GET") == 0)
    ret = http.sendResponse(200, webpage1);
  
  else if (strcmp(requestType, "POST") == 0) {
    const char *p;
    if ((p = strstr(post, "gto=")) != NULL) {
        if (myRobot.processButton(p[4]))
            ret = http.sendResponse(200, "Robot operation completed.");
        else
            ret = http.sendResponse(400, "Unknown robot operation."); 
    }
    else
        ret = http.sendResponse(400, "Unknown request.");
  }
  
  Serial.print("sendResponse returned ");
  Serial.println(ret);
}

void setup() {
  Serial.begin(115200);   // the baud rate here needs to match the esp-link config
  Serial.println("EL-Client starting!");

  // Sync-up with esp-link, this is required at the start of any sketch and initializes the
  // callbacks to the wifi status change callback. The callback gets called with the initial
  // status right after Sync() below completes.
  bool ok;
  do {
    ok = esp.Sync();      // sync up with esp-link, blocks for up to 2 seconds
    if (!ok) Serial.println("EL-Client sync failed!");
  } while(!ok);
  Serial.println("EL-Client synced!");

  // setup a callback function to handle incoming HTTP requests
  http.requestCb.attach(httpCbHandler);
  
  // tell the ESP what URL we are prepared to handle
  int ret = http.setPath("/robot");
  Serial.print("setPath returned ");
  Serial.println(ret);
}

void loop() {
  // process any callbacks coming from esp_link
  esp.Process();
}

int main(void)
{
  delay(1000);
  setup();
  for (;;)
    loop();
  return 0;
}
