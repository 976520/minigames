#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

#define WIDTH 20
#define HEIGHT 25

//��Ʈ�ι̳� ��� ����
int tetrominoes[7][4][4] = {
    {{1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},  // ��
    {{1, 1, 1, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},  // �Ųٷ� ��
    {{1, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},  // ��
    {{1, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},  // ��
    {{0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},  // �Ųٷ� ��
    {{1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},  // ��
    {{1, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}   // ��
};

// ��Ʈ�ι̳� ���� ����
int tetrominoColors[7] = {
    9,  // �� - Blue
    1,  // �Ųٷ� ��- Dark Blue
    14, // �� - Yellow
    6,  // �� - Cyan
    2,  // �Ųٷ� �� - Green
    4,  // �� - Red
    5   // �� - Magenta
};

int board[HEIGHT][WIDTH] = { 0 };
int colorBoard[HEIGHT][WIDTH] = { 0 }; // �� ����
int currentTetromino[4][4];
int currentColor;
int currentX = 0, currentY = 0;
int score = 0;

HANDLE hConsole;

// ���
void drawBoard() {
    system("cls");
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            int block = board[y][x];
            int color = 7; // �⺻ ���� ���

            // ���� ��ǥ�� ��� ���
            if (y >= currentY && y < currentY + 4 && x >= currentX && x < currentX + 4) {
                if (currentTetromino[y - currentY][x - currentX] == 1) {
                    block = 1;
                    color = currentColor;
                }
            }
            else if (block) {
                color = colorBoard[y][x];
            }

            SetConsoleTextAttribute(hConsole, color);
            printf(block ? "#" : " ");
        }
        printf("\n");
    }
    SetConsoleTextAttribute(hConsole, 7);  // �⺻������ �ٽ� ����
    for (int f = 0; f < WIDTH; f++) {
        printf("-");
    }

    printf("\nScore: %d\n", score);
}

// ��Ʈ�ι̳� ����
void copyTetromino(int dst[4][4], int src[4][4]) {
    for (int y = 0; y < 4; y++)
        for (int x = 0; x < 4; x++)
            dst[y][x] = src[y][x];
}

//��Ʈ�ι̳� ����
int newTetromino() {
    int t = rand() % 7;
    copyTetromino(currentTetromino, tetrominoes[t]);
    currentColor = tetrominoColors[t];
    currentX = WIDTH / 2 - 2;
    currentY = 0;

    //������������
    for (int x = 0; x < 4; x++) {
        if (currentTetromino[0][x] == 1 && board[currentY][currentX + x] == 1) {
            return 0; // ���� ����
        }
    }
    return 1; // ��� ����
}


// �浹 ����
int checkCollision(int nx, int ny) {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (currentTetromino[y][x] == 1) {
                int bx = nx + x;
                int by = ny + y;
                if (bx < 0 || bx >= WIDTH || by < 0 || by >= HEIGHT || board[by][bx] == 1) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

// ��Ʈ�ι̳� ����
void fixTetromino() {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (currentTetromino[y][x] == 1) {
                board[currentY + y][currentX + x] = 1;
                colorBoard[currentY + y][currentX + x] = currentColor; //fix�� ��Ʈ�ι̳��� �������� ����
            }
        }
    }
}

//���� ä������
void clearLines() {
    for (int y = 0; y < HEIGHT; y++) {
        int filled = 1;
        for (int x = 0; x < WIDTH; x++) {
            if (board[y][x] == 0) {
                filled = 0;
                break;
            }
        }
        if (filled) {
            for (int ny = y; ny > 0; ny--) {
                for (int nx = 0; nx < WIDTH; nx++) {
                    board[ny][nx] = board[ny - 1][nx];
                    colorBoard[ny][nx] = colorBoard[ny - 1][nx]; // ���� ������ �Բ� �̵�
                }
            }
            for (int nx = 0; nx < WIDTH; nx++) {
                board[0][nx] = 0;
                colorBoard[0][nx] = 0; // ������ ����ְԲ�
            }
            score++;  // ���� ����
        }
    }
}

// e�������� ȸ��
void rotateTetrominoClockwise() {
    int rotated[4][4];
    for (int y = 0; y < 4; y++)
        for (int x = 0; x < 4; x++)
            rotated[y][x] = currentTetromino[3 - x][y];

    if (!checkCollision(currentX, currentY)) {
        copyTetromino(currentTetromino, rotated);
    }
}

// q�������� ������
void rotateTetrominoCounterClockwise() {
    int rotated[4][4];
    for (int y = 0; y < 4; y++)
        for (int x = 0; x < 4; x++)
            rotated[y][x] = currentTetromino[x][3 - y];

    if (!checkCollision(currentX, currentY)) {
        copyTetromino(currentTetromino, rotated);
    }
}

int main() {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    srand(time(0));

    if (!newTetromino()) {
        printf("Game Over\n");
        return 0;
    }

    clock_t lastFallTime = clock();
    while (1) {
        drawBoard();

        //�Է�
        if (_kbhit()) {
            switch (_getch()) {
            case 'a':  //��
                if (!checkCollision(currentX - 1, currentY)) currentX--;
                break;
            case 'd':  //��
                if (!checkCollision(currentX + 1, currentY)) currentX++;
                break;
            case 's':  //��
                if (!checkCollision(currentX, currentY + 1)) currentY++;
                else {
                    fixTetromino();
                    clearLines();
                    if (!newTetromino()) {
                        printf("Game Over\n");
                        return 0;
                    }
                }
                break;
            case 'q':  //ȸ��
                rotateTetrominoCounterClockwise();
                break;
            case 'e':  //�ݽð�ȸ��
                rotateTetrominoClockwise();
                break;
            }
        }

        // 500ms �������� ��Ʈ�ι̳� ��������
        clock_t currentTime = clock();
        if ((currentTime - lastFallTime) * 1000 / CLOCKS_PER_SEC >= 500) {
            if (!checkCollision(currentX, currentY + 1)) currentY++;
            else {
                fixTetromino();
                clearLines();
                if (!newTetromino()) {
                    printf("Game Over\n");
                    return 0;
                }
            }
            lastFallTime = currentTime;
        }

        Sleep(10);  // CPU ��뷮�� ���̱� ���� �ణ�� ��� �ð� �߰�
    }
    return 0;
}
