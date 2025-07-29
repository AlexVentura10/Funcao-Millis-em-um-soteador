Por que descidi usar a Função Millis?

O loop() do Arduino: Uma Tarefa por Vez
O Arduino, em sua essência, é um microcontrolador que executa seu código de forma sequencial e única. 
Ele não é como um computador que pode rodar vários programas ao mesmo tempo em diferentes "threads".

No void loop(), ele começa da primeira linha, executa, vai para a próxima, executa, e assim por diante. 
Quando chega ao final do loop(), ele simplesmente volta para o início e recomeça tudo. Isso acontece muito, muito rápido.

O Problema do delay(): Ele PAUSA TUDO
No meu código anterior, sem o millis(), você tinha:

void loop() 
{
  // Faz o LED verde piscar continuamente
  digitalWrite(LED_VERDE, HIGH);
  delay(1000);  // <-- AQUI ESTÁ O PROBLEMA!
  digitalWrite(LED_VERDE, LOW);
  delay(1000);  // <-- E AQUI!

  // ... (código do botão e sorteio) ...
}

O comando delay(1000); faz o Arduino PARAR completamente por 1000 milissegundos (1 segundo).

O que estava acontecendo:

1 - O Arduino ligava o LED verde (digitalWrite(LED_VERDE, HIGH);).

2 - Ele parava por 1 segundo (delay(1000);). Durante esse 1 segundo, ele não estava fazendo ABSOLUTAMENTE NADA mais. 
Ele não estava lendo o botão, não estava lendo sensores, não estava atualizando outros LEDs. Estava "dormindo".

3 - Depois de 1 segundo, ele desligava o LED verde (digitalWrite(LED_VERDE, LOW);).

4- Ele parava por mais 1 segundo (delay(1000);). Novamente, "dormindo".

5 - Somente depois desses 2 segundos totais de delay, o Arduino finalmente seguia para as próximas linhas do loop(), 
que incluíam a leitura do botão: int estadoBotaoAtual = digitalRead(BOTAO);.

O resultado prático para você:

Você sentia que precisava pressionar o botão por muito tempo (ou em um momento "certo") para que o sorteio ocorresse porque:

- Se você pressionasse o botão durante um dos delay(1000), o Arduino simplesmente não registraria essa pressão.
Ele só "acordaria" depois que o delay terminasse e, então, finalmente leria o pino do botão.

- Você teria que manter o botão pressionado até que o delay terminasse e o Arduino chegasse à linha digitalRead(BOTAO); 
para que sua pressão fosse detectada. Se você soltasse o botão antes disso, o clique seria "perdido".

Por que millis() resolve isso:
Com millis(), o delay() é substituído por uma checagem de tempo não bloqueante.

C++

void loop() {
  // ... (código do pisca-pisca com millis) ...

  // O Arduino NÃO PARA AQUI. Ele continua executando rapidamente.
  // Ele apenas verifica se já passou tempo suficiente.

  // ... (código do botão e sorteio, que é verificado o tempo todo) ...
}

No modelo com millis():

- O Arduino liga ou desliga o LED verde, se for a hora (if (tempoAtual - ultimoTempoPisca >= intervaloPisca)).

- Imediatamente após isso, ele continua para as próximas linhas do loop(). Ele não espera.

- Ele lê o estado do botão (digitalRead(BOTAO);) quase que instantaneamente após a parte do LED verde.

- Como o loop() está sempre correndo muito rápido (milhares de vezes por segundo), ele está constantemente verificando o estado do botão.

Isso significa que sua pressão no botão é detectada quase no mesmo instante em que você a faz, 
independentemente de o LED verde estar aceso, apagado, ou se preparando para mudar de estado.

Em resumo:

- Com delay(): O Arduino para e fica cego/surdo por um período de tempo.

- Com millis(): O Arduino está sempre "ativo" e pode lidar com várias tarefas (como piscar um LED e ler um botão) simultaneamente, respondendo a eventos em tempo real.

É por isso que o millis() é preferível para a maioria dos projetos interativos, pois torna seu código muito mais responsivo e capaz de realizar múltiplas tarefas de forma eficiente.
