// *****************************************************
// MY INSTAGRAM PLAQUE FOLLOWERS COUNTER
// Year: 2020
// Authors: Gabriel Froes & Vanessa Weber
// Youtube / Instagram: @codigofontetv
// *****************************************************

// Required Libs
#include <stdlib.h>
#include <string.h>
#include <Wire.h>
#include <InstagramApi.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "config.h"

// Display
LiquidCrystal_I2C lcd(PCF8574_ADDR_A21_A11_A01);

// Wifi
WiFiClient client;

// Youtube API
InstagramApi api(INSTAGRAM_URL, client);
unsigned long api_mtbs = 3600000;      // (1 hour) Time between API requests
unsigned long api_lasttime = -3600000; // Last time API request has been hit

// *****************************************************
// Arduino setup function
// *****************************************************
void setup()
{
  Serial.begin(9600);

  // ------------------------
  // 1. START DISPLAY
  // ------------------------
  lcd.begin(16, 2);

  lcd.backlight();
  delay(200);
  lcd.noBacklight();
  delay(200);
  lcd.backlight();

  print_display("Loading", "Instagram...");

  // ------------------------
  // 2. CONNECT WIFI
  // ------------------------
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Attempt to connect to Wi-fi network:
  Serial.print("\nConnecting to:");
  Serial.println(WIFI_SSID);
  print_display("Connecting Wi-fi:", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("Wi-fi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);
  print_display("Connected!", "");
}
// *****************************************************

// *****************************************************
// Arduino loop function
// *****************************************************
void loop()
{
  if (millis() > api_lasttime + api_mtbs)
  {
    print_display("Loading...", "");
    Serial.println("Loading...");
    if (api.getAccountStatistics())
    {
      api_lasttime = millis();
      Serial.println("-----Account Stats-----");
      Serial.print("Followers Count: ");
      Serial.println(api.accountStats.followersCount);

      print_stats("Followers", api.accountStats.followersCount);
    }
    else
    {
      print_display("Oops!", "Trying again...");
      delay(2000);
      api_lasttime = 0;
    }
  }
}
// *****************************************************

// *****************************************************
// Set Message on Display
// *****************************************************
void print_stats(String msg, long val)
{
  print_display(msg, String(val));
}
void print_display(String msg1, String msg2)
{
  lcd.clear();
  lcd.home();
  lcd.print(msg1);
  lcd.setCursor(0, 1); // next line
  lcd.print(msg2);
  lcd.backlight();
  delay(200);
  lcd.noBacklight();
  delay(200);
  lcd.backlight();
}
// *****************************************************