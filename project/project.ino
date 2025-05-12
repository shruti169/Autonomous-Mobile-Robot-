#include <WiFi.h>
#include <PubSubClient.h>

// WiFi Credentials
const char* ssid = "poco";
const char* password = "ssss1111";

// MQTT Broker details
const char* mqtt_server = "192.168.57.113";
const int mqtt_port = 1883;
const char* mqtt_topic = "test/topic";

// Motor pins
int motor1Pin1 = 14;
int motor1Pin2 = 27;
int enable1Pin = 13;

int motor2Pin1 = 26;
int motor2Pin2 = 25;
int enable2Pin = 32;

WiFiClient espClient;
PubSubClient client(espClient);

// Callback function when a message is received
void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message received on topic: ");
    Serial.println(topic);
    
    Serial.print("Message: ");
    String message = "";
    for (int i = 0; i < length; i++) {
        message += (char)payload[i];
    }
    Serial.println(message);

    // Control motors based on message
    if (message == "forward") {
        digitalWrite(motor1Pin1, HIGH);
        digitalWrite(motor1Pin2, LOW);
        analogWrite(enable1Pin, 250);

        digitalWrite(motor2Pin1, HIGH);
        digitalWrite(motor2Pin2, LOW);
        analogWrite(enable2Pin, 250);
    } else if (message == "backward") {
        digitalWrite(motor1Pin1, LOW);
        digitalWrite(motor1Pin2, HIGH);
        analogWrite(enable1Pin, 250);

        digitalWrite(motor2Pin1, LOW);
        digitalWrite(motor2Pin2, HIGH);
        analogWrite(enable2Pin, 250);
    } else if (message == "stop") {
        digitalWrite(motor1Pin1, LOW);
        digitalWrite(motor1Pin2, LOW);
        analogWrite(enable1Pin, 0);

        digitalWrite(motor2Pin1, LOW);
        digitalWrite(motor2Pin2, LOW);
        analogWrite(enable2Pin, 0);
    }
}

void setup() {
    Serial.begin(115200);

    // Initialize motor pins
    pinMode(motor1Pin1, OUTPUT);
    pinMode(motor1Pin2, OUTPUT);
    pinMode(enable1Pin, OUTPUT);
    
    pinMode(motor2Pin1, OUTPUT);
    pinMode(motor2Pin2, OUTPUT);
    pinMode(enable2Pin, OUTPUT);

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
            Serial.println("Subscribed to topic: " + String(mqtt_topic));
        } else {
            Serial.print("Failed, rc=");
            Serial.print(client.state());
            Serial.println(" Retrying in 5 seconds...");
            delay(5000);
        }
    }
}

void loop() {
    client.loop(); // Keep MQTT connection alive
}
