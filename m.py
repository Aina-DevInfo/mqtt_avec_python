import paho.mqtt.client as mqtt

# Public Mosquitto broker
broker_address = "192.168.240.168"
topic = "esp32/led"

def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected to broker")
    else:
        print("Connection failed with code", rc)

def publish_message(client, message):
    client.publish(topic, message)
    print(f"Message '{message}' sent to topic '{topic}'")

if __name__ == "__main__":
    # Create an MQTT client instance
    client = mqtt.Client()

    # Assign event callbacks
    client.on_connect = on_connect

    # Connect to the broker
    client.connect(broker_address, 1883, 60)

    # Start the loop to process callbacks
    client.loop_start()

    while True:
        command = input("Enter 'on' to turn LED on, 'off' to turn LED off: ").strip()
        if command in ["on", "off"]:
            publish_message(client, command)
        else:
            print("Invalid command. Please enter 'on' or 'off'.")
