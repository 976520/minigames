#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

#define WIDTH 20
#define HEIGHT 25

//테트로미노 모양 지정
int tetrominoes[7][4][4] = {
    {{1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},  // ㅣ
    {{1, 1, 1, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},  // 거꾸로 ㄴ
    {{1, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},  // ㄴ
    {{1, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},  // ㅁ
    {{0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},  // 거꾸로 ㄹ
    {{1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},  // ㄹ
    {{1, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}   // ㅗ
};

// 테트로미노 색상 지엉
int tetrominoColors[7] = {
    9,  // ㅣ - Blue
    1,  // 거꾸로 ㄴ- Dark Blue
    14, // ㄴ - Yellow
    6,  // ㅁ - Cyan
    2,  // 거꾸로 ㄹ - Green
    4,  // ㄹ - Red
    5   // ㅗ - Magenta
};

int board[HEIGHT][WIDTH] = { 0 };
int colorBoard[HEIGHT][WIDTH] = { 0 }; // 색 저장
int currentTetromino[4][4];
int currentColor;
int currentX = 0, currentY = 0;
int score = 0;

HANDLE hConsole;

// 출력
void drawBoard() {
    system("cls");
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            int block = board[y][x];
            int color = 7; // 기본 색상 흰색

            // 현재 좌표에 블록 출력
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
    SetConsoleTextAttribute(hConsole, 7);  // 기본색으로 다시 ㄱㄱ
    for (int f = 0; f < WIDTH; f++) {
        printf("-");
    }

    printf("\nScore: %d\n", score);
}

// 테트로미노 복사
void copyTetromino(int dst[4][4], int src[4][4]) {
    for (int y = 0; y < 4; y++)
        for (int x = 0; x < 4; x++)
            dst[y][x] = src[y][x];
}

//테트로미노 생성
int newTetromino() {
    int t = rand() % 7;
    copyTetromino(currentTetromino, tetrominoes[t]);
    currentColor = tetrominoColors[t];
    currentX = WIDTH / 2 - 2;
    currentY = 0;

    //게임종료조건
    for (int x = 0; x < 4; x++) {
        if (currentTetromino[0][x] == 1 && board[currentY][currentX + x] == 1) {
            return 0; // 게임 종료
        }
    }
    return 1; // 계속 진행
}


// 충돌 감지
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

// 테트로미노 고정
void fixTetromino() {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (currentTetromino[y][x] == 1) {
                board[currentY + y][currentX + x] = 1;
                colorBoard[currentY + y][currentX + x] = currentColor; //fix된 테트로미노의 색상으로 설정
            }
        }
    }
}

//한줄 채웠을때
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
                    colorBoard[ny][nx] = colorBoard[ny - 1][nx]; // 색상 정보도 함께 이동
                }
            }
            for (int nx = 0; nx < WIDTH; nx++) {
                board[0][nx] = 0;
                colorBoard[0][nx] = 0; // 맨윗줄 비어있게끔
            }
            score++;  // 점수 증가
        }
    }
}

// e눌렀을때 회전
void rotateTetrominoClockwise() {
    int rotated[4][4];
    for (int y = 0; y < 4; y++)
        for (int x = 0; x < 4; x++)
            rotated[y][x] = currentTetromino[3 - x][y];

    if (!checkCollision(currentX, currentY)) {
        copyTetromino(currentTetromino, rotated);
    }
}

// q눌렀을때 히ㅗ전
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

        //입력
        if (_kbhit()) {
            switch (_getch()) {
            case 'a':  //왼
                if (!checkCollision(currentX - 1, currentY)) currentX--;
                break;
            case 'd':  //오
                if (!checkCollision(currentX + 1, currentY)) currentX++;
                break;
            case 's':  //밑
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
            case 'q':  //회전
                rotateTetrominoCounterClockwise();
                break;
            case 'e':  //반시계회전
                rotateTetrominoClockwise();
                break;
            }
        }

        // 500ms 간격으로 테트로미노 내려오기
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

        Sleep(10);  // CPU 사용량을 줄이기 위해 약간의 대기 시간 추가
    }
    return 0;
}
