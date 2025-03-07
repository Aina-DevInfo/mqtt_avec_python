#include <WiFi.h>
#include <PubSubClient.h>

// Informations sur le réseau WiFi
const char* ssid = "TON_SSID";
const char* password = "TON_PASSWORD";

// Adresse IP de ton serveur MQTT
const char* mqtt_server = "192.168.1.10"; // Exemple d'adresse IP locale

WiFiClient espClient;
PubSubClient client(espClient);

const int ledPin = 2; // GPIO2 correspond à la LED intégrée de l'ESP32

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connexion à ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connecté");
  Serial.println("Adresse IP: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message reçu sur le topic : ");
  Serial.println(topic);

  String messageTemp;

  for (int i = 0; i < length; i++) {
    messageTemp += (char)message[i];
  }

  Serial.print("Message : ");
  Serial.println(messageTemp);

  // Contrôle de la LED
  if (String(topic) == "esp32/led") {
    if (messageTemp == "on") {
      digitalWrite(ledPin, HIGH); // Allume la LED
    } else if (messageTemp == "off") {
      digitalWrite(ledPin, LOW);  // Éteint la LED
    }
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Tentative de connexion au serveur MQTT...");
    if (client.connect("ESP32Client")) {
      Serial.println("connecté");
      client.subscribe("esp32/led"); // S'abonner au topic pour contrôler la LED
    } else {
      Serial.print("échec, rc=");
      Serial.print(client.state());
      Serial.println(" nouvelle tentative dans 5 secondes");
      delay(5000);
    }
  }
}

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);  // Assurez-vous que la LED est éteinte au départ
  
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback); // Définir la fonction callback pour gérer les messages reçus
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
