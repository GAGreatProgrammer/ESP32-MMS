void systemInfo() {
  String Device = "ESP32: " + WiFi.macAddress();
  client.publish(System_Info_Topic, Device.c_str());
}


void systemStatus() {
  String Status = "Connected";
  client.publish(System_Status_Topic, Status.c_str());
  client.publish(System_Error_Topic, "No Error");
}


void systemError(String _error) {
  client.publish(System_Error_Topic, _error.c_str());
  digitalWrite(LED_WARNING, HIGH);
}

void WifiStatus(){
  if (millis() - lastMillisWiFi >= 30000) {
    switch (WiFi.status()) {
      case WL_NO_SSID_AVAIL:
        Serial.println("Configured SSID cannot be reached");
        digitalWrite(LED_WARNING, HIGH);
        break;
      case WL_CONNECT_FAILED:
        Serial.println("Connection failed");
        digitalWrite(LED_WARNING, HIGH);
        break;
    }
    lastMillisWiFi = millis();
  }
}

void MQTTStatus(){
    if (millis() - lastMillisMQTT >= 30000) {
    switch (client.connected()) {
      case 0:
        initMQTT();
        break;
    }
    lastMillisMQTT = millis();
  }
}


void dhtValue() {
  if (millis() - lastMillisHumidity >= 5000) {
    lastMillisHumidity = millis();

    if (!digitalRead(S1_DIP) == 1) {

      float h = dht.readHumidity();

      float t = dht.readTemperature() - Temperature_Offset;

      if (isnan(h) || isnan(t)) {
        systemError("DHT11 Error");
        digitalWrite(LED_ALERT, HIGH);
        return;
      }

      float hic = dht.computeHeatIndex(t, h, false);

      DHT11_Temperature = t;
      DHT11_Humidity = h;
      DHT11_Heat_Index = hic;

      client.publish(Humidity_Sensor_Humidity_Topic, String(h).c_str());
      client.publish(Humidity_Sensor_Temperature_Topic, String(t).c_str());
      client.publish(Humidity_Sensor_HeatIndex_Topic, String(hic).c_str());
    }
  }
}


void BMP180Vaue() {

  if (millis() - lastMillisBMP180Sensor >= 1000) {
    lastMillisBMP180Sensor = millis();

    if (!digitalRead(S1_DIP) == 1) {

      float T = bmp.readTemperature();

      float P = bmp.readPressure();

      float A = bmp.readAltitude(101500);

      BMP180_Temperature = T;
      BMP180_Pressure = P;
      BMP180_Altitude = A;

      client.publish(BMP180_Sensor_Temperature_Topic, String(T).c_str());
      client.publish(BMP180_Sensor_Pressure_Topic, String(P).c_str());
      client.publish(BMP180_Sensor_Altitude_Topic, String(A).c_str());
    }
  }
}


void MPU6050Values() {

  if (millis() - lastMillisMPU6050Sensor >= 10) {
    lastMillisMPU6050Sensor = millis();

    if (!digitalRead(S2_DIP) == 1) {

      accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

      char Data[100];
      snprintf(Data, 100, "%d,%d,%d,%d,%d,%d", ax, ay, az, gx, gy, gz);

      Serial.println(Data);
    }

    if (!digitalRead(S3_DIP) == 1) {

      accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

      Serial.println(ax);
    }

    if (!digitalRead(S4_DIP) == 1) {

      accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

      Serial.println(ay);
    }

    if (!digitalRead(S5_DIP) == 1) {

      accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

      Serial.println(az);
    }
  }
}
