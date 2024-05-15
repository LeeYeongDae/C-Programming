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
Direction;	// 적들의 이동 방향에 대한 열거형


// Structs

typedef struct
{
	int x;
	int y;
}
Vector2;	// 오브젝트의 위치나 사각형의 크기 등에 적용할 수 있는 2개의 값을 가지는 구조체

typedef struct
{
	Vector2 pos;
}
Player;		// 플레이어 구조체

typedef struct
{
	int isAlive;

	Vector2 pos;
	Direction dir;
}
Enemy;		// 적 구조체

// Variables

Vector2 boardPosition = { 0, 1 };	// 게임 판의 위치
Vector2 boardSize = { 20, 20 };		// 게임 판의 크기

Player player;						// 플레이어
Enemy enemys[ENEMY_SIZE];			// 적

const float deltaTime = 0.1f;		// 게임 플레이 중 업데이트 되는 프레임의 간격 시간
int score;							// 점수
int level;							// 레벨 (오를수록 난이도 증가)


// Functions

void gotoxy(int x, int y);	// gotoxy
int isKeyDown(int key);		// 인자로 받은 키값이 눌려젔는지 확인값 반환 //getch

void OnIntro();				// 게임 시작 전 타이틀과 조작법 표시
void OnGame();				// 게임 실행
void OnGameOver();			// 게임 오버
int OnResult();				// 게임 오버 이후 결과창 출력

void MovePlayer();			// 플레이어 이동
void CreateEnemy();			// 적 생성
Enemy InitEnemy();			// 적이 생성될 때마다 적의 정보 초기화
void MoveEnemy();			// 적 이동
void DeleteEnemy();			// 적이 경계에 도달할 시 제거

void DrawGameBoard(int isDrawRect);					// 게임 판 그리기
void EraseGameBoard();								// 게임 판 지우기
void DrawGameStats();								// 게임의 현재 상황 (스코어, 레벨) 표시
void DrawRect(int x, int y, int width, int height);	// 게임 판의 사각형 영역 그리기

int GetPlayerContainsEnemy();						// 플레이어가 적과 충돌했는지 확인값 반환
Vector2 GetBoardPosition(Vector2 pos);				// 게임 판 안 오브젝트의 위치를 콘솔 창 기준으로 변경한 위치값 반환

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
	COORD pos = { 2 * x, y };						// x,y를 열, 행으로 분리
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

int isKeyDown(int key)
{
	return ((GetAsyncKeyState(key) & 0x8000) != 0);	// 키가 누르지 않다가 해당 상황에서 누른 경우로 인식
}	//비동기중인 키의 상태를 가져온다. // 0x8000 : 안 눌렸다가 호출 시점에서 눌린 상태

void OnIntro()
{
	printf("\n");
	printf("    ■■■    ■      ■    ■■■    ■■■■■  ■■■■        ■■■■     ■■■    ■          ■  ■■■■■\n");
	printf("   ■    ■   ■      ■   ■    ■       ■      ■     ■      ■      ■   ■    ■   ■■      ■■  ■\n");
	printf("  ■      ■  ■      ■  ■      ■      ■      ■      ■    ■           ■      ■  ■ ■    ■ ■  ■\n");
	printf("  ■■■■■   ■    ■   ■      ■      ■      ■      ■    ■   ■■■  ■■■■■  ■  ■  ■  ■  ■■■■■\n");
	printf("  ■      ■    ■  ■    ■      ■      ■      ■      ■    ■       ■  ■      ■  ■   ■■   ■  ■\n");
	printf("  ■      ■     ■■      ■    ■       ■      ■     ■      ■      ■  ■      ■  ■    ■    ■  ■\n");
	printf("  ■      ■      ■        ■■■    ■■■■■  ■■■■        ■■■ ■  ■      ■  ■          ■  ■■■■■\n\n");
	printf("   사방에서 날라오는\n");
	printf("   장애물(▲)들을 피하면서\n");
	printf("   최대한 오래 살아남자!\n\n");
	printf("   조작법 : 방향키\n\n");
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
	printf("       당신의 점수는 %05d점입니다. \n", score);
	printf("============================================\n");
	printf("\n");
	printf("\n");
	printf("다시 하시겠습니까? (Y/N)\n");

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