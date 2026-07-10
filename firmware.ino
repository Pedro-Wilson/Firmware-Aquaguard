#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 18     //VERMELHO: 3.3V;  PRETO: GND;  AMARELO: PINO
#define SENSOR_NIVEL 26     //GND E PINO
#define RELE_BOMBA 33
#define RELE_AQUECEDOR 23
#define RELE_COOLER 22

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

float temp_media = 27;
float tolerancia = 0.5;

void setup() {
  Serial.begin(115200);

  pinMode(RELE_AQUECEDOR, OUTPUT);
  pinMode(RELE_COOLER, OUTPUT);
  pinMode(RELE_BOMBA, OUTPUT);
  pinMode(SENSOR_NIVEL, INPUT_PULLUP);

  digitalWrite(RELE_AQUECEDOR, HIGH);
  digitalWrite(RELE_COOLER, HIGH);
  digitalWrite(RELE_BOMBA, HIGH);

  sensors.begin();

  Serial.println();
  Serial.println("Inicializando DS18B20...");
  Serial.println("Sistema de controle de temperatura e nivel iniciado");

  if (sensors.getDeviceCount() == 0) {
    Serial.println("Nenhum sensor encontrado!");
  } else{
    Serial.print("Sensores encontrados: ");
    Serial.println(sensors.getDeviceCount());
  }
}

void loop() {
  sensors.requestTemperatures();

  float temp = sensors.getTempCByIndex(0);
  int nivel = digitalRead(SENSOR_NIVEL);

  if (temp == DEVICE_DISCONNECTED_C || temp < 0 || temp > 60) {
    Serial.println("Erro de leitura da temperatura.");
    digitalWrite(RELE_AQUECEDOR, HIGH);
    digitalWrite(RELE_COOLER, HIGH);
  } else {
    Serial.print("Temperatura: ");
    Serial.print(temp);
    Serial.println(" °C");

    if (digitalRead(RELE_AQUECEDOR) == LOW && temp < temp_media){
      digitalWrite(RELE_AQUECEDOR, LOW);
      digitalWrite(RELE_COOLER, HIGH);
      Serial.println("Aquecedor LIGADO | Cooler DESLIGADO");
    } else if (digitalRead(RELE_COOLER) == LOW && temp > temp_media){
      digitalWrite(RELE_AQUECEDOR, HIGH);
      digitalWrite(RELE_COOLER, LOW);
      Serial.println("Aquecedor DESLIGADO | Cooler LIGADO");
    } else{
      if (temp >= (temp_media + tolerancia)){
        digitalWrite(RELE_AQUECEDOR, HIGH);
        digitalWrite(RELE_COOLER, LOW);
        Serial.println("Aquecedor DESLIGADO | Cooler LIGADO");
      } else if (temp <= (temp_media - tolerancia)){
        digitalWrite(RELE_AQUECEDOR, LOW);
        digitalWrite(RELE_COOLER, HIGH);
        Serial.println("Aquecedor LIGADO | Cooler DESLIGADO");
      } else{
        digitalWrite(RELE_AQUECEDOR, HIGH);
        digitalWrite(RELE_COOLER, HIGH);
        Serial.println("Aquecedor DESLIGADO | Cooler DESLIGADO");
      }
    }
  }

  Serial.print("Nivel sensor: ");
  Serial.println(nivel);

  if (nivel == LOW) {
    //água atingiu o nível
    Serial.println("NIVEL CORRETO - BOMBA DESLIGADA");

    digitalWrite(RELE_BOMBA, HIGH); // desliga relé
  } else {
    //água abaixo do nível
    Serial.println("NIVEL BAIXO - BOMBA LIGADA");

    digitalWrite(RELE_BOMBA, LOW); //liga relé
  }

  Serial.println("----------------------");
  delay(1000);
}
