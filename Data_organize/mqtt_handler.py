import paho.mqtt.client as paho

def on_connect(client, userdata, flags, rc):
    print('CONNACK received with code %d.' % (rc))
    if rc == 0:
        client.subscribe("home/sensors/csv")

def on_message(client, userdata, msg):
    with open("demofile.csv","a") as f:
        f.write(msg.payload.decode())
        f.write("\n")

client = paho.Client()
client.on_connect = on_connect
client.on_message = on_message
client.connect('192.168.1.170', 1883)
client.loop_forever()
