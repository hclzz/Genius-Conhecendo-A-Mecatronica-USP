#include <LiquidCrystal_I2C.h>
#include <Adafruit_NeoPixel.h> // biblioteca do anel de leds

// configura o lcd no endereço 0x27 pra 16 caracteres e 2 linhas
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define MAX 10
#define PIN_NEO 13 // pino de dados do anel neopixel
#define NUM_LEDS 24 // quantidade de leds no anel

// configura o anel neopixel
Adafruit_NeoPixel anel = Adafruit_NeoPixel(NUM_LEDS, PIN_NEO, NEO_GRB + NEO_KHZ800);

// botao de start no pino 3
const int pinoStart = 3; 
// buzzer no pino 2 
const int pinoBuzzer = 2; 

// leds: vermelho(4) azul(5) amarelo(6) verde(7)
const int pinosLeds[4] = {4, 5, 6, 7};

// botoes: vermelho(8) azul(9) amarelo(10) verde(11)
const int pinosBotoes[4] = {8, 9, 10, 11};

//botao de velocidade/modo(12)
const int botaoVelocidade = 12;

// frequencias dos sons (do, re, mi, sol)
const int tons[4] = {262, 294, 330, 392}; 

// variaveis do jogo
int sequencia[MAX];
int rodadaAtual = 0;

// Variáveis de Modo de Jogo
int modoJogo = 0; // 0 = CLASSIC, 1 = BOMB
bool modoAlterado = false;
unsigned long tempoBomba = 3000; // Começa agora com apenas 3 segundos!

void setup() {
  Serial.begin(9600);
  
  // inicia o anel de leds
  anel.begin();
  anel.show(); // apaga tudo inicialmente
  
  // inicia o lcd
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("GENIUS ARDUINO");
  delay(2000);
  
  // configs de entrada e saida
  pinMode(pinoStart, INPUT_PULLUP);
  pinMode(pinoBuzzer, OUTPUT);
  pinMode(botaoVelocidade, INPUT_PULLUP);
  
  for (int i = 0; i < 4; i++) {
    pinMode(pinosLeds[i], OUTPUT);
    pinMode(pinosBotoes[i], INPUT_PULLUP);
  }

  //gera seed random
  randomSeed(analogRead(A0));
}

void loop() {
  
  // a logica espera pra ser iniciada dps de apertar o botao 3 (start)
  esperarStart();
  
  //a lógica espera a selecao do modo (CLASSIC ou BOMB)
  esperarModo();

  // inicia o game
  Serial.println("JOGO INICIADO!!"); //ja mostrando no serial
  
  // mostrando na lcd
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("JOGO INICIADO!!");
  
  delay(1000); // espera 

  // sorteia a sequência completa do game
  for (int i = 0; i < MAX; i++) {
    sequencia[i] = random(0, 4);
  }

  // loop das rodada
  // comeca na rodada 1 e vai ate o maximo (10)
  for (rodadaAtual = 1; rodadaAtual <= MAX; rodadaAtual++) {
    Serial.print("Rodada: "); Serial.println(rodadaAtual);
    
    // mostrando no lcd
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("RODADA: ");
    lcd.print(rodadaAtual);
    
    // --- ALTERAÇÃO AQUI: LÓGICA DE TEMPO INSANA ---
    // Começa com 3000ms (3s) e tira 250ms a cada rodada.
    // Na rodada 1: 2750ms
    // Na rodada 5: 1750ms
    // Na rodada 10: 500ms (Muito rápido)
    long calculoTempo = 3000 - (rodadaAtual * 250);
    
    // Trava mínima de segurança: nunca fica menor que 0.5 segundos (500ms)
    // Se ficar menor que isso é impossível humano reagir
    if (calculoTempo < 500) calculoTempo = 500;
    
    tempoBomba = calculoTempo; 
    // ------------------------------------------------

    // primeiro mostra a sequencia pro jogador
    reproduzirSequencia();

    //depois le o que o jogador aperta
    if (lerJogada() == false) {
      //se a funcao retornar "false", o jogador ERROUUU (leia com a voz do faustão)
      gameOver();
      break; // sai do loop das rodada e volta para o modo de espera
    }

    // se o jogador acertou a rodada inteira, entao:
    delay(1000); // espera um cadin antes da proxima rodada
  }

  // se saiu do loop porque venceu todas as rodadas
  if (rodadaAtual > MAX) {
    vitoria();
  }
}

// funçoes usadas no codigo
void esperarModo() {
  
  modoAlterado = false;
  modoJogo = 0; // Reinicia no classico
  
  //Mostra na serial e LCD o modo inicial
  atualizarDisplayModo();
  
  while(modoAlterado == false) {
    //Espera até o usuário pressionar um botão
    while (digitalRead(botaoVelocidade) == HIGH && digitalRead(pinoStart) == HIGH);
  
    //Se o botao de velocidade for pressionado, altera o modo
    if (digitalRead(botaoVelocidade) == LOW) {
        modoJogo++;
        if(modoJogo > 1) modoJogo = 0; // Alterna entre 0 e 1
      
        atualizarDisplayModo();
        delay(200); // debounce simples
    }
    
    if (digitalRead(pinoStart) == LOW) {
        Serial.print("Modo escolhido: "); 
        if(modoJogo == 0) Serial.println("CLASSIC");
        else Serial.println("BOMB");
        modoAlterado = true;
        delay(200);
    }
    delay(100);
  }
  
  // Limpa o anel ao sair da seleção
  for(int i=0; i<NUM_LEDS; i++) anel.setPixelColor(i, 0);
  anel.show();
}

void atualizarDisplayModo() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("SELECIONE MODO:");
  lcd.setCursor(0,1);
  
  if (modoJogo == 0) {
    lcd.print("> CLASSIC");
    // Anel Verde para Classic
    for(int i=0; i<NUM_LEDS; i++) anel.setPixelColor(i, anel.Color(0, 255, 0));
  } else {
    lcd.print("> BOMB (FAST)");
    // Anel Vermelho para Bomb
    for(int i=0; i<NUM_LEDS; i++) anel.setPixelColor(i, anel.Color(255, 0, 0));
  }
  anel.show();
}

// funçoes usadas no codigo
void esperarStart() {
  Serial.println("aguardando START ( botao pino 3)...");
  
  // mostrando na lcd
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("AGUARDANDO START");
  lcd.setCursor(0,1);
  lcd.print("APERTE O BOTAO");
  
  //enquanto o botão start nao for pressionado (HIGH)
  while (digitalRead(pinoStart) == HIGH) {
    //pisca os led pra chamar atenção
    for (int i = 0; i < 4; i++) digitalWrite(pinosLeds[i], HIGH);
    delay(150);
    for (int i = 0; i < 4; i++) digitalWrite(pinosLeds[i], LOW);
    delay(150);
  }
  
  //se saiu do while eh porque apertou o botao
  //som de início
  tone(pinoBuzzer, 1000, 200);
  delay(200);
  tone(pinoBuzzer, 2000, 300);
  delay(500); 
}

void reproduzirSequencia() {
  for (int i = 0; i < rodadaAtual; i++) {
    int cor = sequencia[i];
    
    digitalWrite(pinosLeds[cor], HIGH);
    tone(pinoBuzzer, tons[cor]);
    delay(500); //delay do led ligado padrao
    
    digitalWrite(pinosLeds[cor], LOW);
    noTone(pinoBuzzer);
    delay(250); // delay entre as notas do buzzer
  }
}

bool lerJogada() {
  
  // Se for modo BOMBA, acende o anel completo
  if (modoJogo == 1) {
     for(int i=0; i<NUM_LEDS; i++) anel.setPixelColor(i, anel.Color(0, 255, 0)); // Verde
     anel.show();
  } else {
     // Modo Classic apaga o anel
     for(int i=0; i<NUM_LEDS; i++) anel.setPixelColor(i, 0); 
     anel.show();
  }

  // verifica cada passo da sequência atual
  for (int i = 0; i < rodadaAtual; i++) {
    int botaoPressionado = -1; // o "-1" significa que nenhum botao foi apertado ainda
    
    // Marca o tempo inicial para o timer da bomba
    unsigned long tempoInicial = millis();

    // fica nesse loop ate o jogador apertar qualquer botao
    while (botaoPressionado == -1) {
      
      // --- LOGICA DO MODO BOMBA (TIMER NO ANEL) ---
      if (modoJogo == 1) {
        unsigned long tempoPassado = millis() - tempoInicial;
        
        // Verifica se o tempo acabou
        if (tempoPassado > tempoBomba) {
          return false; // GAME OVER por tempo
        }
        
        // Atualiza o anel visualmente (vai apagando)
        // Mapeia o tempo passado para a quantidade de leds apagados
        int ledsAcesos = map(tempoBomba - tempoPassado, 0, tempoBomba, 0, NUM_LEDS);
        
        // Define a cor baseada na urgência (Verde -> Amarelo -> Vermelho)
        uint32_t cor = anel.Color(0, 255, 0); // Verde
        if (ledsAcesos < NUM_LEDS/2) cor = anel.Color(255, 255, 0); // Amarelo
        if (ledsAcesos < NUM_LEDS/4) cor = anel.Color(255, 0, 0);   // Vermelho
        
        for(int k=0; k<NUM_LEDS; k++) {
           if (k < ledsAcesos) anel.setPixelColor(k, cor);
           else anel.setPixelColor(k, 0); // Apaga os que ja passaram
        }
        anel.show();
      }
      // --------------------------------------------
      
      // verifica os 4 botoes para ver se algum foi apertado
      for (int j = 0; j < 4; j++) {
        if (digitalRead(pinosBotoes[j]) == LOW) {
          delay(50); // Espera vibrar o contato
          if (digitalRead(pinosBotoes[j]) == LOW) { // Confirma se ainda está apertado
            
            botaoPressionado = j; // Registra qual botão foi
            
            // Feedback: Acende o LED e toca o som ENQUANTO o botão estiver apertado
            digitalWrite(pinosLeds[j], HIGH);
            tone(pinoBuzzer, tons[j]);
            
            // Trava o código aqui até o jogador SOLTAR o botão
            // Isso resolve o problema de pular rodadas
            while(digitalRead(pinosBotoes[j]) == LOW); 
            
            // Ao soltar:
            delay(50); // Debounce de soltura
            digitalWrite(pinosLeds[j], LOW);
            noTone(pinoBuzzer);
            
            // Se acertou o botão no modo bomba, reseta o timer visual pro proximo toque
            if (modoJogo == 1) {
               for(int k=0; k<NUM_LEDS; k++) anel.setPixelColor(k, anel.Color(0, 255, 0));
               anel.show();
               tempoInicial = millis(); // Reseta o contador pra proxima cor da sequencia
            }
          }
        }
      }
    }

    // aqui verifica se o botao que o jogador apertou foi o certo
    if (botaoPressionado != sequencia[i]) {
      return false; // se errou a cor, retorna falso
    }
  }
  
  return true; // se ele passou por todo o loop 'for", acertou a sequencia toda
}

void gameOver() {
  Serial.println("GAME OVER!!"); // print no serialzin
  
  // Efeito visual de explosão no anel se for modo bomba
  for(int k=0; k<3; k++) {
    for(int i=0; i<NUM_LEDS; i++) anel.setPixelColor(i, anel.Color(255, 0, 0));
    anel.show();
    delay(100);
    for(int i=0; i<NUM_LEDS; i++) anel.setPixelColor(i, 0);
    anel.show();
    delay(100);
  }
  
  //mostrando na lcd
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("GAME OVER!!");
  lcd.setCursor(0,1);
  lcd.print("TENTE NOVAMENTE");

  // som de derrota q o gpt fez, pq faço programa nao musicakkkk
  tone(pinoBuzzer, 150, 1000);
  
  // pisca tudo 3 vezes
  for (int k = 0; k < 3; k++) {
    for (int i = 0; i < 4; i++) digitalWrite(pinosLeds[i], HIGH);
    delay(300);
    for (int i = 0; i < 4; i++) digitalWrite(pinosLeds[i], LOW);
    delay(300);
  }
}

void vitoria() {
  Serial.println("VOCE VENCEU!");
  
  // Efeito arco-iris no anel
  for(int j=0; j<256; j+=50) {
    for(int i=0; i<NUM_LEDS; i++) {
      anel.setPixelColor(i, anel.Color(j, 255-j, 100));
    }
    anel.show();
    delay(20);
  }
  
  //mostrando na lcd
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("VOCE VENCEU!");
  lcd.setCursor(0,1);
  lcd.print("PARABENS!");

  // som de vitória do gpt again
  tone(pinoBuzzer, 500, 100); delay(100);
  tone(pinoBuzzer, 600, 100); delay(100);
  tone(pinoBuzzer, 700, 100); delay(100);
  tone(pinoBuzzer, 800, 300);
  
  // pisca pisca da vitória
  for (int k = 0; k < 5; k++) {
    for (int i = 0; i < 4; i++) {
      digitalWrite(pinosLeds[i], HIGH);
      delay(50);
      digitalWrite(pinosLeds[i], LOW);
    }
  }
}