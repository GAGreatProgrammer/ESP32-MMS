void setup() {
  pinMode(BUZZER, OUTPUT);
  pinMode(LED_OK, OUTPUT);
  pinMode(LED_ALERT, OUTPUT);
  pinMode(LED_WARNING, OUTPUT);
  pinMode(HUMIDITY_SENSOR, INPUT);
  pinMode(SELF_TEST_BUTTON, INPUT_PULLUP);
  pinMode(S1_DIP, INPUT_PULLUP);
  pinMode(S2_DIP, INPUT_PULLUP);
  pinMode(S3_DIP, INPUT_PULLUP);
  pinMode(S4_DIP, INPUT_PULLUP);
  pinMode(S5_DIP, INPUT_PULLUP);

  if (!digitalRead(SELF_TEST_BUTTON) == HIGH) {
    digitalWrite(LED_OK, HIGH);
    digitalWrite(LED_ALERT, HIGH);
    digitalWrite(LED_WARNING, HIGH);
  }

  else if (!digitalRead(SELF_TEST_BUTTON) == LOW) {
    digitalWrite(LED_OK, LOW);
    digitalWrite(LED_ALERT, LOW);
    digitalWrite(LED_WARNING, LOW);
  }

  Serial.begin(9600);

  Wire.begin();

  EasyBuzzer.setPin(BUZZER);

  dht.begin();

  bmp.begin();

  initWiFi();
  initMQTT();
  
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "text/plain", "ESP32 MMS");
  });

  AsyncElegantOTA.begin(&server);
  server.begin();

  accelgyro.initialize();

  accelgyro.CalibrateAccel(15);
  accelgyro.CalibrateGyro(15);

  accelgyro.setFullScaleAccelRange(MPU6050_ACCEL_FS_16);
  accelgyro.setFullScaleGyroRange(MPU6050_GYRO_FS_2000);

  Serial.println(accelgyro.getFullScaleAccelRange());
  Serial.println(accelgyro.getFullScaleGyroRange());

  accelgyro.PrintActiveOffsets();
}
