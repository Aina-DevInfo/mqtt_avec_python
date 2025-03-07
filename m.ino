#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "NY";  // Replace with your WiFi SSID
const char* password = "00000000";  // Replace with your WiFi password
const char* mqtt_server = "test.mosquitto.org";  // Use the public Mosquitto broker

WiFiClient espClient;
PubSubClient client(espClient);
const int ledPin = 5;  // GPIO pin for the LED

void setup_wifi() {
  delay(10);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;

  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  if (String(topic) == "esp32/led") {
    if (messageTemp == "on") {
      digitalWrite(ledPin, HIGH);  // Turn LED on
      Serial.println("LED ON");
    } else if (messageTemp == "off") {
      digitalWrite(ledPin, LOW);  // Turn LED off
      Serial.println("LED OFF");
    }
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
      client.subscribe("esp32/led");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  pinMode(ledPin, OUTPUT);
  setup_wifi();
  client.setServer(mqtt_server, 1883);  // Connect to Mosquitto public broker
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
