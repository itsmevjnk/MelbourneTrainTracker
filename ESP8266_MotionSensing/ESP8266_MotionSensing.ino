#include <ESP8266WiFi.h>
#include <Preferences.h>

#define LED_ACTY                  D4 // activity LED
#define LED_STATE                 D3 // sensing state LED
#define PIR_OUTPUT                D7 // HC-SR501 output pin (trigger time should be set to min)

volatile bool sensorActivated = false;
volatile uint32_t sensorActivateTime;
void ICACHE_RAM_ATTR onSensorActivation() {
  sensorActivated = true;
  sensorActivateTime = millis();
}

Preferences prefs;
#define PREFS_NAMESPACE           "config" // preferences namespace

size_t readStringInput(char* buf, size_t len) {
  size_t offset = 0;
  bool done = false;
  while (!done) {
    while (!Serial.available()); // wait until there's a character
    char c = Serial.read(); 
    switch (c) {
      case '\b': // backspace
        if (offset > 0) {
          offset--;
          Serial.print("\b \b"); // clear character
        }
        break;
      case '\r': // carriage return - ignore
        break;
      case '\n': // new line - terminate
        done = true;
        Serial.println();
        break;
      default:
        if (offset < len - 1) {
          Serial.write(c); // echo
          buf[offset++] = c;
        }
        break;
    }
  }
  buf[offset] = '\0';
  return offset;
}

void configPrefs() {
  if (!prefs.begin(PREFS_NAMESPACE, false)) {
    Serial.println("Cannot open preferences namespace " PREFS_NAMESPACE " in RW mode - halting");
    while (true); // fatal error
  }

  char stringBuf[64]; // string buffer

  /* Wi-Fi SSID */
  size_t bufLength = prefs.getBytesLength("ssid");
  if (bufLength == 0 || bufLength > sizeof(stringBuf))
    Serial.println("No Wi-Fi SSID has been set.");
  else {
    if (!prefs.getBytes("ssid", stringBuf, bufLength))
      Serial.println("Error reading Wi-Fi SSID.");
    else {
      Serial.print("The currently set Wi-Fi SSID is: "); Serial.println(stringBuf);
    }
  }
  Serial.print("Enter the Wi-Fi SSID to connect to: ");
  bufLength = readStringInput(stringBuf, sizeof(stringBuf));
  if (!prefs.putBytes("ssid", stringBuf, bufLength + 1)) { // write terminator char too
    Serial.println("Cannot write Wi-Fi SSID - halting");
    while (true);
  }

  /* Wi-Fi password */
  bufLength = prefs.getBytesLength("pw");
  if (bufLength == 0 || bufLength > sizeof(stringBuf))
    Serial.println("No Wi-Fi password has been set.");
  else {
    if (!prefs.getBytes("pw", stringBuf, bufLength))
      Serial.println("Error reading Wi-Fi password.");
    else {
      Serial.print("The currently set Wi-Fi password is: "); Serial.println(stringBuf);
    }
  }
  Serial.print("Enter the Wi-Fi password (or leave it empty for open networks): ");
  bufLength = readStringInput(stringBuf, sizeof(stringBuf));
  if (!prefs.putBytes("pw", stringBuf, bufLength + 1)) { // write terminator char too
    Serial.println("Cannot write Wi-Fi password - halting");
    while (true);
  }

  prefs.end();
  if (!prefs.begin(PREFS_NAMESPACE, true)) {
    Serial.println("Cannot open preferences namespace " PREFS_NAMESPACE " in RO mode - halting");
    while (true); // fatal error
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(74880); // match ESP8266 boot log
  pinMode(PIR_OUTPUT, INPUT);
  pinMode(LED_ACTY, OUTPUT); digitalWrite(LED_ACTY, HIGH); // will turn off after init
  pinMode(LED_STATE, OUTPUT); digitalWrite(LED_STATE, HIGH); // will turn off once preferences have been read

  if (!prefs.begin(PREFS_NAMESPACE, false)) {
    Serial.println("Cannot open preferences namespace " PREFS_NAMESPACE " in RO mode - entering config mode");
    configPrefs();
  }

readConfig:
  char wifiSSID[64], wifiPassword[64];
  size_t bufLength = prefs.getBytesLength("ssid");
  if (bufLength == 0 || bufLength > sizeof(wifiSSID)) {
    Serial.println("Wi-Fi SSID has not been configured - entering config mode");
    prefs.end(); configPrefs(); goto readConfig;
  }
  else if (!prefs.getBytes("ssid", wifiSSID, bufLength)) {
    Serial.println("Cannot read Wi-Fi SSID - entering config mode");
    prefs.end(); configPrefs(); goto readConfig;
  }

  bufLength = prefs.getBytesLength("pw");
  if (bufLength == 0 || bufLength > sizeof(wifiPassword)) {
    Serial.println("Wi-Fi password has not been configured - entering config mode");
    prefs.end(); configPrefs(); goto readConfig;
  }
  else if (!prefs.getBytes("pw", wifiPassword, bufLength)) {
    Serial.println("Cannot read Wi-Fi password - entering config mode");
    prefs.end(); configPrefs(); goto readConfig;
  }

  WiFi.begin(wifiSSID, (wifiPassword[0] == '\0') ? NULL : wifiPassword); // open network if password is empty
  Serial.print("Connecting to Wi-Fi SSID "); Serial.print(wifiSSID); Serial.print("...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print('.');
  }
  Serial.println("done.");

  digitalWrite(LED_STATE, LOW);

  attachInterrupt(digitalPinToInterrupt(PIR_OUTPUT), onSensorActivation, RISING);

  digitalWrite(LED_ACTY, LOW);
}

bool state = false; // sensing state (timed)
uint32_t stateUpdate;

void loop() {
  // put your main code here, to run repeatedly:
  if (sensorActivated) {
    Serial.print("Sensor activated at "); Serial.println(sensorActivateTime);
    sensorActivated = false;
    state = true;
    stateUpdate = sensorActivateTime;
  }
}
