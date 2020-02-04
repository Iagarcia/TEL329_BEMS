#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define STASSID "<STASSID>"
#define STAPSK  "<STAPSK>"
#define BROKER  "<IP_BROKER>"
#define PORT    1883 /*DEFAUL MQTT PORT*/
#define TOPIC   "<TOPIC>"

// Initialize the client library
WiFiClient espClient;
PubSubClient client;

void setup_WiFi() {
  WiFi.begin(STASSID, STAPSK);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
  return;
}

void connect_MQTT() {
  while(client.connected() == 0) {
    delay(500);
    Serial.println("CONNECT MQTT ");
    Serial.print(".");
    client.connect("SR1_TEMPERATURE1");
  }
  Serial.println("MQTT CLIENT CONNECTED");
  client.publish(TOPIC, "HOLA DESDE ESP8266");
  return;
}

void setup()
{
  pinMode(BUILTIN_LED, OUTPUT);
  Serial.begin(115200);
  Serial.println();
  
  setup_WiFi();

  client.setClient(espClient);
  client.setServer(BROKER, PORT);
}

void loop() {    
  delay(500);
  Serial.println("LOOP AGAIN");
  connect_MQTT();
}
