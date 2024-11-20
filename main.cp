#include <Wire.h>
#include "RTClib.h"
#include <ArduinoJson.h>
#include <WiFi.h>
#include <PubSubClient.h>

const String lugar = "Quarto";

// ---- Configurações de Wi-Fi e MQTT
const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqtt_server = "mqtt-dashboard.com";
const int mqtt_port = 1883; 

WiFiClient espClient;
PubSubClient client(espClient);

const char* topic_luminosity = "esp32/luminosity";

RTC_DS1307 rtc;
#define LDR_PIN 34

// Constantes do LDR
const float GAMMA = 0.7;
const float RL10 = 50;

// Resolução analógica do ESP32
#define ADC_MAX 4095
#define V_REF 5

int valorAnterior = 0;
#define SDA_PIN 12
#define SCL_PIN 14

StaticJsonDocument<200> data;

void setup_wifi() {
    delay(10);
    Serial.println();
    Serial.print("Conectando a ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nWiFi conectado");
    Serial.println("Endereço IP: ");
    Serial.println(WiFi.localIP());
}

void reconnect() {
    while (!client.connected()) {
        Serial.print("Tentando conexão MQTT...");
        String clientId = "ESP32Client-";
        clientId += String(random(0xffff), HEX);
        if (client.connect(clientId.c_str())) {
            Serial.println("Conectado");
        } else {
            Serial.print("Falha, rc=");
            Serial.print(client.state());
            Serial.println(" Tentando novamente em 5 segundos");
            delay(5000);
        }
    }
}

void setup() {
    Serial.begin(115200);
    setup_wifi();
    client.setServer(mqtt_server, mqtt_port);

    Wire.begin(SDA_PIN, SCL_PIN);
    if (!rtc.begin()) {
        Serial.println("Não foi possível encontrar o módulo RTC");
        while (1);
    }
    if (!rtc.isrunning()) {
        Serial.println("RTC não está em funcionamento, configurando a data/hora...");
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
    Serial.println("RTC inicializado com sucesso");
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();
    verifyLuminosity();
    delay(1000);
}

void saveLuminosity(float lux) {
    DateTime now = rtc.now();

    String day = String(now.year()) + "/" + String(now.month()) + "/" + String(now.day());
    String time = String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());

    data["day"] = day;
    data["time"] = time;
    data["luminosity"] = lux;
    data["local"] = lugar;


    // Serializar o JSON para uma string
    char buffer[256];
    serializeJson(data, buffer);

    // Publicar o JSON no tópico MQTT
    client.publish(topic_luminosity, buffer);
}

void verifyLuminosity() {
    int analogValue = analogRead(LDR_PIN);
    if (analogValue == valorAnterior) return;
    valorAnterior = analogValue;

    float voltage = analogValue / float(ADC_MAX) * V_REF;
    float resistance = 2000 * voltage / (1 - voltage / V_REF);
    float lux = pow(RL10 * 1e3 * pow(10, GAMMA) / resistance, (1 / GAMMA));

    saveLuminosity(lux);
}
