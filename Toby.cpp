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
void cursor(int n) // Ŀ�� ���̱� 1 & ����� 0
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
	system("title PET_BY_DOAE"); // Ÿ��Ʋ ����
	system("mode con: cols=80 lines=30");
	cursor(0);
	
	char name_[10]; //�̸� 
	char *name; //�̸� ������ 
	char copy[4096]; // ī�ǿ� ���ں���
	int copy_; // ī�ǿ� ���ں��� 
	
	int first_char; // ù���� 
	int level; // ���� 
	int favor; // ȣ���� 
	int hungry; // �����
	int favor_cycle; //  ȣ���� ����Ŭ 
	int hungry_cycle; // ����� ����Ŭ
	int conversation_cycle; // ��ȭ ����Ŭ 
	int name_len; // �̸����� 
	int conversation_rand; //��������
	int rd; //��������
	int save=0; //�ڵ����� ����
	int tic=0;
	
	srand((unsigned)time(NULL));
	
	// �ּ� �� C:\\Users\\~
	FILE *data_name; //���� ������ ���� data�� ���� 
	FILE *data_value; //���� ������ ���� data�� ����
	FILE *data_Backup; // ī�� ���� ���� data�� ���� 
	data_name = fopen("name.txt", "r+");
	data_value = fopen("stored_value.txt", "rb");
	
	
	if(data_name == NULL)
	{
		perror("�̸� ���� �б� ���� ����");
		return 1;
	}
	if(data_value == NULL)
	{
		perror("�� ���� �б� ���� ����");
		return 1;
	}
	
	data_value = fopen("stored_value.txt", "r+");
	
	first_char = fgetc(data_name); // �޸����� ù �κи� ���� �б� 
	fseek(data_name,0L,SEEK_SET); // ���� ������ �� ó������ �̵�
	
	//###
	
	if(first_char==-1) // �̸��� ���� ��� ���� 
	{
		printf("�̸��� �������ּ���: ");
		name = gets(name_); // �̸� ���� 
		fprintf(data_name,"%s",name); // ���Ͽ� �̸� ����ֱ�
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
		fgets(name_,10,data_name); // �̸��� �־ �ҷ���.
		fscanf(data_value, "%d\n%d\n%d\n%d\n%d\n%d",&level,&favor,&hungry,&favor_cycle,&hungry_cycle,&conversation_cycle); // data_value���� ���� �ҷ���. 
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
		if(_kbhit()) //z������ ���� 
		{
			if(getch()=='z')
			{
				fseek(data_value,0L,SEEK_SET); // ���� �� ó������ 
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
		
		if(save>=5400000) // 30�и��� �ڵ�����
		{
			save=0;
			data_Backup = fopen("Backup.txt", "w+");
			fseek(data_Backup,0L,SEEK_SET); // ���� �� ó������
			fprintf(data_Backup,"%d\n%d\n%d\n%d\n%d\n%d",level,favor,hungry,favor_cycle,hungry_cycle,conversation_cycle);
			fclose(data_Backup);
		}
		else save++;
		
		if(hungry_cycle >= 720000) // 4�и��� ����� 1���� 
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
		
		if(favor_cycle>=10800000) // 60�и��� ȣ���� 1����
		{
			favor_cycle=0;
			favor++;
		}
		else favor_cycle++;
		
		
		if(conversation_cycle>=1080000) // 6�и��� ��ȭ
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
			if(rd==0)	printf("%s�� ����ļ� ���� ������ ���ư����ϴ�.",name_);
			else printf("%s�� ȭ������ ���� ������ ���ư����ϴ�.",name_);
			
			fseek(data_value,0L,SEEK_SET); // ���� �� ó������
			fprintf(data_value,"%d\n%d\n%d\n%d\n%d\n%d",level,favor,hungry,favor_cycle,hungry_cycle,conversation_cycle);
			break;
		}
	}
	
	fclose(data_value);
	data_value = fopen("stored_value.txt", "rb");
	data_Backup = fopen("Backup.txt", "wb");
	
	if(data_Backup == NULL)
	{
		perror("���� ���� ���� ���� ����");
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
			perror("�� ���� �б� ����");
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
				perror("���� ���� ���� ����");
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
	printf("��������������������������������������������������������");
	setcolor(WHITE,0);
	switch(rd)
	{
		case 0 :
			gotoxy(2,24);
			printf("%s : ���� �����ϰ� �ִ�??",name_);
			break;
		case 1 :
			gotoxy(2,24);
			printf("%s : ���� �ϴ°� �ƴ���??",name_);
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
			printf("%s : ����1",name_);
			break;
		case 7 :
			gotoxy(2,24);
			printf("%s : ����2",name_);
			break;
		case 8 :
			gotoxy(2,24);
			printf("%s : ����1",name_);
			break;
		case 9 :
			gotoxy(2,24);
			printf("%s : ����2",name_);
			break;
		case 10 :
			gotoxy(2,24);
			printf("%s : ��������!",name_);
			break;
		case 11 :
			gotoxy(2,24);
			printf("%s : ��������!",name_);
			break;	
	}
}

void conversation2(int rd, char name_[])
{	
	setcolor(0,0);
	gotoxy(2,24);
	printf("��������������������������������������������������������");
	setcolor(WHITE,0);
	switch(rd)
	{
		case 0 :
			gotoxy(2,24);
			printf("%s : ��... �� �� �������...",name_);
			break;
		case 1 :
			gotoxy(2,24);
			printf("%s : �����1",name_);
			break;
		case 2 :
			gotoxy(2,24);
			printf("%s : �����2",name_);
			break;
		case 3 :
			gotoxy(2,24);
			printf("%s : �����3",name_);
			break;
		case 4 :
			gotoxy(2,24);
			printf("%s : �����4",name_);
			break;
		case 5 :
			gotoxy(2,24);
			printf("%s : �����5",name_);
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
	printf("������������");
	gotoxy(64,3);
	printf("������������");
	gotoxy(64,4);
	printf("������������");
	setcolor(BLUE,0);
	gotoxy(64,1);
	printf("�̸�: %s",name_);
	gotoxy(64,2);
	printf("����: %d",level);
	gotoxy(64,3);
	printf("ȣ����: %d",favor);
	gotoxy(64,4);
	printf("�����: %d",hungry);
}

void info()
{
	// ������� ��� 
	
	setcolor(WHITE,0);
	gotoxy(0,0);
	printf("��");
	for(int i=1; i<60;i++)
	{
		gotoxy(i,0);
		printf("��"); 
	}
	printf("��");
	for(int i=1; i<20;i++)
	{
		gotoxy(0,i);
		printf("��");
		gotoxy(60,i);
		printf("��"); 
	}
	
	gotoxy(0,20);
	printf("��");
	for(int i=1; i<60;i++)
	{
		gotoxy(i,20);
		printf("��"); 
	}
	printf("��");
	
	// ###
	
	// ###
	
	// Print; Information Border 
	
	gotoxy(62,0);
	printf("����������������������������������");
	for(int i=1; i<5;i++)
	{
		gotoxy(62,i);
		printf("��");
		gotoxy(78,i);
		printf("��"); 
	}
	gotoxy(62,5);
	printf("����������������������������������");
	
	// ###
	
	// ###
	
	// Print; Timer Border
	
	gotoxy(62,6);
	printf("����������������������������������");
	for(int i=7; i<12;i++)
	{
		gotoxy(62,i);
		printf("��");
		gotoxy(78,i);
		printf("��"); 
	}
	gotoxy(62,12);
	printf("����������������������������������");
	
	// ###
	
	// ###
	
	// Print; Status Border
	
	gotoxy(62,13);
	printf("����������������������������������");
	for(int i=14; i<26; i++)
	{
		gotoxy(62,i);
		printf("��");
		gotoxy(78,i);
		printf("��"); 
	}
	gotoxy(62,26);
	printf("����������������������������������");
	 
	gotoxy(64,14);
	printf("����: 'O'");
	gotoxy(64,15);
	printf("��ȣ�ۿ�: 'T'");
	gotoxy(64,16);
	printf("��庯��: 'M'");
	gotoxy(64,17);
	printf("����: 'Z'");
	
	
	
	// ###
	
	// ###
	
	// Print; Conversation Border
	
	gotoxy(0,22);
	printf("��");
	for(int i=1;i<60;i++)
	{
		printf("��");
	}
	printf("��");
	
	for(int i=23; i<26;i++)
	{
		gotoxy(0,i);
		printf("��");
		gotoxy(60,i);
		printf("��"); 
	}
	
	gotoxy(0,26);
	printf("��");
	for(int i=1;i<60;i++)
	{
		printf("��");
	}
	printf("��");
	
	/* dodol ���
	 
	setcolor(BLUE,BLUE);
	gotoxy(64,22);
	printf("����");
	gotoxy(64,23);
	printf("��");
	gotoxy(68,23);
	printf("��");
	gotoxy(64,24);
	printf("��");
	gotoxy(68,24);
	printf("��");
	gotoxy(64,25);
	printf("��");
	gotoxy(68,25);
	printf("��");
	gotoxy(64,26);
	printf("����");
	setcolor(YELLOW,YELLOW);
	gotoxy(70,24);
	printf("����");
	gotoxy(70,25);
	printf("��");
	gotoxy(74,25);
	printf("��");
	gotoxy(70,26);
	printf("����");
	
	*/
	
}

void face1()
{
	setcolor(WHITE,WHITE);
	gotoxy(20,6);
	printf("���");
	gotoxy(36,6);
	printf("���");
	gotoxy(20,7);
	printf("���");
	gotoxy(36,7);
	printf("���");
	setcolor(0,0);
	gotoxy(18,8);
	printf("��������");
	gotoxy(34,8);
	printf("��������");
	gotoxy(20,13);
	printf("��������������������");
	gotoxy(22,14);
	printf("����������������");
	gotoxy(20,15);
	printf("��������������������");
	setcolor(WHITE,WHITE);
	gotoxy(20,8);
	printf("���");
	gotoxy(36,8);
	printf("���");
	gotoxy(20,9);
	printf("���");
	gotoxy(36,9);
	printf("���");
	gotoxy(20,10);
	printf("���");
	gotoxy(36,10);
	printf("���");
	gotoxy(20,13);
	printf("���");
	gotoxy(36,13);
	printf("���");
	gotoxy(24,14);
	printf("��");
	gotoxy(34,14);
	printf("��"); 
	gotoxy(26,15);
	printf("�����");
	setcolor(PINK,PINK);
	gotoxy(12,12);
	printf("����");
	gotoxy(42,12);
	printf("����");
}

void face2()
{
	setcolor(0,0);
	gotoxy(20,6);
	printf("����");
	gotoxy(36,6);
	printf("����");
	gotoxy(20,7);
	printf("����");
	gotoxy(36,7);
	printf("����");
	setcolor(WHITE,WHITE);
	gotoxy(18,8);
	printf("�����");
	gotoxy(34,8);
	printf("�����");
	setcolor(0,0);
	gotoxy(20,9);
	printf("����");
	gotoxy(36,9);
	printf("����");
	gotoxy(20,10);
	printf("����");
	gotoxy(36,10);
	printf("����");
	setcolor(WHITE,WHITE);
	gotoxy(20,13);
	printf("���");
	gotoxy(36,13);
	printf("���");
	gotoxy(24,14);
	printf("��");
	gotoxy(34,14);
	printf("��"); 
	gotoxy(26,15);
	printf("�����");
	setcolor(PINK,PINK);
	gotoxy(12,12);
	printf("����");
	gotoxy(42,12);
	printf("����");
}

void face3()
{
	setcolor(WHITE,WHITE);
	gotoxy(20,6);
	printf("���");
	gotoxy(36,6);
	printf("���");
	gotoxy(20,7);
	printf("���");
	gotoxy(36,7);
	printf("���");
	setcolor(0,0);
	gotoxy(18,8);
	printf("��������");
	gotoxy(34,8);
	printf("��������");
	gotoxy(20,13);
	printf("��������������������");
	gotoxy(22,14);
	printf("����������������");
	gotoxy(22,15);
	printf("����������������");
	setcolor(WHITE,WHITE);
	gotoxy(20,8);
	printf("���");
	gotoxy(36,8);
	printf("���");
	gotoxy(20,9);
	printf("���");
	gotoxy(36,9);
	printf("���");
	gotoxy(20,10);
	printf("���");
	gotoxy(36,10);
	printf("���");
	gotoxy(24,13);
	printf("�������");
	gotoxy(22,14);
	printf("��");
	gotoxy(36,14);
	printf("��"); 
	gotoxy(20,15);
	printf("��"); 
	gotoxy(38,15);
	printf("��");
	setcolor(PINK,PINK);
	gotoxy(12,12);
	printf("����");
	gotoxy(42,12);
	printf("����");
}

void face4()
{
	setcolor(0,0);
	gotoxy(20,6);
	printf("����");
	gotoxy(36,6);
	printf("����");
	gotoxy(20,7);
	printf("����");
	gotoxy(36,7);
	printf("����");
	setcolor(WHITE,WHITE);
	gotoxy(18,8);
	printf("�����");
	gotoxy(34,8);
	printf("�����");
	setcolor(0,0);
	gotoxy(20,9);
	printf("����");
	gotoxy(36,9);
	printf("����");
	gotoxy(20,10);
	printf("����");
	gotoxy(36,10);
	printf("����");
	setcolor(WHITE,WHITE);
	gotoxy(24,13);
	printf("�������");
	gotoxy(22,14);
	printf("��");
	gotoxy(36,14);
	printf("��"); 
	gotoxy(20,15);
	printf("��"); 
	gotoxy(38,15);
	printf("��");
	setcolor(PINK,PINK);
	gotoxy(12,12);
	printf("����");
	gotoxy(42,12);
	printf("����");
} 
