#ifndef _SETTINGS_H_
#define _SETTINGS_H_
// ============================================================================
// Configuration parameters
// ============================================================================

// Serial log settings
//-----------------------------------------------------------------------------
// SERIAL_SPEED: the boudrate to use for USB serial connection.
// SERIAL_DEBUG: Comment this to disable serial debug logging.
#define SERIAL_SPEED 115200
#define SERIAL_DEBUG

// WiFi settings
// ----------------------------------------------------------------------------
// WIFI_SSID: the name of the WiFi we want to connect
// WIFI_PASS: the WiFi password
// WIFI_TIMEOUT: mils to wait before to consider wifi connection failed
#define WIFI_SSID ""
#define WIFI_PASS ""
#define WIFI_TIMEOUT 5000

// Clock settings
// ----------------------------------------------------------------------------
// WORLD_TIME_API_TIMEZONE: the timezone used to fetch the correct time.
#define WORLD_TIME_API_TIMEZONE "Europe/Rome"
#define WORLD_TIME_API ("http://worldtimeapi.org/api/timezone/" WORLD_TIME_API_TIMEZONE)

// NeoMatrix settings
// ----------------------------------------------------------------------------
// MATRIX_W:          Led matrix width  (number of led/pixel)
// MATRIX_H:          Led matrix height (number of led/pixel)
// MATRIX_PIN:        Pin number for data connection.
// MATRIX_BRIGHTNESS: Brightness setting, 0=minimum (off), 255=brightest.
// MATRIX_TYPE:       Defines the layout of the matrix. See https://github.com/adafruit/Adafruit_NeoMatrix/blob/master/Adafruit_NeoMatrix.h
// MATRIX_PIXEL_TYPE: Defines the type of the led used. See https://github.com/adafruit/Adafruit_NeoPixel/blob/master/Adafruit_NeoPixel.h
#define MATRIX_W 8
#define MATRIX_H 8
#define MATRIX_PIN 15
#define MATRIX_BRIGHTNESS 64
#define MATRIX_TYPE (NEO_MATRIX_BOTTOM + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG)
#define MATRIX_PIXEL_TYPE (NEO_GRB + NEO_KHZ800)

// Temp setting
// ------------------------------------------------------------------------------
// T_SENS_PIN: Pin number for data connection
// T_SENS_UNIT_CELSIUS: comment this to read temperature as Fahrenheit
#define T_SENS_PIN 13
#define T_SENS_UNIT_CELSIUS
// ============================================================================
#endif
