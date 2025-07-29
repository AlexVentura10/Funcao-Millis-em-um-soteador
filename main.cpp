#include <Arduino.h>

// Pinos dos componentes: onde cada parte está conectada no Arduino
const int LED_VERDE = 8;        // LED verde no pino 8
const int LED_VERMELHO = 12;    // LED vermelho no pino 12
const int BOTAO = 13;           // Botão no pino 13


// Variáveis para controlar o piscar do LED verde (sem travar o código)
long intervaloPisca = 400;      // Tempo que o LED verde fica ligado/desligado (em milissegundos)
long ultimoTempoPisca = 0;      // Guarda a última vez que o LED verde mudou de estado
int estadoLedVerde = LOW;       // Estado atual do LED verde (LOW = apagado, HIGH = ligado)


int estadoBotaoAnterior = HIGH; // Guarda o estado anterior do botão (HIGH = não pressionado)

void setup() 

{
  // Configura os pinos:
  pinMode(LED_VERDE, OUTPUT);       // LED verde é uma SAÍDA (manda sinal)
  pinMode(LED_VERMELHO, OUTPUT);    // LED vermelho é uma SAÍDA
  pinMode(BOTAO, INPUT_PULLUP);     // Botão é uma ENTRADA com resistor interno

  // Inicia a comunicação com o computador (para ver mensagens)
  Serial.begin(9600);

  // Prepara o gerador de números aleatórios (para sorteios diferentes)
  randomSeed(analogRead(A0)); // Usa um pino vazio para um valor inicial aleatório
}



void loop() 
{
  // --- PARTE 1: Fazer o LED verde piscar sem parar o programa ---

  // Pega o tempo atual do "cronômetro" do Arduino (em milissegundos)
  // A função millis() retorna o tempo desde que o Arduino foi ligado, gerenciando e otimizando o tempo de execução das tarefas
  
  unsigned long tempoAtual = millis();

  // Verifica se já passou o tempo para mudar o LED verde E se o LED vermelho está apagado
  if (tempoAtual - ultimoTempoPisca >= intervaloPisca && digitalRead(LED_VERMELHO) == LOW) 
  {
    ultimoTempoPisca = tempoAtual; // Atualiza o "cronômetro" do LED verde para começar a contar de novo

    // Troca o estado do LED verde (se estava ligado, desliga; se estava desligado, liga)
    if (estadoLedVerde == LOW) 
    {
      estadoLedVerde = HIGH;
    } else {
      estadoLedVerde = LOW;
    }
    digitalWrite(LED_VERDE, estadoLedVerde); // Aplica o novo estado ao LED verde
  }

  // --- PARTE 2: Ler o botão e fazer o sorteio ---

  // Lê o estado atual do botão (pressionado = LOW, não pressionado = HIGH)
  int estadoBotaoAtual = digitalRead(BOTAO);

  // Se o botão foi pressionado AGORA (passou de HIGH para LOW) E o LED vermelho está apagado...
  if (estadoBotaoAtual == LOW && estadoBotaoAnterior == HIGH && digitalRead(LED_VERMELHO) == LOW) {
    delay(50); // Pausa curta para 'limpar' o sinal do botão (debounce)

    // Confirma que o botão AINDA está pressionado após a pausa
    if (digitalRead(BOTAO) == LOW) 
    {
      Serial.println("Botao apertado! Sorteando..."); // Mensagem no Monitor Serial

      digitalWrite(LED_VERDE, LOW); // Apaga o LED verde para focar no sorteio

      // Gera um número aleatório entre 0 e 99
      int resultadoSorteio = random(100);

      Serial.print("Numero sorteado: "); // Mostra o número sorteado
      Serial.println(resultadoSorteio);

      // Se o número sorteado for MENOR que 20 (20% de chance de ganhar)...
      if (resultadoSorteio < 20) 
      {
        digitalWrite(LED_VERMELHO, HIGH); // Liga o LED vermelho (GANHOU!)
        Serial.println("!!! LED VERMELHO ACENDEU !!!");

        
        delay(500); // Pequena pausa antes de apagar o LED
        digitalWrite(LED_VERMELHO, LOW);  // Apaga o LED vermelho
        Serial.println("LED Vermelho apagado.");
      } 
   
    }
  }

  // Guarda o estado atual do botão para a próxima verificação do "clique"
  estadoBotaoAnterior = estadoBotaoAtual;
}