#include "SSD1306Wire.h"
#include <Wire.h>

// Inicializa o display Oled
SSD1306Wire display(0x3c, D1, D2);

// Linhas de texto no display
int lines[5] = {1, 12, 24, 36, 48};

// Pino do sensor
const int pinoSensor = 10;   // GPIO10 - S3 no ESP8266

// Variável para armazenar o estado do sensor
bool sensorState = false;
bool lastSensorState = false;  // Para rastrear o estado anterior do sensor

void setup() {
  // Configuração dos pinos
  pinMode(pinoSensor, INPUT);       // Pino do sensor como entrada
  pinMode(LED_BUILTIN, OUTPUT);     // LED embutido como saída
  digitalWrite(LED_BUILTIN, HIGH);  // LED embutido começa desligado
  Serial.begin(115200);             // Inicia a Serial com 115200 para ESP8266

  // Inicialização do display
  display.init();
  display.flipScreenVertically();
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_10);
}


void showScreenMessage(String msg){
    display.clear();

    int comprimento = msg.length();
    int totalPosicoes = 0;

    int maxLetrasPorPosicao = 25;
    int maxPosicoes = 100; // Defina um número máximo de posições que você espera
    
    char mensagemDividida[maxPosicoes][maxLetrasPorPosicao + 1]; // +1 para o terminador nulo
    
    int cont = 0;
    for (int i = 0; i < comprimento; i += maxLetrasPorPosicao) {
      msg.substring(i, i + maxLetrasPorPosicao).toCharArray(mensagemDividida[totalPosicoes], maxLetrasPorPosicao + 1);
      totalPosicoes++;
  
      display.drawString(63, lines[cont], mensagemDividida[cont]);
      cont++;
      
      // Para evitar ultrapassar o número máximo de posições
      if (totalPosicoes >= maxPosicoes) {
        break;
      }
    }
    
    display.display();
}

void loop() {
  // Leitura do estado do sensor
  sensorState = digitalRead(pinoSensor);

  // Verifica se houve mudança de estado no sensor
  if (sensorState != lastSensorState) {
    // Atualiza o estado do LED e exibe mensagem
    if (sensorState == HIGH) {  // Sensor ativado
      digitalWrite(LED_BUILTIN, LOW);    // Acende o LED embutido
      Serial.println("Sensor ativado");  // Mensagem no Monitor Serial
      showScreenMessage("Sensor ativado"); // Mensagem no display
    } else {  // Sensor desativado
      digitalWrite(LED_BUILTIN, HIGH);   // Apaga o LED embutido
      Serial.println("Sensor desativado"); // Mensagem no Monitor Serial
      showScreenMessage("Sensor desativado"); // Mensagem no display
    }
    lastSensorState = sensorState;  // Atualiza o estado anterior do sensor
  }

  delay(100);  // Pequeno delay para estabilizar a leitura
}
