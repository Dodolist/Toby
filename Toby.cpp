#include <Turboc.h>

#define BLUE 9
#define PINK 13
#define YELLOW 14
#define WHITE 15

int hour=0;
int minute=0;
int second=0;

int face=0;

void setcolor(unsigned short text, unsigned short back)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), text | (back << 4));
}
void cursor(int n) // 커서 보이기 1 & 숨기기 0
{
    HANDLE hConsole;
    CONSOLE_CURSOR_INFO ConsoleCursor;
 
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
 
    ConsoleCursor.bVisible = n;
    ConsoleCursor.dwSize = 1;
 
    SetConsoleCursorInfo(hConsole , &ConsoleCursor);
}
void conversation1(int rd, char name_[]);
void conversation2(int rd, char name_[]);
void interfaced(char name_[], int level, int favor, int hungry);
void info();
void face1();
void face2();
void face3();
void face4();
void timer();



int main(void)
{
	system("title PET_BY_DOAE"); // 타이틀 변경
	system("mode con: cols=80 lines=30");
	cursor(0);
	
	char name_[10]; //이름 
	char *name; //이름 포인터 
	char copy[4096]; // 카피용 문자변수
	int copy_; // 카피용 숫자변수 
	
	int first_char; // 첫문자 
	int level; // 레벨 
	int favor; // 호감도 
	int hungry; // 배고픔
	int favor_cycle; //  호감도 사이클 
	int hungry_cycle; // 배고픔 사이클
	int conversation_cycle; // 대화 사이클 
	int name_len; // 이름길이 
	int conversation_rand; //랜덤변수
	int rd; //랜덤변수
	int save=0; //자동저장 변수
	int tic=0;
	
	srand((unsigned)time(NULL));
	
	// 주소 앞 C:\\Users\\~
	FILE *data_name; //파일 포인터 변수 data을 정의 
	FILE *data_value; //파일 포인터 변수 data을 정의
	FILE *data_Backup; // 카피 파일 변수 data을 정의 
	data_name = fopen("name.txt", "r+");
	data_value = fopen("stored_value.txt", "rb");
	
	
	if(data_name == NULL)
	{
		perror("이름 파일 읽기 개방 에러");
		return 1;
	}
	if(data_value == NULL)
	{
		perror("값 파일 읽기 개방 에러");
		return 1;
	}
	
	data_value = fopen("stored_value.txt", "r+");
	
	first_char = fgetc(data_name); // 메모장의 첫 부분만 문자 읽기 
	fseek(data_name,0L,SEEK_SET); // 파일 포인터 맨 처음으로 이동
	
	//###
	
	if(first_char==-1) // 이름이 없는 경우 실행 
	{
		printf("이름을 설정해주세요: ");
		name = gets(name_); // 이름 설정 
		fprintf(data_name,"%s",name); // 파일에 이름 적어넣기
		level = 1;
		favor = 0;
		hungry=100;
		hungry_cycle=0;
		favor_cycle=0;
		conversation_cycle=0;
		info();
		interfaced(name_,level,favor,hungry);
		conversation1(11,name_);
		fprintf(data_value,"%d\n%d\n%d\n%d\n%d\n%d",level,favor,hungry,favor_cycle,hungry_cycle,conversation_cycle);
	}
	else
	{
		fgets(name_,10,data_name); // 이름이 있어서 불러옴.
		fscanf(data_value, "%d\n%d\n%d\n%d\n%d\n%d",&level,&favor,&hungry,&favor_cycle,&hungry_cycle,&conversation_cycle); // data_value에서 값을 불러옴. 
		if(level<=1) 
		{
			data_Backup = fopen("Backup.txt", "r+");
			fscanf(data_Backup, "%d\n%d\n%d\n%d\n%d\n%d",&level,&favor,&hungry,&favor_cycle,&hungry_cycle,&conversation_cycle);
			fclose(data_Backup);
		}
		if(hungry==0) hungry+=5;
		info();
		interfaced(name_,level,favor,hungry);
		conversation1(10,name_);
	}
	
	//###
	
	data_value = fopen("stored_value.txt", "w+");
	
	while(1)
	{
		if(_kbhit()) //z누르면 종료 
		{
			if(getch()=='z')
			{
				fseek(data_value,0L,SEEK_SET); // 파일 맨 처음으로 
				fprintf(data_value,"%d\n%d\n%d\n%d\n%d\n%d",level,favor,hungry,favor_cycle,hungry_cycle,conversation_cycle);
				break;	
			}
			else if(getch()=='o')
			{
				hungry+=5;
				favor++;
				if(hungry>100)
				{
					hungry=100;
					favor--;
				}
				interfaced(name_,level,favor,hungry);
				conversation_rand=rand()%3+7;
				conversation1(conversation_rand,name_);
			}
		}
		
		if(tic==3000)
		{
			second++;
			tic=0;	
		}
		else tic++;
		if(second==60)
		{
			second=0;
			minute++;
		}
		if(minute==60)
		{
			minute=0;
			hour++;
		}
		timer();
		
		if(save>=5400000) // 30분마다 자동저장
		{
			save=0;
			data_Backup = fopen("Backup.txt", "w+");
			fseek(data_Backup,0L,SEEK_SET); // 파일 맨 처음으로
			fprintf(data_Backup,"%d\n%d\n%d\n%d\n%d\n%d",level,favor,hungry,favor_cycle,hungry_cycle,conversation_cycle);
			fclose(data_Backup);
		}
		else save++;
		
		if(hungry_cycle >= 720000) // 4분마다 배고픔 1감소 
		{
			hungry_cycle=0;
			hungry--;

			interfaced(name_,level,favor,hungry);
			if(hungry>=50)
			{
				conversation_rand=rand()%7;
				conversation1(conversation_rand,name_);
				conversation_cycle=0;
			}
			else
			{
				conversation_rand=rand()%6;
				conversation2(conversation_rand,name_);
				conversation_cycle=0;
			}
		}
		else hungry_cycle+=1;
		
		if(favor_cycle>=10800000) // 60분마다 호감도 1증가
		{
			favor_cycle=0;
			favor++;
		}
		else favor_cycle++;
		
		
		if(conversation_cycle>=1080000) // 6분마다 대화
		{
			interfaced(name_,level,favor,hungry);
			
			if(hungry>=50)
			{
				conversation_rand=rand()%7;
				conversation1(conversation_rand,name_);
				conversation_cycle=0;
			}
			else
			{
				conversation_rand=rand()%6;
				conversation2(conversation_rand,name_);
				conversation_cycle=0;
			}
		}
		else conversation_cycle++;
		
		if(favor>level*level) level++;
		
		if(face==12000)
		{
			if(hungry>=50) face2();
			else face4();
			face++;
		}
		else if(face==0)
		{
			if(hungry>=50) face1();
			else face3();
			face++;
		}
		else if(face==13000)
		{
			face=0;
		}
		else face++;
		
		if(hungry==0)
		{
			system("cls");
			
			rd=rand()%2;
			if(rd==0)	printf("%s이 배고파서 폴더 속으로 돌아갔습니다.",name_);
			else printf("%s이 화가나서 폴더 속으로 돌아갔습니다.",name_);
			
			fseek(data_value,0L,SEEK_SET); // 파일 맨 처음으로
			fprintf(data_value,"%d\n%d\n%d\n%d\n%d\n%d",level,favor,hungry,favor_cycle,hungry_cycle,conversation_cycle);
			break;
		}
	}
	
	fclose(data_value);
	data_value = fopen("stored_value.txt", "rb");
	data_Backup = fopen("Backup.txt", "wb");
	
	if(data_Backup == NULL)
	{
		perror("복사 파일 쓰기 개방 에러");
		fclose(data_name);
		fclose(data_value);
		fclose(data_Backup);
		return 1;
	}
	
	while(feof(data_value) == 0)
	{
		copy_= fread(&copy, 1, 1, data_value); 
		if(ferror(data_value))
		{
			perror("값 파일 읽기 에러");
			fclose(data_name);
			fclose(data_value);
			fclose(data_Backup);
			return 1;
		}
		
		if(copy_>0)
		{
			fwrite(&copy,1,1,data_Backup);
			if(ferror(data_Backup))
			{
				perror("복사 파일 쓰기 에러");
				fclose(data_name);
				fclose(data_value);
				fclose(data_Backup);
				return 1;
			}
		}
	}
	fclose(data_Backup);	
	fclose(data_name);
	fclose(data_value);
	return 1;
}


void conversation1(int rd, char name_[])
{
	setcolor(0,0);
	gotoxy(2,24);
	printf("　　　　　　　　　　　　　　　　　　　　　　　　　　　　");
	setcolor(WHITE,0);
	switch(rd)
	{
		case 0 :
			gotoxy(2,24);
			printf("%s : 지금 공부하고 있니??",name_);
			break;
		case 1 :
			gotoxy(2,24);
			printf("%s : 딴짓 하는건 아니지??",name_);
			break;
		case 2 :
			gotoxy(2,24);
			printf("%s : Eng1",name_);
			break;
		case 3 :
			gotoxy(2,24);
			printf("%s : Eng2",name_);
			break;
		case 4 :
			gotoxy(2,24);
			printf("%s : Eng3",name_);
			break;
		case 5 :
			gotoxy(2,24);
			printf("%s : Eng4",name_);
			break;
		case 6 :
			gotoxy(2,24);
			printf("%s : 접속1",name_);
			break;
		case 7 :
			gotoxy(2,24);
			printf("%s : 접속2",name_);
			break;
		case 8 :
			gotoxy(2,24);
			printf("%s : 먹이1",name_);
			break;
		case 9 :
			gotoxy(2,24);
			printf("%s : 먹이2",name_);
			break;
		case 10 :
			gotoxy(2,24);
			printf("%s : 공부하자!",name_);
			break;
		case 11 :
			gotoxy(2,24);
			printf("%s : 공부하자!",name_);
			break;	
	}
}

void conversation2(int rd, char name_[])
{	
	setcolor(0,0);
	gotoxy(2,24);
	printf("　　　　　　　　　　　　　　　　　　　　　　　　　　　　");
	setcolor(WHITE,0);
	switch(rd)
	{
		case 0 :
			gotoxy(2,24);
			printf("%s : 아... 나 좀 배고프다...",name_);
			break;
		case 1 :
			gotoxy(2,24);
			printf("%s : 배고픔1",name_);
			break;
		case 2 :
			gotoxy(2,24);
			printf("%s : 배고픔2",name_);
			break;
		case 3 :
			gotoxy(2,24);
			printf("%s : 배고픔3",name_);
			break;
		case 4 :
			gotoxy(2,24);
			printf("%s : 배고픔4",name_);
			break;
		case 5 :
			gotoxy(2,24);
			printf("%s : 배고픔5",name_);
			break;
	}
}

void timer()
{
	setcolor(WHITE,0);
	gotoxy(65,8);
	printf("=PLAY  TIME=");
	gotoxy(65,10);
	printf("%02d : %02d : %02d",hour,minute,second);
}

void interfaced(char name_[], int level, int favor, int hungry)
{
	setcolor(0,0);
	gotoxy(64,2);
	printf("　　　　　　");
	gotoxy(64,3);
	printf("　　　　　　");
	gotoxy(64,4);
	printf("　　　　　　");
	setcolor(BLUE,0);
	gotoxy(64,1);
	printf("이름: %s",name_);
	gotoxy(64,2);
	printf("레벨: %d",level);
	gotoxy(64,3);
	printf("호감도: %d",favor);
	gotoxy(64,4);
	printf("배고픔: %d",hungry);
}

void info()
{
	// 좌측상단 출력 
	
	setcolor(WHITE,0);
	gotoxy(0,0);
	printf("┏");
	for(int i=1; i<60;i++)
	{
		gotoxy(i,0);
		printf("━"); 
	}
	printf("┓");
	for(int i=1; i<20;i++)
	{
		gotoxy(0,i);
		printf("┃");
		gotoxy(60,i);
		printf("┃"); 
	}
	
	gotoxy(0,20);
	printf("┗");
	for(int i=1; i<60;i++)
	{
		gotoxy(i,20);
		printf("━"); 
	}
	printf("┛");
	
	// ###
	
	// ###
	
	// Print; Information Border 
	
	gotoxy(62,0);
	printf("┏━━━━━━━━━━━━━━━┓");
	for(int i=1; i<5;i++)
	{
		gotoxy(62,i);
		printf("┃");
		gotoxy(78,i);
		printf("┃"); 
	}
	gotoxy(62,5);
	printf("┗━━━━━━━━━━━━━━━┛");
	
	// ###
	
	// ###
	
	// Print; Timer Border
	
	gotoxy(62,6);
	printf("┏━━━━━━━━━━━━━━━┓");
	for(int i=7; i<12;i++)
	{
		gotoxy(62,i);
		printf("┃");
		gotoxy(78,i);
		printf("┃"); 
	}
	gotoxy(62,12);
	printf("┗━━━━━━━━━━━━━━━┛");
	
	// ###
	
	// ###
	
	// Print; Status Border
	
	gotoxy(62,13);
	printf("┏━━━━━━━━━━━━━━━┓");
	for(int i=14; i<26; i++)
	{
		gotoxy(62,i);
		printf("┃");
		gotoxy(78,i);
		printf("┃"); 
	}
	gotoxy(62,26);
	printf("┗━━━━━━━━━━━━━━━┛");
	 
	gotoxy(64,14);
	printf("먹이: 'O'");
	gotoxy(64,15);
	printf("상호작용: 'T'");
	gotoxy(64,16);
	printf("모드변경: 'M'");
	gotoxy(64,17);
	printf("종료: 'Z'");
	
	
	
	// ###
	
	// ###
	
	// Print; Conversation Border
	
	gotoxy(0,22);
	printf("┏");
	for(int i=1;i<60;i++)
	{
		printf("━");
	}
	printf("┓");
	
	for(int i=23; i<26;i++)
	{
		gotoxy(0,i);
		printf("┃");
		gotoxy(60,i);
		printf("┃"); 
	}
	
	gotoxy(0,26);
	printf("┗");
	for(int i=1;i<60;i++)
	{
		printf("━");
	}
	printf("┛");
	
	/* dodol 출력
	 
	setcolor(BLUE,BLUE);
	gotoxy(64,22);
	printf("■■■");
	gotoxy(64,23);
	printf("■");
	gotoxy(68,23);
	printf("■");
	gotoxy(64,24);
	printf("■");
	gotoxy(68,24);
	printf("■");
	gotoxy(64,25);
	printf("■");
	gotoxy(68,25);
	printf("■");
	gotoxy(64,26);
	printf("■■■");
	setcolor(YELLOW,YELLOW);
	gotoxy(70,24);
	printf("■■■");
	gotoxy(70,25);
	printf("■");
	gotoxy(74,25);
	printf("■");
	gotoxy(70,26);
	printf("■■■");
	
	*/
	
}

void face1()
{
	setcolor(WHITE,WHITE);
	gotoxy(20,6);
	printf("■■");
	gotoxy(36,6);
	printf("■■");
	gotoxy(20,7);
	printf("■■");
	gotoxy(36,7);
	printf("■■");
	setcolor(0,0);
	gotoxy(18,8);
	printf("　　　　");
	gotoxy(34,8);
	printf("　　　　");
	gotoxy(20,13);
	printf("　　　　　　　　　　");
	gotoxy(22,14);
	printf("　　　　　　　　");
	gotoxy(20,15);
	printf("　　　　　　　　　　");
	setcolor(WHITE,WHITE);
	gotoxy(20,8);
	printf("■■");
	gotoxy(36,8);
	printf("■■");
	gotoxy(20,9);
	printf("■■");
	gotoxy(36,9);
	printf("■■");
	gotoxy(20,10);
	printf("■■");
	gotoxy(36,10);
	printf("■■");
	gotoxy(20,13);
	printf("■■");
	gotoxy(36,13);
	printf("■■");
	gotoxy(24,14);
	printf("■");
	gotoxy(34,14);
	printf("■"); 
	gotoxy(26,15);
	printf("■■■■");
	setcolor(PINK,PINK);
	gotoxy(12,12);
	printf("■■■");
	gotoxy(42,12);
	printf("■■■");
}

void face2()
{
	setcolor(0,0);
	gotoxy(20,6);
	printf("　　");
	gotoxy(36,6);
	printf("　　");
	gotoxy(20,7);
	printf("　　");
	gotoxy(36,7);
	printf("　　");
	setcolor(WHITE,WHITE);
	gotoxy(18,8);
	printf("■■■■");
	gotoxy(34,8);
	printf("■■■■");
	setcolor(0,0);
	gotoxy(20,9);
	printf("　　");
	gotoxy(36,9);
	printf("　　");
	gotoxy(20,10);
	printf("　　");
	gotoxy(36,10);
	printf("　　");
	setcolor(WHITE,WHITE);
	gotoxy(20,13);
	printf("■■");
	gotoxy(36,13);
	printf("■■");
	gotoxy(24,14);
	printf("■");
	gotoxy(34,14);
	printf("■"); 
	gotoxy(26,15);
	printf("■■■■");
	setcolor(PINK,PINK);
	gotoxy(12,12);
	printf("■■■");
	gotoxy(42,12);
	printf("■■■");
}

void face3()
{
	setcolor(WHITE,WHITE);
	gotoxy(20,6);
	printf("■■");
	gotoxy(36,6);
	printf("■■");
	gotoxy(20,7);
	printf("■■");
	gotoxy(36,7);
	printf("■■");
	setcolor(0,0);
	gotoxy(18,8);
	printf("　　　　");
	gotoxy(34,8);
	printf("　　　　");
	gotoxy(20,13);
	printf("　　　　　　　　　　");
	gotoxy(22,14);
	printf("　　　　　　　　");
	gotoxy(22,15);
	printf("　　　　　　　　");
	setcolor(WHITE,WHITE);
	gotoxy(20,8);
	printf("■■");
	gotoxy(36,8);
	printf("■■");
	gotoxy(20,9);
	printf("■■");
	gotoxy(36,9);
	printf("■■");
	gotoxy(20,10);
	printf("■■");
	gotoxy(36,10);
	printf("■■");
	gotoxy(24,13);
	printf("■■■■■■");
	gotoxy(22,14);
	printf("■");
	gotoxy(36,14);
	printf("■"); 
	gotoxy(20,15);
	printf("■"); 
	gotoxy(38,15);
	printf("■");
	setcolor(PINK,PINK);
	gotoxy(12,12);
	printf("■■■");
	gotoxy(42,12);
	printf("■■■");
}

void face4()
{
	setcolor(0,0);
	gotoxy(20,6);
	printf("　　");
	gotoxy(36,6);
	printf("　　");
	gotoxy(20,7);
	printf("　　");
	gotoxy(36,7);
	printf("　　");
	setcolor(WHITE,WHITE);
	gotoxy(18,8);
	printf("■■■■");
	gotoxy(34,8);
	printf("■■■■");
	setcolor(0,0);
	gotoxy(20,9);
	printf("　　");
	gotoxy(36,9);
	printf("　　");
	gotoxy(20,10);
	printf("　　");
	gotoxy(36,10);
	printf("　　");
	setcolor(WHITE,WHITE);
	gotoxy(24,13);
	printf("■■■■■■");
	gotoxy(22,14);
	printf("■");
	gotoxy(36,14);
	printf("■"); 
	gotoxy(20,15);
	printf("■"); 
	gotoxy(38,15);
	printf("■");
	setcolor(PINK,PINK);
	gotoxy(12,12);
	printf("■■■");
	gotoxy(42,12);
	printf("■■■");
} 
