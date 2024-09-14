#include <WiFi.h>
#include <DHT.h>


// Definindo o tipo de sensor DHT e o pino ao qual está conectado (DHT11 no pino 4)
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);


// Pinos dos sensores e da bomba d'água
const int rainSensorPin = 33;
const int soilMoisturePin = 34;
const int pumpPin = 26;


// Constantes para configuração do WiFi
const char* ssid = "ESP32";
const char* password = "12345678";


WiFiServer server(80);


String header;


unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000;


void setup() {
  // Inicializa o monitor serial
  Serial.begin(115200);


  while (!Serial) {
    ; // Aguarda conexão
  }
  Serial.println("Serial inicializado");


  // Configurar o ESP32 como um ponto de acesso Wi-Fi
  WiFi.softAP(ssid, password);


  IPAddress IP = WiFi.softAPIP();
  Serial.print("Endereço IP do ponto de acesso: ");
  Serial.println(IP);


  // Inicializa os pinos
  pinMode(rainSensorPin, INPUT_PULLUP); // Configura o pino com pull-up
  pinMode(soilMoisturePin, INPUT);
  pinMode(pumpPin, OUTPUT);


  // Inicializa o sensor DHT
  dht.begin();


  // Inicializa o servidor
  server.begin();
  Serial.println("Servidor iniciado");
}


void loop() {
  WiFiClient client = server.available();


  if (client) {
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("Novo Cliente.");
    String currentLine = "";
    while (client.connected() && currentTime - previousTime <= timeoutTime) {
      currentTime = millis();
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        header += c;
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();


            float temperature = getTemperature();
            float humidity = getHumidity();
            String rainStatus = isRaining() ? "Com Chuva" : "Sem Chuva";
            int soilMoisture = getSoilMoisture();
            String pumpStatus = digitalRead(pumpPin) == HIGH ? "Ligada" : "Desligada";


            client.println("<!DOCTYPE html><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: Arial; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".div { margin-right: 50%; border: 2px solid; border-color: green; border-radius: 4px; margin: auto; width: 70%; }");
            client.println("</style>");
            client.println("<script>setInterval(function() { location.reload(); }, 10000);</script>"); // Adiciona o script para atualizar a página a cada 10 segundos
            client.println("</head>");
            client.println("<body><h1>Estacao Climatica</h1>");


            client.println("<div class=\"div\">");
            client.println("<p>Temperatura: " + String(temperature) + " °C</p>");
            client.println("<p>Umidade do Ar: " + String(humidity) + " %</p>");
            client.println("<p>Sensor de Chuva: " + rainStatus + "</p>");
            client.println("<p>Umidade do Solo: " + String(soilMoisture) + " %</p>");
            client.println("<p>Bomba de Irrigacao: " + pumpStatus + "</p>");
            client.println("</div> <br>");
            client.println("<h4>Venceslau Vieira Batista <br> Feira de Ciencias 2024</h4>");


            client.println("</body></html>");


            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    header = "";
    client.stop();
    Serial.println("Cliente desconectado.");
    Serial.println("");
  }


  // Lê os sensores e controla a bomba d'água periodicamente
  float temperature = getTemperature();
  float humidity = getHumidity();
  int soilMoisture = getSoilMoisture();
  bool raining = isRaining();


  // Exibe os dados dos sensores no Serial Monitor
  Serial.print("Temperatura: ");
  Serial.print(temperature);
  Serial.println(" °C");
  Serial.print("Umidade do Ar: ");
  Serial.print(humidity);
  Serial.println(" %");
  Serial.print("Sensor de Chuva: ");
  Serial.println(raining ? "Com Chuva" : "Sem Chuva");
  Serial.print("Umidade do Solo: ");
  Serial.print(soilMoisture);
  Serial.println(" %");


  // Controla a bomba d'água com base na umidade do solo e no estado do sensor de chuva
  if (soilMoisture < 20) {
    digitalWrite(pumpPin, HIGH); // Ativa a bomba d'água
    Serial.println("Bomba de Irrigacao: Ligada");
  } else {
    digitalWrite(pumpPin, LOW); // Desativa a bomba d'água
    Serial.println("Bomba de Irrigacao: Desligada");
  }


  // Atraso para leituras periódicas (por exemplo, a cada 10 segundos)
  delay(10000);
}


float getTemperature() {
  // Lê a temperatura do sensor DHT
  return dht.readTemperature();
}


float getHumidity() {
  // Lê a umidade do sensor DHT
  return dht.readHumidity();
}


int getSoilMoisture() {
  // Lê a umidade do solo (HL69)
  int sensorValue = analogRead(soilMoisturePin);
  // Converte para porcentagem de umidade do solo (0-100%)
  int moisturePercent = map(sensorValue, 0, 4095, 100, 0);
  return moisturePercent;
}


bool isRaining() {
  // Lê o estado do sensor de chuva e retorna true se estiver chovendo
  int rainValue = digitalRead(rainSensorPin);
  return rainValue == HIGH; // Assumindo que LOW indica presença de água (chuva)
}



