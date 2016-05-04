#include <SPI.h>
#include <Adafruit_WINC1500.h>

const unsigned int TEMP_SENSOR_PIN = A0;
const float        SUPPLY_VOLTAGE  = 5.0;

const unsigned int PUMP_CYCLE_TEST_PIN = 2;
const unsigned int PUMP_AC_TEST_PIN    = 3;

const unsigned int BAUD_RATE       = 9600;

// Define the WINC1500 board connections below.
// If you're following the Adafruit WINC1500 board
// guide you don't need to modify these:
#define WINC_CS   8
#define WINC_IRQ  7
#define WINC_RST  4
// The SPI pins of the WINC1500 (SCK, MOSI, MISO) should be
// connected to the hardware SPI port of the Arduino.
// On an Uno or compatible these are SCK = #13, MISO = #12, MOSI = #11.
// On an Arduino Zero use the 6-pin ICSP header, see:
//   https://www.arduino.cc/en/Reference/SPI
  
// Setup the WINC1500 connection with the pins above and the default hardware SPI.
Adafruit_WINC1500 WiFi(WINC_CS, WINC_IRQ, WINC_RST);

const char ssid[] = "Apple Network ad29eb";      //  your network SSID (name)
const char pass[] = "yqtianja4cgdu0wa";   // your network password
int status = WL_IDLE_STATUS;

// Initialize the Wifi client library
Adafruit_WINC1500Client client;

// server address:
IPAddress server(192,168,254,51);  // numeric IP for test page (no DNS)

unsigned long       lastTempConnectionTime = 0;
const unsigned long tempPostingInterval    = 60L * 1000L;

bool pump_status = 0; // Pull-up resistor causes '1' to represent pump OFF; '0' for pump ON.
bool ac_status = 1;   // Pull-up resistor causes '0' to represent pump AC ON; '1' for pump AC OFF.

void setup() {
  Serial.begin(BAUD_RATE); //Initialize serial and wait for port to open
  while (!Serial) {;} // wait for serial port to connect. Needed for native USB port only

  pinMode(PUMP_CYCLE_TEST_PIN, INPUT);
  digitalWrite(PUMP_CYCLE_TEST_PIN, HIGH); // Set internal pull-up resistor.

  pinMode(PUMP_AC_TEST_PIN, INPUT);
  digitalWrite(PUMP_AC_TEST_PIN, HIGH); // Set internal pull-up resistor.
  
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true); // don't continue:
  }

  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass); // Connect to WPA/WPA2 network.
    delay(10000); // wait 10 seconds for connection
  }
  
  printWifiStatus(); // you're connected now, so print out the status:
}

void loop() {
  int pump_cycle_current_state = digitalRead(PUMP_CYCLE_TEST_PIN);
  if (pump_cycle_current_state != pump_status) {
    pump_status = pump_cycle_current_state;
    httpRequest("/cycle", !pump_status);
  }

  int pump_ac_current_state = digitalRead(PUMP_AC_TEST_PIN);
  if (pump_ac_current_state != ac_status) {
    ac_status = pump_ac_current_state;
    httpRequest("/ac", !ac_status);
  }

  if (millis() - lastTempConnectionTime > tempPostingInterval) {
    httpRequest("/hub_temp", hubTemp());
    lastTempConnectionTime = millis();
  }
}

int hubTemp() {
  const int sensor_voltage = analogRead(TEMP_SENSOR_PIN);
  const float voltage = sensor_voltage * SUPPLY_VOLTAGE / 1024;
  return scaled_value((voltage * 1000 - 500) / 10); 
}

long scaled_value(const float value) {
  float round_offset = value < 0 ? -0.5 : 0.5;
  return (long)(value * 100 + round_offset);
}

void httpRequest(char *endpoint, int data) {
  // close any connection before send a new request.
  // This will free the socket on the WiFi shield
  client.stop();

  // if there's a successful connection:
  if (client.connect(server, 80)) {
    String payload("{\"state\":\"");
    payload += data;
    payload += "\"}";
    Serial.println("connecting...");
    Serial.print("Endpoint: "); Serial.println(endpoint);
    Serial.print("Payload: "); Serial.println(payload);
    // Make a HTTP request:
    client.print("POST ");
    client.print(endpoint); client.println(" HTTP/1.1");
    client.print("Host: "); client.println(server);
    client.print("Content-Type: "); client.println("application/json");
    client.print("Content-Length: "); client.println(payload.length());
    client.println("Connection: close");
    client.println();
    client.println(payload);
    client.println();
    client.flush();

    printResponse();
   }
  else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
    Serial.print("Failed endpoint: "); Serial.println(endpoint);
    Serial.print("Failed data: "); Serial.println(data);
  }
}

void printResponse() {
  Serial.write("Waiting for response...");
  while(!client.available()){;} //wait for the request to finish.
    
  // if there's incoming data from the net connection.
  // send it out the serial port.  This is for debugging
  // purposes only:
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
