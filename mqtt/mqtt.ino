#include <WiFi.h>
#include <PubSubClient.h>

// WiFi Credentials
const char* ssid = "poco";
const char* password = "ssss1111";

// MQTT Broker details
const char* mqtt_server = "192.168.243.113";  // Your laptop's IP address
const int mqtt_port = 1883;
const char* mqtt_topic = "test/topic";  // Topic to subscribe to

WiFiClient espClient;
PubSubClient client(espClient);

// Callback function when a message is received
void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message received on topic: ");
    Serial.println(topic);
    
    Serial.print("Message: ");
    for (int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
    }
    Serial.println();
}

void setup() {
    Serial.begin(115200);

    // Connect to WiFi
    Serial.print("Connecting to WiFi...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to WiFi!");
    Serial.print("ESP32 IP Address: ");
    Serial.println(WiFi.localIP());

    // Connect to MQTT Broker
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);

    while (!client.connected()) {
        Serial.print("Connecting to MQTT...");
        if (client.connect("ESP32_Client")) {
            Serial.println("Connected!");
            client.subscribe(mqtt_topic);
            Serial.println("Subscribed to topic: test/topic");
        } else {
            Serial.print("Failed, rc=");
            Serial.print(client.state());
            Serial.println(" Retrying in 5 seconds...");
            delay(5000);
        }
    }
}

void loop() {
    client.loop();  // Keep MQTT connection alive
}
