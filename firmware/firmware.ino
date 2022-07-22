#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Arduino_JSON.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_GFX.h>
#include <Fonts/Tiny3x3a2pt7b.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "settings.h"
#include "debug.h"
#include "bitmaps.h"

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(
    MATRIX_W, MATRIX_H,
    MATRIX_PIN,
    MATRIX_TYPE,
    MATRIX_PIXEL_TYPE
);

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(T_SENS_PIN);
// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensors(&oneWire);

void setup() {
    // Setup serial port
    Serial.begin(SERIAL_SPEED);

    // Start the DS18B20 sensor
    sensors.begin();

    // Initialize Led Matrix
    matrix.begin();
    matrix.clear();
    matrix.setTextWrap(false);
    matrix.setRotation(0);
    matrix.setBrightness(MATRIX_BRIGHTNESS);

    rainbow();

    // Begin WiFi
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    DEBUG_PRINT("matrix type: ");
    DEBUG_PRINTLN(MATRIX_TYPE);

/*

    
    for (int x=0; x<MATRIX_W; x++) {
        for (int y=0; y<MATRIX_H; y++) {
            int i = x*MATRIX_W+y;
            uint32_t color = matrix.Color(
              256*i/(MATRIX_W*MATRIX_H),
              256*i/(MATRIX_W*MATRIX_H),
              256*i/(MATRIX_W*MATRIX_H)
            );
            matrix.drawPixel(x, y, color);
            delay(50);
            matrix.show();
        }
    }
    */



    // Initialize the LED_BUILTIN pin as an output
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
 

 
    // Connecting to WiFi...
    DEBUG_PRINT("Connecting to ");
    DEBUG_PRINTLN(WIFI_SSID);
  
    // Loop continuously while WiFi is not connected
    unsigned long wifi_conn_start = millis();
    display_scrollText(matrix.Color(64, 0, 0), "Connecting to", 40);
    
    do {
        display_scrollText(matrix.Color(64, 64, 64), WIFI_SSID, 40);
        digitalWrite(LED_BUILTIN, HIGH);
        delay(50);
        digitalWrite(LED_BUILTIN, LOW);
        delay(50);
        DEBUG_PRINT(".");
    } while (WiFi.status() != WL_CONNECTED && millis() - wifi_conn_start <= WIFI_TIMEOUT);

    if (WiFi.status() != WL_CONNECTED) {
        DEBUG_PRINTLN();
        DEBUG_PRINT("Connection FAILED");
        display_image(BMP_FAIL);
        delay(3000);
        display_scrollText(matrix.Color(64, 0, 0), "Connection FAILED", 40);
    } else {
        // Connected to WiFi
        DEBUG_PRINTLN();
        DEBUG_PRINT("Connected! IP address: ");
        DEBUG_PRINTLN(WiFi.localIP());
        display_image(BMP_OK);
        delay(3000);
        // display_scrollText(matrix.Color(0, 64, 0), "Connected! IP address:", 40);
        display_scrollText(matrix.Color(0, 64, 0), WiFi.localIP().toString().c_str(), 40);
    }
 
    
}

char datetime[32] = {0};
char inner_temp[8] = {0};

void loop() {
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    if(WiFi.status()== WL_CONNECTED){
      display_image(BMP_CLOCK);
      delay(3000);
      fetchDateTime(datetime);
      // display_scrollText(matrix.Color(128, 0, 128), datetime, 75);
      // display_scrollText(matrix.Color(128, 0, 128), datetime, 75);
      DEBUG_PRINT("Now: ");
      DEBUG_PRINTLN(datetime);
      display_datetime(datetime, matrix.Color(128, 0, 128), 75);
      delay(10000);
    } else {
      DEBUG_PRINTLN("WiFi Disconnected");
    }
    display_image(BMP_OUTDOOR);
    delay(3000);
    read_inner_temp(inner_temp);
    display_scrollText(matrix.Color(0, 128, 128), inner_temp, 75);
    delay(1000);
}

void fetchDateTime(char str[]) {
    WiFiClient client;
    HTTPClient http;

    String serverPath = WORLD_TIME_API;
    const char* datetime;
  
    // Your Domain name with URL path or IP address with path
    http.begin(client, serverPath.c_str());
  
    // Send HTTP GET request
    int httpResponseCode = http.GET();
  
    if (httpResponseCode>0) {
        DEBUG_PRINT("HTTP Response code: ");
        DEBUG_PRINTLN(httpResponseCode);
    
        JSONVar myObject = JSON.parse(http.getString());
        if (JSON.typeof(myObject) == "undefined") {
            DEBUG_PRINTLN("Parsing input failed!");
        } else {
            DEBUG_PRINTLN(myObject);
            datetime = (const char*) myObject["datetime"];
            char c;
            for (int i=0; i< 19; i++) {
              c = *(datetime + i);
              str[i] = c == 'T' ? ' ' : c;
            }
        }

    } else {
        DEBUG_PRINT("Error code: ");
        DEBUG_PRINTLN(httpResponseCode);
    }
    // Free resources
    http.end();
}

void display_scrollText(uint16_t color, const char text[], uint delayMils) {
    int len = 0;
    char c;
    while ( (c = text[len]) != 0) len++;

    matrix.clear();
    matrix.show();
    matrix.setTextColor(color);
    matrix.setFont();
    for(int x=MATRIX_W; x > (len+1)*-6; x--){
        matrix.clear();
        matrix.setCursor(x, 0);
        matrix.print(text);
        matrix.show();
        delay(delayMils);
    }
}

void display_image(const uint16_t bitmap[]) {
    matrix.clear();
    matrix.drawRGBBitmap(0, 0, bitmap, 8, 8);
    matrix.show();
}

void read_inner_temp(char str_out[]) {
    char str_temp[8];
    float temperature;

    sensors.requestTemperatures();

#ifdef T_SENS_UNIT_CELSIUS
    temperature = sensors.getTempCByIndex(0);
#else
    temperature = sensors.getTempFByIndex(0);
#endif
    DEBUG_PRINT(temperature);
    /* 6 is mininum width, 2 is precision; float value is copied onto str_temp*/
    dtostrf(temperature, 6, 2, str_temp);

#ifdef T_SENS_UNIT_CELSIUS
    DEBUG_PRINTLN(" °C");
    sprintf(str_out,"%s C", str_temp);
#else
    DEBUG_PRINTLN(" °F");
    sprintf(str_out,"%s F", str_temp);
#endif
}

void rainbow() {
    uint16_t colors[] = {0xF800, 0x8400, 0xFFE0, 0x07E0, 0x0410, 0x07FF, 0x001F, 0x8010};
    matrix.clear();
    for (int s=0; s<MATRIX_W*MATRIX_W; s++) {
        for (int y=0; y<MATRIX_H; y++) {
            for (int x=0; x<MATRIX_W; x++) {
                matrix.drawPixel((x+y+s)%MATRIX_W, y, colors[(y*MATRIX_H+x)%8]);
            }
        }
        matrix.show();
        delay(75);
    }
}

void display_datetime(const char datetime_str[], uint16_t color, int delay_mils) {
    //yyyy-mm-dd
    char date_str[11] = {0x00};
    //hh:mm
    char hh_str[3] = {0x00};
    char mm_str[3] = {0x00};
    
    for (int i=0, j=0, h=0; i<19; i++) {
        char c = datetime_str[i];
        if (i<10) {
            date_str[i] = c;
        }
        if (10<i && i<13) {
            hh_str[j] = c;
            j++;
        }
        if (13<i && i<16) {
            mm_str[h] = c;
            h++;
        }
    }

    // uint16_t color = matrix.Color(128, 0, 128);
    display_scrollText(color, date_str, delay_mils);
    matrix.setTextColor(color);
    matrix.setFont(&Tiny3x3a2pt7b);
    matrix.setCursor(1, 2);
    matrix.print(hh_str);
    matrix.setCursor(1, 7);
    matrix.print(mm_str);
    matrix.show();
}
