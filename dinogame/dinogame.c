#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define WIDTH 50
#define HEIGHT 20
#define DINO_START_X 5
#define DINO_START_Y HEIGHT - 4  // ������ �ʱ� Y ��ǥ (Ű 3ĭ)
#define DINO_CHAR 'O'
#define DINO_DUCK_CHAR '='
#define DINO_HEIGHT 3
#define DINO_DUCK_WIDTH 3
#define GRAVITY 1
#define INITIAL_JUMP_VELOCITY -4  // ���� �ʱ� �ӵ�
#define INITIAL_OBSTACLE_SPEED 1   // �ʱ� ��ֹ� �ӵ�
#define MAX_OBSTACLE_SPEED 5       // �ִ� ��ֹ� �ӵ�
#define AIR_OBSTACLE_FREQUENCY 20  // ���� ��ֹ� ���� �� (1 �̻��� ����)

int dinoX = DINO_START_X;
int dinoY = DINO_START_Y;
int groundObstacleX = WIDTH - 1;
int groundObstacleSize = 1;  // �� ��ֹ� ũ��
int airObstacleX = WIDTH - 1;
int airObstacleY = HEIGHT - 3;  // ���� ��ֹ� Y ��ǥ
float obstacleSpeed = INITIAL_OBSTACLE_SPEED; // ��ֹ� �ӵ�
int score = 0;
int isJumping = 0;
int isDucking = 0;  // ���帮�� ����
int jumpVelocity = 0;
int airObstacleActive = 0;  // ���� ��ֹ� Ȱ��ȭ ����

void gotoxy(int x, int y) {
    COORD coord = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void draw() {
    system("cls");
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (isDucking) {
                // ���帰 ������ �׸���
                if (i == dinoY + DINO_HEIGHT - 1 && j >= dinoX && j < dinoX + DINO_DUCK_WIDTH) {
                    printf("%c", DINO_DUCK_CHAR);
                }
                else if (i == HEIGHT - 1) {
                    printf("��");
                }
                else if (i == HEIGHT - 2 && j >= groundObstacleX && j < groundObstacleX + groundObstacleSize) {
                    printf("��");
                }
                else if (airObstacleActive && i == airObstacleY && j == airObstacleX) {
                    printf("��");  // ���� ��ֹ�
                }
                else {
                    printf(" ");
                }
            }
            else {
                // �� �ִ� ������ �׸���
                if (i >= dinoY && i < dinoY + DINO_HEIGHT && j == dinoX) {
                    printf("%c", DINO_CHAR);
                }
                else if (i == HEIGHT - 1) {
                    printf("��");
                }
                else if (i == HEIGHT - 2 && j >= groundObstacleX && j < groundObstacleX + groundObstacleSize) {
                    printf("��");
                }
                else if (airObstacleActive && i == airObstacleY && j == airObstacleX) {
                    printf("��");  // ���� ��ֹ�
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
            jumpVelocity = INITIAL_JUMP_VELOCITY;  // ���� �ʱ� �ӵ� ����
        }
    }

    // ���帮�� ���¸� ���������� Ȯ��
    if (GetAsyncKeyState('S') & 0x8000) {
        if (!isJumping) {
            isDucking = 1;  // ���帮�� ���·� ��ȯ
        }
    }
    else {
        isDucking = 0;  // ���帮�� ���� ����
    }

    if (isJumping) {
        dinoY += jumpVelocity / 2;  // �ӵ��� �������� ������ �� �ε巴�� ����
        jumpVelocity += GRAVITY;
        if (dinoY >= DINO_START_Y) {
            dinoY = DINO_START_Y;
            isJumping = 0;
            jumpVelocity = 0;
        }
    }

    // �� ��ֹ� ������Ʈ
    groundObstacleX -= obstacleSpeed;
    if (groundObstacleX < 0) {
        groundObstacleX = WIDTH - 1;
        groundObstacleSize = (rand() % 2 + 1) * 3 - 2;  // ��ֹ� ũ�� 1 �Ǵ� 3
        score++;
        if (obstacleSpeed < MAX_OBSTACLE_SPEED) {
            obstacleSpeed += 0.05; // ��ֹ� �ӵ� ����
        }
    }

    // ���� ��ֹ� ������Ʈ
    if (airObstacleActive) {
        airObstacleX -= obstacleSpeed;
        if (airObstacleX < 0) {
            airObstacleActive = 0;
        }
    }
    else if (score % AIR_OBSTACLE_FREQUENCY == 0) {
        airObstacleActive = 1;
        airObstacleX = WIDTH - 1;
        airObstacleY = HEIGHT - 4 - (rand() % 5); // ���� ��ֹ��� Y ��ǥ ���� ����
    }

    // �� �����Ӹ��� ������ ������Ŵ
    score++;

    // ��ֹ��� ��Ҵ��� Ȯ��
    if (isDucking) {
        // ���帰 ������ �浹 �˻�
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
        // �� �ִ� ������ �浹 �˻�
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
    srand(time(NULL));  // ���� �õ� �ʱ�ȭ
    while (1) {
        draw();
        update();
        Sleep(50);  // ���� �ӵ��� ������ �Ͽ� �� �ε巴�� ����
    }
    return 0;
}

