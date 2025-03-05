/*
██╗     ██╗   ██╗ ██████╗ ██████╗  █████╗ ███████╗
██║     ██║   ██║██╔═════╗██╔══██╗██╔══██╗██╔════╝
██║     ██║   ██║██║     ║██████╔╝███████║███████╗
██║     ██║   ██║██║     ║██╔══██╗██╔══██║╚════██║
███████╗╚██████╔╝╚██████╔╝██║  ██║██║  ██║███████║
╚══════╝ ╚═════╝  ╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═╝╚══════╝
-----------------------------------------------------------
📌 Autor: Lucas Galindo
🔗 GitHub: https://github.com/Lucras22
📧 E-mail: devlucasgalindo@email.com
💼 LinkedIn: https://www.linkedin.com/in/lucasgalindoiot/
-----------------------------------------------------------
📜 Instruções de Uso:
- 
-----------------------------------------------------------
📂 Repositório do Projeto:
🔗 https://github.com/Lucras22/IrrigaClima
-----------------------------------------------------------
🛠️ Licença: ....
*/


/* Fluxo do que o codigo faz:

1. Conectar ao Wi-Fi.
2. Inicializar os sensores (BME280, chuva, umidade do solo, UV).
3. Loop principal:
   a. Ler dados dos sensores.
   b. Exibir dados no monitor serial.
   c. Controlar o relé de irrigação com base nas leituras.
   d. Montar uma mensagem com os dados.
   e. Enviar a mensagem para o bot no Telegram.
   f. Esperar 10 segundos e repetir o processo.
   
*/

// incluindo bibliotecas
#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1013.25)

// definindo os nomes dos pinos
#define RAIN_SENSOR_PIN 34
#define SOIL_MOISTURE_PIN 32
#define UV_SENSOR_PIN 25
#define RELAY_PIN 27

// token e id do bot do Telegram
const String botToken = "7216059515:AAEDxW2u7SX1LDhAnxY75iOgoUomGTyzTCU";
const String chatId = "7003158288"; // ID do chat ou grupo para enviar os dados

// Nome e Senha do WiFi
const char* ssid = "Lucas Galindo | Poco C65"; 
const char* password = "lucras22"; 

// Configuração do sensor BME280
Adafruit_BME280 bme;
unsigned long delayTime;

void setup() {
  Serial.begin(115200);
  
  // Conectando ao WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }
  Serial.println("WiFi conectado");

  // Inicializando BME280
  bool status = bme.begin(0x76);  
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }

  // Configurando os pinos dos sensores e relé
  pinMode(RAIN_SENSOR_PIN, INPUT);
  pinMode(SOIL_MOISTURE_PIN, INPUT);
  pinMode(UV_SENSOR_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); 
}

void loop() {
  // Leitura dos sensores
  int rainSensorValue = digitalRead(RAIN_SENSOR_PIN);
  int soilMoistureValue = analogRead(SOIL_MOISTURE_PIN);
  int leitura_porta = analogRead(UV_SENSOR_PIN);

  // Convertendo a umidade do solo para uma porcentagem
  float soilMoisturePercentage = map(soilMoistureValue, 0, 4095, 0,  100);

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
  
  // Verifica a umidade do solo primeiro
if (soilMoisturePercentage > 40) {
    digitalWrite(RELAY_PIN, LOW);  // Desativa a irrigação se o solo estiver úmido
    irrigationStatus = "Irrigação desativada: Solo com umidade suficiente.";
} 
// Verifica se está chovendo
else if (isRaining) {
    digitalWrite(RELAY_PIN, LOW);  // Desativa a irrigação se estiver chovendo
    irrigationStatus = "Irrigação desativada: Está chovendo.";
} 
// Verifica o índice UV
else if (uvIndex <= 6) {
    digitalWrite(RELAY_PIN, LOW);  // Desativa a irrigação se o índice UV for 6 ou menor
    irrigationStatus = "Irrigação desativada: Índice UV abaixo de 7.";
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
  Serial.println(irrigationStatus);

  // Exibindo os dados do BME280
  // Temperatura
  Serial.print("Temperature = ");
  Serial.print(bme.readTemperature());
  Serial.println(" °C");
  // Pressão
  Serial.print("Pressure = ");
  Serial.print(bme.readPressure() / 100.0F);
  Serial.println(" hPa");
  // Altitude
  Serial.print("Approx. Altitude = ");
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(" m");
  // Umidade
  Serial.print("Humidity = ");
  Serial.print(bme.readHumidity());
  Serial.println(" %");

  Serial.println();

  // Montando a mensagem para o Telegram
  String message = "Dados IrrigaClima:\n \n";
  message += isRaining ? "Chuva: Chuva detectada\n" : "Chuva: Não está chovendo\n";
  message += "Umidade do Solo: " + String(soilMoisturePercentage) + "%\n";
  message += "Índice UV: " + String(uv) + "\n";
  message += "Temperatura: " + String(bme.readTemperature()) + " °C\n";
  message += "Pressão: " + String(bme.readPressure() / 100.0F) + " hPa\n";
  message += "Altitude Aproximada: " + String(bme.readAltitude(SEALEVELPRESSURE_HPA)) + " m\n";
  message += "Umidade: " + String(bme.readHumidity()) + " %\n\n";
  message += irrigationStatus + "\n";

  // Enviando os dados para o bot no Telegram
  sendMessage(message);

  delay(10000); // Espera 10 segundos para enviar os dados novamente
}

// Função para enviar mensagem ao Telegram
void sendMessage(String message) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "https://api.telegram.org/bot" + botToken + "/sendMessage";
    
    http.begin(url);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    // Montando os parâmetros da requisição POST
    String postData = "chat_id=" + chatId + "&text=" + message;
    
    // Enviando a requisição no metodo post
    int httpResponseCode = http.POST(postData);

    // Esperando a resposta do servidor
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Código de resposta: " + String(httpResponseCode));
      Serial.println("Resposta: " + response);
    } else {
      Serial.print("Erro na requisição: ");
      Serial.println(httpResponseCode);
    }
    
    http.end();
  } else {
    Serial.println("WiFi desconectado");
  }
}
