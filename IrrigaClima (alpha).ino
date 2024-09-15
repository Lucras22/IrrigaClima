#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <WiFi.h>
#include <HTTPClient.h>

// Definições dos pinos
#define RAIN_SENSOR_PIN 34
#define SOIL_MOISTURE_PIN 32
#define UV_SENSOR_PIN 25
#define RELAY_PIN 27

// Configuração do sensor BME280
Adafruit_BME280 bme;
#define SEALEVELPRESSURE_HPA (1013.25)

// Configuração do Telegram
const String BOT_TOKEN = "7216059515:AAEDxW2u7SX1LDhAnxY75iOgoUomGTyzTCU";
const String CHAT_ID = "7003158288"; // ID do chat ou grupo para enviar os dados

// Configuração do WiFi
const char* ssid = "Lucas Galindo | Poco C65"; 
const char* password = "lucras22"; 

void setup() {
  Serial.begin(115200);

  // Conectando ao WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }
  Serial.println("WiFi conectado");

  // Iniciando o sensor BME280
  if (!bme.begin(0x76)) {
    Serial.println("Falha ao inicializar o BME280.");
    while (1);
  }

  // Configurando os pinos dos sensores e relé
  pinMode(RAIN_SENSOR_PIN, INPUT);
  pinMode(SOIL_MOISTURE_PIN, INPUT);
  pinMode(UV_SENSOR_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Relé desligado inicialmente
}

void loop() {
  // Leitura dos sensores
  int rainSensorValue = digitalRead(RAIN_SENSOR_PIN);
  int soilMoistureValue = analogRead(SOIL_MOISTURE_PIN);
  int leitura_porta = analogRead(UV_SENSOR_PIN);

  float temperature = bme.readTemperature();
  float humidity = bme.readHumidity();
  float pressure = bme.readPressure() / 100.0F;

  // Convertendo a umidade do solo para uma porcentagem
  float soilMoisturePercentage = map(soilMoistureValue, 0, 4095, 0, 100);

  // Verificando se está chovendo
  bool isRaining = (rainSensorValue == LOW); // LOW indica que está chovendo

  // Definindo o índice UV com base na leitura analógica
  int uv;
  if (leitura_porta <= 10) {
    uv = 0;
  } else if (leitura_porta > 10 && leitura_porta <= 46) {
    uv = 1;
  } else if (leitura_porta > 46 && leitura_porta <= 65) {
    uv = 2;
  } else if (leitura_porta > 65 && leitura_porta <= 83) {
    uv = 3;
  } else if (leitura_porta > 83 && leitura_porta <= 103) {
    uv = 4;
  } else if (leitura_porta > 103 && leitura_porta <= 124) {
    uv = 5;
  } else if (leitura_porta > 124 && leitura_porta <= 142) {
    uv = 6;
  } else if (leitura_porta > 142 && leitura_porta <= 162) {
    uv = 7;
  } else if (leitura_porta > 162 && leitura_porta <= 180) {
    uv = 8;
  } else if (leitura_porta > 180 && leitura_porta <= 200) {
    uv = 9;
  } else if (leitura_porta > 200 && leitura_porta <= 221) {
    uv = 10;
  } else {
    uv = 11;
  }

  // Controle do relé de irrigação
  String irrigationStatus = "";
  
  // Verifica se está chovendo
  if (isRaining) {
    digitalWrite(RELAY_PIN, LOW);  // Desativa a irrigação se estiver chovendo
    irrigationStatus = "Irrigação desativada: Está chovendo.";
  }
  // Verifica a umidade do solo
  else if (soilMoisturePercentage > 40) {
    digitalWrite(RELAY_PIN, LOW);  // Desativa a irrigação se o solo estiver úmido
    irrigationStatus = "Irrigação desativada: Solo com umidade suficiente.";
  }
  // Verifica condições de temperatura e umidade do ar
  else if (temperature < 25 || humidity > 60) {
    digitalWrite(RELAY_PIN, LOW);  // Desativa a irrigação se as condições de temperatura e umidade não exigirem
    irrigationStatus = "Irrigação desativada: Temperatura baixa ou umidade do ar alta.";
  }
  // Verifica se o índice UV é 7 ou mais
  else if (uv >= 7) {
    digitalWrite(RELAY_PIN, HIGH);  // Ativa a irrigação se o índice UV for 7 ou mais
    irrigationStatus = "Irrigação ativada: Índice UV elevado.";
  }
  // Condições normais para ativar a irrigação
  else {
    digitalWrite(RELAY_PIN, HIGH);  // Ativa a irrigação se todas as condições forem atendidas
    irrigationStatus = "Irrigação ativada: Condições ambientais necessitam irrigação.";
  }

  // Exibindo os dados no monitor serial
  Serial.println("Dados dos Sensores:");
  Serial.printf("Chuva: %s\n", isRaining ? "Chuva detectada" : "Não está chovendo");
  Serial.printf("Umidade do Solo: %.2f%%\n", soilMoisturePercentage);
  Serial.printf("Índice UV: %d\n", uv);
  Serial.printf("Temperatura: %.2f°C\n", temperature);
  Serial.printf("Umidade: %.2f%%\n", humidity);
  Serial.printf("Pressão: %.2f hPa\n", pressure);
  Serial.println(irrigationStatus);

  // Enviando os dados para o bot no Telegram
  enviarDadosTelegram(isRaining, soilMoisturePercentage, uv, temperature, humidity, pressure, irrigationStatus);

  delay(10000); // Espera 10 segundos para enviar os dados novamente
}

void enviarDadosTelegram(bool isRaining, float soilMoisture, int uv, float temp, float hum, float pressure, String irrigationStatus) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    String mensagem = "Dados IrrigaClima:\n \n";
    mensagem += isRaining ? "Chuva: Chuva detectada\n" : "Chuva: Não está chovendo\n";
    mensagem += "Umidade do Solo: " + String(soilMoisture) + "%\n";
    mensagem += "Índice UV: " + String(uv) + "\n";
    mensagem += "Temperatura: " + String(temp) + "°C\n";
    mensagem += "Umidade: " + String(hum) + "%\n";
    mensagem += "Pressão: " + String(pressure) + " hPa\n";
    mensagem += irrigationStatus;

    String url = "https://api.telegram.org/bot" + BOT_TOKEN + "/sendMessage?chat_id=" + CHAT_ID + "&text=" + mensagem;
    http.begin(url);
    int httpResponseCode = http.GET();
    
    if (httpResponseCode > 0) {
      Serial.println("Dados enviados para o Telegram com sucesso.");
    } else {
      Serial.printf("Erro ao enviar mensagem: %d\n", httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("Erro de conexão WiFi.");
  }
}
