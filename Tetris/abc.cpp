
//����1��һ������һ�У�����һ�߾�����һ�С�
//����2�����߶������ظ�������Ϸ���ܼ�¼��߷֡�
//�������ܣ��������֣�������ͣ������
#include <conio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include "colorConsole.h"     

#include <WINDOWS.H>
#include <mmsystem.h>
#pragma comment(lib, "Winmm.lib")

#define KEY_UP      56
#define KEY_DOWN    53
#define KEY_LEFT    52
#define KEY_RIGHT   54
#define KEY_ESC     27

#define IKEY_UP      'w'
#define IKEY_DOWN    's'
#define IKEY_LEFT    'a'
#define IKEY_RIGHT   'd'
#define IKEY_PAUSE   'p'

#define MAPW    12     //��ͼ�Ŀ��
#define MAPH    20     //��ͼ�ĸ߶�

void Initall();//��ʼ��
void Init();//��ʼ���߳�1
void Init1();//��ʼ���߳�2
void Turn(int a[][4],int w,int h,int *x,int y,int MAP[][MAPW]);    //����ת��
bool IsAvailable(int a[],int x,int y,int w,int h, int MAP[][MAPW]); //�ж��Ƿ��ܷ���
void DrawBlocks(int a[], int w, int h, int x, int y, WORD wColors[], int nColors, int p, int q);//������
void ClearSquare(int *a, int w, int h, int x, int y, int p, int q);//�������
void GameOver();//�߳�1����
void GameOver1();//�߳�2����
void clearThreadFunc();//�߳�1���������������
void clearThreadFunc1();//�߳�2���������������
void DeleteLine(int m[][MAPW], int row, int p, int q);  //����һ��
void AddLine(int m[][MAPW], int row, int p, int q);  //����һ��
DWORD WINAPI ThreadFunc(LPVOID lpParam);//�߳�1
DWORD WINAPI ThreadFunc1(LPVOID lpParam1);//�߳�2
//7�ֲ�ͬ��״�ķ���
int b[7][4][4]={	{{1},{1,1,1}},
					{{0,2},{2,2,2}},
					{{3,3},{0,3,3}},
					{{0,0,4},{4,4,4}},
					{{0,5,5},{5,5}},
                    {{6,6,6,6}},
                    {{7,7},{7,7}}
                };
WORD SQUARE_COLOR[7] ={ FOREGROUND_RED|FOREGROUND_INTENSITY,
                        FOREGROUND_GREEN|FOREGROUND_INTENSITY,
						FOREGROUND_BLUE|FOREGROUND_INTENSITY,
						FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_INTENSITY,
						FOREGROUND_RED|FOREGROUND_BLUE|FOREGROUND_INTENSITY,
						FOREGROUND_GREEN|FOREGROUND_BLUE|FOREGROUND_INTENSITY,
						FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE|FOREGROUND_INTENSITY
					 };		
int map[MAPH][MAPW] = { 0 }; //���湤����������1
int map1[MAPH][MAPW] = { 0 }; //���湤����������2
HANDLE handle;
int dx = 12 ; //��ʼ����Ļʱ��ʼ����
int dy = 3;
int dm = 44;
int dn = 3;
int maxscore = -1;//��ʼ����߷�
int maxscore1 = -1;
DWORD dwThreadId;
HANDLE hThread;
HANDLE hThread1;
CRITICAL_SECTION cs;
int trytime1=1,trytime = 1;//��ʼ����Ϸ���д���
int mode = 0;//��ʼ����Ϸ��˫��ģʽ
int level_twin = 0;//��ʼ����Ϸ����
int dmode = 0;//��ʼ��˫��ģʽ��һ��ģʽ
int life = 3;//��ʼ����սģʽ������ֵ
int life1 = 3;

int main()
{
	Initall();
	InitializeCriticalSection(&cs);
	if (mode == 2)//����˫��ģʽ
	{
		hThread = CreateThread(NULL, 0, ThreadFunc, NULL, 0, &dwThreadId);
		hThread1 = CreateThread(NULL, 0, ThreadFunc1, NULL, 0, &dwThreadId);
	}
	if (mode==1)//���뵥��ģʽ
		hThread = CreateThread(NULL, 0, ThreadFunc, NULL, 0, &dwThreadId);
	while (1)//���ű�������
	{
		PlaySound("F:\\Beat It_Michael Jackson (1).wav", NULL, SND_LOOP);
	}
}

void Initall()
{
	handle = initiate();
	srand(time(NULL));
	WORD wColors[1] = { FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY };
	wColors[0] = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
	textout(handle, 25, 10, wColors, 1, "Press any key to have fun!");
	int ch = _getch();//ѡ����Ϸ��ʼ�ȼ�1��9
	textout(handle, 25, 10, wColors, 1, "                          ");
	textout(handle, 27, 10, wColors, 1, "Select Start Level:1~9");
    ch1_s:int ch1 = _getch();
	if ((ch1 >= 49) && (ch1<=57))
	level_twin = ch1 - 48;
	else goto ch1_s;
	textout(handle, 27, 10, wColors, 1, "                      ");
	textout(handle, 15, 10, wColors, 1, "Select Game Mode:1.single player 2.double players");
	ch2_s:int ch2 = _getch();//ѡ��˫��ģʽ
	if (ch2 == 49) mode = 1;
	else if (ch2 == 50) mode = 2;
	else goto ch2_s;
	textout(handle, 15, 10, wColors, 1, "                                                  ");
	if (ch2 == 50)
	{
		textout(handle, 22, 10, wColors, 1, "1.�޾�ģʽ 2.��սģʽ(���λ���)");//��˫��ģʽ��ѡ����һ��ģʽ
	ch3_s:int ch3 = _getch();
		if (ch3 == 49) dmode = 1;
		else if (ch3 == 50) dmode = 2;
		else goto ch3_s;
		textout(handle, 22, 10, wColors, 1, "                               ");
	}
	textout(handle, 34, 10, wColors, 1, "HAPPY TETRIS!");//��ʼ��Ϸ
	_sleep(1000);
	textout(handle, 34, 10, wColors, 1, "             ");
}

void Init()
{
	handle=initiate();
	for(int i=0;i<20;i++)//��ʼ��������
	{
		map1[i][0]=-2;
		for (int j = 1; j < 11; j++)
		{
			map1[i][j] = 0;
		}
		map1[i][11]=-2;
	}
	for(int i=0;i<12;i++)
	{
		map1[0][i] = -1;
		map1[19][i]=-1;
	}
	map1[0][0]=-3;
	map1[0][11]=-3;
	map1[19][0]=-3;
	map1[19][11] = -3;
	WORD wColors[1]={FOREGROUND_RED| FOREGROUND_GREEN|FOREGROUND_INTENSITY};//��ʼ����ʼ����
	textout(handle, dm + 27, 16 + dn, wColors, 1, "TRYTIME");
	textout(handle, dm + 27, 1 + dn, wColors, 1, "PLAYER2");
	textout(handle,dm+27,3 + dn, wColors, 1, "SCORE");
	textout(handle,dm+27,7 + dn, wColors, 1, "LEVEL");
	textout(handle,dm+27,11+ dn, wColors, 1, "NEXT");
	wColors[0]=FOREGROUND_RED|FOREGROUND_BLUE|FOREGROUND_INTENSITY;
	DrawBlocks(&map1[0][0],12,20,0,0,wColors,1,dm,dn);
	textout(handle,dm,dn,wColors, 1, "��T�T�T�T�T�T�T�T�T�T��");
	wColors[0]=FOREGROUND_RED| FOREGROUND_GREEN|FOREGROUND_INTENSITY;
	textout(handle, dm , 0, wColors, 1, "MAXSCORE��");
}

void Init1()
{
	handle = initiate();
	for (int i = 0; i<20; i++)//��ʼ��������
	{
		map[i][0] = -2;
	
		for (int j = 1; j < 11; j++)
		{
			map[i][j] = 0;
		
		}
		map[i][11] = -2;
	
	}
	for (int i = 0; i<12; i++)
	{
		map[0][i] = -1;
		
		map[19][i] = -1;
		
	}
	map[0][0] = -3;
	map[0][11] = -3;
	map[19][0] = -3;
	map[19][11] = -3;
	
	WORD wColors[1] = { FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY };//��ʼ����Ϸ��ʼ����
	textout(handle, dx - 8, 1 + dy, wColors, 1, "PLAYER1");
	textout(handle, dx - 8, 3 + dy, wColors, 1, "SCORE");
	textout(handle, dx - 8, 7 + dy, wColors, 1, "LEVEL");
	textout(handle, dx - 8, 11 + dy, wColors, 1, "NEXT");
	textout(handle, dx - 8, 16 + dy, wColors, 1, "TRYTIME");

	wColors[0] = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
	DrawBlocks(&map[0][0], 12, 20, 0, 0, wColors, 1, dx, dy);
	textout(handle, dx, dy, wColors, 1, "��T�T�T�T�T�T�T�T�T�T��");
	wColors[0] = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
	textout(handle, dx - 8, 0, wColors, 1, "MAXSCORE��");
}

bool IsAvailable(int a[],int x,int y,int w,int h,int MAP[][MAPW])
{
	for(int i=y;i<y+h;i++)
		for (int j = x;j < x + w;j++)
			if (MAP[i][j] && a[w*(i - y) + j - x])
				return 0;
					
	return 1;
}

void DrawBlocks(int a[],int w,int h,int x,int y,WORD wColors[],int nColors,int p,int q)
{
	int temp;
	for(int i=0;i<h;i++)
		for (int j = 0; j < w; j++)
		
			if ((temp = a[i*w + j]) && y + i > 0)
			{
				if (temp == -3)
					textout(handle, 2 * (x + j) + p, y + i + q, wColors, nColors, "��");
				else if (temp == -2)
					textout(handle, 2 * (x + j) + p, y + i + q, wColors, nColors, "�U");
				else if (temp == -1)
					textout(handle, 2 * (x + j) + p, y + i + q, wColors, nColors, "�T");
				else if (temp >= 1)
					textout(handle, 2 * (x + j) + p, y + i + q, wColors, nColors, "��");
			}	
}

void Turn(int a[][4],int w,int h,int *x,int y,int MAP[][MAPW])
{
	int b[4][4]={{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
	int sign=0,line=0;
	for(int i=h-1;i>=0;i--)
	{
		for(int j=0;j<w;j++)
			if(a[i][j])
			{
				b[j][line]=a[i][j];
				sign=1;
			}
		if(sign) 
		{
			line++;
			sign=0;
		}
	}
	for(int i=0;i<4;i++)
		if(IsAvailable(b[0],*x-i,y,w,h,MAP))
		{
			*x-=i;
			for(int k=0;k<h;k++)
				for(int j=0;j<w;j++)
					a[k][j]=b[k][j];
			break;
		}
}

void ClearSquare(int *a,int w,int h,int x,int y,int p,int q)
{
	WORD wColors[1]={0};
	for(int i=0;i<h;i++)
		for(int j=0;j<w;j++)
		{
			if(a[i*w+j]>0 && (i+y>0))
			{
				textout(handle,2*(x+j)+p,y+i+q,wColors,1,"  ");
			}
		}
}

void clearThreadFunc()
{
	WORD wColors[1] = { 0 };
	for (int i = 0; i < 33; i++)
		for (int j = 0; j < 30;j++)
			textout(handle, dm + i, dn + j, wColors, 1, "  ");
}

void clearThreadFunc1()
{
	WORD wColors[1] = { 0 };
	for (int i = -10; i < 23; i++)
		for (int j = 0; j < 30; j++)
			textout(handle, dx + i, dy + j, wColors, 1, "  ");
}

void GameOver1()
{
	WORD wColors[1]={FOREGROUND_RED| FOREGROUND_GREEN|FOREGROUND_INTENSITY};
	textout(handle,20,10,wColors,1,"GAME OVER");
	_sleep(1000);
	textout(handle, 13, 10, wColors, 1, " PRESS S TO START OVER");//����S������������Ϸ
	next:int key = _getch();
	if (key == IKEY_DOWN)
		clearThreadFunc1();
	else goto next;
	_sleep(1000);
}

void GameOver()
{
	WORD wColors[1] = { FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY };
	textout(handle, 52, 10, wColors, 1, "GAME OVER");
	_sleep(1000);
	textout(handle, 45, 10, wColors, 1, "PRESS �� TO START OVER");//���¨�������������Ϸ
	next:int key = _getch();
	if (key == KEY_DOWN)
	  clearThreadFunc();
	else goto next;
    _sleep(1000);
}

void DeleteLine(int m[][MAPW],int row, int p, int q)
{
	WORD wColors[1]={FOREGROUND_RED| FOREGROUND_GREEN|FOREGROUND_INTENSITY};
	textout(handle,2+p,row+q,wColors,1,"�k�k�k�k�k�k�k�k�k�k");
	_sleep(100);

	for(int i=row;i>1;i--)
	{
		ClearSquare(&m[i][1],MAPW-2,1,1,i,p,q);
		for(int j=1;j<MAPW-1;j++)
		{
			m[i][j]=m[i-1][j];
			if (m[i][j] == 0)
				wColors[0] = 0;
			else
				wColors[0] = SQUARE_COLOR[m[i][j]-1];
			DrawBlocks(&m[i][j],1,1,j,i,wColors,1,p,q);
		}
	}
	for(int i=1;i<MAPW-1;i++)
		m[1][i]=0;
}

void AddLine(int m[][MAPW], int row, int p, int q)
{
	WORD wColors[1] = { FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY };
	_sleep(100);
	for (int i = 1;i<MAPH-2;i++)
	{
		for (int j = 1;j<MAPW - 1;j++)
		{
			m[i][j] = m[i+1][j];//���·��Ĺ����������Ϸ�
			if (m[i][j] == 0)
			{
				WORD wColors1[1] = { 0 };
				textout(handle, 2*j + p, i + q, wColors1, 1, " ");
			}
			else
				wColors[0] = SQUARE_COLOR[m[i][j] - 1];
			DrawBlocks(&m[i][j], 1, 1, j, i, wColors, 1, p, q);
		}
	}
	for (int i = 1;i < MAPW - 1;i++)
		m[MAPH - 2][i] = 5;
	wColors[0] = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
	DrawBlocks(m[MAPH-2], 12,1, 0, MAPH-2, wColors, 1, p, q);
}

DWORD WINAPI ThreadFunc(LPVOID lpParam)
{
    next://518�д�
	Init();
	srand(time(NULL));
	int score = 0;  //��ʼ������

	int level = level_twin;  //��ʼ����Ϸ����

	int Num = rand() % 7; //������һ��������
	int nextNum = Num;    //������һ��������

	int blank;  //��¼ÿ��������ʼλ��
	int x = 0, y = 0;  //��¼��Ϸ��ʼ���������

	int a1[4][4] = { 0 }; //��ʱʹ���������浱ǰ����


	while (1)
	{
		for (int i = 0;i<4;i++)          //���Ʒ���
			for (int j = 0;j<4;j++)
				if (a1[i][j] = b[nextNum][i][j])
					blank = i;

		y = 1 - blank;
		x = 4;

		//������һ������
		Num = nextNum;
		ClearSquare(b[Num][0], 4, 4, 13, 13,dm,dn);
		nextNum = rand() % 7;
		WORD wColors[1] = { SQUARE_COLOR[nextNum] };
		DrawBlocks(b[nextNum][0], 4, 4, 13, 13, wColors, 1,dm,dn);

		wColors[0] = SQUARE_COLOR[Num];
		DrawBlocks(&a1[0][0], 4, 4, x, y, wColors, 1,dm,dn);

		//��ʾ������Ϣ
		char string[5];
		wColors[0] = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		textout(handle, dm + 27, 18 + dn, wColors, 1, itoa(trytime, string, 10));
		textout(handle, dm+27, 5 + dy, wColors, 1, itoa(score, string, 10));
		textout(handle, dm+27, 9 + dy, wColors, 1, itoa(level, string, 10));

		int max_delay = 100 - 10 * level; //���㲻ͬ��Ϸ���������ʱ����

		while (1)
		{
			int delay = 0; //�ӳ���
			while (delay<max_delay)
			{
				if (_kbhit())  //��if���ⰴס��ʹ���鿨ס
				{
					EnterCriticalSection(&cs);
					int key = _getch();
					switch (key)
					{
					case KEY_UP:
					{
						ClearSquare(&a1[0][0], 4, 4, x, y, dm, dn);
						Turn(a1, 4, 4, &x, y,map1);
						wColors[0] = SQUARE_COLOR[Num];
						DrawBlocks(&a1[0][0], 4, 4, x, y, wColors, 1,dm,dn);
					}
					break;
					case KEY_DOWN:
						delay = max_delay;
						break;
					case KEY_LEFT:
					{
						if (IsAvailable(&a1[0][0], x - 1, y, 4, 4,map1))
						{
							ClearSquare(&a1[0][0], 4, 4, x, y, dm, dn);
							x--;
							wColors[0] = SQUARE_COLOR[Num];
							DrawBlocks(&a1[0][0], 4, 4, x, y, wColors, 1,dm,dn);
						}
					}
					break;
					case KEY_RIGHT:
					{
						if (IsAvailable(&a1[0][0], x + 1, y, 4, 4, map1))
						{
							ClearSquare(&a1[0][0], 4, 4, x, y, dm, dn);
							x++;
							wColors[0] = SQUARE_COLOR[Num];
							DrawBlocks(&a1[0][0], 4, 4, x, y, wColors, 1,dm,dn);
						}
					}
					break;
					case IKEY_PAUSE://��ͣ
					{
					next1:int k = _getch();//ָ��465��
						if (k == IKEY_PAUSE) break;
						else goto next1;////goto 463�У�ֱ���ٰ�����ͣ�ż�����Ϸ
					}
					case KEY_ESC:
						exit(EXIT_SUCCESS);
						break;
					}
					LeaveCriticalSection(&cs);

				}
				_sleep(4);delay++;
			}
			if (IsAvailable(&a1[0][0], x, y + 1, 4, 4, map1)) //�Ƿ�������
			{
				ClearSquare(&a1[0][0], 4, 4, x, y, dm, dn);
				y++;
				wColors[0] = SQUARE_COLOR[Num];
				DrawBlocks(&a1[0][0], 4, 4, x, y, wColors, 1,dm,dn);
			}
			else
			{
				if (y <= 1)
				{
					if (score > maxscore)//������߷ֲ���ʾ
					{
						maxscore = score;
						char string[5];
						wColors[0] = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
						textout(handle, dm + 15, 0, wColors, 1, itoa(maxscore, string, 10));
					}
					if (mode == 2)//����˫��ģʽ
					{
						if (dmode==2)//�����սģʽ
						life--;
						if (life == 0)//�ж϶�սģʽ�и������Ϸ����
						{
							clearThreadFunc();
							char string[5];
						end:wColors[0] = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;//513��
							textout(handle, dm, 8, wColors, 1, "PLAYER 2");
							textout(handle, dm, 10, wColors, 1, "FINALSCORE��");
							textout(handle, dm + 15, 10, wColors, 1, itoa(maxscore, string, 10));
							textout(handle, dm, 0, wColors, 1, "                           ");
							if (life1 == 0)//�ж϶�սģʽ��Ϸ����
							{
							    int end = _getch();
								if (end == KEY_ESC)
									exit(EXIT_SUCCESS);
							}
							goto end;//502�У�ѭ����ӡ����
						}
					}
					GameOver();
					trytime++;//������Ϸ����
					goto next;//372�д���������һ����Ϸ
				}     //�Ƿ����
				for (int i = 0;i<4;i++)     //���·��飬���¹�����
					for (int j = 0;j<4;j++)
						if (a1[i][j] && ((i + y)<MAPH - 1) && ((j + x)<MAPW - 1))
							map1[i + y][j + x] = a1[i][j];

				int full, c, k = 0;
				for (int i = 1; i< MAPH - 1; i++)
				{
					full = 1;  c = 1;
					for (int j = 1; j<11; j++)
						if (map1[i][j] == 5) c++;
					if (c == MAPW - 1) c = 0;//��֤�����ӵ��в�������
					for (int j = 1; j<11; j++)
						if (!map1[i][j] || !c) full = 0;
					if (full)   //����һ��
					{
						DeleteLine(map1, i,dm,dn);
						if ((mode==2)&&(life1!=0))
						AddLine(map, i, dx, dy);
						k++;
						score = score + k;
						level = min(score / 30, 9);
						max_delay = 100 - 10 * level;
					}
				}
				break;
			}
		}
	}
}

DWORD WINAPI ThreadFunc1(LPVOID lpParam1)
{
	next://ָ��724��
	Init1();
	srand(time(NULL)+1);
	int score = 0;  //��ʼ������
	int level = level_twin;  //��ʼ����Ϸ����

	int Num = rand() % 7; //������һ��������
	int nextNum = Num;    //������һ��������

	int blank;  //��¼ÿ��������ʼλ��
	int x = 0, y = 0;  //��¼��Ϸ��ʼ���������

	int a[4][4] = { 0 }; //��ʱʹ���������浱ǰ����


	while (1)
	{
		for (int i = 0; i<4; i++)          //���Ʒ���
			for (int j = 0; j<4; j++)
				if (a[i][j] = b[nextNum][i][j])
					blank = i;

		y = 1 - blank;
		x = 4;

		//������һ������
		Num = nextNum;
		ClearSquare(b[Num][0], 4, 4, 13, 13, dx - 36, dy);
		nextNum = rand() % 7;
		WORD wColors[1] = { SQUARE_COLOR[nextNum] };
		DrawBlocks(b[nextNum][0], 4, 4, 13, 13, wColors, 1, dx - 36, dy);

		wColors[0] = SQUARE_COLOR[Num];
		DrawBlocks(&a[0][0], 4, 4, x, y, wColors, 1, dx, dy);

		//��ʾ������Ϣ
		char string[5];
		wColors[0] = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		textout(handle, dx - 8, 18 + dy, wColors, 1, itoa(trytime1, string, 10));
		textout(handle, dx - 8, 5 + dy, wColors, 1, itoa(score, string, 10));
		textout(handle, dx - 8, 9 + dy, wColors, 1, itoa(level, string, 10));

		int max_delay = 100 - 10 * level; //���㲻ͬ��Ϸ���������ʱ����

		while (1)
		{
			int delay = 0; //�ӳ���
			while (delay<max_delay)
			{
				if (_kbhit())  //��if���ⰴס��ʹ���鿨ס
				{
					EnterCriticalSection(&cs);
					int key = _getch();
					switch (key)
					{
					case IKEY_UP:
					{
						ClearSquare(&a[0][0], 4, 4, x, y, dx, dy);
						Turn(a, 4, 4, &x, y, map);
						wColors[0] = SQUARE_COLOR[Num];
						DrawBlocks(&a[0][0], 4, 4, x, y, wColors, 1, dx, dy);
					}
					break;
					case IKEY_DOWN:
						delay = max_delay;
						break;
					case IKEY_LEFT:
					{
						if (IsAvailable(&a[0][0], x - 1, y, 4, 4, map))
						{
							ClearSquare(&a[0][0], 4, 4, x, y, dx, dy);
							x--;
							wColors[0] = SQUARE_COLOR[Num];
							DrawBlocks(&a[0][0], 4, 4, x, y, wColors, 1, dx, dy);
						}
					}
					break;
					case IKEY_RIGHT:
					{
						if (IsAvailable(&a[0][0], x + 1, y, 4, 4, map))
						{
							ClearSquare(&a[0][0], 4, 4, x, y, dx, dy);
							x++;
							wColors[0] = SQUARE_COLOR[Num];
							DrawBlocks(&a[0][0], 4, 4, x, y, wColors, 1, dx, dy);
						}
					}
					break;
					case IKEY_PAUSE://��ͣ
					{
					next1:int k = _getch();//ָ��645��
						if (k == IKEY_PAUSE) break;
						else goto next1;//goto 643�У�ֱ���ٰ�����ͣ�ż�����Ϸ
					}
					case KEY_ESC:
						exit(EXIT_SUCCESS);
						break;
					}
					LeaveCriticalSection(&cs);

				}
				_sleep(4); delay++;
			}
			if (IsAvailable(&a[0][0], x, y + 1, 4, 4, map)) //�Ƿ�������
			{
				ClearSquare(&a[0][0], 4, 4, x, y, dx, dy);
				y++;
				wColors[0] = SQUARE_COLOR[Num];
				DrawBlocks(&a[0][0], 4, 4, x, y, wColors, 1, dx, dy);
			}
			else
			{
				if (y <= 1)
				{
					if (score > maxscore1)//������߷ֲ���ʾ
					{
						maxscore1 = score;
						char string[5];
						wColors[0] = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
						textout(handle, dx + 7, 0, wColors, 1, itoa(maxscore1, string, 10));
					}
					if (dmode==2)//�жϽ����սģʽ
					life1--;
					if (life1 == 0)//�ж϶�սģʽ�и������Ϸ����
					{
						clearThreadFunc1();
						char string[5];
						wColors[0] = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
					end://ָ��720��
						textout(handle, dx, 8, wColors, 1, "PLAYER 1");
						textout(handle, dx, 10, wColors, 1, "FINALSCORE��");
						textout(handle, dx + 15, 10, wColors, 1, itoa(maxscore1, string, 10));
						textout(handle, dx - 8, 0, wColors, 1, "                              ");
						_sleep(1000);
						if (life == 0)//�ж϶�ս��Ϸ����
						{
							textout(handle, dx, 8, wColors, 1, "                    ");
							textout(handle, dx, 10, wColors, 1, "                           ");
							textout(handle, dm, 8, wColors, 1, "                     ");
							textout(handle, dm, 10, wColors, 1, "                           ");
							_sleep(1000);
							//��ʾʤ�������
							if (maxscore > maxscore1)
							{
							p1:textout(handle, 20, 10, wColors, 1, "PLAYER 2 is the winner!Congratulations!");//ָ��701��
								int end = _getch();
								if (end == KEY_ESC)
									exit(EXIT_SUCCESS); 
								else goto p1;//goto 697�У��������˳����˳���Ϸ
							}
							else if (maxscore == maxscore1)
							{
							p2:textout(handle, 20, 10, wColors, 1, "Both players are winners!Congratulations!");//ָ��709��
								int end = _getch();
							if (end == KEY_ESC)
								exit(EXIT_SUCCESS); 
							else goto p2;//goto 705��
							}
							else
							{
							p3:textout(handle, 20, 10, wColors, 1, "PLAYER 1 is the winner!Congratulations!");//ָ��717��
								int end = _getch();		
							if (end == KEY_ESC)
								exit(EXIT_SUCCESS); 
							else goto p3;//goto 713��
							}
						}
						 goto end;//goto 681�У�ѭ����ӡ��Ϸ���
					}
					GameOver1();     //�Ƿ����
					trytime1++;
					goto next;//goto 553�У��ظ�������Ϸ
				
			    }
				for (int i = 0; i<4; i++)     //���·��飬���¹�����
					for (int j = 0; j<4; j++)
						if (a[i][j] && ((i + y)<MAPH - 1) && ((j + x)<MAPW - 1))
							map[i + y][j + x] = a[i][j];

				int full, c, k = 0;
				for (int i = 1; i< MAPH - 1; i++)
				{
					full = 1;  c = 1;
					for (int j = 1; j<11; j++)//ʹ�����ӵ�һ�в�������
						if (map[i][j] == 5) c++;
					if (c == MAPW - 1) c = 0;
					for (int j = 1; j<11; j++)
						if (!map[i][j] || !c) full = 0;

					if (full)   //����һ��
					{
						DeleteLine(map, i, dx, dy);
						if (life!=0)
						AddLine(map1, i, dm, dn);
						k++;
						score = score + k;
						level = min(score / 30, 9);
						max_delay = 100 - 10 * level;
					}
				}
				break;
			}
		}
	}
}


