#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>

#define WIDTH 20
#define HEIGHT 10

typedef struct
{
	int x;
	int y;

	int act;
}Poo;

typedef struct
{
	int x;
}Player;

Poo poo[WIDTH];
Player player;

int score;							// 점수
int level;

void gotoxy(int x, int y)
{
	COORD pos = { 2 * x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

int isKeyDown(int key)
{
	return ((GetAsyncKeyState(key) & 0x8000) != 0);
}

void init()
{
	int i;

	for (i = 0; i < WIDTH; i++)
		poo[i].act = FALSE;

	player.x = WIDTH / 2;
}

void CreatePoo()
{
	int i;

	for (i = 0; i < WIDTH; i++)
	{
		if (!poo[i].act)
		{
			poo[i].x = rand() % WIDTH;
			poo[i].y = HEIGHT - 1;

			poo[i].act = TRUE;

			return;
		}
	}
}

void MovePoo()
{
	int i;

	for (i = 0; i < WIDTH; i++)
	{
		if (poo[i].act)
		{
			poo[i].y--;
		}
	}
}

void DeletePoo()
{
	int i;

	for (i = 0; i < WIDTH; i++)
	{
		if (poo[i].act && poo[i].y < 0)
		{
			poo[i].act = FALSE;
		}
	}
}

int PlayerContainsPoo()
{
	int i;

	for (i = 0; i < WIDTH; i++)
	{
		if ((poo[i].act) && (poo[i].y == 0) && (poo[i].x == player.x))
		{
			return TRUE;
		}
	}

	return FALSE;
}

void MovePlayer()
{
	if (isKeyDown(VK_LEFT))
		player.x -= 1;
	if (isKeyDown(VK_RIGHT))
		player.x += 1;

	if (player.x < 0)
		player.x = 0;
	if (player.x > WIDTH - 1)
		player.x = WIDTH - 1;
}

void PrintMap()
{
	system("cls");

	int i, j;

	printf("%c%c", 0xA6, 0xA3);
	for (i = 0; i < WIDTH + 18; i++)
		printf("%c%c", 0xA6, 0xA1);
	printf("%c%c", 0xA6, 0xA4);

	for (i = 0; i < HEIGHT; i++)
	{
		gotoxy(0, i + 1);

		printf("%c%c", 0xA6, 0xA2);
		for (j = 0; j < WIDTH - 1; j++)
			printf("  ");
		printf("%c%c", 0xA6, 0xA2);
	}

	for (i = 0; i < WIDTH; i++)
	{
		if (poo[i].act)
		{
			gotoxy(poo[i].x, HEIGHT - poo[i].y);
			printf("%c%c", 0xA1, 0xE3);
		}
	}

	gotoxy(player.x, HEIGHT);
	printf("□");

	gotoxy(0, HEIGHT + 1);
	for (i = 0; i < WIDTH; i++)
		printf("▤");

}

void OnIntro()
{
	printf("\n");
	printf("  0000   000   000     000   000  0     0 00000\n");
	printf("  0   0 0   0 0   0   0   0 0   0 00   00 0\n");
	printf("  0   0 0   0 0   0   0     0   0 0 0 0 0 0\n");
	printf("  0000  0   0 0   0   0 000 00000 0 0 0 0 00000\n");
	printf("  0     0   0 0   0   0   0 0   0 0  0  0 0\n");
	printf("  0     0   0 0   0   0   0 0   0 0  0  0 0\n");
	printf("  0      000   000     0000 0   0 0     0 00000\n\n");
	printf("   위에서 내려오는\n");
	printf("   똥(■)을 피해\n");
	printf("   살아남자!\n\n");
	printf("   조작법 : 좌우 방향키\n\n");
	printf("  Press Any Key To START");

	getch();

	system("cls");
}

void DrawGameStats()
{
	int writeLineX = WIDTH + 3;

	gotoxy(writeLineX, 1);
	printf("%c%cSCORE%c%c", 0xA6, 0xA2, 0xA6, 0xA2);
	gotoxy(writeLineX, 2);
	printf("%c%c%05d%c%c", 0xA6, 0xA2, score, 0xA6, 0xA2);

	gotoxy(writeLineX, 4);
	printf("LV.%d", level);
}

int OnResult()
{
	system("cls");
	printf("\n");
	printf("\n");
	printf("============================================\n");
	printf("       당신의 점수는 %05d점입니다. \n", score);
	printf("============================================\n");
	printf("\n");
	printf("\n");
	printf("아무 키나 누르면 종료됩니다.\n");

	getchar();

}

void main()
{
	char key;

	init();
	OnIntro();
	do
	{
		srand((int)malloc(NULL));

		CreatePoo();
		MovePoo();
		DeletePoo();

		MovePlayer();

		PrintMap();
		DrawGameStats();

		score += 1;
		level = (score / 100) + 1;

		Sleep(100);
	} while (!(PlayerContainsPoo()));

	OnResult();
}