#include <U8g2lib.h>
#include <SPI.h>
#include <ezButton.h>
#include "images.h"

#define OLED_CS D8   // Chân CS
#define OLED_RST D1  // Chân RST
#define OLED_DC D2   // Chân DC

U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI u8g2(U8G2_R0, OLED_CS, OLED_DC, OLED_RST);

#define BUTTON_PIN D0

ezButton button(BUTTON_PIN);

const int screenWidth = 128;
const int screenHeight = 64;
const int birdWidth = 17;
const int birdHeight = 12;
const float gravity = 0.45;
const float jumpStrength = -2.5;
const int pipeWidth = 10;
const int pipeGap = 33;
const int pipeSpeed = 2;

int birdX = 20;
int birdY = screenHeight / 2;
float birdVelocity = 0;
int birdFrameCount = 0;

int pipeX = screenWidth;
int pipeHeight = random(10, 30);
int pipeHeightHead = 4;

bool isGameOver = false;
bool hasScored = false; // Trạng thái đã ghi điểm

unsigned long previousMillis = 0;
const long interval = 50;  // Điều chỉnh tốc độ khung hình

#define MAIN_MENU_SCREEN 0
#define GAMEPLAY_SCREEN 1
#define GAMEOVER_SCREEN 2

int buttonState;             // the current reading from the input pin
int score = 0;
int gameState = MAIN_MENU_SCREEN;

void setup() {
  u8g2.begin();
  u8g2.setFont(u8g2_font_ncenB08_tr);  // Chọn font để hiển thị
  Serial.begin(9600);
  // pinMode(BUTTON_PIN, INPUT_PULLUP);  // Cài đặt nút nhấn
  button.setDebounceTime(50); // set debounce time to 50 milliseconds
}

void loop() {
  button.loop();  // Cập nhật trạng thái nút
  // buttonState = digitalRead(buttonState);
  buttonState = button.getState();
  Serial.println(buttonState);
  if (gameState == MAIN_MENU_SCREEN) {
    drawMainScreen();
  } else if (gameState == GAMEPLAY_SCREEN) {
    drawPlayScreen();
  } else if (gameState == GAMEOVER_SCREEN) {
    drawGameOverScreen();
  }
}

void drawBird() {
  if (birdFrameCount < ANIM_FRAME / 3) {
      u8g2.drawXBMP(birdX, birdY, birdWidth, birdHeight, bird_frame_1);
  } else if (birdFrameCount < ANIM_FRAME / 2) {
      u8g2.drawXBMP(birdX, birdY, birdWidth, birdHeight, bird_frame_2);
  } else {
      u8g2.drawXBMP(birdX, birdY, birdWidth, birdHeight, bird_frame_3);
  }
  birdFrameCount++;
  if (birdFrameCount >= ANIM_FRAME) birdFrameCount = 0;
}

void drawPipe() {
  // Ống trên
  u8g2.drawBox(pipeX, 0, pipeWidth, pipeHeight);
  u8g2.drawBox(pipeX - 1, pipeHeight, pipeWidth + 2, pipeHeightHead);
  
  // Ống dưới
  int bottomPipeHeight = screenHeight - pipeHeight - pipeGap;
  u8g2.drawBox(pipeX - 1, pipeHeight + pipeGap, pipeWidth + 2, pipeHeightHead);
  u8g2.drawBox(pipeX, pipeHeight + pipeGap + pipeHeightHead, pipeWidth, bottomPipeHeight);
}


void drawPlayScreen() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    if (!isGameOver) {
      // Xử lý logic trò chơi
      birdVelocity += gravity;
      birdY += birdVelocity;
      // Di chuyển ống cản trở
      pipeX -= pipeSpeed;
      if (pipeX < -pipeWidth) {
        pipeX = screenWidth;
        pipeHeight = random(10, screenHeight - pipeGap - 10);
        hasScored = false; // Đặt lại trạng thái đã ghi điểm khi tạo ống mới
      }

      // Kiểm tra nếu chim vượt qua ống
      if (!hasScored && pipeX + pipeWidth < birdX) {
        score++;
        hasScored = true; // Đã ghi điểm cho ống này
      }

      // Nhấn nút để làm cho chim bay lên
      // if (digitalRead(BUTTON_PIN) == LOW) {
      //   birdVelocity = jumpStrength;
      // }
      if (buttonState == HIGH) {
        Serial.println("The button is pressed");
        birdVelocity = jumpStrength;
      }

      // Kiểm tra va chạm
      if (birdY > screenHeight - birdHeight || birdY < 0 || 
          (birdX + birdWidth > pipeX && birdX < pipeX + pipeWidth &&
           ((birdY < pipeHeight + pipeHeightHead) ||
            (birdY + birdHeight > pipeHeight + pipeGap - pipeHeightHead)))) {
        isGameOver = true;
      }
      // Vẽ trò chơi
      u8g2.clearBuffer();
      drawBird();
      drawPipe();
      drawScore();
      u8g2.sendBuffer();
    } else {
      // drawGameOverScreen();
      gameState = GAMEOVER_SCREEN;
    }
  }

  u8g2.sendBuffer();
}

void drawGameOverScreen() {
  // Vẽ màn hình game over
  u8g2.clearBuffer();
  u8g2.drawStr(28, 15, "GAME OVER");
  char scoreStr[10];
  sprintf(scoreStr, "Score: %d", score);
  u8g2.drawStr(36, 30, scoreStr);
  u8g2.drawStr(18, 45, "(Press to restart)");
  u8g2.sendBuffer();
  // Nhấn nút để khởi động lại trò chơi
  // if (digitalRead(BUTTON_PIN) == LOW) {
  //   resetGame();
  // }
  if (buttonState == HIGH) {
    delay(1000);
    resetGame();
  }
}

void drawMainScreen() {
  // Vẽ màn hình game over
  u8g2.clearBuffer();
  u8g2.drawStr(20, 15, "Game Chim Vui");
  u8g2.drawXBMP((screenWidth / 2) - (birdHeight / 2), 24, birdWidth, birdHeight, bird_frame_1);
  u8g2.drawStr(24, 45, "(Press to play)");
  u8g2.sendBuffer();

  // Nhấn nút để khởi động lại trò chơi
  // if (digitalRead(BUTTON_PIN) == LOW) {
  //   gameState = GAMEPLAY_SCREEN;
  // }
  if (buttonState == HIGH) {
    gameState = GAMEPLAY_SCREEN;
  }
}

void drawScore() {
  char scoreStr[10];
  sprintf(scoreStr, "%d", score);
  u8g2.drawStr(2, 10, scoreStr);
}

void resetGame() {
  score = 0;
  birdY = screenHeight / 2;
  birdVelocity = 0;
  pipeX = screenWidth;
  pipeHeight = random(10, screenHeight - pipeGap - 10);
  isGameOver = false;
  hasScored = false;
  gameState = GAMEPLAY_SCREEN;
}
