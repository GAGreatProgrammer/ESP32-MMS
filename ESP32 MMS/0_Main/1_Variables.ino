#define MPU6050_ACCEL_FS_2          0x00
#define MPU6050_ACCEL_FS_4          0x01
#define MPU6050_ACCEL_FS_8          0x02
#define MPU6050_ACCEL_FS_16         0x03

#define MPU6050_GYRO_FS_250         0x00
#define MPU6050_GYRO_FS_500         0x01
#define MPU6050_GYRO_FS_1000        0x02
#define MPU6050_GYRO_FS_2000        0x03

float DHT11_Temperature, DHT11_Humidity, DHT11_Heat_Index;
float BMP180_Temperature, BMP180_Pressure, BMP180_Altitude;

int16_t ax, ay, az;
int16_t gx, gy, gz;


//WiFi
const char *ssid = "";
const char *password = "";


// MQTT Broker
const char *mqtt_broker = "broker.emqx.io";
const char *mqtt_username = "emqx";
const char *mqtt_password = "public";
const int   mqtt_port = 1883;


byte willQoS = 0;
const char* willTopic = "JHogcfcSZjkAhnp/Main/SYS/Status";
const char* willMessage = "Disconnected";
boolean willRetain = false;

const char *MainTopicID = "JHogcfcSZjkAhnp";

const char *System_Info_Topic =   "JHogcfcSZjkAhnp/Main/SYS/Info";
const char *System_Status_Topic = "JHogcfcSZjkAhnp/Main/SYS/Status";
const char *System_Error_Topic =  "JHogcfcSZjkAhnp/Main/SYS/Error";

const char *Humidity_Sensor_Humidity_Topic =    "JHogcfcSZjkAhnp/Main/Sensors/Humidity/Humidity";
const char *Humidity_Sensor_Temperature_Topic = "JHogcfcSZjkAhnp/Main/Sensors/Humidity/Temperature";
const char *Humidity_Sensor_HeatIndex_Topic =   "JHogcfcSZjkAhnp/Main/Sensors/Humidity/HeatIndex";


const char *BMP180_Sensor_Temperature_Topic = "JHogcfcSZjkAhnp/Main/Sensors/BMP180/Temperature";
const char *BMP180_Sensor_Pressure_Topic =    "JHogcfcSZjkAhnp/Main/Sensors/BMP180/Pressure";
const char *BMP180_Sensor_Altitude_Topic =    "JHogcfcSZjkAhnp/Main/Sensors/BMP180/Altitude";

String MAC;
String IP;

bool MQTT_Connection_Error = false;


float Temperature_Offset = 0;
float Humidity_Offset = 0;
bool HighTemperature = false;

bool Alarm = false;

AsyncWebServer server(80);

WiFiClient espClient;

PubSubClient client(espClient);

DHT dht(HUMIDITY_SENSOR, DHTTYPE);

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }

  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);

  MAC = WiFi.macAddress();
  IP = WiFi.localIP().toString();

  delay(100);
  Serial.println("Connected to the WiFi Network!");

  Serial.print("RSSI: ");
  Serial.println(WiFi.RSSI());
}

void initMQTT() {
  client.setServer(mqtt_broker, mqtt_port);

  client.setCallback(callback);

  while (!client.connected()) {
    String client_id = "ESP32-MMS-Client-";
    
    client_id += String(WiFi.macAddress());

    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password, willTopic, willQoS, willRetain, willMessage)) {
      Serial.println("Broker connected!");

      MQTT_Connection_Error = false;

      Initialize = true;

      digitalWrite(LED_OK, HIGH);

      digitalWrite(LED_WARNING, LOW);

      tone(BUZZER, 2700, 1000);

      client.subscribe(System_Info_Topic);
      client.subscribe(System_Status_Topic);
      client.subscribe(System_Error_Topic);

      client.subscribe(BMP180_Sensor_Pressure_Topic);
      client.subscribe(BMP180_Sensor_Temperature_Topic);
      client.subscribe(BMP180_Sensor_Altitude_Topic);

      client.subscribe(Humidity_Sensor_Humidity_Topic);
      client.subscribe(Humidity_Sensor_Temperature_Topic);
      client.subscribe(Humidity_Sensor_HeatIndex_Topic);

    }

    else {
      Serial.print("Failed with state ");
      Serial.print(client.state());

      digitalWrite(LED_WARNING, HIGH);

      MQTT_Connection_Error = true;

      return;
    }
  }

  systemInfo();
  systemStatus();
}
