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

// Incluindo as bibliotecas
#include <WiFi.h>
#include <DHT.h>


// tipo de sensor dht e o pino
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);


// definindo nome para os pinos 
const int rainSensorPin = 33;
const int soilMoisturePin = 34;
const int pumpPin = 26;


// nome e senha do wifi
const char* ssid = "ESP32";
const char* password = "12345678";


WiFiServer server(80);


String header;


unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000;


void setup() {
  // iniciando o monitor serial
  Serial.begin(115200);


  while (!Serial) {
    ; // aguando a inicialização
  }
  Serial.println("Serial inicializado");


  // transformando o esp em um ponto de acesso
  WiFi.softAP(ssid, password);

// obtendo ip da esp
  IPAddress IP = WiFi.softAPIP();
  Serial.print("Endereço IP do ponto de acesso: ");
  Serial.println(IP);


 // iniciando a funcionalidade dos pinos
  pinMode(rainSensorPin, INPUT_PULLUP); 
  pinMode(soilMoisturePin, INPUT);
  pinMode(pumpPin, OUTPUT);


  // Iniciar dht
  dht.begin();


  // iniciando o webserver
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

            // coleta dos dados dos sensores
            float temperature = getTemperature();
            float humidity = getHumidity();
            String rainStatus = isRaining() ? "Com Chuva" : "Sem Chuva";
            int soilMoisture = getSoilMoisture();
            String pumpStatus = digitalRead(pumpPin) == HIGH ? "Ligada" : "Desligada";

            // HTML e CSS do site com um js para atualizar sozinho
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


  // ler os dados dos sensores e controlar a bomba d'agua
  float temperature = getTemperature();
  float humidity = getHumidity();
  int soilMoisture = getSoilMoisture();
  bool raining = isRaining();


  // mostra os dados dos sensores no monitor serial
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


  // condições para ligar a irrigação com base nos dados do solo
  if (soilMoisture < 20) {
    digitalWrite(pumpPin, HIGH); // Ativa a bomba d'água
    Serial.println("Bomba de Irrigacao: Ligada");
  } else {
    digitalWrite(pumpPin, LOW); // Desativa a bomba d'água
    Serial.println("Bomba de Irrigacao: Desligada");
  }


  // ler a cada 10 segundos
  delay(10000);
}


float getTemperature() {
  // Lê a temperatura do sensor DHT
  return dht.readTemperature();
}


float getHumidity() {
  // umidade do sensor DHT
  return dht.readHumidity();
}


int getSoilMoisture() {
  // umidade do solo sensor HL69
  int sensorValue = analogRead(soilMoisturePin);
  // Converte para porcentagem de umidade do solo (0-100%)
  int moisturePercent = map(sensorValue, 0, 4095, 100, 0);
  return moisturePercent;
}


bool isRaining() {
  // verifica a presença de chuva e retorna um booleano se tem ou não chuva
  int rainValue = digitalRead(rainSensorPin);
  return rainValue == HIGH; 
}



