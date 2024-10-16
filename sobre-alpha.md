# Bibliotecas Utilizadas:

### WiFi.h:
Responsável por gerenciar a conexão Wi-Fi do ESP32, permitindo que o dispositivo se conecte à internet.

### HTTPClient.h:
Usada para enviar solicitações HTTP, neste caso, para enviar dados para o bot no Telegram.

### Wire.h:
Biblioteca para comunicação I2C, utilizada para comunicação com o sensor BME280 (sensor de pressão, temperatura e umidade).

### Adafruit_Sensor.h e Adafruit_BME280.h:
Bibliotecas da Adafruit para utilizar o sensor BME280, que mede temperatura, umidade e pressão atmosférica. A primeira é uma biblioteca genérica de sensores, e a segunda é específica para o BME280.

# Sensores e Componentes:

### Sensor de Chuva (YL-83):
Conectado ao pino 34, este sensor detecta a presença de água (chuva) e retorna um valor digital. Se a chuva for detectada, o valor do pino será LOW.

### Sensor de Umidade do Solo (HL-69):
Conectado ao pino 32, ele mede a umidade do solo. O valor lido é analógico, variando de 0 a 4095, e é convertido em porcentagem no código.

### Sensor de Índice UV (GUVA-S12SD):
Conectado ao pino 25, mede o nível de radiação ultravioleta. A leitura analógica é categorizada em um índice UV que vai de 0 a 11.

### Relé:
Conectado ao pino 27, é usado para controlar a bomba de irrigação. Se ativado, a bomba é ligada e a irrigação começa; caso contrário, a bomba é desligada.

### Sensor BME280:
Este sensor I2C mede a temperatura, umidade e pressão atmosférica. A altitude também é calculada usando a pressão atmosférica em relação ao nível do mar (definida como 1013.25 hPa).
