#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<ctime>
#include<cmath>
#include<conio.h>
#include<iostream>
#include<windows.h>
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)
using namespace std;
struct node{
	int cost,attack,health,death,treat,quality,time,hurt,special;
	int spell,level,beam;
};
FILE *data;
node deck[3][70],hand[3][20],nu,war[3][20],swap1;
node card[4],card1[40],card2[40];
int mode;//游戏模式
int dotask1,dotask2;//完成任务
int ac[20],chapter;//通过冒险模式，冒险关卡
int flag[3][70];
int hea[3];//英雄血量
int num1,num2,hf1[20],hf2[20],tot1,tot2,fa1,fa2;//tot是牌库，num是手牌，fa是疲劳
int top1,top2;//牌库顶
int mana1,mana2,tma1,tma2;//水晶和上限
int alive[3],maxhealth;
int fc1,fc2,over;//连击和游戏结束
int X,Y,ix,iy,jx,jy,xx,yy;
int hpcost[10]={0,2,4,0,1,2,1,2,2,5};//英雄技能费用
int spcost[40]={0,2,2,2,2,0,6,0,1,3,0,2,2,2,2,3,1,3,5,2,0,3,1,2,2,1,6,2};
int win[10],lose[10],twin,tlose;
int hpuse1,hpuse2,maxhp2;//本回合是否使用了英雄技能
char occu[10][40]={
"","语 文","数 学","英 语","物 理","化 学","生 物","政 治","历 史","地 理"
};
char name[50],name1[50]="学 生";//对手姓名
int clas;
int en_sum;
void HideCursor()//隐藏控制台的光标
{
	CONSOLE_CURSOR_INFO cursor_info={1,0};
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursor_info);
}
void search()//返回鼠标位置
{
	LONG zx=-1;
	LONG zy=-1;
	POINT ptB={0,0};
	LPPOINT xy=&ptB;  //位置变量
	GetCursorPos(xy);   //获取鼠标当前位置
	if ((zx!=xy->x) || (zy!=xy->y))
	{
		X=xy->x;
		Y=xy->y;
	}
}
void gotoxy(int y,int x)//改变输出光标的位置
{
    CONSOLE_SCREEN_BUFFER_INFO    csbiInfo;
    HANDLE    hConsoleOut;
    hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hConsoleOut,&csbiInfo);
    csbiInfo.dwCursorPosition.X = x;
    csbiInfo.dwCursorPosition.Y = y;
    SetConsoleCursorPosition(hConsoleOut,csbiInfo.dwCursorPosition);
}
void swap(int &x,int &y)//交换
{
	int t;
	t=x;x=y;y=t;
}
void change(int x,int y)//数学计算光标所点击的格子
{
	yy=(double)(x-ix)/(jx-ix)*100+0.5;
	xx=(double)(y-iy)/(jy-iy)*34+0.5;
}
void pause()//按空格键继续
{
	printf("按空格键继续！\n");
	char c1=0;
	while(c1!=' ') c1=getch();
}
void calibrate()//校准屏幕
{
	system("cls");
	printf("O  <-左键这个圆圈的中心\n");
	while(1)
	{
		if(KEY_DOWN(MOUSE_MOVED))
		{
			search();
			ix=X,iy=Y;
			printf("(%d,%d)很好！\n",ix,iy);
			break;
		}
	}
	pause();
	for(int i=1;i<=31;i++) printf("\n");
	for(int i=1;i<=70;i++) printf(" ");
	printf("        左键这个圆圈的中心->  O\n");
	while(1)
	{
		if(KEY_DOWN(MOUSE_MOVED))
		{
			search();
			jx=X,jy=Y;
			printf("(%d,%d)很好！\n不要再移动你的窗口了！！！\n",jx,jy);
			break;
		}
	}
	pause();

}
void initialize()//初始化程序
{
	printf("正在加载，请稍后………………\n");
	Sleep(100);
	srand(time(0));
	HideCursor();
	data=fopen("data.txt","r");
	if(data!=NULL)
	{
		for(int i=1;i<=9;i++) fscanf(data,"%d%d",&win[i],&lose[i]);
		fscanf(data,"%d%d",&twin,&tlose);
		for(int i=1;i<=9;i++) fscanf(data,"%d",&ac[i]);
		fscanf(data,"\n");
		fgets(name,9,data);
		if(strlen(name)==0) strcpy(name,name1);
		fclose(data);
	}
	if(strlen(name)==0) strcpy(name,name1);
	system("color 60");
	system("mode con cols=120 lines=40");
	printf("加载已完成，即将进入校准模式\n");
	pause();
	calibrate();
}
void clear()//清理屏幕并进入对战场景
{
	system("cls");
	gotoxy(35,0);
	for(int i=1;i<=100;i++) printf("-");
	printf("/");
	for(int i=1;i<=35;i++) gotoxy(i-1,100),printf("|");
	gotoxy(0,0);
}
void create(int k)//创建牌库
{
	int t;
	for(int i=1;i<=(k==1?tot1:tot2);i++)
	{
		t=rand()%12;
		if(k==1 && mode==4 && clas==7 && dotask1>=6) t=0;
		if(k==2 && chapter==9) t=0;
		if(t<9)
		{
			t=rand()%100;
			if(mode==4 && clas==7 && dotask1>=6) t=0;
			if(k==1 && chapter==7) t=66;
			if(k==2 && chapter==9) t=0;
			if(t==0) deck[k][i].quality=4;
			else if(t<=5) deck[k][i].quality=3;
			else if(t>=80) deck[k][i].quality=2;
			else deck[k][i].quality=1;
			if(k==2 && chapter==8 && deck[k][i].quality==1) deck[k][i].quality=2;
			deck[k][i].attack=rand()%10+1;
			deck[k][i].health=rand()%10+1;
			deck[k][i].cost=rand()%10+1;
			deck[k][i].spell=0;
			if(k==1 && chapter==3) deck[k][i].cost=(rand()%5+1)*2;
			deck[k][i].time=1;
			while(deck[k][i].attack<deck[k][i].cost-4) deck[k][i].attack=rand()%10+1;
			while(deck[k][i].health<deck[k][i].cost-4) deck[k][i].health=rand()%10+1;
			if(deck[k][i].quality==4)
			{
				deck[k][i].time=rand()%3+2;
				if(deck[k][i].health<deck[k][i].cost+1) deck[k][i].health=deck[k][i].cost+1;
				if(deck[k][i].health>10) deck[k][i].health=10;
			}
			if(deck[k][i].quality==3)
			{
				deck[k][i].death=rand()%6+2;
				int t1=deck[k][i].attack,t2=deck[k][i].health;
				deck[k][i].attack=deck[k][i].health=(t1+t2+1)/2;
			}
			if(deck[k][i].quality==2)
			{
				deck[k][i].treat=rand()%5+1;
				if(deck[k][i].attack>deck[k][i].cost*2) deck[k][i].attack=deck[k][i].cost*2+1;
				if(deck[k][i].health>deck[k][i].cost*2) deck[k][i].health=deck[k][i].cost*2+1;
			}
			if(deck[k][i].quality==1)
			{
				while(deck[k][i].attack>deck[k][i].cost*2) deck[k][i].attack=rand()%10+1;
				while(deck[k][i].health>deck[k][i].cost*2) deck[k][i].health=rand()%10+1;
			}
			if(deck[k][i].attack<deck[k][i].cost-4) deck[k][i].attack=deck[k][i].cost-4;
			if(deck[k][i].health<deck[k][i].cost-4) deck[k][i].health=deck[k][i].cost-4;
			if(k==2 && chapter==4) deck[k][i].cost--;
		}
		else
		{
			t=rand()%100;
			if(k==1 && chapter==7) t=66;
			if(t<=20) deck[k][i].quality=3;
			else if(t>=70) deck[k][i].quality=2;
			else deck[k][i].quality=1;
			if(k==2 && chapter==8 && deck[k][i].quality==1) deck[k][i].quality=2;
			if(k==1) deck[k][i].cost=spcost[(clas-1)*3+deck[k][i].quality];
			else deck[k][i].cost=3;
			deck[k][i].spell=1;
		}
	}
}
void print1(int t)//输出起始手牌
{
	if(!deck[1][t].spell)
	{
		printf("\t随从牌:%d/%d-%d",deck[1][t].cost,deck[1][t].attack,deck[1][t].health);
		if(deck[1][t].quality==1) printf("(普通)\n");
		if(deck[1][t].quality==2) printf("(稀有):战吼：为你的英雄恢复 %d 点生命值。\n",deck[1][t].treat);
		if(deck[1][t].quality==3) printf("(史诗):亡语：对敌方英雄造成 %d 点伤害。\n",deck[1][t].death);
		if(deck[1][t].quality==4) printf("(传说):随机风怒第 %d 级\n",deck[1][t].time);
	}
	else
	{
		printf("\t法术牌:%d/",deck[1][t].cost);
		if(deck[1][t].quality==1)
		{
			if(clas==1) printf("(普通)：使你的所有“书籍”获得+2+1");
			if(clas==2) printf("(普通)：召唤一个2-3的“直尺”");
			if(clas==3) printf("(普通): I only know this is a common cell card.");
			if(clas==4) printf("(普通)：仅在本回合，获得一点法力水晶，且不能超过10点");
			if(clas==5) printf("(普通)：抽一张牌；如果本回合使用过英雄技能，则改为抽两张牌");
			if(clas==6) printf("(普通)：召唤一个“细胞”抽一张牌");
			if(clas==7) printf("(普通)：抽2张牌，将一张传说卡牌洗入你对手的牌库");
			if(clas==8) printf("(普通)：抽一张牌；如果该牌被弃置，则自动释放");
			if(clas==9) printf("(普通)：从你的牌库里将1张法术牌的复制置入你的手牌");
			printf("\n");

		}
		if(deck[1][t].quality==2)
		{
			if(clas==1) printf("(稀有)：你每控制一个“书籍”，抽一张牌");
			if(clas==2) printf("(稀有)：消耗你的所有法力水晶，然后每消耗2点法力水晶便抽一张牌");
			if(clas==3) printf("(稀有): I only know this is a rare cell card.");
			if(clas==4) printf("(稀有)：你每控制一个“秒表”，抽一张牌");
			if(clas==5) printf("(稀有)：使所有随从的生命值变为1");
			if(clas==6) printf("(稀有)：使敌方最右侧随从变为0-0");
			if(clas==7) printf("(稀有)：使你的所有手牌获得-1/+1+1，你的牌库每有一张牌，该牌的法力值消耗便增加1点");
			if(clas==8) printf("(稀有)：将对手的一张手牌洗入你的牌库");
			if(clas==9) printf("(稀有)：召唤你牌库中一个6攻及以上的随从的复制");
			printf("\n");
		}
		if(deck[1][t].quality==3)
		{
			if(clas==1) printf("(史诗)：使你的所有书籍返回你的手牌");
			if(clas==2) printf("(史诗)：将你的法力水晶变为等量的2-2的“直尺”");
			if(clas==3) printf("(史诗): I only know this is an epic cell card.");
			if(clas==4) printf("(史诗)：摧毁你的所有法力水晶，召唤一个生命值不小于6的传说随从“光”，在该随从死亡后复原你的法力水晶");
			if(clas==5) printf("(史诗)：移除所有攻击力等同于其生命值的随从");
			if(clas==6) printf("(史诗)：使双方英雄变为30血");
			if(clas==7) printf("(史诗)：你的对手抽2张牌，为你的英雄恢复8点生命值");
			if(clas==8) printf("(史诗)：使你的所有随从获得+1+1（弃掉1张牌后升级）");
			if(clas==9) printf("(史诗)：将你手牌里的所有随从牌的复制洗入你的牌库");
			printf("\n");
		}
	}
}
void shuffle(int l,int r,int k)//洗牌
{
	int totm=r-l+1;
	for(int i=1;i<=1000;i++)
	{
		int x=rand()%totm+l;
		int y=rand()%totm+l;
		node t;
		t=deck[k][x];
		deck[k][x]=deck[k][y];
		deck[k][y]=t;
	}
}
void begin()//开始游戏，选择起始手牌
{
	//for(int i=1;i<=tot1;i++) flag[1][i]=flag[2][i]=1;
	clear();
	printf("\t你的职业： %s",occu[clas]);
	printf("\n\t选择起始手牌\n");
	printf("\t请点击要换掉的牌的汉字\n");
	print1(1);
	printf("\n");
	print1(2);
	printf("\n");
	print1(3);
	printf("\n");
	printf("        确认！");
	int w1=0,w2=0,w3=0,w4=0,ow=1;
	while(ow)
	{
		if(KEY_DOWN(MOUSE_MOVED))
		{
			search();
			change(X,Y);
			switch(xx)
			{
				case 3:gotoxy(3,0);
					   if(w1==0) printf("X"),w1=1;
					   else		 printf(" "),w1=0;
					   break;
				case 5:gotoxy(5,0);
					   if(w2==0) printf("X"),w2=1;
					   else		 printf(" "),w2=0;
					   break;
				case 7:gotoxy(7,0);
					   if(w3==0) printf("X"),w3=1;
					   else		 printf(" "),w3=0;
					   break;
				case 9:gotoxy(9,8);
					   printf("         ");
					   ow=0;
					   break;
			}
			Sleep(150);
		}
	}
	if(w1==1)
	{
		node t;
		t=deck[1][1];
		deck[1][1]=deck[1][4];
		deck[1][4]=t;
	}
	if(w2==1)
	{
		node t;
		t=deck[1][2];
		deck[1][2]=deck[1][5];
		deck[1][5]=t;
	}
	if(w3==1)
	{
		node t;
		t=deck[1][3];
		deck[1][3]=deck[1][6];
		deck[1][6]=t;
	}
	shuffle(4,tot1,1);
	gotoxy(10,0);
	printf("    现在，这是你的起始手牌\n");
	print1(1);
	printf("\n");
	print1(2);
	printf("\n");
	print1(3);
	printf("\n");
	num1=0;
	hand[1][++num1]=deck[1][1];
	hand[1][++num1]=deck[1][2];
	hand[1][++num1]=deck[1][3];
	//flag[1][1]=0;
	//flag[1][2]=0;
	//flag[1][3]=0;
	tot1-=3;top1=4;
	if(chapter==5) num1=0;
	//your opponent
	w1=w2=w3=w4=0;
	if(deck[2][1].cost>4 || deck[2][1].spell && deck[2][1].quality==2) w1=1;
	if(deck[2][2].cost>4 || deck[2][2].spell && deck[2][2].quality==2) w2=1;
	if(deck[2][3].cost>4 || deck[2][3].spell && deck[2][3].quality==2) w3=1;
	if(deck[2][4].cost>4 || deck[2][4].spell && deck[2][4].quality==2) w4=1;
	if(w1==1)
	{
		node t;
		t=deck[2][1];
		deck[2][1]=deck[2][5];
		deck[2][5]=t;
	}
	if(w2==1)
	{
		node t;
		t=deck[2][2];
		deck[2][2]=deck[2][6];
		deck[2][6]=t;
	}
	if(w3==1)
	{
		node t;
		t=deck[2][3];
		deck[2][3]=deck[2][7];
		deck[2][7]=t;
	}
	if(w4==1)
	{
		node t;
		t=deck[2][4];
		deck[2][4]=deck[2][8];
		deck[2][8]=t;
	}
	shuffle(5,tot2,2);
	num2=0;
	hand[2][++num2]=deck[2][1];
	if(hand[2][num2].cost) hand[2][num2].cost--;
	hand[2][++num2]=deck[2][2];
	if(hand[2][num2].cost) hand[2][num2].cost--;
	hand[2][++num2]=deck[2][3];
	if(hand[2][num2].cost) hand[2][num2].cost--;
	hand[2][++num2]=deck[2][4];
	if(hand[2][num2].cost) hand[2][num2].cost--;
	//flag[2][1]=0;
	//flag[2][2]=0;
	//flag[2][3]=0;
	//flag[2][4]=0;
	tot2-=4;top2=5;
	pause();
}
void print2(int x,int y,int i)//输出玩家手牌
{
	gotoxy(x,y-1);
	printf("%2d ==",hand[1][i].cost);
	gotoxy(++x,y);
	printf("[");
	if(hand[1][i].special==1) printf("书]");
	else
	{
		if(hand[1][i].quality==1) printf("白]");
		if(hand[1][i].quality==2) printf("蓝]");
		if(hand[1][i].quality==3) printf("紫]");
		if(hand[1][i].quality==4) printf("橙]");
	}
	gotoxy(++x,y);
	if(!hand[1][i].spell)
	{
		if(hand[1][i].quality==2) printf("B:%d",hand[1][i].treat);
		if(hand[1][i].quality==3) printf("D:%d",hand[1][i].death);
		if(hand[1][i].quality==4) printf("A:%d",hand[1][i].time);
	}
	else if(clas==8 && hand[1][i].quality==3) printf("L:%d",hand[1][i].level+1);
	gotoxy(++x,y-1);
	if(!hand[1][i].spell) printf("%2d %2d",hand[1][i].attack,hand[1][i].health);
	else printf(" 法术 ");
	gotoxy(++x,y);
	printf("====");
}
void p1()//更新玩家手牌
{
	gotoxy(30,0);
	for(int i=1;i<=99;i++) printf(" ");
	gotoxy(31,0);
	for(int i=1;i<=99;i++)printf(" ");
	gotoxy(32,0);
	for(int i=1;i<=99;i++)printf(" ");
	gotoxy(33,0);
	for(int i=1;i<=99;i++)printf(" ");
	gotoxy(34,0);
	for(int i=1;i<=99;i++)printf(" ");
	int position=5;
	for(int i=1;i<=num1;i++) print2(30,position,i),position+=9;
}
void p2()//更新对手手牌
{
	int position=10;
	for(int i=1;i<=num2;i++)
	{
		gotoxy(0,position);
		printf("[====]");
		gotoxy(1,position);
		printf("[卡牌]");
		gotoxy(2,position);
		printf("[====]");
		position+=8;
	}
}
void p3(int k)//更新战场
{
	int px[3]={0,19,11};
	gotoxy(px[k],0);
	for(int i=1;i<=90;i++) printf(" ");
	gotoxy(px[k]+1,0);
	for(int i=1;i<=90;i++) printf(" ");
	gotoxy(px[k]+2,0);
	for(int i=1;i<=90;i++) printf(" ");
	float mid=alive[k]/2.0;
	for(int i=1;i<=alive[k];i++)
	{
		float dis=42+(i-mid)*10;
		gotoxy(px[k],dis);
		printf("/ == \\");
		gotoxy(px[k]+1,dis);
		printf("||  ||");
		gotoxy(px[k]+2,dis-1);
		if(war[k][i].quality<3) printf("%2d   %2d",war[k][i].attack,war[k][i].health);
		if(war[k][i].quality==3) printf("%2d D%d %d",war[k][i].attack,war[k][i].death,war[k][i].health);
		if(war[k][i].quality==4) printf("%2d A%d %d",war[k][i].attack,war[k][i].time,war[k][i].health);
		if(war[k][i].special==1) gotoxy(px[k]+1,dis+2),printf("书");
		if(war[k][i].special==2) gotoxy(px[k]+1,dis+2),printf("尺");
		if(war[k][i].special==4) gotoxy(px[k]+1,dis+2),printf("表");
		if(war[k][i].special==6) gotoxy(px[k]+1,dis+2),printf("胞");
		if(war[k][i].special==14)gotoxy(px[k]+1,dis+2),printf("光");
	}
}
void p4()//更新英雄血量
{
	gotoxy(7,48);
	printf("%3d",hea[2]);
	gotoxy(27,48);
	printf("%3d",hea[1]);
}
void p5(int i)//历史记录
{
	int x=0,y=107;
	gotoxy(x,y-1);
	printf("         ");
	gotoxy(x+1,y-1);
	printf("         ");
	gotoxy(x+2,y-1);
	printf("         ");
	gotoxy(x+3,y-1);
	printf("         ");
	gotoxy(x+4,y-1);
	printf("         ");
	gotoxy(x+5,y-1);
	printf("         ");
	gotoxy(x,y-1);
	printf("历史记录：");
	x++;
	gotoxy(x,y-1);
	printf("%2d ==",hand[2][i].cost);
	gotoxy(++x,y);
	printf("[");
	if(hand[2][i].quality==1) printf("白]");
	if(hand[2][i].quality==2) printf("蓝]");
	if(hand[2][i].quality==3) printf("紫]");
	if(hand[2][i].quality==4) printf("橙]");
	gotoxy(++x,y);
	if(!hand[2][i].spell)
	{
		if(hand[2][i].quality==2) printf("B:%d",hand[2][i].treat);
		if(hand[2][i].quality==3) printf("D:%d",hand[2][i].death);
		if(hand[2][i].quality==4) printf("A:%d",hand[2][i].time);
	}
	else
	{
		if(hand[2][i].quality==1) printf(" 召 ");
		if(hand[2][i].quality==2) printf(" 灭 ");
		if(hand[2][i].quality==3) printf(" 续 ");
	}
	gotoxy(++x,y-1);
	if(!hand[2][i].spell) printf("%2d %2d",hand[2][i].attack,hand[2][i].health);
	else printf(" 法术 ");
	gotoxy(++x,y);
	printf("====");
}
void p5_1(int t)//历史记录—英雄技能
{
	int x=0,y=107;
	gotoxy(x,y-1);
	printf("         ");
	gotoxy(x+1,y-1);
	printf("         ");
	gotoxy(x+2,y-1);
	printf("         ");
	gotoxy(x+3,y-1);
	printf("         ");
	gotoxy(x+4,y-1);
	printf("         ");
	gotoxy(x+5,y-1);
	printf("         ");
	gotoxy(x,y-1);
	printf("历史记录：");
	x++;
	gotoxy(x,y);
	printf("2 ==");
	gotoxy(++x,y);
	printf("[HP]");
	gotoxy(++x,y-1);
	printf("%2d %2d",t,t);
	gotoxy(++x,y);
	printf("====");
}
void p6(int t,int k)//爆牌
{
	int x=7,y=107;
	gotoxy(x,y-1);
	printf("         ");
	gotoxy(x+1,y-1);
	printf("         ");
	gotoxy(x+2,y-1);
	printf("         ");
	gotoxy(x+3,y-1);
	printf("         ");
	gotoxy(x+4,y-1);
	printf("         ");
	gotoxy(x+5,y-1);
	printf("         ");
	if(t==0) return;
	gotoxy(x,y-1);
	printf(" 爆牌：");
	x++;
	gotoxy(x,y-1);
	printf("%2d ==",deck[k][t].cost);
	gotoxy(++x,y);
	printf("[");
	if(deck[k][t].quality==1) printf("白]");
	if(deck[k][t].quality==2) printf("蓝]");
	if(deck[k][t].quality==3) printf("紫]");
	if(deck[k][t].quality==4) printf("橙]");
	gotoxy(++x,y);
	if(!deck[k][t].spell)
	{
		if(deck[k][t].quality==2) printf("B:%d",deck[k][t].treat);
		if(deck[k][t].quality==3) printf("D:%d",deck[k][t].death);
		if(deck[k][t].quality==4) printf("A:%d",deck[k][t].time);
	}
	else if(k==2)
	{
		if(deck[k][t].quality==1) printf(" 召 ");
		if(deck[k][t].quality==2) printf(" 灭 ");
		if(deck[k][t].quality==3) printf(" 续 ");
	}
	gotoxy(++x,y-1);
	if(!deck[k][t].spell) printf("%2d %2d",deck[k][t].attack,deck[k][t].health);
	else printf(" 法术 ");
	gotoxy(++x,y);
	printf("====");
}
void p7()//更新法力水晶
{
	gotoxy(26,83);
	printf("%2d/%2d",mana1,tma1);
	gotoxy(4,11);
	printf("%2d/%2d",mana2,tma2);
}
void p8()//更新任务进度
{
	gotoxy(3,45);
	printf("%d/6",dotask2);
	gotoxy(23,44);
	if(clas==1) printf("%2d/1 ",dotask1);
	if(clas==2) printf("%2d/1 ",dotask1);
	if(clas==3) printf("%2d/1 ",en_sum);
	if(clas==4) printf("%2d/6 ",dotask1);
	if(clas==5) printf("%2d/1 ",dotask1);
	if(clas==6) printf("%2d/20",dotask1);
	if(clas==7) printf("%2d/6 ",dotask1);
	if(clas==8) printf("%2d/3 ",dotask1);
	if(clas==9) printf("%2d/3 ",dotask1);
}
void chou1()//我方抽牌
{
	p6(0,1);
	if(tot1>0)
	{
		//flag[1][top1]=0;
		tot1--;
		if(num1<10) hand[1][++num1]=deck[1][top1],deck[1][top1]=nu;
		else p6(top1,1);
		top1++;
	}
	else fa1++,hea[1]-=fa1;
	p4();
	gotoxy(26,93);
	if(tot1>0) printf("%2d",tot1);
	else printf("%2d",-fa1);
	p1();
}
void chou2()//对方抽牌
{
	p6(0,2);
	if(tot2>0)
	{
		//flag[2][top2]=0;
		tot2--;
		if(num2<10) hand[2][++num2]=deck[2][top2];
		else p6(top2,2);
		top2++;
	}
	else fa2++,hea[2]-=fa2;
	p4();
	gotoxy(6,93);
	if(tot2>0) printf("%2d",tot2);
	else printf("%2d",-fa2);
}
void start()//双方英雄入场
{
	clear();
	gotoxy(4,40);
	printf("  /-------\\  ");
	gotoxy(5,40);
	printf("//         \\\\");
	int t=strlen(name);
	t=46-t/2;
	gotoxy(6,t);
	puts(name);
	gotoxy(6,40);
	printf("||");
	gotoxy(6,51);
	printf("||");
	gotoxy(7,40);
	printf("||       -1||");
	gotoxy(8,40);
	printf("=============");

	gotoxy(24,40);
	printf("  /-------\\  ");
	gotoxy(25,40);
	printf("//         \\\\");
	gotoxy(26,40);
	printf("||  %s  ||",occu[clas]);
	gotoxy(27,40);
	printf("||       -1||");
	gotoxy(28,40);
	printf("=============");

	gotoxy(4,58);
	printf("2 //----\\\\  ");
	gotoxy(5,58);
	printf(" //      \\\\ ");
	gotoxy(6,58);
	printf("||英雄技能||");
	gotoxy(7,58);
	printf(" \\\\      // ");
	gotoxy(8,58);
	printf("  \\\\----//  ");

	gotoxy(24,57);
	printf("%2d //----\\\\  ",hpcost[clas]);
	gotoxy(25,58);
	printf(" //      \\\\ ");
	gotoxy(26,58);
	printf("||英雄技能||");
	gotoxy(27,58);
	printf(" \\\\      // ");
	gotoxy(28,58);
	printf("  \\\\----//  ");

	gotoxy(5,90);
	printf("[ 牌库 ]");
	gotoxy(6,90);
	printf("[  %2d  ]",tot2);
	gotoxy(7,90);
	printf("[ 剩余 ]");

	gotoxy(25,90);
	printf("[ 牌库 ]");
	gotoxy(26,90);
	printf("[  %2d  ]",tot1);
	gotoxy(27,90);
	printf("[ 剩余 ]");

	gotoxy(26,83);
	printf("%2d/%2d",mana1,tma1);
	gotoxy(4,11);
	printf("%2d/%2d",mana2,tma2);

	gotoxy(33,107);
	printf("投 降");

	gotoxy(36,6);
	if(clas==1) printf("2普通：使你的所有“书籍”获得+2+1");
	if(clas==2) printf("2普通：召唤一个2-3的“直尺”");
	if(clas==3) printf("0common: I only know this is a common cell card.");
	if(clas==4) printf("0普通：仅在本回合，获得一点法力水晶，且不能超过10点");
	if(clas==5) printf("2普通：抽一张牌；如果本回合使用过英雄技能，则改为抽两张牌");
	if(clas==6) printf("1普通：召唤一个“细胞”抽一张牌");
	if(clas==7) printf("2普通：抽2张牌，将一张传说卡牌洗入你对手的牌库");
	if(clas==8) printf("1普通：抽一张牌；如果该牌被弃置，则自动释放");
	if(clas==9) printf("1普通：从你的牌库里将1张法术牌的复制置入你的手牌");
	gotoxy(37,6);
	if(clas==1) printf("2稀有：你每控制一个“书籍”，抽一张牌");
	if(clas==2) printf("0稀有：消耗你的所有法力水晶，然后每消耗2点法力水晶便抽一张牌");
	if(clas==3) printf("1rare: I only know this is a rare cell card.");
	if(clas==4) printf("2稀有：你每控制一个“秒表”，抽一张牌");
	if(clas==5) printf("2稀有：使所有随从的生命值变为1");
	if(clas==6) printf("3稀有：使敌方最右侧随从变为0-0");
	if(clas==7) printf("0稀有：使你的所有手牌获得-1/+1+1，你的牌库每有一张牌，该牌的法力值消耗便增加1点");
	if(clas==8) printf("2稀有：将对手的一张手牌洗入你的牌库");
	if(clas==9) printf("6稀有：召唤你牌库中一个6攻及以上的随从的复制");
	gotoxy(38,6);
	if(clas==1) printf("2史诗：使你的所有书籍返回你的手牌");
	if(clas==2) printf("6史诗：将你的法力水晶变为等量的2-2的“直尺”");
	if(clas==3) printf("3epic: I only know this is an epic cell card.");
	if(clas==4) printf("2史诗：摧毁你的所有法力水晶，召唤一个生命值不小于6的传说随从“光”，在该随从死亡后复原你的法力水晶");
	if(clas==5) printf("3史诗：移除所有攻击力等同于其生命值的随从");
	if(clas==6) printf("5史诗：使双方英雄变为30血");
	if(clas==7) printf("3史诗：你的对手抽2张牌，为你的英雄恢复8点生命值");
	if(clas==8) printf("2史诗：使你的所有随从获得+1+1（弃掉1张牌后升级）");
	if(clas==9) printf("2史诗：将你手牌里的所有随从牌的复制洗入你的牌库");

	p2();
	p1();
	p4();
}
void end(int k)//回合结束时自动攻击
{
	int k1=k==1?2:1;
	float mid[3];
	mid[1]=alive[1]/2.0+0.5;
	mid[2]=alive[2]/2.0+0.5;
	if(mid[1]==0.5) mid[1]=0;
	if(mid[2]==0.5) mid[2]=0;
	for(int i=1;i<=alive[k];i++)
	{
		float dis=i-mid[k];
		int hit=1;
		if(mid[k1]+dis<=0 || mid[k1]+dis>=alive[k1]+1 || alive[k1]==0) hit=0;
		if(hit==0)
		{
			if(k==2 || chapter!=6) hea[k1]-=war[k][i].attack;
			else hea[k1]-=war[k][i].attack>2?2:war[k][i].attack;
		}
		else for(float j=-0.5;j<=0.5;j+=0.5)
		{
			float to=mid[k1]+dis+j;
			if(to-(int)to!=0) continue;
			war[k1][(int)to].health-=war[k][i].attack*war[k][i].time;
			if(mode==4 && clas==6 && dotask1<20 && war[k1][(int)to].special==6) dotask1+=war[k][i].attack*war[k][i].time,p8();
		}
		p4();
		p3(k1);
		Sleep(400);
	}
	int al=alive[k1];
	for(int i=1;i<=al;i++)
		if(war[k1][i].health<=0)
		{
			if(k1==1 && war[k1][i].special==14) tma1+=war[k1][i].beam,tma1=tma1>10?10:tma1;
			alive[k1]--;
			hea[k]-=war[k1][i].death;
			war[k1][i]=nu;
		}
	int numm=0;
	for(int i=1;i<=al;i++)
		if(war[k1][i].health) war[k1][++numm]=war[k1][i];
	p3(k1);
	Sleep(300);
	p4();
	Sleep(300);
	if(hea[1]<=0 && hea[2]<=0) over=2;
	else if(hea[1]<=0) over=3;
	else if(hea[2]<=0) over=1;
}
void your()//你的回合
{
	p6(0,1);
	if(mode==4) p8();
	if(tma1<10) tma1++;
	mana1=tma1;
	if(mode==4 && clas==2 && dotask1==0)
	{
		if(tma1==10) dotask1=1;
		if(mana1==5) dotask1=-1;
		p8();
	}
	hpuse1=0;
	gotoxy(16,45);
	printf("你的回合！");
	Sleep(1000);
	gotoxy(16,45);
	printf("          ");
	gotoxy(16,94);
	printf("结束");
	gotoxy(29,107);
	printf("暂 停");
	chou1();
	if(hea[1]<=0){over=3;return;}
	if(mode==4 && clas==7 && dotask1<6 && fa1)
	{
		dotask1+=fa1;
		if(dotask1>=6)
		{
			tot1=6;
			create(1);
			top1=1;
			//for(int i=1;i<=6;i++) flag[1][i]=1;
			chou1();
		}
		p8();
	}
	if(mode==4 && clas==8 && dotask1==3 && alive[1]<7)
	{
		war[1][++alive[1]]=nu;
		war[1][alive[1]].attack=3;
		war[1][alive[1]].health=3;
		war[1][alive[1]].time=1;
		war[1][alive[1]].quality=1;
		p3(1);
	}
	fc1=1;
	gotoxy(26,83);
	printf("%2d/%2d",mana1,tma1);
	int ow=1,suspend=0;
	while(ow)
	{
		if(KEY_DOWN(MOUSE_MOVED))
		{
			search();
			change(X,Y);
			if(suspend && xx!=29) continue;
			if(xx==29 && yy>106 && yy<113)
			{
				suspend^=1;
				gotoxy(29,107);
				if(suspend) printf("继 续");
				else printf("暂 停");
			}
			if(xx==33 && yy>106 && yy<113)
			{
				over=3;
				return;
			}
			if(xx>28 && yy>0 && yy<105)
			{
				int t=(yy-2)/9+1;
				if(t>num1 || mana1<hand[1][t].cost || alive[1]==7)
				{
					Sleep(400);
					continue;
				}
				if(hand[1][t].spell && clas==7 && hand[1][t].quality==2 && mana1<tot1)
				{
					Sleep(400);
					continue;
				}
				mana1-=hand[1][t].cost;
				if(mode==4 && clas==2 && dotask1==0 && mana1==5) dotask1=-1,p8();
				int y1=(t-1)*9+3;
				gotoxy(30,y1);
				printf("         ");
				gotoxy(31,y1);
				printf("         ");
				gotoxy(32,y1);
				printf("         ");
				gotoxy(33,y1);
				printf("         ");
				gotoxy(34,y1);
				printf("         ");
				Sleep(500);
				node hand1=hand[1][t];
				hand[1][t]=nu;
				for(int i=t+1;i<=num1;i++) hand[1][i-1]=hand[1][i];
				num1--;
				if(!hand1.spell)
				{
					hea[1]+=hand1.treat;
					if(hea[1]>maxhealth) hea[1]=maxhealth;
					if(hand1.special==1 && mode==4 && dotask1<1)
					{
						int num=0;
						for(int i=1;i<=alive[1];i++) if(war[1][i].special) num++;
						if(num>=2) dotask1=1;
						p8();
					}
					p4();
					war[1][++alive[1]]=hand1;
				}
				else
				{
					if(clas==1)
					{
						if(hand1.quality==1)
						{
							for(int i=1;i<=alive[1];i++)
								if(war[1][i].special==1) war[1][i].attack+=2,war[1][i].health+=2;
						}
						if(hand1.quality==2)
						{
							int num=0;
							for(int i=1;i<=alive[1];i++)
								if(war[1][i].special==1) num++;
							for(int i=1;i<=num;i++) chou1();
							if(hea[1]<=0){over=3;return;}
						}
						if(hand1.quality==3)
						{
							int flag1[10]={0};
							for(int i=1;i<=alive[1];i++) flag1[i]=1;
							for(int i=1;i<=alive[1];i++)
								if(war[1][i].special==1)
									if(num1<7)
									{
										hand[1][++num1]=war[1][i];
										war[1][i]=nu;flag1[i]=0;
									}
							int num=0;
							for(int i=1;i<=alive[1];i++)
								if(flag1[i]) war[1][++num]=war[1][i];
							alive[1]=num;
						}
					}
					if(clas==2)
					{
						if(hand1.quality==1 && alive[1]<7)
						{
							war[1][++alive[1]]=nu;
							war[1][alive[1]].health=3;
							war[1][alive[1]].attack=2;
							war[1][alive[1]].time=1;
							war[1][alive[1]].cost=2;
							war[1][alive[1]].special=2;
						}
						if(hand1.quality==2)
						{
							int num=mana1/2;
							mana1=0;
							p7();
							for(int i=1;i<=num;i++) chou1();
							if(hea[1]<=0){over=3;return;}
						}
						if(hand1.quality==3)
						{
							int num=7-alive[1];
							if(num>tma1) num=tma1;
							for(int i=1;i<=num;i++)
							{
								tma1--;
								war[1][++alive[1]]=nu;
								war[1][alive[1]].health=2;
								war[1][alive[1]].attack=2;
								war[1][alive[1]].time=1;
								war[1][alive[1]].cost=2;
								war[1][alive[1]].special=2;
							}
						}
					}
					if(clas==3)
					{
						if(hand1.quality==1)
						{
							en_sum++;
						}
						if(hand1.quality==2)
						{
							en_sum++;chou1();
							if(hea[1]<=0){over=3;return;}
						}
						if(hand1.quality==3)
						{
							en_sum+=3;
						}
					}
					if(clas==4)
					{
						if(hand1.quality==1)
						{
							mana1++;
							if(mana1>10) mana1=10;
						}
						if(hand1.quality==2)
						{
							int num=0;
							for(int i=1;i<=alive[1];i++)
								if(war[1][i].special==4) num++;
							for(int i=1;i<=num;i++) chou1();
							if(hea[1]<=0){over=3;return;}
						}
						if(hand1.quality==3 && alive[1]<7)
						{
							int num=tma1;
							mana1=0;tma1=0;
							war[1][++alive[1]]=nu;
							war[1][alive[1]].beam=num;
							war[1][alive[1]].cost=rand()%10+1;
							war[1][alive[1]].attack=rand()%10+1;
							war[1][alive[1]].health=rand()%10+1;
							war[1][alive[1]].special=14;
							war[1][alive[1]].time=rand()%3+2;
							while(war[1][alive[1]].attack<war[1][alive[1]].cost-4) war[1][alive[1]].attack=rand()%10+1;
							while(war[1][alive[1]].health<war[1][alive[1]].cost-4) war[1][alive[1]].health=rand()%10+1;
							if(war[1][alive[1]].health<war[1][alive[1]].cost+1) war[1][alive[1]].health=war[1][alive[1]].cost+1;
							if(war[1][alive[1]].health>10) war[1][alive[1]].health=10;
							if(war[1][alive[1]].health<6) war[1][alive[1]].health=6;
						}
					}
					if(clas==5)
					{
						if(hand1.quality==1)
						{
							chou1();
							if(hpuse1) chou1();
							if(hea[1]<=0){over=3;return;}
						}
						if(hand1.quality==2)
						{
							for(int i=1;i<=alive[1];i++) war[1][i].health=1;
							for(int i=1;i<=alive[2];i++) war[2][i].health=1;
						}
						if(hand1.quality==3)
						{
							for(int i=1;i<=alive[1];i++)
								if(war[1][i].attack==war[1][i].health) war[1][i]=nu;
							for(int i=1;i<=alive[2];i++)
								if(war[2][i].attack==war[2][i].health) war[2][i]=nu;
							int num=0;
							for(int i=1;i<=alive[1];i++)
								if(war[1][i].health) war[1][++num]=war[1][i];
							alive[1]=num;
							num=0;
							for(int i=1;i<=alive[2];i++)
								if(war[2][i].health) war[2][++num]=war[1][i];
							alive[2]=num;
						}
					}
					if(clas==6)
					{
						if(hand1.quality==1)
						{
							if(alive[1]<7)
							{
								war[1][++alive[1]]=nu;
								war[1][alive[1]].time=1;
								war[1][alive[1]].special=6;
							}
							chou1();
							if(hea[1]<=0){over=3;return;}
						}
						if(hand1.quality==2 && alive[2])
						{
							war[2][alive[2]].health=0;
							war[2][alive[2]].attack=0;
						}
						if(hand1.quality==3)
						{
							hea[1]=hea[2]=30;
						}
					}
					if(clas==7)
					{
						if(hand1.quality==1)
						{
							chou1();chou1();
							if(hea[1]<=0){over=3;return;}
							if(tot2<30)
							{
								tot2++;
								int t=0,k=2,i=--top2;
								deck[k][i].quality=4;
								deck[k][i].attack=rand()%10+1;
								deck[k][i].health=rand()%10+1;
								deck[k][i].cost=rand()%10+1;
								deck[k][i].spell=0;
								while(deck[k][i].attack<deck[k][i].cost-4) deck[k][i].attack=rand()%10+1;
								while(deck[k][i].health<deck[k][i].cost-4) deck[k][i].health=rand()%10+1;
								deck[k][i].time=rand()%3+2;
								if(deck[k][i].health<deck[k][i].cost+1) deck[k][i].health=deck[k][i].cost+1;
								if(deck[k][i].health>10) deck[k][i].health=10;
								shuffle(top2,30,2);
								gotoxy(6,93);
								if(tot2>0) printf("%2d",tot2);
								else printf("%2d",-fa2);
							}
						}
						if(hand1.quality==2)
						{
							mana1-=tot1;
							for(int i=1;i<=num1;i++)
							{
								hand[1][i].cost--;
								if(hand[1][i].cost<0) hand[1][i].cost=0;
								hand[1][i].attack++;
								if(hand[1][i].attack>10) hand[1][i].attack=10;
								hand[1][i].health++;
								if(hand[1][i].health>10) hand[1][i].health=10;
							}
						}
						if(hand1.quality==3)
						{
							chou2();
							chou2();
							hea[1]+=8;
							if(hea[1]>maxhealth) hea[1]=maxhealth;
							if(hea[2]<=0){over=1;return;}
						}
					}
					if(clas==8)
					{
						if(hand1.quality==1)
						{
							chou1();
							if(hea[1]<=0){over=3;return;}
						}
						if(hand1.quality==2 && num2 && tot1<30)
						{
							int t=rand()%num2+1;
							deck[1][--top1]=hand[2][t];
							tot1++;
							for(int i=t+1;i<=num2;i++) hand[2][i-1]=hand[2][i];
							hand[2][num2]=nu;
							num2--;
						}
						if(hand1.quality==3)
						{
							for(int i=1;i<=alive[1];i++)
								war[1][i].attack+=hand1.level+1,
								war[1][i].health+=hand1.level+1;
						}
					}
					if(clas==9)
					{
						if(hand1.quality==1 && tot1>0)
						{
							int num=0,t=rand()%(tot1-top1+1)+top1;
							for(int i=top1;i<=tot1;i++) if(deck[1][i].spell) num=1;
							if(num)
							{
								while(!deck[1][t].spell) t=rand()%(tot1-top1+1)+top1;
								hand[1][++num1]=deck[1][t];
							}
						}
						if(hand1.quality==2 && alive[1]<7)
						{
							int num=0,t=rand()%(tot1-top1+1)+top1;
							for(int i=top1;i<=tot1;i++) if(deck[1][i].attack>=6) num=1;
							if(num)
							{
								while(deck[1][t].attack<6) t=rand()%(tot1-top1+1)+top1;
								war[1][++alive[1]]=deck[1][t];
							}
						}
						if(hand1.quality==3)
						{
							for(int i=1;i<=num1;i++)
							{
								if(tot1>=30) break;
								deck[1][--top1]=hand[1][i];
								tot1++;
							}
							shuffle(top1,30,1);
						}
					}

				}
				p1();p2();
				p3(1);p3(2);p4();
				fc1=0;
				p7();
			}
			if(xx>23 && xx<29 && !hpuse1 && yy>56 && yy<71)
			{
				if(clas==1)//语文
				{
					if(mana1<2) continue;
					mana1-=2;
					hpuse1=1;
					if(alive[1]==7) continue;
					war[1][++alive[1]]=nu;
					war[1][alive[1]].health=2;
					war[1][alive[1]].attack=1;
					war[1][alive[1]].time=1;
					war[1][alive[1]].cost=2;
					war[1][alive[1]].special=1;
					if(dotask1==1) war[1][alive[1]].health=6,war[1][alive[1]].attack=5;
					if(mode==4 && dotask1<1)
					{
						int num=0;
						for(int i=1;i<=alive[1];i++) if(war[1][i].special) num++;
						if(num>=2) dotask1=1;
						p8();
					}
					p1();p7();p3(1);p4();
				}
				if(clas==2)//数学
				{
					if(mana1<4) continue;
					mana1-=4;
					if(mode==4 && clas==2 && dotask1==0 && mana1==5) dotask1=-1,p8();
					hpuse1=1;
					if(tma1<10) tma1++;
					else
					{
						p6(0,1);
						if(tot1>0)
						{
							//flag[1][top1]=0;
							tot1--;
							if(num1<10)
							{
								hand[1][++num1]=deck[1][top1];
								deck[1][top1]=nu;
								if(mode==4 && dotask1)
								{
									hand[1][num1].cost-=4;
									if(hand[1][num1].cost<0) hand[1][num1].cost=0;
								}
							}
							else p6(top1,1);
							top1++;
						}
						else fa1++,hea[1]-=fa1;
						p4();
						gotoxy(26,93);
						if(tot1>0) printf("%2d",tot1);
						else printf("%2d",-fa1);
						p1();
					}
					if(hea[1]<=0){over=3;return;}
					p1();p7();p3(1);p4();
				}
				if(clas==3)//english
				{
					en_sum++;
					if(en_sum>=35 || mode==4 && en_sum>=30)
					{
						hea[2]=0;en_sum=0;
						p4();
						over=1;return;
					}
					if(en_sum==1) MessageBox(NULL,"No skill. Please don't try.","Warning!",MB_OK);
					if(en_sum==2) MessageBox(NULL,"It is actually no skill.","Warning!",MB_OK);
					if(en_sum==3) MessageBox(NULL,"Why don't you believe me?","Warning!",MB_OK);
					if(en_sum==4) MessageBox(NULL,"Give up!There is nothing uesful!","Warning!",MB_OK);
					if(en_sum==5) MessageBox(NULL,"If you don't stop,you will regret doing this.","Warning!",MB_OK);
					if(en_sum==6) MessageBox(NULL,"Back to game!Time is up!","Warning!",MB_OK);
					if(en_sum==7) MessageBox(NULL,"I am angry,and you will be punished soon.","Warning!",MB_OK);
					if(en_sum==8) MessageBox(NULL,"You're a reckless person,and recklessness is worng.","Warning!",MB_OK);
					if(en_sum==9) MessageBox(NULL,"I have warned.This is your fault.","Warning!",MB_OK);
					if(en_sum==10) MessageBox(NULL,"Don't grudge me.It's none of my business.","Warning!",MB_OK);
					if(en_sum>10)
					{
						gotoxy(16,95);
						printf("   ");
						ow=0;
					}
				}
				if(clas==4)//物理
				{
					if(mana1<1) continue;
					mana1-=1;
					hpuse1=1;
					tma1--;
					if(alive[1]==7) continue;
					war[1][++alive[1]]=nu;
					war[1][alive[1]].time=1;
					war[1][alive[1]].attack=3;
					war[1][alive[1]].health=3;
					if(mode==4 && dotask1<6)
					{
						dotask1++;
						if(dotask1==6)
						{
							mana1=10;
							tma1=10;
							p7();
						}
						p8();
					}
					p1();p7();p3(1);p4();
				}
				if(clas==5)//化学
				{
					if(mana1<2) continue;
					mana1-=2;
					hpuse1=1;
					for(int i=1;i<=alive[1];i++)
						swap(war[1][i].attack,war[1][i].health);
					for(int i=1;i<=alive[2];i++)
						swap(war[2][i].attack,war[2][i].health);
					if(mode==4 && alive[2]>=2 && dotask1<1)
					{
						int ok=1;
						for(int i=1;i<=alive[2];i++) if(war[2][i].attack!=1) ok=0;
						if(ok==1)
						{
							dotask1=1;
							for(int i=1;i<=num2;i++) hand[2][i].attack=1;
							for(int i=1;i<=tot2;i++) deck[2][i+top2-1].attack=1;
							p8();
						}
					}
					p1();p7();p3(1);p4();p3(2);
				}
				if(clas==6)//生物
				{
					if(mana1<1) continue;
					mana1-=1;
					hpuse1=1;
					if(alive[1]==7) continue;
					war[1][++alive[1]]=nu;
					war[1][alive[1]].time=1;
					war[1][alive[1]].special=6;
					if(mode==4 && dotask1>=20)
					{
						for(int i=1;i<=2;i++)
						{
							if(alive[1]==7) break;
							war[1][++alive[1]]=nu;
							war[1][alive[1]].time=1;
							war[1][alive[1]].special=6;
						}
						dotask1=20;
						p8();
					}
					p7();p3(1);p4();
				}
				if(clas==7)//政治
				{
					if(mana1<2) continue;
					mana1-=2;
					hpuse1=1;
					tot1=0;top1=30;
					tot2=0;top2=30;
					gotoxy(26,93);
					if(tot1>0) printf("%2d",tot1);
					else printf("%2d",-fa1);
					gotoxy(6,93);
					if(tot2>0) printf("%2d",tot2);
					else printf("%2d",-fa2);
					p1();p2();p4();
					p7();p3(1);p3(2);
				}
				if(clas==8)//历史
				{
					if(mana1<2) return;
					mana1-=2;
					hpuse1=1;
					if(num1)
					{
						int t=rand()%num1+1;
						if(mode==4 && dotask1<3 && hand[1][t].quality>1) dotask1++,p8();
						hand[1][t]=nu;
						for(int i=t+1;i<=num1;i++) hand[1][i-1]=hand[1][i];
						hand[1][num1]=nu;
						num1--;
						for(int i=1;i<=num1;i++)
							if(hand[1][i].spell && hand[1][i].quality==3) hand[1][i].level++;
						int y1=(t-1)*9+3;
						gotoxy(30,y1);
						printf("         ");
						gotoxy(31,y1);
						printf("         ");
						gotoxy(32,y1);
						printf("         ");
						gotoxy(33,y1);
						printf("         ");
						gotoxy(34,y1);
						printf("         ");
						Sleep(250);
						p1();
						Sleep(250);
					}
					chou1();
					if(hea[1]<=0){over=3;return;}
					p1();p2();p4();
					p7();p3(1);
				}
				if(clas==9)//地理
				{
					if(mana1<5) continue;
					mana1-=5;
					hpuse1=1;
					if(alive[1]==7 || tot1==0) continue;
					//flag[1][top1]=0;
					tot1--;
					war[1][++alive[1]]=deck[1][top1];
					deck[1][top1]=nu;
					top1++;
					if(mode==4 && dotask1<3 && war[1][alive[1]].attack<=3)
					{
						dotask1++;
						if(dotask1==3)
							for(int i=1;i<=3;i++)
							{
								if(alive[1]==7 || tot1==0) break;
								//flag[1][top1]=0;
								tot1--;
								war[1][++alive[1]]=deck[1][top1];
								deck[1][top1]=nu;
								top1++;
							}
						p8();
					}
					gotoxy(26,93);
					if(tot1>0) printf("%2d",tot1);
					else printf("%2d",-fa1);
					p1();p2();p4();
					p7();p3(1);
				}
			}
			if(xx<17 && xx>10 && yy>70 && yy<105)
			{
				gotoxy(16,95);
				printf("   ");
				ow=0;
			}
			Sleep(400);
		}
	}
	end(1);
}
void opponent()//对手回合
{
	if(mode==4) p8();
	if(tma2<10) tma2++;
	mana2=tma2;
	chou2();
	if(hea[2]<=0){over=1;return;}
	fc2=1;
	gotoxy(4,11);
	printf("%2d/%2d",mana2,tma2);
	p2();
	//AI
	double dpf[20]={0};
	int ow=1,hpuse2=0;
	while(ow)
	{
		double maxx=-1;
		int fff=0;
		if(alive[2]==7) break;
		for(int i=1;i<=num2;i++)
		{
			if(!hand[2][i].spell)
				dpf[i]=(1.0*hand[2][i].attack*(hand[2][i].attack-1)+hand[2][i].health)/(hand[2][i].cost);
			else
			{
				if(hand[2][i].quality==1)
				{
					dpf[i]=4.1;
					if(num2==10 || alive[2]==7) dpf[i]=0;
					if(tot2<=0) dpf[i]=1;
				}
				if(hand[2][i].quality==2 && alive[1]) dpf[i]=war[1][1].health;
				if(hand[2][i].quality==3)
				{
					if(mode==4 && dotask2==6) dpf[i]=7;
					else dpf[i]=3;
				}
			}
			if(maxx<dpf[i] && mana2>=hand[2][i].cost) maxx=dpf[i],fff=i;
		}
		if((fff==0 && hpuse2<maxhp2 && mana2>=2 && alive[2]<7)
		||((maxx<=4 || mode==4 && dotask2==6 && maxx<=13) && hpuse2<maxhp2 && mana2>=2 && alive[2]<7))
		{
			mana2-=2;
			hpuse2++;
			war[2][++alive[2]]=nu;
			int t=rand()%4+2;
			war[2][alive[2]].attack=war[2][alive[2]].health=t;
			war[2][alive[2]].time=1;
			if(chapter==9) war[2][alive[2]].quality=3,war[2][alive[2]].death=t;
			else war[2][alive[2]].quality=1;
			if(mode==4 && dotask2==6) war[2][alive[2]].attack=war[2][alive[2]].health=5,t=5;
			if(mode==4 && dotask2<6) dotask2++,p8();
			p5_1(t);p3(2);p7();Sleep(2500);
			continue;
		}
		if(fff==0) break;
		if(hand[2][fff].spell)
		{
			mana2-=hand[2][fff].cost;
			if(hand[2][fff].quality==1)
			{
				war[2][++alive[2]]=nu;
				int t=rand()%4+2;
				war[2][alive[2]].attack=war[2][alive[2]].health=t;
				war[2][alive[2]].time=1;
				war[2][alive[2]].quality=1;
				chou2();
				if(hea[2]<=0){over=1;return;}
				p3(2);p7();
			}
			if(hand[2][fff].quality==2)
			{
				war[1][1]=nu;
				for(int i=2;i<=alive[1];i++) war[1][i-1]=war[1][i];
				war[1][alive[1]]=nu;
				alive[1]--;
				p3(1);
			}
			if(hand[2][fff].quality==3)
			{
				maxhp2++;
				gotoxy(3,63);
				printf("+%d",maxhp2-1);
			}
		}
		else
		{
			war[2][++alive[2]]=hand[2][fff];
			mana2-=hand[2][fff].cost;
			hea[2]+=hand[2][fff].treat;
			if(hea[2]>maxhealth) hea[2]=maxhealth;
		}
		int y1=fff*8+2;
		gotoxy(0,y1);
		printf("        ");
		gotoxy(1,y1);
		printf("        ");
		gotoxy(2,y1);
		printf("        ");
	//	gotoxy(3,y1);
	//	printf("        ");
		p5(fff);
		p3(2);
		gotoxy(4,11);
		printf("%2d/%2d",mana2,tma2);
		Sleep(1500);
		y1=num2*8+2;
		gotoxy(0,y1);
		printf("         ");
		gotoxy(1,y1);
		printf("         ");
		gotoxy(2,y1);
		printf("         ");
	//	gotoxy(3,y1);
	//	printf("         ");
		hand[2][fff]=nu;
		for(int i=fff+1;i<=num2;i++) hand[2][i-1]=hand[2][i];
		hand[2][num2]=nu;
		num2--;
		p2();
		Sleep(1500);
		fc2=0;
	}
	end(2);
}
void game()//进行游戏
{
	while(!over)
	{
		your();
		if(over) break;
		opponent();
	}
	gotoxy(15,45);
	if(over==1) printf("Victory!");
	if(mode!=3 && over==1) win[clas]++,twin++;
	if(over==3) printf(" Fail!");
	if(mode!=3 && over==3) lose[clas]++,tlose++;
	if(over==2) printf("Evened!");
	if(mode==3 && over==1) ac[chapter]=1;
	data=fopen("data.txt","w");

	for(int i=1;i<=9;i++) fprintf(data,"%d %d\n",win[i],lose[i]);
	fprintf(data,"%d %d\n",twin,tlose);
	for(int i=1;i<=9;i++) fprintf(data,"%d\n",ac[i]);
	fputs(name,data);
	fclose(data);

	gotoxy(17,42);
	pause();
}
void create1()//创建竞技模式卡池
{
	node card0;
	int t=rand()%100;
	if(t==0) card0.quality=4;
	else if(t<=5) card0.quality=3;
	else if(t>=80) card0.quality=2;
	else card0.quality=1;
	for(int i=1;i<=3;i++)
	{
		t=rand()%100;if(card0.quality==4) t=99;
		if(t<20) card[i].spell=1;
		else card[i].spell=0;
		if(card[i].spell)
		{
			card[i].quality=card0.quality;
			card[i].cost=spcost[(clas-1)*3+card[i].quality];
		}
		else
		{
			card[i].quality=card0.quality;
			card[i].attack=rand()%10+1;
			card[i].health=rand()%10+1;
			card[i].cost=rand()%10+1;
			card[i].time=1;
			if(card[i].quality==4)
			{
				card[i].time=rand()%3+2;
				if(card[i].health<card[i].cost+1) card[i].health=card[i].cost+1;
				if(card[i].health>10) card[i].health=10;
			}
			if(card[i].quality==3)
			{
				card[i].death=rand()%6+2;
				int t1=card[i].attack,t2=card[i].health;
				card[i].attack=card[i].health=(t1+t2+1)/2;
			}
			if(card[i].quality==2)
			{
				card[i].treat=rand()%5+1;
				if(card[i].attack>card[i].cost*2) card[i].attack=card[i].cost*2+1;
				if(card[i].health>card[i].cost*2) card[i].health=card[i].cost*2+1;
			}
			if(card[i].quality==1)
			{
				while(card[i].attack>card[i].cost*2) card[i].attack=rand()%10+1;
				while(card[i].health>card[i].cost*2) card[i].health=rand()%10+1;
			}
			if(card[i].attack<card[i].cost-4) card[i].attack=card[i].cost-4;
			if(card[i].health<card[i].cost-4) card[i].health=card[i].cost-4;
		}
	}
}
void deck_copy()//复制套牌
{
	for(int i=1;i<=tot1;i++) deck[1][i]=card1[i];
	for(int i=1;i<=tot2;i++) deck[2][i]=card1[i];
	for(int i=1;i<=tot2;i++) if(deck[2][i].spell) deck[2][i].cost=3;
}
void init()//初始化游戏
{
	memset(deck,0,sizeof deck);
	memset(hand,0,sizeof hand);
	memset(war,0,sizeof war);
	memset(alive,0,sizeof alive);
	//memset(flag,0,sizeof flag);
	fc1=fc2=num1=num2=tot1=tot2=0;
	tot1=30;
	tot2=30;
	tma1=tma2=fa1=fa2=0;
	mana1=mana2=0;
	if(mode==1 || mode==4) create(1),create(2);
	if(mode==2) deck_copy();
	if(mode==3)
	{
		if(chapter==1) tot1=20,create(1),create(2);
		else if(chapter==2) tot1=tot2=10,create(1),create(2);
		else create(1),create(2);
	}
	over=0;
	dotask1=0;dotask2=0;
	hpuse1=hpuse2=0;maxhp2=1;
	hea[1]=hea[2]=maxhealth=30;
	en_sum=0;
}
void onegame()//进入一场游戏
{
	init();
	shuffle(1,tot1,1);
	shuffle(1,tot2,2);
	begin();
	start();
	game();
}
void rand_mode()//随机模式
{
	system("cls");chapter=0;
	gotoxy(2,0);
	printf("     欢迎来到随机模式！        返回主界面");
	gotoxy(4,0);
	printf("        请选择职业：\n");
	gotoxy(6,10);printf("%s",occu[1]);
	gotoxy(6,30);printf("%s",occu[2]);
	gotoxy(6,50);printf("%s",occu[3]);
	gotoxy(8,10);printf("%s",occu[4]);
	gotoxy(8,30);printf("%s",occu[5]);
	gotoxy(8,50);printf("%s",occu[6]);
	gotoxy(10,10);printf("%s",occu[7]);
	gotoxy(10,30);printf("%s",occu[8]);
	gotoxy(10,50);printf("%s",occu[9]);
	gotoxy(13,6);printf("（%3.0lf%%）2语文：召唤一个1-2的“书籍”。",1.0*win[1]/(win[1]+lose[1])*100);
	gotoxy(14,6);printf("（%3.0lf%%）4数学：获得一个空的法力水晶，到达10点法力水晶后改抽一张牌。",1.0*win[2]/(win[2]+lose[2])*100);
	gotoxy(15,6);printf("（%3.0lf%%）0英语：This hero power is unknown.",1.0*win[3]/(win[3]+lose[3])*100);
	gotoxy(16,6);printf("（%3.0lf%%）1物理：摧毁你的一个法力水晶上限，召唤一个3-3的“秒表”。",1.0*win[4]/(win[4]+lose[4])*100);
	gotoxy(17,6);printf("（%3.0lf%%）2化学：使所有随从交换生命值和攻击力。",1.0*win[5]/(win[5]+lose[5])*100);
	gotoxy(18,6);printf("（%3.0lf%%）1生物：召唤一个0-0的“细胞”，它只能存活一个回合。",1.0*win[6]/(win[6]+lose[6])*100);
	gotoxy(19,6);printf("（%3.0lf%%）2政治：摧毁双方牌库。",1.0*win[7]/(win[7]+lose[7])*100);
	gotoxy(20,6);printf("（%3.0lf%%）2历史：弃掉一张手牌然后抽一张牌。",1.0*win[8]/(win[8]+lose[8])*100);
	gotoxy(21,6);printf("（%3.0lf%%）5地理：从你的牌库里将一个随从置入战场。",1.0*win[9]/(win[9]+lose[9])*100);
	gotoxy(22,6);printf("（%3.0lf%%）2学生：随机召唤一个攻击力和生命值相等的随从(2~5)。",1.0*tlose/(twin+tlose)*100);
	int ow=1;
	while(ow)
	{
		if(KEY_DOWN(MOUSE_MOVED))
		{
			search();
			change(X,Y);
			if(xx==2) return;
			if(xx==6 && (yy>= 8 && yy<=13)) clas=1,ow=0;
			if(xx==6 && (yy>=29 && yy<=34)) clas=2,ow=0;
			if(xx==6 && (yy>=50 && yy<=55)) clas=3,ow=0;
			if(xx==8 && (yy>= 8 && yy<=13)) clas=4,ow=0;
			if(xx==8 && (yy>=29 && yy<=34)) clas=5,ow=0;
			if(xx==8 && (yy>=50 && yy<=55)) clas=6,ow=0;
			if(xx==10&& (yy>= 8 && yy<=13)) clas=7,ow=0;
			if(xx==10&& (yy>=29 && yy<=34)) clas=8,ow=0;
			if(xx==10&& (yy>=50 && yy<=55)) clas=9,ow=0;
			Sleep(150);
		}
	}
	gotoxy(0,0);
	onegame();
}
void print_card(int x,int y,int k)//输出竞技卡牌
{
	if(k==0)
	{
		gotoxy(x,y-1); printf("          ");
		gotoxy(++x,y-1); printf("          ");
		gotoxy(++x,y-1); printf("          ");
		gotoxy(++x,y-1); printf("          ");
		gotoxy(++x,y-1); printf("          ");
		return;
	}
	gotoxy(x,y-1);
	printf("%2d ==",card[k].cost);
	gotoxy(++x,y);
	printf("[");
	if(card[k].quality==1) printf("白]");
	if(card[k].quality==2) printf("蓝]");
	if(card[k].quality==3) printf("紫]");
	if(card[k].quality==4) printf("橙]");
	gotoxy(++x,y);
	if(!card[k].spell)
	{
		if(card[k].quality==1) printf("    ");
		if(card[k].quality==2) printf("B:%d",card[k].treat);
		if(card[k].quality==3) printf("D:%d",card[k].death);
		if(card[k].quality==4) printf("A:%d",card[k].time);
	}
	else printf("    ");
	gotoxy(++x,y-1);
	if(!card[k].spell) printf("%2d %2d",card[k].attack,card[k].health);
	else printf(" 法术 ");
	gotoxy(++x,y);
	printf("====");
}
void xuanpai()//竞技模式选牌
{
	system("cls");
	gotoxy(2,10);
	printf("你选择的职业为：%s",occu[clas]);
	gotoxy(4,8);
	printf("请在以下卡牌中选取30张牌作为你的套牌！");
	gotoxy(5,8);
	printf("请谨慎选择！你的对手也会更强！");
	gotoxy(7,10);
	if(clas==1) printf("2普通：使你的所有“书籍”获得+2+1");
	if(clas==2) printf("2普通：召唤一个2-3的“直尺”");
	if(clas==3) printf("0common: I only know this is a common cell card.");
	if(clas==4) printf("0普通：仅在本回合，获得一点法力水晶，且不能超过10点");
	if(clas==5) printf("2普通：抽一张牌；如果本回合使用过英雄技能，则改为抽两张牌");
	if(clas==6) printf("1普通：召唤一个“细胞”抽一张牌");
	if(clas==7) printf("2普通：抽2张牌，将一张传说卡牌洗入你对手的牌库");
	if(clas==8) printf("1普通：抽一张牌；如果该牌被弃置，则自动释放");
	if(clas==9) printf("1普通：从你的牌库里将1张法术牌的复制置入你的手牌");
	gotoxy(8,10);
	if(clas==1) printf("2稀有：你每控制一个“书籍”，抽一张牌");
	if(clas==2) printf("0稀有：消耗你的所有法力水晶，然后每消耗2点法力水晶便抽一张牌");
	if(clas==3) printf("1rare: I only know this is a rare cell card.");
	if(clas==4) printf("2稀有：你每控制一个“秒表”，抽一张牌");
	if(clas==5) printf("2稀有：使所有随从的生命值变为1");
	if(clas==6) printf("3稀有：使敌方最右侧随从变为0-0");
	if(clas==7) printf("0稀有：使你的所有手牌获得-1/+1+1，你的牌库每有一张牌，该牌的法力值消耗便增加1点");
	if(clas==8) printf("2稀有：将对手的一张手牌洗入你的牌库");
	if(clas==9) printf("6稀有：召唤你牌库中一个6攻及以上的随从的复制");
	gotoxy(9,10);
	if(clas==1) printf("2史诗：使你的所有书籍返回你的手牌");
	if(clas==2) printf("6史诗：将你的法力水晶变为等量的2-2的“直尺”");
	if(clas==3) printf("3epic: I only know this is an epic cell card.");
	if(clas==4) printf("2史诗：摧毁你的所有法力水晶，召唤一个生命值不小于6的传说随从“光”，在该随从死亡后复原你的法力水晶");
	if(clas==5) printf("3史诗：移除所有攻击力等同于其生命值的随从");
	if(clas==6) printf("5史诗：使双方英雄变为30血");
	if(clas==7) printf("3史诗：你的对手抽2张牌，为你的英雄恢复8点生命值");
	if(clas==8) printf("2史诗：使你的所有随从获得+1+1（弃掉1张牌后升级）");
	if(clas==9) printf("2史诗：将你手牌里的所有随从牌的复制洗入你的牌库");

	gotoxy(5,65);
	printf("已选卡牌：");
	gotoxy(20,8);
	printf("总计：");
	for(int i=1;i<=10;i++)
		gotoxy(20+i,8),printf("%2d : ",i);
	int jilu[20];
	for(int i=1;i<=10;i++) jilu[i]=0;
	int choose=0;
	for(int i=1;i<=30;i++)
	{
		gotoxy(5,75);printf("共%2d张",i-1);
		create1();
		print_card(12,18,1);
		print_card(12,32,2);
		print_card(12,46,3);
		int ow=1;
		while(ow)
		{
			if(KEY_DOWN(MOUSE_MOVED))
			{
				search();
				change(X,Y);
				if(xx>11 && xx<17 && yy>17 && yy<22) choose=1,ow=0;
				if(xx>11 && xx<17 && yy>31 && yy<36) choose=2,ow=0;
				if(xx>11 && xx<17 && yy>45 && yy<50) choose=3,ow=0;
				if(!ow)
				{
					print_card(12,14,0);
					print_card(12,28,0);
					print_card(12,42,0);
				}
				Sleep(750);
			}
		}
		card1[i]=card[choose];
		gotoxy(5+i,75);
		if(card[choose].spell) printf("%2d/      法术",card[choose].cost);
		else
		{
			printf("%2d/%2d-%2d ",card[choose].cost,card[choose].attack,card[choose].health);
			if(card[choose].quality==2) printf("  B : %d",card[choose].treat);
			if(card[choose].quality==3) printf("  D : %d",card[choose].death);
			if(card[choose].quality==4) printf("  A : %d",card[choose].time);
		}
		gotoxy(5,75);printf("共%2d张",i);
		jilu[card[choose].cost]++;
		int t=card[choose].cost;
		gotoxy(20+t,14);printf("%2d",jilu[t]);
	}
	gotoxy(32,8);
	pause();
}
void arena_mode()//竞技模式
{
	system("cls");chapter=0;
	gotoxy(2,0);
	printf("     欢迎来到竞技模式！        返回主界面");
	gotoxy(4,0);
	printf("        请选择职业：         继续上个游戏\n");
	gotoxy(6,10);printf("%s",occu[1]);
	gotoxy(6,30);printf("%s",occu[2]);
	gotoxy(6,50);printf("%s",occu[3]);
	gotoxy(8,10);printf("%s",occu[4]);
	gotoxy(8,30);printf("%s",occu[5]);
	gotoxy(8,50);printf("%s",occu[6]);
	gotoxy(10,10);printf("%s",occu[7]);
	gotoxy(10,30);printf("%s",occu[8]);
	gotoxy(10,50);printf("%s",occu[9]);
	gotoxy(13,6);printf("（%3.0lf%%）2语文：召唤一个1-2的“书籍”。",1.0*win[1]/(win[1]+lose[1])*100);
	gotoxy(14,6);printf("（%3.0lf%%）4数学：获得一个空的法力水晶，到达10点法力水晶后改抽一张牌。",1.0*win[2]/(win[2]+lose[2])*100);
	gotoxy(15,6);printf("（%3.0lf%%）0英语：This hero power is unknown.",1.0*win[3]/(win[3]+lose[3])*100);
	gotoxy(16,6);printf("（%3.0lf%%）1物理：摧毁你的一个法力水晶上限，召唤一个3-3的“秒表”。",1.0*win[4]/(win[4]+lose[4])*100);
	gotoxy(17,6);printf("（%3.0lf%%）2化学：使所有随从交换生命值和攻击力。",1.0*win[5]/(win[5]+lose[5])*100);
	gotoxy(18,6);printf("（%3.0lf%%）1生物：召唤一个0-0的“细胞”，它只能存活一个回合。",1.0*win[6]/(win[6]+lose[6])*100);
	gotoxy(19,6);printf("（%3.0lf%%）2政治：摧毁双方牌库。",1.0*win[7]/(win[7]+lose[7])*100);
	gotoxy(20,6);printf("（%3.0lf%%）2历史：弃掉一张手牌然后抽一张牌。",1.0*win[8]/(win[8]+lose[8])*100);
	gotoxy(21,6);printf("（%3.0lf%%）5地理：从你的牌库里将一个随从置入战场。",1.0*win[9]/(win[9]+lose[9])*100);
	gotoxy(22,6);printf("（%3.0lf%%）2学生：随机召唤一个攻击力和生命值相等的随从(2~5)。",1.0*tlose/(twin+tlose)*100);
	int ow=1,conti=0;
	while(ow)
	{
		if(KEY_DOWN(MOUSE_MOVED))
		{
			search();
			change(X,Y);
			if(xx==2) return;
			if(xx==4 && yy>=26 && yy<=39 && clas) conti=1,ow=0;
			if(xx==6 && yy>= 8 && yy<=13) clas=1,ow=0;
			if(xx==6 && yy>=29 && yy<=34) clas=2,ow=0;
			if(xx==6 && yy>=50 && yy<=55) clas=3,ow=0;
			if(xx==8 && yy>= 8 && yy<=13) clas=4,ow=0;
			if(xx==8 && yy>=29 && yy<=34) clas=5,ow=0;
			if(xx==8 && yy>=50 && yy<=55) clas=6,ow=0;
			if(xx==10&& yy>= 8 && yy<=13) clas=7,ow=0;
			if(xx==10&& yy>=29 && yy<=34) clas=8,ow=0;
			if(xx==10&& yy>=50 && yy<=55) clas=9,ow=0;
			Sleep(500);
		}
	}
	if(!conti) xuanpai();
	onegame();
}
void adven_mode()//冒险模式
{
	system("cls");
	gotoxy(2,0);
	printf("       欢迎来到冒险模式！        返回主界面");
	gotoxy(4,0);
	printf("          请选择关卡：\n");
	gotoxy(6,8); if(ac[1]) printf("√");else printf("×");
	printf("验收：你的牌库只有20张卡牌。");
	gotoxy(8,8); if(ac[2]) printf("√");else printf("×");
	printf("月考：双方牌库只有10张卡牌。");
	gotoxy(10,8);if(ac[3]) printf("√");else printf("×");
	printf("期中：你没有奇数费用的随从。");
	gotoxy(12,8);if(ac[4]) printf("√");else printf("×");
	printf("期末：对手所有随从减少1费。");
	gotoxy(14,8);if(ac[5]) printf("√");else printf("×");
	printf("结课：对局开始时弃掉你的起始手牌。");
	gotoxy(16,8);if(ac[6]) printf("√");else printf("×");
	printf("一模：你的随从对敌方英雄造成伤害最多为2。");
	gotoxy(18,8);if(ac[7]) printf("√");else printf("×");
	printf("二模：你的所有随从都为普通卡。");
	gotoxy(20,8);if(ac[8]) printf("√");else printf("×");
	printf("三模：对手的所有随从都不为普通卡。");
	gotoxy(22,8);if(ac[9]) printf("√");else printf("×");
	printf("高考：对手的所有随从都为传说卡，英雄技能召唤的随从变为史诗级。");
	gotoxy(24,8);
	int ow=1;
	while(ow)
	{
		if(KEY_DOWN(MOUSE_MOVED))
		{
			search();
			change(X,Y);
			if(xx== 2) return;
			if(xx== 6) chapter=1,ow=0;
			if(xx== 8) chapter=2,ow=0;
			if(xx==10) chapter=3,ow=0;
			if(xx==12) chapter=4,ow=0;
			if(xx==14) chapter=5,ow=0;
			if(xx==16) chapter=6,ow=0;
			if(xx==18) chapter=7,ow=0;
			if(xx==20) chapter=8,ow=0;
			if(xx==22) chapter=9,ow=0;
			Sleep(500);
		}
	}
	system("cls");
	gotoxy(2,0);
	printf("     你选择了第%d关！        返回主界面",chapter);
	gotoxy(4,0);
	printf("        请选择职业：\n");
	gotoxy(6,10);printf("%s",occu[1]);
	gotoxy(6,30);printf("%s",occu[2]);
	gotoxy(6,50);printf("%s",occu[3]);
	gotoxy(8,10);printf("%s",occu[4]);
	gotoxy(8,30);printf("%s",occu[5]);
	gotoxy(8,50);printf("%s",occu[6]);
	gotoxy(10,10);printf("%s",occu[7]);
	gotoxy(10,30);printf("%s",occu[8]);
	gotoxy(10,50);printf("%s",occu[9]);
	gotoxy(13,8);printf("2语文：召唤一个1-2的“书籍”。");
	gotoxy(14,8);printf("4数学：获得一个空的法力水晶，到达10点法力水晶后改抽一张牌。");
	gotoxy(15,8);printf("0英语：This hero power is unknown.");
	gotoxy(16,8);printf("1物理：摧毁你的一个法力水晶上限，召唤一个3-3的“秒表”。");
	gotoxy(17,8);printf("2化学：使所有随从交换生命值和攻击力。");
	gotoxy(18,8);printf("1生物：召唤一个0-0的“细胞”，它只能存活一个回合。");
	gotoxy(19,8);printf("2政治：摧毁双方牌库。");
	gotoxy(20,8);printf("2历史：弃掉一张手牌然后抽一张牌。");
	gotoxy(21,8);printf("5地理：从你的牌库里将一个随从置入战场。");
	gotoxy(22,8);printf("2学生：随机召唤一个攻击力和生命值相等的随从(2~5)。");
	ow=1;
	while(ow)
	{
		if(KEY_DOWN(MOUSE_MOVED))
		{
			search();
			change(X,Y);
			if(xx==2) {chapter=0;return;}
			if(xx==6 && yy>= 8 && yy<=13) clas=1,ow=0;
			if(xx==6 && yy>=29 && yy<=34) clas=2,ow=0;
			if(xx==6 && yy>=50 && yy<=55) clas=3,ow=0;
			if(xx==8 && yy>= 8 && yy<=13) clas=4,ow=0;
			if(xx==8 && yy>=29 && yy<=34) clas=5,ow=0;
			if(xx==8 && yy>=50 && yy<=55) clas=6,ow=0;
			if(xx==10&& yy>= 8 && yy<=13) clas=7,ow=0;
			if(xx==10&& yy>=29 && yy<=34) clas=8,ow=0;
			if(xx==10&& yy>=50 && yy<=55) clas=9,ow=0;
			Sleep(500);
		}
	}
	gotoxy(0,0);
	onegame();
}
void task_mode()//任务模式
{
	system("cls");
	gotoxy(2,0);
	printf("       欢迎来到任务模式！        返回主界面");
	gotoxy(4,0);
	printf("        请选择职业：\n");
	gotoxy(6,10);printf("%s",occu[1]);
	gotoxy(6,30);printf("%s",occu[2]);
	gotoxy(6,50);printf("%s",occu[3]);
	gotoxy(8,10);printf("%s",occu[4]);
	gotoxy(8,30);printf("%s",occu[5]);
	gotoxy(8,50);printf("%s",occu[6]);
	gotoxy(10,10);printf("%s",occu[7]);
	gotoxy(10,30);printf("%s",occu[8]);
	gotoxy(10,50);printf("%s",occu[9]);
	gotoxy(13,10);printf("语文：任务：你同时控制两个书籍");
	gotoxy(14,10);printf("      奖励：新召唤的“书籍”改为5-6的身材");
	gotoxy(15,10);printf("数学：任务：在剩余法力水晶从未到达5的情况下最大法力水晶到达10点");
	gotoxy(16,10);printf("      奖励：你的英雄技能抽的牌法力值消耗减少4点");
	gotoxy(17,10);printf("英语：任务：Sorry,I don't know what this is.                        ");
	gotoxy(18,10);printf("      奖励：I don't know what this is,either.");
	gotoxy(19,10);printf("物理：任务：摧毁6个法力水晶上限");
	gotoxy(20,10);printf("      奖励：获得10点法力水晶（包括剩余和上限）");
	gotoxy(21,10);printf("化学：任务：在使用英雄技能后导致对手所有随从变为1点攻且随从数多于1");
	gotoxy(22,10);printf("      奖励：对手随从牌的攻击力变为1点");
	gotoxy(23,10);printf("生物：任务：细胞承受了20点伤害");
	gotoxy(24,10);printf("      奖励：你的英雄技能改为召唤3个细胞");
	gotoxy(25,10);printf("政治：任务：受到6点疲劳伤害");
	gotoxy(26,10);printf("      奖励：将6张传说牌置入你的牌库");
	gotoxy(27,10);printf("历史：任务：弃掉3张非普通品质的卡牌");
	gotoxy(28,10);printf("      奖励：每个回合开始时召唤1个3-3的随从");
	gotoxy(29,10);printf("地理：任务：招募3个3攻及以下的随从");
	gotoxy(30,10);printf("      奖励：招募3个随从");
	gotoxy(31,10);printf("学生：任务：使用6次英雄技能");
	gotoxy(32,10);printf("      奖励：英雄技能永远召唤5-5的随从");
	int ow=1;
	while(ow)
	{
		if(KEY_DOWN(MOUSE_MOVED))
		{
			search();
			change(X,Y);
			if(xx==2) {chapter=0;return;}
			if(xx==6 && yy>= 8 && yy<=13) clas=1,ow=0;
			if(xx==6 && yy>=29 && yy<=34) clas=2,ow=0;
			if(xx==6 && yy>=50 && yy<=55) clas=3,ow=0;
			if(xx==8 && yy>= 8 && yy<=13) clas=4,ow=0;
			if(xx==8 && yy>=29 && yy<=34) clas=5,ow=0;
			if(xx==8 && yy>=50 && yy<=55) clas=6,ow=0;
			if(xx==10&& yy>= 8 && yy<=13) clas=7,ow=0;
			if(xx==10&& yy>=29 && yy<=34) clas=8,ow=0;
			if(xx==10&& yy>=50 && yy<=55) clas=9,ow=0;
			Sleep(500);
		}
	}
	gotoxy(0,0);
	onegame();
}
void mainmenu()//主界面
{
	system("cls");
	gotoxy(0,0);
	printf("\n\n        欢迎来到《炉小石》—随机纪元\n\n\n");
	printf("\n            点击进入随机模式\n");
	printf("\n            点击进入竞技模式\n");
	printf("\n            点击进入冒险模式\n");
	printf("\n            点击进入任务模式\n");
	printf("\n            更多模式即将开启\n");
	printf("\n            作者\n");
	int ow=1;
	while(ow)
	{
		if(KEY_DOWN(MOUSE_MOVED))
		{
			search();
			change(X,Y);
			if(xx==6) mode=1,ow=0;
			if(xx==8) mode=2,ow=0;
			if(xx==10) mode=3,ow=0;
			if(xx==12) mode=4,ow=0;
			Sleep(300);
		}
	}
	if(mode==1) rand_mode();
	if(mode==2) arena_mode();
	if(mode==3) adven_mode();
	if(mode==4) task_mode();
}
int main()//主函数
{
	initialize();
	while(1) mainmenu();
	return 0;
}
//if(KEY_DOWN(MOUSE_MOVED)  KEY_DOWN(MOUSE_EVENT)   KEY_DOWN(MOUSE_WHEELED)
