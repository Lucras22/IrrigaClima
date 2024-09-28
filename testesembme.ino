#include <WiFi.h>
#include <HTTPClient.h>

// Definições dos pinos
#define RAIN_SENSOR_PIN 34
#define SOIL_MOISTURE_PIN 32
#define UV_SENSOR_PIN 25
#define RELAY_PIN 27

// Configuração do Telegram
const String botToken = "7216059515:AAEDxW2u7SX1LDhAnxY75iOgoUomGTyzTCU";
const String chatId = "7003158288"; // ID do chat ou grupo para enviar os dados

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

  // Convertendo a umidade do solo para uma porcentagem
  float soilMoisturePercentage = map(soilMoistureValue, 0, 4095, 100, 0);

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
  // Condições normais para ativar a irrigação
  else {
    digitalWrite(RELAY_PIN, HIGH);  // Ativa a irrigação se todas as condições forem atendidas
    irrigationStatus = "Irrigação ativada: Condições ambientais necessitam irrigação.";
  }

  // Exibindo os dados no monitor serial
  Serial.println("Dados dos Sensores:");
  Serial.printf("Chuva: %s\n", isRaining ? "Chuva detectada" : "Não está chovendo");
  Serial.printf("Umidade do Solo: %.2f%%\n", soilMoisturePercentage);
  Serial.printf("Índice UV: %d\n \n", uv);
  Serial.println(irrigationStatus);

  // Montando a mensagem para o Telegram
  String message = "Dados IrrigaClima:\n \n";
  message += isRaining ? "Chuva: Chuva detectada\n" : "Chuva: Não está chovendo\n";
  message += "Umidade do Solo: " + String(soilMoisturePercentage) + "%\n";
  message += "Índice UV: " + String(uv) + "\n \n";
  message += irrigationStatus;

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
    
    // Enviando a requisição
    int httpResponseCode = http.POST(postData);
    
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
