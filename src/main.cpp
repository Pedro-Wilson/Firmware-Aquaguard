
// ==================================================
// AQUAGUARDIAN
// ==================================================
//
// Ligação nas tomadas:
// Tomada 01 - Bomba
// Tomada 02 - Aquecedor
// Tomada 03 - Fan
//
// ==================================================

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// ==================================================
// DEFINIÇÃO DOS GPIO
// ==================================================

#define ONE_WIRE_BUS 18     // Vermelho: 3.3V | Preto: GND | Amarelo: GPIO
#define SENSOR_NIVEL 26     // GND e GPIO

#define RELE_BOMBA 33
#define RELE_AQUECEDOR 23
#define RELE_COOLER 22

// ==================================================
// SENSOR DS18B20
// ==================================================

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// ==================================================
// CONFIGURAÇÕES
// ==================================================

float temp_media = 27;
float tolerancia = 0.5;

// ==================================================
// POSIÇÕES DA TELA
// ==================================================

#define LIN_TEMP_ATUAL      3
#define LIN_TEMP_ALVO       4
#define LIN_TOLERANCIA      5
#define LIN_AQUECEDOR_ACAO  7
#define LIN_COOLER_ACAO     8

#define LIN_SENSOR_NIVEL    16
#define LIN_BOMBA_ACAO      17

#define LIN_STATUS_BOMBA    23
#define LIN_STATUS_AQUEC    24
#define LIN_STATUS_COOLER   25

#define COL_VALOR           24

// ==================================================
// CURSOR ANSI
// ==================================================

void cursorEm(int linha, int coluna) {
  Serial.print("\033[");
  Serial.print(linha);
  Serial.print(";");
  Serial.print(coluna);
  Serial.print("H");
}

// ==================================================
// ESCREVE SOMENTE A VARIÁVEL
// ==================================================

void escreverValor(int linha, int coluna, const String &texto) {

  cursorEm(linha, coluna);

  // Apaga qualquer valor anterior daquela linha
  Serial.print("\033[K");

  Serial.print(texto);
}

// ==================================================
// LIMPA A TELA UMA ÚNICA VEZ
// ==================================================

void limparTelaCompleta() {
  Serial.print("\033[2J");
  Serial.print("\033[3J");
  Serial.print("\033[H");
}

// ==================================================
// ASCII
// ==================================================

void art_ascii() {

  Serial.println("       _                                   _");
  Serial.println("      / \\   __ _ _   _  __ _  __ _ _   _  / \\");
  Serial.println("     / _ \\ / _` | | | |/ _` |/ _` | | | |/ _ \\");
  Serial.println("    / ___ \\ (_| | |_| | (_| | (_| | |_| / ___ \\");
  Serial.println("   /_/   \\_\\__, |\\__,_|\\__, |\\__,_|\\__,_/_/   \\_\\");
  Serial.println("             |___/       |___/");
}

// ==================================================
// MENU INICIAL
// ==================================================

void menu() {

  Serial.println();
  Serial.println("Iniciando Aquaguardian ...");
  Serial.println();
  Serial.println("----------------------------------------------");

  art_ascii();

  Serial.println();
  Serial.println("Desenvolvido por : Pedro Wilson");

  Serial.println("----------------------------------------------");
}

// ==================================================
// DESENHA A MOLDURA UMA ÚNICA VEZ
// ==================================================

void desenharMoldeFixo() {

  limparTelaCompleta();

  Serial.println();
  Serial.println();

  Serial.println("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒ AQUAGUARDIAN ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒");

  Serial.println();
  Serial.println();

  Serial.println("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┑");
  Serial.println("┃ Temperatura        : ");
  Serial.println("┃ Temperatura alvo   : ");
  Serial.println("┃ Tolerancia         : ");
  Serial.println("┃                      ");
  Serial.println("┃ Aquecedor          : ");
  Serial.println("┃ Cooler             : ");
  Serial.println("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┙");

  Serial.println();
  Serial.println();

  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("CONTROLE DE NIVEL");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");

  Serial.println();

  Serial.println("➞ Sensor             : ");
  Serial.println("➞ Bomba              : ");

  Serial.println();

  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("STATUS DOS DISPOSITIVOS");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");

  Serial.println();

  Serial.println("• Bomba              : ");
  Serial.println("• Aquecedor          : ");
  Serial.println("• Cooler             : ");

  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
}

// ==================================================
// SETUP
// ==================================================

void setup() {

  Serial.begin(115200);

  // ==================================================
  // CONFIGURAÇÃO DOS RELÉS
  // ==================================================

  pinMode(RELE_AQUECEDOR, OUTPUT);
  pinMode(RELE_COOLER, OUTPUT);
  pinMode(RELE_BOMBA, OUTPUT);

  // ==================================================
  // SENSOR DE NÍVEL
  // ==================================================

  pinMode(SENSOR_NIVEL, INPUT_PULLUP);

  // ==================================================
  // DESLIGA TODOS OS RELÉS AO INICIAR
  // ==================================================

  digitalWrite(RELE_AQUECEDOR, HIGH);
  digitalWrite(RELE_COOLER, HIGH);
  digitalWrite(RELE_BOMBA, HIGH);

  // ==================================================
  // INICIA SENSOR
  // ==================================================

  sensors.begin();

  // ==================================================
  // MENU INICIAL
  // ==================================================

  menu();

  Serial.println();
  Serial.println("Inicializando DS18B20...");

  if (sensors.getDeviceCount() == 0) {

    Serial.println("ERROR : Nenhum sensor encontrado!");

  } else {

    Serial.print("Sensores encontrados: ");
    Serial.println(sensors.getDeviceCount());
  }

  delay(3000);

  // ==================================================
  // DESENHA A MOLDURA UMA ÚNICA VEZ
  // ==================================================

  desenharMoldeFixo();
}

// ==================================================
// LOOP
// ==================================================

void loop() {

  // ==================================================
  // LEITURA DOS SENSORES
  // ==================================================

  sensors.requestTemperatures();

  float temp = sensors.getTempCByIndex(0);
  int nivel = digitalRead(SENSOR_NIVEL);

  // ==================================================
  // CONTROLE DE TEMPERATURA
  // ==================================================

  if (temp == DEVICE_DISCONNECTED_C || temp < 0 || temp > 60) {

    digitalWrite(RELE_AQUECEDOR, HIGH);
    digitalWrite(RELE_COOLER, HIGH);

    escreverValor(
      LIN_TEMP_ATUAL,
      COL_VALOR,
      "ERRO DE LEITURA"
    );

    escreverValor(
      LIN_TEMP_ALVO,
      COL_VALOR,
      "27.00 °C"
    );

    escreverValor(
      LIN_TOLERANCIA,
      COL_VALOR,
      "±0.50 °C"
    );

    escreverValor(
      LIN_AQUECEDOR_ACAO,
      COL_VALOR,
      "DESLIGADO"
    );

    escreverValor(
      LIN_COOLER_ACAO,
      COL_VALOR,
      "DESLIGADO"
    );

  } else {

    escreverValor(
      LIN_TEMP_ATUAL,
      COL_VALOR,
      String(temp, 2) + " °C"
    );

    escreverValor(
      LIN_TEMP_ALVO,
      COL_VALOR,
      String(temp_media, 2) + " °C"
    );

    escreverValor(
      LIN_TOLERANCIA,
      COL_VALOR,
      "±" + String(tolerancia, 2) + " °C"
    );

    // ==================================================
    // CONTROLE DO AQUECEDOR E COOLER
    // ==================================================

    if (digitalRead(RELE_AQUECEDOR) == LOW && temp < temp_media) {

      digitalWrite(RELE_AQUECEDOR, LOW);
      digitalWrite(RELE_COOLER, HIGH);

    } else if (digitalRead(RELE_COOLER) == LOW && temp > temp_media) {

      digitalWrite(RELE_AQUECEDOR, HIGH);
      digitalWrite(RELE_COOLER, LOW);

    } else {

      if (temp >= (temp_media + tolerancia)) {

        digitalWrite(RELE_AQUECEDOR, HIGH);
        digitalWrite(RELE_COOLER, LOW);

      } else if (temp <= (temp_media - tolerancia)) {

        digitalWrite(RELE_AQUECEDOR, LOW);
        digitalWrite(RELE_COOLER, HIGH);

      } else {

        digitalWrite(RELE_AQUECEDOR, HIGH);
        digitalWrite(RELE_COOLER, HIGH);
      }
    }

    // ==================================================
    // ATUALIZA AÇÃO DO AQUECEDOR
    // ==================================================

    escreverValor(
      LIN_AQUECEDOR_ACAO,
      COL_VALOR,
      digitalRead(RELE_AQUECEDOR) == LOW
        ? "LIGADO"
        : "DESLIGADO"
    );

    // ==================================================
    // ATUALIZA AÇÃO DO COOLER
    // ==================================================

    escreverValor(
      LIN_COOLER_ACAO,
      COL_VALOR,
      digitalRead(RELE_COOLER) == LOW
        ? "LIGADO"
        : "DESLIGADO"
    );
  }

  // ==================================================
  // CONTROLE DE NÍVEL
  // ==================================================

  if (nivel == LOW) {

    digitalWrite(RELE_BOMBA, HIGH);

    escreverValor(
      LIN_SENSOR_NIVEL,
      COL_VALOR,
      "NIVEL CORRETO"
    );

    escreverValor(
      LIN_BOMBA_ACAO,
      COL_VALOR,
      "DESLIGADA"
    );

  } else {

    digitalWrite(RELE_BOMBA, LOW);

    escreverValor(
      LIN_SENSOR_NIVEL,
      COL_VALOR,
      "NIVEL BAIXO"
    );

    escreverValor(
      LIN_BOMBA_ACAO,
      COL_VALOR,
      "LIGADA"
    );
  }

  // ==================================================
  // STATUS DOS GPIO
  // ==================================================

  escreverValor(
    LIN_STATUS_BOMBA,
    COL_VALOR,
    digitalRead(RELE_BOMBA) == LOW
      ? "LIGADA"
      : "DESLIGADA"
  );

  escreverValor(
    LIN_STATUS_AQUEC,
    COL_VALOR,
    digitalRead(RELE_AQUECEDOR) == LOW
      ? "LIGADO"
      : "DESLIGADO"
  );

  escreverValor(
    LIN_STATUS_COOLER,
    COL_VALOR,
    digitalRead(RELE_COOLER) == LOW
      ? "LIGADO"
      : "DESLIGADO"
  );

  // ==================================================
  // CURSOR FORA DA ÁREA DOS DADOS
  // ==================================================

  cursorEm(26, 1);

  // ==================================================
  // ATUALIZA A CADA 1 SEGUNDO
  // ==================================================

  delay(1000);
}
