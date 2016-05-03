#include <SPI.h>
#include <Adafruit_WINC1500.h>

// Define the WINC1500 board connections below.
// If you're following the Adafruit WINC1500 board
// guide you don't need to modify these:
#define WINC_CS   8
#define WINC_IRQ  7
#define WINC_RST  4
#define WINC_EN   2     // or, tie EN to VCC and comment this out
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

unsigned long lastConnectionTime = 0;            // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 10L * 1000L; // delay between updates, in milliseconds

bool pump_status = 0;
bool ac_status = 0;

void setup() {
#ifdef WINC_EN
  pinMode(WINC_EN, OUTPUT);
  digitalWrite(WINC_EN, HIGH);
#endif

  Serial.begin(9600); //Initialize serial and wait for port to open
  while (!Serial) {;} // wait for serial port to connect. Needed for native USB port only

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
  // if ten seconds have passed since your last connection,
  // then connect again and send data:
  if (millis() - lastConnectionTime > postingInterval) {
      httpRequest("/cycle", pump_status);
      httpRequest("/ac", ac_status);
      pump_status = !pump_status;
      ac_status = 1;
  }
}

void httpRequest(char *endpoint, int data) {
  // close any connection before send a new request.
  // This will free the socket on the WiFi shield
  client.stop();

  // if there's a successful connection:
  if (client.connect(server, 80)) {
    Serial.println("connecting...");
    Serial.print("Endpoint: "); Serial.println(endpoint);
    Serial.print("Pump Data: "); Serial.println(data);
    // Make a HTTP request:
    client.print("POST ");
    client.print(endpoint); client.println(" HTTP/1.1");
    client.print("Host: "); client.println(server);
    client.print("Content-Type: "); client.println("application/json");
    client.print("Content-Length: "); client.println("13");
    client.println("Connection: close");
    client.println();
    client.print("{\"state\":\""); client.print(data); client.println("\"}");
    client.println();
    client.flush();

    printResponse();
    
    // note the time that the connection was made:
    lastConnectionTime = millis();
   }
  else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
  }
}

void printResponse() {
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
