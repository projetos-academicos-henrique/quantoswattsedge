# Projeto de Monitoramento de Luminosidade com ESP32 e MQTT

## 📋 Descrição
Este projeto utiliza um **ESP32** e um sensor LDR para monitorar mudanças na luminosidade de um ambiente. A cada alteração detectada, o ESP32 registra o horário e o nível de luminosidade, enviando esses dados via **MQTT** para um servidor. O sistema é ideal para monitorar se luzes estão ligadas ou desligadas em um ambiente, contribuindo para o uso eficiente de energia.

## 🚀 Tecnologias Utilizadas
- **ESP32**
- **Sensor LDR**
- **MQTT** (Broker: `mqtt-dashboard.com`)
- **RTC DS1307** (Para registrar o horário)

## 📦 Componentes
- ESP32
- Módulo RTC
- Sensor LDR

## 📡 Configuração MQTT
- **Servidor MQTT**: `mqtt-dashboard.com`
- **Porta**: `1883`
- **Tópico**: `esp32/luminosity`

## ⚙️ Funcionamento
1. O ESP32 mede a luminosidade do ambiente a cada segundo.
2. Ao detectar uma mudança, ele registra o horário usando o RTC.
3. Envia os dados de luminosidade via MQTT para o broker, contendo:
   - Dia e hora
   - Nível de luminosidade (em lux)
   - Localidade configurada no código

## 📄 Estrutura da Mensagem JSON
A mensagem enviada via MQTT segue o seguinte formato:
```json
{
  "day": "2024/11/13",
  "time": "14:30:45",
  "luminosity": 123.45,
  "local": "Quarto"
}
```

## 🔍 Simulação Wokwi
Você pode testar o projeto através do link de simulação do Wokwi:
[Simulação Wokwi]((https://wokwi.com/projects/414349387244707841))

## 📝 Resultados Esperados
- Monitoramento em tempo real de mudanças na luminosidade.
- Armazenamento dos registros de mudança com horário.
- Comunicação eficiente com servidor MQTT para uso em aplicações de automação residencial.

## 📚 Dependências
- **ArduinoJson**
- **PubSubClient**
- **RTClib**


## 🎥 Vídeo Explicativo
[Link para o vídeo demonstrativo](URL_DO_VIDEO)
