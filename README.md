# 🎮 Genius Bomb: Dual Mode Project

Este projeto é uma releitura do clássico jogo de memória "Genius" (Simon Says), implementado com Arduino. Ele apresenta dois modos de jogo distintos, selecionáveis via hardware, e utiliza um Anel de LEDs (NeoPixel) para feedback visual avançado.

<img width="1321" height="562" alt="Genius Oficial (1)" src="https://github.com/user-attachments/assets/4fa8c56f-b034-4a5b-b4d3-6827234dd539" />

[Genius Oficial.pdf](https://github.com/user-attachments/files/24782081/Genius.Oficial.pdf)


## 🚀 Funcionalidades

* **Modo Clássico (Switch HIGH):** Jogo de memória tradicional. O anel de LEDs serve como feedback visual das cores. Sem pressão de tempo.
* **Modo Bomba (Switch LOW):** Jogo de memória sob pressão. O anel de LEDs se torna um "timer visual" que diminui a cada rodada. Se o tempo acabar: Game Over.
* **Display LCD:** Exibe o status do jogo, rodada atual, tempo restante e mensagens de vitória/derrota.
* **Sistema de Áudio:** Feedback sonoro com notas específicas para cada cor e efeitos de explosão/vitória.

## 🛠️ Lista de Componentes

| Componente | Quantidade | Conexão (Pino) |
| :--- | :---: | :--- |
| Arduino Uno | 1 | - |
| Display LCD 16x2 (I2C) | 1 | SDA (A4), SCL (A5) |
| NeoPixel Ring (12 LEDs) | 1 | D12 |
| Slide Switch | 1 | D5 |
| Botões Push-Button | 4 | D8, D9, D10, D11 |
| LEDs Difusos (4 cores) | 4 | D4, D5, D6, D7 |
| Buzzer Piezo | 1 | D2 |
| Botão de Start | 1 | D3 |
| Resistores | Diversos | - |

## 📚 Bibliotecas Necessárias

Para rodar este código, instale as seguintes bibliotecas na IDE do Arduino:
1.  **Adafruit_LiquidCrystal** (para o chip MCP23008 ou PCF8574)
2.  **Adafruit_NeoPixel**
3.  **Wire** (Nativa)

## 🕹️ Como Jogar

1.  Selecione o modo de jogo usando o **Switch Deslizante**.
2.  Aperte o **Botão Start**.
3.  Observe a sequência de cores e sons.
4.  Repita a sequência pressionando os botões coloridos.
    * No **Modo Bomba**, seja rápido antes que o anel vermelho se complete!
5.  Vença 10 rodadas para desarmar a bomba ou ganhar o jogo.

---
*Desenvolvido pelo Grupo 2 do evento Conhecendo a Mecatrônica da Escola Politécnica da Universidade de São Paulo, Edição 2026.1*
