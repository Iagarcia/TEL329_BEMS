/*
 Basic ESP8266 MQTT example

 This sketch demonstrates the capabilities of the pubsub library in combination
 with the ESP8266 board/library.

 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic" every two seconds
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "inTopic" is an 1, switch ON the ESP Led,
    else switch it off

 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.

 To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"

*/

#define ID_RED      1
#define ID_YELLOW   2
#define ID_GREEN    3
#define ID_BLUE     4
#define LED_RED    D1
#define LED_YELLOW D2
#define LED_GREEN  D3
#define LED_BLUE   D4

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

const char* ssid = "Evilness";
const char* password = "todocalzapollo";
const char* mqtt_server = "192.168.43.214";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

int illumination_level(char led_illumination){
  switch(led_illumination){
    case '1':
      return 256;
    case '2':
      return 512;
    case '3':
      return 768;
    case '4':
      return 1023;
  }
}

void handle_change(byte* payload){
  Serial.println("Inside handle_change");
  Serial.println((char*)payload);
  String data = (char*)payload;
  Serial.println("Before data");
  Serial.println(data);
  Serial.println(data[0]);
  Serial.println(data[1]);
  // Turn OFF led in position 0 indepedent from its Duty Cycle
  if ((char)payload[1] == '0'){
    Serial.println("Enters first if");
    char led_id = (char)payload[0];
    switch (led_id) {
      case '1':
        Serial.println("Turning OFF RED");
        analogWrite(LED_RED, 0);
        break;
      case '2':
        Serial.println("Turning OFF YELLOW");
        analogWrite(LED_YELLOW, 0);
        break;
      case '3':
        Serial.println("Turning OFF GREEN");
        analogWrite(LED_GREEN, 0);
        break;
      case '4':
        Serial.println("Turning OFF BLUE");
        analogWrite(LED_BLUE, 0);
        break;
    }
  } else{
    Serial.println("Enters else");
    char led_id = (char)payload[0];
    char led_illumination = (char)payload[2];
    switch (led_id){
      case '1':
        Serial.println("Changing illumination of RED");
        analogWrite(LED_RED, illumination_level(led_illumination));
        break;
     case '2':
        Serial.println("Changing illumination of YELLOW");
        analogWrite(LED_YELLOW, illumination_level(led_illumination));
        break;
     case '3':
        Serial.println("Changing illumination of GREEN");
        analogWrite(LED_GREEN, illumination_level(led_illumination));
        break;
     case '4':
        Serial.println("Changing illumination of BLUE");
        analogWrite(LED_BLUE, illumination_level(led_illumination));
        break;
    }
  }
  
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  //if ((char)payload[0] == '1') {
    //digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  //} else {
    //digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  //}
  Serial.println("Before handle_change");
  handle_change(payload);

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("bems/room/temperature", "Sending temp conditions...");
      // ... and resubscribe
      client.subscribe("bems/room/light");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void set_PWM(){
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  
  analogWrite(LED_RED, 512);
  analogWrite(LED_YELLOW, 512);
  analogWrite(LED_GREEN, 512);
  analogWrite(LED_BLUE, 512);
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output

  // LED Config for PWM
  set_PWM();
  
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    snprintf (msg, 50, "hello world #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("bems/room/temperature", msg);
  }
}
