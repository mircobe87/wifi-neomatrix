#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "settings.h"

WiFiUDP ntpUDP;

#ifdef NTP_SERVER
    #ifdef NTP_OFFSET
NTPClient timeClient(ntpUDP, NTP_SERVER, NTP_OFFSET);
    #else
NTPClient timeClient(ntpUDP, NTP_SERVER);
    #endif
#else
    #ifdef NTP_OFFSET
NTPClient timeClient(ntpUDP, NTP_OFFSET);
    #else
NTPClient timeClient(ntpUDP);
    #endif
#endif

void setup() {
#ifdef SERIAL_SPEED
    // Setup serial port
    Serial.begin(115200);
#endif

    // Initialize the LED_BUILTIN pin as an output
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
 
    // Begin WiFi
    WiFi.begin(WIFI_SSID, WIFI_PASS);
 
    // Connecting to WiFi...
    SERIAL_PRINT("Connecting to ");
    SERIAL_PRINT(WIFI_SSID);
    SERIAL_PRINT("\n");
  
    // Loop continuously while WiFi is not connected
    while (WiFi.status() != WL_CONNECTED) {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(100);
        digitalWrite(LED_BUILTIN, LOW);
        delay(900);
        SERIAL_PRINT(".");
    }
 
    // Connected to WiFi
    SERIAL_PRINT("\n");
    SERIAL_PRINT("Connected! IP address: ");
    SERIAL_PRINT(WiFi.localIP());
    SERIAL_PRINT("\n");

    timeClient.begin();
}
 
void loop() {
    // put your main code here, to run repeatedly:
    timeClient.update();
    SERIAL_PRINT(timeClient.getFormattedTime());
    SERIAL_PRINT("\n");
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
}
