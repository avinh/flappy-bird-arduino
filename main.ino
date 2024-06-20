#include <U8g2lib.h>
#include <SPI.h>

#define OLED_CS D8   // Chân CS
#define OLED_RST D1  // Chân RST
#define OLED_DC D2   // Chân DC

U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI u8g2(U8G2_R0, OLED_CS, OLED_DC, OLED_RST);

#define BUTTON_PIN D6  // Chân nút nhấn (GPIO0)

const int screenWidth = 128;
const int screenHeight = 64;
const int birdWidth = 6;
const int birdHeight = 6;
const float gravity = 0.45;
const float jumpStrength = -2.5;
const int pipeWidth = 10;
const int pipeGap = 25;
const int pipeSpeed = 2;

int birdX = 20;
int birdY = screenHeight / 2;
float birdVelocity = 0;
int pipeX = screenWidth;
int pipeHeight = random(10, 30);

bool isGameOver = false;
bool hasScored = false; // Trạng thái đã ghi điểm

unsigned long previousMillis = 0;
const long interval = 50;  // Điều chỉnh tốc độ khung hình

#define MAIN_MENU_SCREEN 0
#define GAMEPLAY_SCREEN 1
#define GAMEOVER_SCREEN 2

int prevButtonState = HIGH;  // the previous state from the input pin
int buttonState;             // the current reading from the input pin
int score = 0;
int gameState = MAIN_MENU_SCREEN;

void setup() {
  u8g2.begin();
  u8g2.setFont(u8g2_font_ncenB08_tr);  // Chọn font để hiển thị
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);  // Cài đặt nút nhấn
}

void loop() {
  buttonState = digitalRead(buttonState);
  if (gameState == MAIN_MENU_SCREEN) {
    drawMainScreen();
  } else if (gameState == GAMEPLAY_SCREEN) {
    drawPlayScreen();
  } else if (gameState == GAMEOVER_SCREEN) {
    drawGameOverScreen();
  }
}

void drawBird() {
  u8g2.drawBox(birdX, birdY, birdWidth, birdHeight);
}

void drawPipe() {
  u8g2.drawBox(pipeX, 0, pipeWidth, pipeHeight);
  u8g2.drawBox(pipeX, pipeHeight + pipeGap, pipeWidth, screenHeight - pipeHeight - pipeGap);
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
      if (digitalRead(BUTTON_PIN) == LOW) {
        birdVelocity = jumpStrength;
      }

      // Kiểm tra va chạm
      if (birdY > screenHeight - birdHeight || birdY < 0 || (birdX + birdWidth > pipeX && birdX < pipeX + pipeWidth && (birdY < pipeHeight || birdY + birdHeight > pipeHeight + pipeGap))) {
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
  u8g2.drawStr(screenWidth / 2 - 30, screenHeight / 3, "Game Over");
  u8g2.drawStr(22, screenHeight / 2, "(Press to play)");
  u8g2.sendBuffer();

  // Nhấn nút để khởi động lại trò chơi
  if (digitalRead(BUTTON_PIN) == LOW) {
    resetGame();
  }
}

void drawMainScreen() {
  // Vẽ màn hình game over
  u8g2.clearBuffer();
  u8g2.drawStr(16, screenHeight / 3, "Game Chim Non");
  u8g2.drawStr(22, screenHeight / 2, "(Press to play)");
  u8g2.sendBuffer();

  // Nhấn nút để khởi động lại trò chơi
  if (digitalRead(BUTTON_PIN) == LOW) {
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
