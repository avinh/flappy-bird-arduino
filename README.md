# Game Chim Non

The "Game Chim Non" is a simple game written in C++ for Arduino, utilizing an OLED display and the U8g2 library for graphics. The game is similar to "Flappy Bird," where you control a bird to navigate through obstacle pipes.

## Hardware Requirements

- Arduino (Uno, Nano, or compatible versions)
- OLED display SH1106 128x64
- Push button
- Connecting wires

## Software Requirements

- [Arduino IDE](https://www.arduino.cc/en/software)
- Library [U8g2](https://github.com/olikraus/u8g2)

## Hardware Connections

Hardware connections are as follows:

- OLED_CS (CS): Connect to Arduino pin D8
- OLED_RST (RST): Connect to Arduino pin D1
- OLED_DC (DC): Connect to Arduino pin D2
- D0 (SCK): Connect to Arduino SCK pin
- D1 (MOSI): Connect to Arduino MOSI pin
- BUTTON_PIN: Connect to Arduino pin D6

## Installation Guide

1. **Install U8g2 Library:**

   - Open Arduino IDE.
   - Go to **Sketch** -> **Include Library** -> **Manage Libraries...**
   - Search for **U8g2** and install it.

2. **Upload Code to Arduino:**

   - Copy the code from the `main.ino` file into the Arduino IDE.
   - Connect your Arduino to your computer.
   - Select **Tools** -> **Board** and choose your board type (e.g., Arduino Uno).
   - Select **Tools** -> **Port** and choose your port.
   - Press **Upload** button to upload the code to Arduino.

3. **Connect Hardware:**

   - Connect OLED display and push button as per the hardware diagram provided above.

## How to Play

- Upon starting, the game displays the main screen with the game title.
- Press the button to start playing.
- Press the button to make the bird flap and avoid pipes.
- The game ends if the bird collides with pipes or falls to the bottom of the screen.
- On the Game Over screen, press the button to play again.

## Main Functions

- drawBird(): Draw the bird character.
- drawPipe(): Draw obstacle pipes.
- drawPlayScreen(): Handle and draw the game screen.
- drawGameOverScreen(): Draw the game over screen.
- drawMainScreen(): Draw the main screen.
- drawScore(): Display the score.
- resetGame(): Reset the game to initial state.
