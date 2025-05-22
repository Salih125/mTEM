import paho.mqtt.publish as publish
import paho.mqtt.subscribe as subscribe
import time

def mqtt_send(topic, message, host):
    publish.single(topic, message, hostname=host)

def mqtt_receive(client, userdata, message):
    print(message.payload)
    userdata["message_count"] += 1
    if userdata["message_count"] >= 5:
        client.disconnect()

#mqtt_send("sim", "Test Server", "test.mosquitto.org")
#subscribe.callback(mqtt_receive, "sim", hostname="test.mosquitto.org", userdata={"message_count": 0})