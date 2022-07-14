// ============================================================================
// Configuration parameters
// ============================================================================
//
// Serial log settings
//-----------------------------------------------------------------------------
// SERIAL_SPEED: the boudrate to use for USB serial connection. Comment this to
// disable serial logging.
#define SERIAL_SPEED 115200
#ifdef SERIAL_SPEED
    #define SERIAL_PRINT(msg) (Serial.print((msg)))
#else
    #define SERIAL_PRINT(msg) 
#endif
// 
// WiFi settings
// ----------------------------------------------------------------------------
// WIFI_SSID: the name of the WiFi we want to connect
// WIFI_PASS: the WiFi password
#define WIFI_SSID ""
#define WIFI_PASS ""
//
// Clock settings
// ----------------------------------------------------------------------------
// NTP_SERVER: hostname of the ntp server (optional). Default pool.ntp.org
// NTP_OFFSET: timezone offset in seconds (optional). Default 0
#define NTP_SERVER "it.pool.ntp.org"
#define NTP_OFFSET 7200 
// ============================================================================
