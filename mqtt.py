import paho.mqtt.client as mqtt
# pip install paho-mqtt

# Adresse IP du serveur MQTT
mqtt_server = "test.mosquitto.com"
mqtt_port = 1883
mqtt_topic = "esp32/led"

# Connexion au serveur MQTT
client = mqtt.Client()
client.connect(mqtt_server, mqtt_port, 60)

# Envoyer une commande pour allumer ou éteindre la LED
def led_on():
    client.publish(mqtt_topic, "on")
    print("LED allumée")

def led_off():
    client.publish(mqtt_topic, "off")
    print("LED éteinte")

# Utilisation
led_on()  # Allume la LED
# led_off() # Éteint la LED

# Important : Ne pas oublier d'ajouter une boucle pour maintenir la connexion active si nécessaire
client.loop_start()  # Si besoin d'une boucle active pour d'autres tâches

