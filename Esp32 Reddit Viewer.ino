#include <Button2.h>

#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

#include <HTTPClient.h>

#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiClient.h>
#include <WiFiGeneric.h>
#include <WiFiMulti.h>
#include <WiFiSTA.h>
#include <WiFiScan.h>
#include <WiFiServer.h>
#include <WiFiType.h>
#include <WiFiUdp.h>
#include <TFT_eSPI.h>

const char* ssid = "???";
const char* password = "???";

String subreddit = "showerthoughts";
String limit = "4";
String timefilter = "day";
int selectedSubSubReddit = 0;

HTTPClient http_reddit;
String reddit_json = "";
String subreddits[] = {"showerthoughts", "technology", "esp32", "pcgaming", "askreddit", "dubai"};
int listlength = 6;
int randomsubbredditid = random(0, listlength-1);
String reddit_server = "https://www.reddit.com/r/" + subreddits[randomsubbredditid] + "/top.json?limit=" + limit + "&t=" + timefilter;

TFT_eSPI tft = TFT_eSPI();

#define btn1_pin 0
#define btn2_pin 14

Button2 btn1 = Button2(btn1_pin);
Button2 btn2 = Button2(btn2_pin);

DynamicJsonDocument doc(49152);

void setup() {
  pinMode(15, OUTPUT);
  digitalWrite(15, HIGH);
  // put your setup code here, to run once:
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0,0);
  tft.setTextSize(3);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.println(randomsubbredditid);
  delay(1000);
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 0);
  WiFi.mode(WIFI_STA);

  tft.println("Starting WiFi");

  WiFi.begin(ssid, password);

  int retrycon = 50;
  tft.println("Connecting...");
  while (WiFi.status() != WL_CONNECTED)
  {  
    delay(500);
    if (--retrycon == 0)
    {
      tft.println("RESTART");
      ESP.restart();
    }
    tft.print(".");
  }

  tft.print("WiFi connected with IP: ");
  tft.println(WiFi.localIP());
  http_reddit.begin(reddit_server);
  int resp = http_reddit.GET();
  if(resp > 0) {
    reddit_json = http_reddit.getString();
  }
  else {
    tft.setTextColor(TFT_RED);
    tft.println("Cannot get reddit post");
  }
  http_reddit.end();
  

  DeserializationError error = deserializeJson(doc, reddit_json);

  if (error) {
    delay(2000);
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.setCursor(0,0);
    tft.fillScreen(TFT_BLACK);
    tft.print("deserializeJson() failed: ");
    tft.print(error.c_str());
    if(error.c_str() == "EmptyInput") {
      tft.setTextSize(1);
      tft.println("\n\n\n" + reddit_server);
    }
    delay(2500);
    ESP.restart();
    return;
  }
  refreshDisplay();
  btn1.setPressedHandler([](Button2 & b) {
        if(selectedSubSubReddit <= 0) {
          refreshDisplay();
        }
        else {
          selectedSubSubReddit--;
          refreshDisplay();
        }
    });
    btn2.setPressedHandler([](Button2 & b) {
        if(selectedSubSubReddit >= limit.toInt()) {
          refreshDisplay();
        }
        else {
          selectedSubSubReddit++;
          refreshDisplay();
        }
    });
    btn2.setLongClickHandler([](Button2 & b) {
        if(randomsubbredditid >= listlength-1) {
          randomsubbredditid = 0;
        }
        else {
          randomsubbredditid++;
        }
        reddit_server = "https://www.reddit.com/r/" + subreddits[randomsubbredditid] + "/top.json?limit=" + limit + "&t=" + timefilter;
        http_reddit.begin(reddit_server);
        int resp = http_reddit.GET();
        if(resp > 0) {
          reddit_json = http_reddit.getString();
        }
        else {
          tft.setTextColor(TFT_RED);
          tft.println("Cannot get reddit post");
        }
        http_reddit.end();


        DeserializationError error = deserializeJson(doc, reddit_json);

        if (error) {
          delay(2000);
          tft.setTextColor(TFT_RED, TFT_BLACK);
          tft.setCursor(0,0);
          tft.fillScreen(TFT_BLACK);
          tft.print("deserializeJson() failed: ");
          tft.print(error.c_str());
          if(error.c_str() == "EmptyInput") {
            tft.setTextSize(1);
            tft.println("\n\n\n" + reddit_server);
          }
          delay(2500);
          ESP.restart();
          return;
        }
        tft.fillScreen(TFT_BLACK);
        tft.setTextColor(TFT_GREEN);
        tft.setCursor(0,0);
        tft.println(subreddits[randomsubbredditid]);
        delay(1500);
        selectedSubSubReddit = 0;
        refreshDisplay();
          });
}
void refreshDisplay() {
  JsonObject data = doc["data"];
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0,0);
  tft.setTextColor(TFT_BLUE, TFT_BLACK);
  const char* title = data["children"][selectedSubSubReddit]["data"]["title"];
  int charnum = 0;
  for(int i = 0; i < sizeof(title); i++) {
    charnum++;
  }
  if(charnum > 118) {
    tft.setTextSize(2);
  }
  else {
    tft.setTextSize(3);
  }
  tft.println(title);
}
void loop() {
  // put your main code here, to run repeatedly:
  btn1.loop();
  btn2.loop();
}
