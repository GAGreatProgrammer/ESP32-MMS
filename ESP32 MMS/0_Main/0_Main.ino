#include "PubSubClient.h"
#include "EasyBuzzer.h"
#include "DHT.h"
#include "Wire.h"
#include "Adafruit_BMP085.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include "WiFi.h"
#include "AsyncTCP.h"
#include "ESPAsyncWebServer.h"
#include "AsyncElegantOTA.h"


#define BUZZER 13
#define LED_OK 14
#define LED_ALERT 15
#define LED_WARNING 16
#define HUMIDITY_SENSOR 17
#define SELF_TEST_BUTTON 18
#define S1_DIP 23
#define S2_DIP 25
#define S3_DIP 26
#define S4_DIP 27
#define S5_DIP 32


#define DHTTYPE DHT11

bool Initialize = false;

Adafruit_BMP085 bmp;

MPU6050 accelgyro(0x68);

unsigned long lastMillisWiFi = 0;
unsigned long lastMillisMQTT = 0;

unsigned long lastMillisBuzzer = 0;

unsigned long lastMillisBMP180Sensor = 0;

unsigned long lastMillisMPU6050Sensor = 0;

unsigned long lastMillisHumidity = 0;

unsigned long lastMillisSelfTestButton = 0;

unsigned long lastMillisOkLed = 0;
unsigned long lastMillisAlertLed = 0;
unsigned long lastMillisWarningLed = 0;
