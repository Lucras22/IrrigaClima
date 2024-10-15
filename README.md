# Introdução

Este projeto visa o desenvolvimento de uma estação meteorológica automatizada utilizando sensores conectados ao microcontrolador ESP32 com conectividade Wi-Fi. A proposta é fornecer dados meteorológicos em tempo real e auxiliar na tomada de decisões para a agricultura familiar. O sistema monitora variáveis como umidade do ar, umidade do solo, temperatura ambiente, pressão atmosférica e índice de radiação UV. Além disso, o projeto integra um sistema automatizado de irrigação, que otimiza o uso da água com base nos parâmetros coletados.
Funcionalidades Propostas

- Monitoramento da umidade do ar: Usando o sensor BME280, é possível obter a umidade relativa do ar, fornecendo informações essenciais para o manejo de cultivos.

- Medição da umidade do solo: O sensor HL69 monitora a umidade da terra, possibilitando decisões mais assertivas sobre a necessidade de irrigação.

- Registro da temperatura ambiente: O sensor BME280 também coleta a temperatura do ambiente, importante para o controle de clima e manejo agrícola.

- Aferição da pressão atmosférica: Com o BME280, o sistema monitora a pressão atmosférica, útil para prever mudanças climáticas.

- Medição do nível de radiação UV (ultravioleta): O sensor GUVA-S12SD mede a radiação UV, ajudando a proteger plantas sensíveis à exposição solar excessiva.

- Detecção de chuva: O sensor YL83 detecta a presença de chuva, desativando automaticamente o sistema de irrigação quando necessário.

- Automação da irrigação: A irrigação é acionada automaticamente com base na umidade do solo e nas condições meteorológicas. A bomba é ativada ou desativada para garantir o uso eficiente da água.

# Benefícios para a Agricultura Familiar

- A estação meteorológica automatizada oferece uma série de vantagens:

- Decisões mais informadas: Os dados fornecidos em tempo real permitem que famílias agrícolas ajustem suas práticas de manejo de forma precisa, otimizando o uso de recursos naturais e melhorando a produtividade.

- Irrigação eficiente: O sistema automatizado de irrigação reduz o desperdício de água ao ativar a irrigação apenas quando necessário. Isso contribui para uma gestão mais sustentável da água, um recurso limitado em muitas regiões.

- Acessibilidade e baixo custo: Desenvolvido com componentes acessíveis, o sistema é uma solução viável para pequenas propriedades agrícolas, que muitas vezes enfrentam dificuldades de acesso a tecnologias avançadas.

# Materiais Utilizados

- ESP32: Módulo microcontrolador com conectividade Wi-Fi para enviar os dados coletados e controlar o sistema de irrigação de forma remota.
- Sensor BME280: Para medir pressão atmosférica, temperatura do ar e umidade relativa.
- Sensor GUVA-S12SD: Sensor de índice UV para medir a radiação ultravioleta.
- Sensor de Chuva YL83: Detecta a presença de chuva, automatizando o desligamento da irrigação quando necessário.
- Sensor de Umidade do Solo HL69: Monitora a umidade do solo para o acionamento da irrigação.
- Relé e Bomba de Irrigação: Controlam o fluxo de água com base nos dados de umidade do solo e detecção de chuva.

# Funcionamento do Sistema

O sistema coleta continuamente os dados dos sensores e os envia para um bot no Telegram, que notifica o agricultor sobre as condições climáticas e de solo. Com base nesses dados, o sistema de irrigação é ativado ou desativado automaticamente:

- Se a umidade do solo estiver abaixo de um limite predefinido (por exemplo, 40%), a bomba de irrigação é ativada.
- Se o sensor de chuva detectar precipitação, a irrigação é suspensa.
- A coleta dos dados é feita a cada 10 segundos, garantindo atualizações frequentes sobre as condições meteorológicas e de solo.

# Conclusão

Este projeto propõe uma solução acessível e eficaz para a agricultura familiar, fornecendo informações meteorológicas precisas e permitindo a automação de processos essenciais, como a irrigação. Através da combinação de sensores e automação com o ESP32, as famílias agrícolas poderão otimizar o uso de recursos naturais, aumentar a produtividade e promover a sustentabilidade em suas práticas agrícolas.

# Próximos Passos

- Implementar uma interface gráfica simples para visualizar os dados meteorológicos de forma mais acessível.
- Explorar novas funcionalidades, como a previsão meteorológica com base em algoritmos de machine learning, para antecipar condições climáticas adversas.

Este projeto pode ser adaptado para atender a diferentes tipos de cultivos e condições ambientais, trazendo melhorias contínuas para a agricultura familiar.

## Fase Beta Simulador:

<img src="https://babbf25fa0.cbaul-cdnwnd.com/ca777225d6fbd038fc560b5e3ff1f1e0/200000012-e1e71e1e74/fase%20beta%20simulador.png?ph=babbf25fa0" alt="simulador beta" width="1000"/>

## Fase Beta Prototipagem:

<img src="https://babbf25fa0.cbaul-cdnwnd.com/ca777225d6fbd038fc560b5e3ff1f1e0/200000028-933ec933ee/imagem_2024-09-15_172730375.png?ph=babbf25fa0" alt="Prototipagem beta" width="1000"/>



## Fase Avançada Prototipagem:

<img src="https://github.com/user-attachments/assets/3f9993af-c654-4991-8046-f82a45742108" alt="Prototipagem avançada" width="500"/>

