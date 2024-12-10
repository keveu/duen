#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <Windows.h>
#include <conio.h>

#define N_LAYER 2
#define MAP_WIDTH 60
#define MAP_HEIGHT 18

char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH] = { 0 };

// 커서 위치
int cursor_x = 1, cursor_y = 1;

// 색상 설정 함수
void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

// 배경 색상 설정 함수
void setBackgroundColor(int bg_color, int fg_color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (bg_color << 4) | fg_color);
}

// 인트로
void intro(void) {
    printf("DUNE 1.5\n");
    Sleep(2000);
    system("cls");
}

void init(void) {
    // layer 0 맵 테두리 
    for (int j = 0; j < MAP_WIDTH; j++) {
        map[0][0][j] = '#';
        map[0][MAP_HEIGHT - 1][j] = '#';
    }

    for (int i = 1; i < MAP_HEIGHT - 1; i++) {
        map[0][i][0] = '#';
        map[0][i][MAP_WIDTH - 1] = '#';
        for (int j = 1; j < MAP_WIDTH - 1; j++) {
            map[0][i][j] = ' ';
        }
    }

    // layer 1(map[1])을 비워 두기(-1로 채움)
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            map[1][i][j] = -1;
        }
    }

    // 아트레이디스 본진(플레이어)
    map[0][MAP_HEIGHT - 3][1] = 'B';
    map[0][MAP_HEIGHT - 3][2] = 'B';
    map[0][MAP_HEIGHT - 2][1] = 'B';
    map[0][MAP_HEIGHT - 2][2] = 'B';

    // 아트레이디스 하베스터
    map[0][MAP_HEIGHT - 4][1] = 'H';

    // 하코넨 본진(AI)
    map[0][1][MAP_WIDTH - 3] = 'B';
    map[0][1][MAP_WIDTH - 2] = 'B';
    map[0][2][MAP_WIDTH - 3] = 'B';
    map[0][2][MAP_WIDTH - 2] = 'B';

    // 하코넨 하베스터
    map[0][3][MAP_WIDTH - 2] = 'H';

    // 스파이스
    map[0][MAP_HEIGHT - 6][1] = '5';
    map[0][5][MAP_WIDTH - 2] = '5';

    // 장판
    map[0][MAP_HEIGHT - 3][3] = 'P';
    map[0][MAP_HEIGHT - 3][4] = 'P';
    map[0][MAP_HEIGHT - 2][3] = 'P';
    map[0][MAP_HEIGHT - 2][4] = 'P';
    map[0][1][MAP_WIDTH - 5] = 'P';
    map[0][1][MAP_WIDTH - 4] = 'P';
    map[0][2][MAP_WIDTH - 5] = 'P';
    map[0][2][MAP_WIDTH - 4] = 'P';

    // 바위
    map[0][MAP_HEIGHT - 13][19] = 'R';
    map[0][MAP_HEIGHT - 13][20] = 'R';
    map[0][MAP_HEIGHT - 12][19] = 'R';
    map[0][MAP_HEIGHT - 12][20] = 'R';

    map[0][MAP_HEIGHT - 5][36] = 'R';
    map[0][MAP_HEIGHT - 5][37] = 'R';
    map[0][MAP_HEIGHT - 4][36] = 'R';
    map[0][MAP_HEIGHT - 4][37] = 'R';

    map[0][MAP_HEIGHT - 6][24] = 'R';

    map[0][MAP_HEIGHT - 10][46] = 'R';

    map[0][MAP_HEIGHT - 13][33] = 'R';
}

void display(void) {
    printf("spice = 0/0, population = 0/0\n");
    // 맵 출력
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            // 커서 위치는 검정 글자에 흰색 배경으로 출력
            if (i == cursor_y && j == cursor_x) {
                setBackgroundColor(15, 0); // 흰색 배경, 검정 글자
                printf(" ");
                setColor(0x7); // 색 초기화
            }
            // 아트레이디스 오브젝트를 파란색으로 출력
            else if ((i == MAP_HEIGHT - 3 || i == MAP_HEIGHT - 2) && (j == 1 || j == 2) || (i == MAP_HEIGHT - 4 && j == 1)) {
                setColor(0x1); // 파란색
                printf("%c", map[0][i][j]);
                setColor(0x7); // 색 초기화
            }
            // 하코넨 오브젝트를 빨간색으로 출력
            else if ((i == 1 || i == 2) && (j == MAP_WIDTH - 3 || j == MAP_WIDTH - 2) || (i == 3 && j == MAP_WIDTH - 2)) {
                setColor(0x4); // 빨간색
                printf("%c", map[0][i][j]);
                setColor(0x7); // 색 초기화
            }
            // 스파이스
            else if ((i == MAP_HEIGHT - 6 && j == 1) || (i == 5 && j == MAP_WIDTH - 2)) {
                setColor(0x6); // 주황색
                printf("%c", map[0][i][j]);
                setColor(0x7); // 색 초기화
            }
            // 바위를 회색으로 출력
            else if (map[0][i][j] == 'R') {
                setColor(0x8); // 회색
                printf("%c", map[0][i][j]);
                setColor(0x7); // 색 초기화
            }
            else {
                printf("%c", map[0][i][j]);
            }
        }
        printf("\n");
    }
}

void move_cursor() {
    // 방향키 입력 처리
    while (1) {
        if (_kbhit()) {
            char key = _getch();
            switch (key) {
            case 72: // 위쪽 화살표
                if (cursor_y > 1) cursor_y--;
                break;
            case 80: // 아래쪽 화살표
                if (cursor_y < MAP_HEIGHT - 2) cursor_y++;
                break;
            case 75: // 왼쪽 화살표
                if (cursor_x > 1) cursor_x--;
                break;
            case 77: // 오른쪽 화살표
                if (cursor_x < MAP_WIDTH - 2) cursor_x++;
                break;
            }
            // 맵을 다시 출력하여 커서 위치를 갱신
            system("cls");
            display();
        }
    }
}

int main(void) {
    srand((unsigned int)time(NULL));

    //intro();
    init();
    display();
    move_cursor();

    return 0;
}
