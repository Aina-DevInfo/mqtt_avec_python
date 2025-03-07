import time
import paho.mqtt.client as mqtt

# Paramètres du broker MQTT
broker = "192.168.45.168"
port = 1883
topic = "home/led"  # Le topic que l'ESP32 écoute

# Fonction pour se connecter au broker
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connecté au broker MQTT")
    else:
        print("Erreur de connexion. Code : ", rc)

# Créer un client MQTT
client = mqtt.Client()

# Attacher la fonction de connexion
client.on_connect = on_connect

# Se connecter au broker
client.connect(broker, port, 60)

# Fonction pour allumer la LED
def turn_led_on():
    client.publish(topic, "ON")
    print("Message 'ON' envoyé au topic")

# Fonction pour éteindre la LED
def turn_led_off():
    client.publish(topic, "OFF")
    print("Message 'OFF' envoyé au topic")

# Exemple d'utilisation
client.loop_start()

turn_led_on()  # Allume la LED
time.sleep(5)  # Garde la LED allumée pendant 5 secondes
turn_led_off()  # Éteint la LED

client.loop_stop()
