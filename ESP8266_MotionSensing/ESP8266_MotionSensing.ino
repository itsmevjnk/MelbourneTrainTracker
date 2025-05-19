#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Preferences.h>
#include <mDNSResolver.h> // https://github.com/lbussy/mDNSResolver

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

#define DEFAULT_HOST                          "melbtrains.local" // default tracker address (IP address/mDNS hostname)

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

  /* tracker hostname */
  bufLength = prefs.getBytesLength("host");
  strcpy(stringBuf, DEFAULT_HOST);
  if (bufLength > 0 && bufLength <= sizeof(stringBuf)) {
    if (!prefs.getBytes("host", stringBuf, bufLength))
      Serial.println("Error reading tracker address, using default value.");
  }
  else Serial.println("Tracker address is not set, using default value.");
  Serial.print("The currently set tracker address/hostname is: "); Serial.println(stringBuf);
  Serial.print("Enter the tracker address (for mDNS hostname, enter the hostname followed by .local): ");
  bufLength = readStringInput(stringBuf, sizeof(stringBuf));
  if (!prefs.putBytes("host", stringBuf, bufLength + 1)) { // write terminator char too
    Serial.println("Cannot write tracker address - halting");
    while (true);
  }

  prefs.end();
  if (!prefs.begin(PREFS_NAMESPACE, true)) {
    Serial.println("Cannot open preferences namespace " PREFS_NAMESPACE " in RO mode - halting");
    while (true); // fatal error
  }
}

char url[4 + 3 + 64 + 7 + 3 + 1] = "http://" DEFAULT_HOST; // http://[address]/driver?s=(0/1)
size_t urlQueryOffset; // offset to insert state

WiFiUDP udp;
mDNSResolver::Resolver resolver(udp);

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
  Serial.print("Local IP address: "); Serial.println(WiFi.localIP());
  resolver.setLocalIP(WiFi.localIP());

  bufLength = prefs.getBytesLength("host");
  char* address = &url[7];
  if (bufLength > 0 && bufLength <= 64) {
    if (!prefs.getBytes("host", address, bufLength)) 
      Serial.println("Cannot read tracker address - using default value");
  }
  else Serial.println("Tracker address is not set - using default value");

  bufLength = strlen(address); // reuse for address length
  if (bufLength > 6 && !strcmp(&address[bufLength - 6], ".local")) { // mDNS hostname detected
    Serial.print("Resolving mDNS hostname "); Serial.print(address); Serial.print("...");
    while (true) {
      IPAddress ip = resolver.search(address);
      if (ip == INADDR_NONE) {
        Serial.print('.');
        delay(1000); // try again
        continue;
      }

      strcpy(address, ip.toString().c_str()); // copy IP address to address field
      Serial.print("done ("); Serial.print(address); Serial.println(')');
      break;
    }
  }
  strcpy(&url[strlen(url)], "/driver?s=0"); // add API endpoint
  Serial.print("API endpoint: "); Serial.println(url);
  urlQueryOffset = strlen(url) - 1; // last character (0/1) - see above

  digitalWrite(LED_STATE, LOW);

  attachInterrupt(digitalPinToInterrupt(PIR_OUTPUT), onSensorActivation, RISING);

  digitalWrite(LED_ACTY, LOW);
}

bool state = false; // sensing state (timed)
uint32_t stateUpdate;
bool update = true; // set when it's time to update driver state

WiFiClient client;
HTTPClient http;

void loop() {
  resolver.loop();

  if (sensorActivated) {
    Serial.print("Sensor activated at "); Serial.println(sensorActivateTime);
    sensorActivated = false;
    if (!state) update = true; // only update on state change
    state = true; digitalWrite(LED_STATE, HIGH);
    stateUpdate = sensorActivateTime;
  }

  if (state && millis() - stateUpdate > 30000) {
    state = false; digitalWrite(LED_STATE, LOW);
    update = true;
  }

  if (update) {
    digitalWrite(LED_ACTY, HIGH);
    url[urlQueryOffset] = (state) ? '1' : '0';
    http.begin(client, url);
    Serial.print("Updating LED driver state ("); Serial.print(url); Serial.print(")...");
    int httpCode = http.POST("");
    if (httpCode <= 0) {
      Serial.print("failed ("); Serial.print(http.errorToString(httpCode)); Serial.println(").");
    } else if (httpCode == HTTP_CODE_OK) {
      Serial.println("done.");
    } else {
      Serial.print("failed (HTTP "); Serial.print(httpCode); Serial.println(").");
    }
    http.end();
    update = false;
    digitalWrite(LED_ACTY, LOW);
  }
}
