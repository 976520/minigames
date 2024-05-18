#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define WIDTH 50
#define HEIGHT 20
#define DINO_START_X 5
#define DINO_START_Y HEIGHT - 4  // 공룡의 초기 Y 좌표 (키 3칸)
#define DINO_CHAR 'O'
#define DINO_DUCK_CHAR '='
#define DINO_HEIGHT 3
#define DINO_DUCK_WIDTH 3
#define GRAVITY 1
#define INITIAL_JUMP_VELOCITY -4  // 점프 초기 속도
#define INITIAL_OBSTACLE_SPEED 1   // 초기 장애물 속도
#define MAX_OBSTACLE_SPEED 5       // 최대 장애물 속도
#define AIR_OBSTACLE_FREQUENCY 20  // 공중 장애물 생성 빈도 (1 이상의 정수)

int dinoX = DINO_START_X;
int dinoY = DINO_START_Y;
int groundObstacleX = WIDTH - 1;
int groundObstacleSize = 1;  // 땅 장애물 크기
int airObstacleX = WIDTH - 1;
int airObstacleY = HEIGHT - 3;  // 공중 장애물 Y 좌표
float obstacleSpeed = INITIAL_OBSTACLE_SPEED; // 장애물 속도
int score = 0;
int isJumping = 0;
int isDucking = 0;  // 엎드리는 상태
int jumpVelocity = 0;
int airObstacleActive = 0;  // 공중 장애물 활성화 여부

void gotoxy(int x, int y) {
    COORD coord = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void draw() {
    system("cls");
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (isDucking) {
                // 엎드린 공룡을 그리기
                if (i == dinoY + DINO_HEIGHT - 1 && j >= dinoX && j < dinoX + DINO_DUCK_WIDTH) {
                    printf("%c", DINO_DUCK_CHAR);
                }
                else if (i == HEIGHT - 1) {
                    printf("ㅡ");
                }
                else if (i == HEIGHT - 2 && j >= groundObstacleX && j < groundObstacleX + groundObstacleSize) {
                    printf("ㅗ");
                }
                else if (airObstacleActive && i == airObstacleY && j == airObstacleX) {
                    printf("ㄷ");  // 공중 장애물
                }
                else {
                    printf(" ");
                }
            }
            else {
                // 서 있는 공룡을 그리기
                if (i >= dinoY && i < dinoY + DINO_HEIGHT && j == dinoX) {
                    printf("%c", DINO_CHAR);
                }
                else if (i == HEIGHT - 1) {
                    printf("ㅡ");
                }
                else if (i == HEIGHT - 2 && j >= groundObstacleX && j < groundObstacleX + groundObstacleSize) {
                    printf("ㅗ");
                }
                else if (airObstacleActive && i == airObstacleY && j == airObstacleX) {
                    printf("ㄷ");  // 공중 장애물
                }
                else {
                    printf(" ");
                }
            }
        }
        printf("\n");
    }
    printf("Score: %d\n", score);
}

void update() {
    if (_kbhit()) {
        char ch = _getch();
        if (ch == ' ' && !isJumping && !isDucking) {
            isJumping = 1;
            jumpVelocity = INITIAL_JUMP_VELOCITY;  // 점프 초기 속도 설정
        }
    }

    // 엎드리는 상태를 지속적으로 확인
    if (GetAsyncKeyState('S') & 0x8000) {
        if (!isJumping) {
            isDucking = 1;  // 엎드리는 상태로 전환
        }
    }
    else {
        isDucking = 0;  // 엎드리는 상태 해제
    }

    if (isJumping) {
        dinoY += jumpVelocity / 2;  // 속도를 절반으로 나누어 더 부드럽게 만듦
        jumpVelocity += GRAVITY;
        if (dinoY >= DINO_START_Y) {
            dinoY = DINO_START_Y;
            isJumping = 0;
            jumpVelocity = 0;
        }
    }

    // 땅 장애물 업데이트
    groundObstacleX -= obstacleSpeed;
    if (groundObstacleX < 0) {
        groundObstacleX = WIDTH - 1;
        groundObstacleSize = (rand() % 2 + 1) * 3 - 2;  // 장애물 크기 1 또는 3
        score++;
        if (obstacleSpeed < MAX_OBSTACLE_SPEED) {
            obstacleSpeed += 0.05; // 장애물 속도 증가
        }
    }

    // 공중 장애물 업데이트
    if (airObstacleActive) {
        airObstacleX -= obstacleSpeed;
        if (airObstacleX < 0) {
            airObstacleActive = 0;
        }
    }
    else if (score % AIR_OBSTACLE_FREQUENCY == 0) {
        airObstacleActive = 1;
        airObstacleX = WIDTH - 1;
        airObstacleY = HEIGHT - 4 - (rand() % 5); // 공중 장애물의 Y 좌표 랜덤 설정
    }

    // 매 프레임마다 점수를 증가시킴
    score++;

    // 장애물에 닿았는지 확인
    if (isDucking) {
        // 엎드린 상태의 충돌 검사
        if (dinoX + DINO_DUCK_WIDTH > groundObstacleX && dinoX < groundObstacleX + groundObstacleSize && dinoY + DINO_HEIGHT - 1 == HEIGHT - 2) {
            printf("Game Over! Final Score: %d\n", score);
            exit(0);
        }
        if (airObstacleActive && dinoX + DINO_DUCK_WIDTH > airObstacleX && dinoX < airObstacleX + 1 && dinoY + DINO_HEIGHT - 1 == airObstacleY) {
            printf("Game Over! Final Score: %d\n", score);
            exit(0);
        }
    }
    else {
        // 서 있는 상태의 충돌 검사
        if (dinoX == groundObstacleX && dinoY + DINO_HEIGHT - 1 == HEIGHT - 2) {
            printf("Game Over! Final Score: %d\n", score);
            exit(0);
        }
        if (airObstacleActive && dinoX == airObstacleX && dinoY == airObstacleY) {
            printf("Game Over! Final Score: %d\n", score);
            exit(0);
        }
    }
}


int main() {
    srand(time(NULL));  // 랜덤 시드 초기화
    while (1) {
        draw();
        update();
        Sleep(50);  // 루프 속도를 빠르게 하여 더 부드럽게 만듦
    }
    return 0;
}

