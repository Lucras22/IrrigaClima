# Explicando as Bibliotecas

### WIFI.h: 
Utilizamos desta biblioteca pois permite que a esp32 possa se conectar com redes wifi, cria um ponto de acesso ou atuar como um servidor web local, utilizamos tais beneficions para exibir os dados dos sensores.

### DHT.h
Esta biblioteca foi utilizada para interagir com o sensor de temperatura e umidade (DHT11). No código, ela é usada para coletar dados de temperatura e umidade do sensor DHT11.

# Explicação dos Sensores

### DHT11 (Sensor de Temperatura e Umidade)
Conectado ao pino 4 do ESP32, o sensor DHT11 coleta dados de temperatura e umidade. A função dht.readTemperature() lê a temperatura em graus Celsius, e dht.readHumidity() coleta a umidade relativa do ar em porcentagem.

### Sensor de Umidade do Solo (HL69)
Conectado ao pino 34, este sensor analógico mede a umidade do solo. A função getSoilMoisture() lê o valor analógico, que é convertido para uma porcentagem de umidade do solo. Um valor baixo indica que o solo está seco.

### Sensor de Chuva (YL-83)
Conectado ao pino 33, este sensor verifica se está chovendo ou não. O valor é lido digitalmente, retornando "HIGH" (chuva) ou "LOW" (sem chuva). A função isRaining() faz essa verificação e retorna um valor booleano.

### Bomba de Irrigação
Conectada ao pino 26, a bomba de irrigação é controlada automaticamente com base no nível de umidade do solo. Se a umidade estiver abaixo de 20%, a bomba é ligada. Caso contrário, ela é desligada.

# WebServer

### Ponto de Acesso Wi-Fi
O ESP32 é configurado como um ponto de acesso (AP) com o nome e senha fornecidos (ssid e password). Qualquer dispositivo pode conectar-se a essa rede para acessar o servidor.

### Servidor Web
Um servidor web é criado na porta 80 utilizando a classe WiFiServer. Esse servidor aguarda conexões de clientes (navegadores web) e, ao receber uma solicitação, responde com uma página HTML que exibe os dados dos sensores.

### Interface Web
A interface web é criada utilizando HTML e CSS básicos. Ela exibe a temperatura, umidade, status da chuva, umidade do solo e o estado da bomba de irrigação. Um script JavaScript foi incluído para atualizar automaticamente a página a cada 10 segundos, garantindo que os dados exibidos estejam sempre atualizados.

### Controle da Bomba de Irrigação
Com base nos dados coletados pelo sensor de umidade do solo, a bomba é ativada ou desativada automaticamente. Isso ocorre a cada 10 segundos, que é o tempo de espera configurado no loop() principal.

# Conclusão

O sistema monitora continuamente as condições climáticas e do solo usando sensores conectados ao ESP32. Esses dados são exibidos em uma página web acessível por qualquer dispositivo conectado à rede Wi-Fi do ESP32. A bomba de irrigação é controlada automaticamente, garantindo que o solo receba água quando necessário, otimizando o uso de recursos hídricos. O sistema foi projetado para ser uma solução simples e eficaz para o monitoramento e automação de pequenas plantações.
