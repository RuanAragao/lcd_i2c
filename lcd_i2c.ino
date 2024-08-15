// Learn about the ESP32 WiFi simulation in
// https://docs.wokwi.com/guides/esp32-wifi

#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C LCD = LiquidCrystal_I2C(0x27, 20, 4);

#define NTP_SERVER     "pool.ntp.org"
#define UTC_OFFSET     -3 * 3600
#define UTC_OFFSET_DST 0

#define BUTTON1_PIN  5  // Exemplo de pino para o botão 1
#define BUTTON2_PIN  15  // Exemplo de pino para o botão 2

bool backlightState = true;

void spinner() {
  static int8_t counter = 0;
  const char* glyphs = "\xa1\xa5\xdb";
  LCD.setCursor(15, 1);
  LCD.print(glyphs[counter++]);
  if (counter == strlen(glyphs)) {
    counter = 0;
  }
}

void printLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    LCD.setCursor(0, 1);
    LCD.println("Connection Err");
    return;
  }

  LCD.setCursor(12, 0);
  LCD.print(&timeinfo, "%H:%M:%S");

  LCD.setCursor(10, 1);
  LCD.print(&timeinfo, "%d/%m/%Y");

  LCD.setCursor(13, 2);
  LCD.print(&timeinfo, "%Z");
}

void inputControl() {
  int button1State = digitalRead(BUTTON1_PIN);
  int button2State = digitalRead(BUTTON2_PIN);

  if (button1State == HIGH) {
    //Serial.println("Botão 1 pressionado");
    if (backlightState) {
      LCD.noBacklight();
    }

    if (!backlightState) {
      LCD.backlight();
    }

    backlightState = !backlightState;
  }
  if (button2State == HIGH) {
    Serial.println("Botão 2 pressionado");
  }

  delay(100);
}

void setup() {
  Serial.begin(115200);

  pinMode(BUTTON1_PIN, INPUT_PULLDOWN);
  pinMode(BUTTON2_PIN, INPUT_PULLDOWN);

  LCD.init();
  LCD.backlight();
  LCD.setCursor(0, 0);
  LCD.print("Connecting to ");
  LCD.setCursor(0, 1);
  LCD.print("WiFi ");

  WiFi.begin("4326853872", "B111013H", 6);
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    spinner();
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  LCD.clear();
  LCD.setCursor(0, 0);
  LCD.println("Online");
  LCD.setCursor(0, 1);
  LCD.println("Updating time...");

  configTime(UTC_OFFSET, UTC_OFFSET_DST, NTP_SERVER);
  setenv("TZ", "America/Sao_Paulo", 1);

  LCD.clear();
}

void loop() {
  printLocalTime();
  delay(250);

  inputControl();
}
