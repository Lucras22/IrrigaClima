#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// Definindo os pinos para os sensores
#define RAIN_SENSOR_PIN 34
#define SOIL_MOISTURE_PIN 32
#define UV_SENSOR_PIN 25
#define RELAY_PIN 27

// Conexão I2C para o BME280
#define BME_SDA 23
#define BME_SCL 22

// Inicializando o sensor BME280
Adafruit_BME280 bme;

// Constantes para configuração do WiFi
const char* ssid = "Hypernet_Rozileide";
const char* password = "sabrina13";

// Configuração do Telegram
const String BOT_TOKEN = "7216059515:AAEDxW2u7SX1LDhAnxY75iOgoUomGTyzTCU";
const String CHAT_ID = "7003158288"; // ID do chat ou grupo para enviar os dados

// Função para enviar mensagem para o Telegram
void sendMessage(String message) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "https://api.telegram.org/bot" + BOT_TOKEN + "/sendMessage?chat_id=" + CHAT_ID + "&text=" + message;
    http.begin(url);
    int httpResponseCode = http.GET();
    
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    } else {
      Serial.print("Erro na requisição: ");
      Serial.println(httpResponseCode);
    }
    
    http.end();
  } else {
    Serial.println("WiFi desconectado");
  }
}

void setup() {
  Serial.begin(115200);

  // Conectar ao Wi-Fi
  Serial.print("Conectando-se ao Wi-Fi ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Conectado ao Wi-Fi");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());

  // Inicializar BME280
  if (!bme.begin(0x76)) {
    Serial.println("Falha ao inicializar o BME280!");
    while (1);
  }

  pinMode(RAIN_SENSOR_PIN, INPUT);
  pinMode(SOIL_MOISTURE_PIN, INPUT);
  pinMode(UV_SENSOR_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
}

void loop() {
  String message = "Monitoramento IrrigaClima: \n \n";

  // Leitura do sensor de chuva (YL83)
  int rainState = digitalRead(RAIN_SENSOR_PIN);
  bool isRaining = (rainState == LOW);  // Baixo indica presença de chuva
  message += "Chuva: " + String(isRaining ? "Sim" : "Não") + "\n";

  // Leitura do sensor de umidade do solo (HL69)
  int soilMoistureValue = analogRead(SOIL_MOISTURE_PIN);
  float soilMoisturePercentage = map(soilMoistureValue, 4095, 0, 0, 100);  // Convertendo para porcentagem
  message += "Umidade do solo: " + String(soilMoisturePercentage) + "%\n";

  // Leitura do sensor de índice UV (GUVA-S12SD)
  int uvValue = digitalRead(UV_SENSOR_PIN);
  bool isDay = (uvValue == HIGH);
  message += "Índice UV: " + String(isDay ? "Baixo (Dia)" : "Alto (Noite)") + "\n";

  // Leitura do sensor BME280
  float temperature = bme.readTemperature();
  float humidity = bme.readHumidity();
  message += "Temperatura: " + String(temperature) + " °C\n";
  message += "Umidade do ar: " + String(humidity) + "%\n";

  // Controle do relé de irrigação
  String irrigationStatus = "";
  if (isRaining) {
    digitalWrite(RELAY_PIN, LOW);  // Desativa a irrigação se estiver chovendo
    irrigationStatus = "Irrigação desativada: Está chovendo.";
  } else if (soilMoisturePercentage > 40) {
    digitalWrite(RELAY_PIN, LOW);  // Desativa a irrigação se o solo estiver úmido
    irrigationStatus = "Irrigação desativada: Solo com umidade suficiente.";
  } else if (temperature < 25 || humidity > 60) {
    digitalWrite(RELAY_PIN, LOW);  // Desativa a irrigação se as condições de temperatura e umidade não exigirem
    irrigationStatus = "Irrigação desativada: Temperatura baixa ou umidade do ar alta.";
  } else {
    digitalWrite(RELAY_PIN, HIGH);  // Ativa a irrigação se todas as condições forem atendidas
    irrigationStatus = "Irrigação ativada: Condições ambientais necessitam irrigação.";
  }

  message += irrigationStatus + "\n";

  // Exibe no monitor serial
  Serial.println(message);

  // Envia status para o Telegram
  sendMessage(message);

  delay(10000);  // Espera 10 segundos antes de repetir
}



