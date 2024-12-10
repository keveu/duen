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

// Ŀ�� ��ġ
int cursor_x = 1, cursor_y = 1;

// ���� ���� �Լ�
void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

// ��� ���� ���� �Լ�
void setBackgroundColor(int bg_color, int fg_color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (bg_color << 4) | fg_color);
}

// ��Ʈ��
void intro(void) {
    printf("DUNE 1.5\n");
    Sleep(2000);
    system("cls");
}

void init(void) {
    // layer 0 �� �׵θ� 
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

    // layer 1(map[1])�� ��� �α�(-1�� ä��)
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            map[1][i][j] = -1;
        }
    }

    // ��Ʈ���̵� ����(�÷��̾�)
    map[0][MAP_HEIGHT - 3][1] = 'B';
    map[0][MAP_HEIGHT - 3][2] = 'B';
    map[0][MAP_HEIGHT - 2][1] = 'B';
    map[0][MAP_HEIGHT - 2][2] = 'B';

    // ��Ʈ���̵� �Ϻ�����
    map[0][MAP_HEIGHT - 4][1] = 'H';

    // ���ڳ� ����(AI)
    map[0][1][MAP_WIDTH - 3] = 'B';
    map[0][1][MAP_WIDTH - 2] = 'B';
    map[0][2][MAP_WIDTH - 3] = 'B';
    map[0][2][MAP_WIDTH - 2] = 'B';

    // ���ڳ� �Ϻ�����
    map[0][3][MAP_WIDTH - 2] = 'H';

    // �����̽�
    map[0][MAP_HEIGHT - 6][1] = '5';
    map[0][5][MAP_WIDTH - 2] = '5';

    // ����
    map[0][MAP_HEIGHT - 3][3] = 'P';
    map[0][MAP_HEIGHT - 3][4] = 'P';
    map[0][MAP_HEIGHT - 2][3] = 'P';
    map[0][MAP_HEIGHT - 2][4] = 'P';
    map[0][1][MAP_WIDTH - 5] = 'P';
    map[0][1][MAP_WIDTH - 4] = 'P';
    map[0][2][MAP_WIDTH - 5] = 'P';
    map[0][2][MAP_WIDTH - 4] = 'P';

    // ����
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
    // �� ���
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            // Ŀ�� ��ġ�� ���� ���ڿ� ��� ������� ���
            if (i == cursor_y && j == cursor_x) {
                setBackgroundColor(15, 0); // ��� ���, ���� ����
                printf(" ");
                setColor(0x7); // �� �ʱ�ȭ
            }
            // ��Ʈ���̵� ������Ʈ�� �Ķ������� ���
            else if ((i == MAP_HEIGHT - 3 || i == MAP_HEIGHT - 2) && (j == 1 || j == 2) || (i == MAP_HEIGHT - 4 && j == 1)) {
                setColor(0x1); // �Ķ���
                printf("%c", map[0][i][j]);
                setColor(0x7); // �� �ʱ�ȭ
            }
            // ���ڳ� ������Ʈ�� ���������� ���
            else if ((i == 1 || i == 2) && (j == MAP_WIDTH - 3 || j == MAP_WIDTH - 2) || (i == 3 && j == MAP_WIDTH - 2)) {
                setColor(0x4); // ������
                printf("%c", map[0][i][j]);
                setColor(0x7); // �� �ʱ�ȭ
            }
            // �����̽�
            else if ((i == MAP_HEIGHT - 6 && j == 1) || (i == 5 && j == MAP_WIDTH - 2)) {
                setColor(0x6); // ��Ȳ��
                printf("%c", map[0][i][j]);
                setColor(0x7); // �� �ʱ�ȭ
            }
            // ������ ȸ������ ���
            else if (map[0][i][j] == 'R') {
                setColor(0x8); // ȸ��
                printf("%c", map[0][i][j]);
                setColor(0x7); // �� �ʱ�ȭ
            }
            else {
                printf("%c", map[0][i][j]);
            }
        }
        printf("\n");
    }
}

void move_cursor() {
    // ����Ű �Է� ó��
    while (1) {
        if (_kbhit()) {
            char key = _getch();
            switch (key) {
            case 72: // ���� ȭ��ǥ
                if (cursor_y > 1) cursor_y--;
                break;
            case 80: // �Ʒ��� ȭ��ǥ
                if (cursor_y < MAP_HEIGHT - 2) cursor_y++;
                break;
            case 75: // ���� ȭ��ǥ
                if (cursor_x > 1) cursor_x--;
                break;
            case 77: // ������ ȭ��ǥ
                if (cursor_x < MAP_WIDTH - 2) cursor_x++;
                break;
            }
            // ���� �ٽ� ����Ͽ� Ŀ�� ��ġ�� ����
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
