#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>

#pragma warning (disable:4996)

#define ENEMY_SIZE 128

// Enumrators

typedef enum Direction
{
	Up,
	Down,
	Left,
	Right
}
Direction;	// ������ �̵� ���⿡ ���� ������


// Structs

typedef struct
{
	int x;
	int y;
}
Vector2;	// ������Ʈ�� ��ġ�� �簢���� ũ�� � ������ �� �ִ� 2���� ���� ������ ����ü

typedef struct
{
	Vector2 pos;
}
Player;		// �÷��̾� ����ü

typedef struct
{
	int isAlive;

	Vector2 pos;
	Direction dir;
}
Enemy;		// �� ����ü

// Variables

Vector2 boardPosition = { 0, 1 };	// ���� ���� ��ġ
Vector2 boardSize = { 20, 20 };		// ���� ���� ũ��

Player player;						// �÷��̾�
Enemy enemys[ENEMY_SIZE];			// ��

const float deltaTime = 0.1f;		// ���� �÷��� �� ������Ʈ �Ǵ� �������� ���� �ð�
int score;							// ����
int level;							// ���� (�������� ���̵� ����)


// Functions

void gotoxy(int x, int y);	// gotoxy
int isKeyDown(int key);		// ���ڷ� ���� Ű���� ���������� Ȯ�ΰ� ��ȯ //getch

void OnIntro();				// ���� ���� �� Ÿ��Ʋ�� ���۹� ǥ��
void OnGame();				// ���� ����
void OnGameOver();			// ���� ����
int OnResult();				// ���� ���� ���� ���â ���

void MovePlayer();			// �÷��̾� �̵�
void CreateEnemy();			// �� ����
Enemy InitEnemy();			// ���� ������ ������ ���� ���� �ʱ�ȭ
void MoveEnemy();			// �� �̵�
void DeleteEnemy();			// ���� ��迡 ������ �� ����

void DrawGameBoard(int isDrawRect);					// ���� �� �׸���
void EraseGameBoard();								// ���� �� �����
void DrawGameStats();								// ������ ���� ��Ȳ (���ھ�, ����) ǥ��
void DrawRect(int x, int y, int width, int height);	// ���� ���� �簢�� ���� �׸���

int GetPlayerContainsEnemy();						// �÷��̾ ���� �浹�ߴ��� Ȯ�ΰ� ��ȯ
Vector2 GetBoardPosition(Vector2 pos);				// ���� �� �� ������Ʈ�� ��ġ�� �ܼ� â �������� ������ ��ġ�� ��ȯ

void main()
{
	OnIntro();
	do
	{
		OnGame();
		OnGameOver();
	} while (OnResult());
}

void gotoxy(int x, int y)
{
	COORD pos = { 2 * x, y };						// x,y�� ��, ������ �и�
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

int isKeyDown(int key)
{
	return ((GetAsyncKeyState(key) & 0x8000) != 0);	// Ű�� ������ �ʴٰ� �ش� ��Ȳ���� ���� ���� �ν�
}	//�񵿱����� Ű�� ���¸� �����´�. // 0x8000 : �� ���ȴٰ� ȣ�� �������� ���� ����

void OnIntro()
{
	printf("\n");
	printf("    ����    ��      ��    ����    ������  �����        �����     ����    ��          ��  ������\n");
	printf("   ��    ��   ��      ��   ��    ��       ��      ��     ��      ��      ��   ��    ��   ���      ���  ��\n");
	printf("  ��      ��  ��      ��  ��      ��      ��      ��      ��    ��           ��      ��  �� ��    �� ��  ��\n");
	printf("  ������   ��    ��   ��      ��      ��      ��      ��    ��   ����  ������  ��  ��  ��  ��  ������\n");
	printf("  ��      ��    ��  ��    ��      ��      ��      ��      ��    ��       ��  ��      ��  ��   ���   ��  ��\n");
	printf("  ��      ��     ���      ��    ��       ��      ��     ��      ��      ��  ��      ��  ��    ��    ��  ��\n");
	printf("  ��      ��      ��        ����    ������  �����        ���� ��  ��      ��  ��          ��  ������\n\n");
	printf("   ��濡�� �������\n");
	printf("   ��ֹ�(��)���� ���ϸ鼭\n");
	printf("   �ִ��� ���� ��Ƴ���!\n\n");
	printf("   ���۹� : ����Ű\n\n");
	printf("  Press Any Key To START");

	getch();

	system("cls");
}

void OnGame()
{
	int i;

	srand((unsigned int)time(NULL));

	score = 0;
	level = 1;

	player.pos.x = boardSize.x / 2;
	player.pos.y = boardSize.y / 2;

	for (i = 0; i < ENEMY_SIZE; i++)
		enemys[i].isAlive = FALSE;

	system("cls");


	DrawGameBoard(TRUE);

	while (!GetPlayerContainsEnemy())
	{
		Sleep(1000 * deltaTime);
		EraseGameBoard();

		MovePlayer();

		MoveEnemy();
		DeleteEnemy();
		CreateEnemy();

		score += 1;
		level = (score / 100) + 1;

		DrawGameBoard(FALSE);
		DrawGameStats();

		gotoxy(0, boardPosition.y + boardSize.y + 2);
	}
}

void OnGameOver()
{
	int i;

	gotoxy(boardPosition.x + boardSize.x / 2 - 1, boardPosition.y + boardSize.y / 2 + 1);
	printf("GAME OVER");
	Sleep(1000);
	system("cls");
}

int OnResult()
{
	char c;
	printf("\n");
	printf("\n");
	printf("============================================\n");
	printf("       ����� ������ %05d���Դϴ�. \n", score);
	printf("============================================\n");
	printf("\n");
	printf("\n");
	printf("�ٽ� �Ͻðڽ��ϱ�? (Y/N)\n");

	do
	{
		c = getchar();
		flushall();

		switch (c)
		{
		case 'Y':
			return TRUE;
		case 'y':
			return TRUE;

		case 'N':
			return FALSE;
		case 'n':
			return FALSE;
		}

	} while (TRUE);
}

void MovePlayer()
{
	if (isKeyDown(VK_UP))
		player.pos.y += 1;
	if (isKeyDown(VK_DOWN))
		player.pos.y -= 1;
	if (isKeyDown(VK_LEFT))
		player.pos.x -= 1;
	if (isKeyDown(VK_RIGHT))
		player.pos.x += 1;


	if (player.pos.x < 0)
		player.pos.x = 0;
	if (player.pos.x > boardSize.x - 1)
		player.pos.x = boardSize.x - 1;
	if (player.pos.y < 0)
		player.pos.y = 0;
	if (player.pos.y > boardSize.y - 1)
		player.pos.y = boardSize.y - 1;
}

void CreateEnemy()
{
	static float enemySpawnCount = 0.0f;
	enemySpawnCount += 0.2f + (level * 0.02f);

	for (; enemySpawnCount >= 1.0f; enemySpawnCount -= 1.0f)
	{
		for (int i = 0; i < ENEMY_SIZE; i++)
		{
			if (!enemys[i].isAlive)
			{
				enemys[i] = InitEnemy();

				break;
			}
		}
	}
}

Enemy InitEnemy()
{
	Enemy enemy;

	enemy.isAlive = TRUE;

	switch (rand() % 4)
	{
	case Up:
		enemy.dir = Up;

		enemy.pos.x = rand() % boardSize.x;
		enemy.pos.y = 0;
		break;

	case Down:
		enemy.dir = Down;

		enemy.pos.x = rand() % boardSize.x;
		enemy.pos.y = boardSize.y - 1;
		break;

	case Left:
		enemy.dir = Left;

		enemy.pos.x = boardSize.x - 2;
		enemy.pos.y = rand() % boardSize.y;
		break;

	case Right:
		enemy.dir = Right;

		enemy.pos.x = 0;
		enemy.pos.y = rand() % boardSize.y;
		break;

	default:
		break;
	}

	return enemy;
}

void MoveEnemy()
{
	for (int i = 0; i < ENEMY_SIZE; i++)
	{
		if (enemys[i].isAlive)
		{
			switch (enemys[i].dir)
			{
			case Up:
				enemys[i].pos.y += 1;
				break;

			case Down:
				enemys[i].pos.y -= 1;
				break;

			case Left:
				enemys[i].pos.x -= 1;
				break;

			case Right:
				enemys[i].pos.x += 1;
				break;

			default:
				break;
			}
		}
	}
}

void DeleteEnemy()
{
	int i;

	for (i = 0; i < ENEMY_SIZE; i++)
	{
		if (enemys[i].isAlive)
		{
			switch (enemys[i].dir)
			{
			case Up:
				if (enemys[i].pos.y > boardSize.y - 1)
					enemys[i].isAlive = FALSE;
				break;

			case Down:
				if (enemys[i].pos.y < 0)
					enemys[i].isAlive = FALSE;
				break;

			case Left:
				if (enemys[i].pos.x < 0)
					enemys[i].isAlive = FALSE;
				break;

			case Right:
				if (enemys[i].pos.x > boardSize.x - 2)
					enemys[i].isAlive = FALSE;
				break;

			default:
				break;
			}
		}
	}
}

void DrawGameBoard(int isDrawRect)
{
	int i;

	if (isDrawRect)
		DrawRect(boardPosition.x, boardPosition.y, boardSize.x + 2, boardSize.y + 2);

	gotoxy(GetBoardPosition(player.pos).x, GetBoardPosition(player.pos).y);
	printf("%c%c", 0xA1, 0xDC);

	for (i = 0; i < ENEMY_SIZE; i++)
	{
		if (enemys[i].isAlive)
		{
			gotoxy(GetBoardPosition(enemys[i].pos).x, GetBoardPosition(enemys[i].pos).y);

			switch (enemys[i].dir)
			{
			case Up:
				printf("%c%c", 0xA1, 0xE3);
				break;

			case Down:
				printf("%c%c", 0xA1, 0xE5);
				break;

			case Left:
				printf("%c%c", 0xA2, 0xB8);
				break;

			case Right:
				printf("%c%c", 0xA2, 0xBA);
				break;

			default:
				break;
			}
		}
	}
}

void EraseGameBoard()
{
	int i;

	gotoxy(GetBoardPosition(player.pos).x, GetBoardPosition(player.pos).y);
	if (GetBoardPosition(player.pos).x == 0)
		printf("%c%c", 0xA6, 0xA2);
	else if (GetBoardPosition(player.pos).x == 20)
		printf(" %c%c", 0xA6, 0xA2);
	else
		printf("  ");

	for (i = 0; i < ENEMY_SIZE; i++)
	{
		if (enemys[i].isAlive)
		{
			gotoxy(GetBoardPosition(enemys[i].pos).x, GetBoardPosition(enemys[i].pos).y);
			if (GetBoardPosition(enemys[i].pos).x == 0)
				printf("%c%c", 0xA6, 0xA2);
			else if (GetBoardPosition(enemys[i].pos).x == 20)
				printf(" %c%c", 0xA6, 0xA2);
			else
				printf("  ");
		}
	}
}

void DrawGameStats()
{
	int writeLineX = boardPosition.x + boardSize.x + 2;

	gotoxy(writeLineX, 1);
	printf("%c%cSCORE%c%c", 0xA6, 0xA2, 0xA6, 0xA2);
	gotoxy(writeLineX, 2);
	printf("%c%c%05d%c%c", 0xA6, 0xA2, score, 0xA6, 0xA2);

	gotoxy(writeLineX, 4);
	printf("LV.%d", level);
}

void DrawRect(int x, int y, int width, int height)
{
	int i, j;

	if ((x < 0 || y < 0) || (width < 2 || height < 2))
		return;

	gotoxy(x, y);

	printf("%c%c", 0xA6, 0xA3);
	for (i = 0; i < width + 18; i++)
		printf("%c%c", 0xA6, 0xA1);
	printf("%c%c", 0xA6, 0xA4);

	for (i = 0; i < height - 2; i++)
	{
		gotoxy(x, y + i + 1);

		printf("%c%c", 0xA6, 0xA2);
		for (j = 0; j < width - 2; j++)
			printf("  ");
		printf("%c%c", 0xA6, 0xA2);
	}

	gotoxy(x, y + i + 1);

	printf("%c%c", 0xA6, 0xA6);
	for (i = 0; i < width + 18; i++)
		printf("%c%c", 0xA6, 0xA1);
	printf("%c%c", 0xA6, 0xA5);
}

int GetPlayerContainsEnemy()
{
	int i;

	for (i = 0; i < ENEMY_SIZE; i++)
	{
		if (enemys[i].isAlive)
		{
			if ((player.pos.x == enemys[i].pos.x) && (player.pos.y == enemys[i].pos.y))
				return TRUE;
		}
	}

	return FALSE;
}

Vector2 GetBoardPosition(Vector2 pos)
{
	Vector2 result;

	result.x = pos.x + boardPosition.x + 1;
	result.y = boardPosition.y + boardSize.y - pos.y;

	return result;
}