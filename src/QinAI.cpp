/*
主体列表记录：
外交控制主体：Foreign_minister 
工具控制主体：Tools_minister
环境控制主体：Environmental_minister
边界控制主体：Border_minister
会议组织主体：Conference_minister
知识管理主体：Knowledge_minister
财政分配主体：Fiscal_minister
开局智能主体：Start_minister
布阵智能主体：Embattle_minister
进攻智能主体：Attack_minister
防御智能主体：Defense_minister
防御护盾主体：Powered_Armor_minister
博弈决策首脑：Head_minister
*/
/*
更正注释：
除地图，知识库，边界主体，棋手用12表示外，搜索部分棋手全用01表示 
*/
/*
环境格式： 0(空)1(我)2(敌)组成的size*size的矩阵
知识库格式：暂无 
*/
/*控制开关*/
const char switch_0[] ="";/*知识库导入方法（空代表默认知识库，否则代表知识库路径）*/
const bool switch_1 =false;/*观察进程*/
const int switch_2 =2;/*默认知识库编号*/
const int switch_3 =5000000;/*搜索状态量上界*/
const bool switch_4 =false;/*调试开关*/
const int switch_5 =-1000;/*防御阈值*/
const int switch_6 =1000;/*进攻阈值*/
/*———————————(*声明*)————————————*/ 
#include<iostream>
#include<fstream>
#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<algorithm>
#include<cmath>
#include<vector>
#include<ctime>
#include<queue>
/*常量*/namespace constant{
const int max_size=20;
const int model_number=19999999;
const int INF=2147483647;
const int move[8][2]={1,0,0,1,-1,0,0,-1,1,1,-1,1,1,-1,-1,-1};//8联通常量
const int real_operator_move[16][2]={-1,0,-2,0,1,0,2,0,0,1,0,2,0,-1,0,-2,-1,-1,-2,-2,-1,1,-2,2,1,-1,2,-2,1,1,2,2};//米状联通常量
const int relative_move[32][2]={-1,0,1,0,0,-1,0,1,1,1,-1,-1,1,-1,-1,1,-2,0,2,0,0,-2,0,2,2,2,-2,-2,-2,2,2,-2,-3,0,3,0,0,-3,0,3,3,3,-3,-3,-3,3,3,-3,-4,0,4,0,0,-4,0,4,4,4,-4,-4,-4,4,4,-4};
}
class Environmental;
class Tools;
using namespace std;
using namespace constant;
/*———————————(*智能辅助*)————————————*/
/*AC自动机*/
struct AC_Auto_Machine{
	/*键值集规模*/
	int m; 
	/*构造函数*/
	AC_Auto_Machine(int a=4)
	{
		m=a;
		node.clear();
		node.push_back(empty);
	}
	/*结点*/
	struct s_node
	{
		int son[4],next,value;
	}empty;
	vector<s_node>node;
	/*状态转移 原型(迭代器,转移键值)*/
	void Push(int &iterator,int key)
	{
		iterator=node[iterator].son[key];
		return;
	}
	/*插入价值模式串 原型(标识串,价值)*/
	void insert(const char* s,int value)
	{
		int l=strlen(s),p=0;
		for(int i=0;i<l;i++)
		{
			if(node[p].son[s[i]-'0'])p=node[p].son[s[i]-'0'];
			else
			{
				node.push_back(empty);
				node[p].son[s[i]-'0']=node.size()-1;
				p=node.size()-1;
			} 
		}
		node[p].value+=value;
		return;
	}
	/*打印AC自动机*/
	void draw()
	{
		for(int i=0;i<node.size();i++)
		printf("node:%d  son:(%d,%d,%d,%d)  next:%d  value:%d\n",i,node[i].son[0],node[i].son[1],node[i].son[2],node[i].son[3],node[i].next,node[i].value);
	}
	/*建立后向匹配指针*/
	void build_next()
	{
		int now,i,j,k;
		queue<int> q;
		for(i=0;i<m;i++)
		if(node[0].son[i])
			q.push(node[0].son[i]);
		while(q.size()>0)
		{
			now=q.front();
			q.pop();
			for(i=0;i<m;i++)
			if(node[now].son[i])
			{
				q.push(node[now].son[i]);
				for(k=node[now].next;k&&!node[k].son[i];k=node[k].next);
				if(node[k].son[i])node[node[now].son[i]].next=node[k].son[i];
			}
		}
		return;
	}
	/*维护结点价值与指针*/
	void build_son()
	{
		int now,i,j,k;
		queue<int> q;
		for(i=0;i<m;i++)
		if(node[0].son[i])
			q.push(node[0].son[i]);
		while(q.size())
		{
			now=q.front();
			q.pop();
			node[now].value+=node[node[now].next].value;
			for(i=0;i<m;i++)
			if(node[now].son[i])q.push(node[now].son[i]);
			else node[now].son[i]=node[node[now].next].son[i];
		}
		return;
	}
	/*建立自动机*/
	void Build()
	{
		if(switch_1)cout<<"AC自动机next指针建立完成\n";
		build_next();
		if(switch_1)cout<<"AC自动机value/son指针建立完成\n";
		build_son();
	}
	/*清空AC自动机*/
	void clear()
	{
		if(switch_1)cout<<"AC自动机清空\n";
		node.clear();
		node.push_back(empty);
		return;
	}
};
/*坐标类*/
class coor{
	public:
		int x,y;
		/*构造函数*/
		coor(int a=0,int b=0){x=a;y=b;} 
		/*米状联通扩散(水平垂直优先)坐标*/
		coor Relative(int);
		void Relative_Move(int);
		/*坐标类偏序*/
		bool operator<(const coor &);
		bool operator==(const coor &);
};
/*决策类*/
class decision:public coor{
	public:
		decision(coor a,int c=0){x=a.x,y=a.y,value=c;}
		int value,Estimated_Cost;
};
bool decision_cmp(const decision &a,const decision &b){
return a.value>b.value;}
/*地图类(0(empty),1(my),2(your))*/
class class_map{
	public:
		int a[max_size][max_size];
		/*打印地图*/
		void draw();
};
/*提案类*/
class Proposal{
	public:
		vector<decision> a;
		void push(const decision &d){a.push_back(d);}
		void adjust(){sort(a.begin(),a.end(),decision_cmp);}
		void clear(){a.clear();}
};
/*博弈树参数集合*/
class Parameter_Collection{
	public:
		/*结点深度*/
		int deep;
		/*立场(0我1敌)*/
		int player;
		/*预计搜索开支*/ 
		int Estimated_Cost;
		/*局面价值评估*/
		int value;
		/*αβ剪枝上界*/
		int cut_value;
		/*决策点*/
		coor Decision_Point;
};
/*———————————(*智能主体*)————————————*/
/*外交控制主体*/
class Foreign{
public:
		void Translate(int,char**,Environmental&);
		int Translate_Decision(coor);
}Foreign_minister;
/*环境控制主体*/
class Environmental{
	public:
		/*地图(第1/2视角)*/class_map map[2];
		/*地图边长*/int size;
		/*知识主体的评估*/int value[2];
		/*4面8方穿越格点粒子的(起始坐标,相对方向)*/
		struct motion_vector{coor start[4];int direction[4];}particle[max_size][max_size];
		/*建立矢量网格*/
		void Build_Motion_Vector();
}Environmental_minister;
/*工具控制主体*/
class Tools{
	public:
		/*字符串转换整数*/
		int cint(char*);
		/*超越边界检测*/
		bool check(const coor &);
}Tools_minister;
/*边界控制主体*/
class Border_AI{
	public:
		/*棋盘边长*/ 
		int size;
		/*坐标局部影响权重(16米状联通和)*/
		class_map map;
		/*提供决策候选序列*/
		vector<coor>list;
		/*边界格式化*/
		void Clear();
		/*汇报工作*/
		void Report(); 
		/*原型(环境主体):依据环境主体初始化工作空间*/
		void Start_Work(); 
		/*原型(决策坐标):返回主体对决策的意见*/
		bool Accepted(const coor &);
		/*原型(环境主体_地图类):接受环境_地图信息进行全局自我更新*/
		void Filter_Map(class_map&);
		/*原型(坐标):对瞄准点插值，进行局部自我更新*/
		void Filter_Map_Aim(coor,int);
		/*原型(坐标):从决策序列中删除'坐标'，返回是否成功*/
		bool Delete(coor);
}Border_minister;
/*会议组织主体*/
class Conference_AI{
	public:
	/*召开工程启动会议*/
	void Start_Work();
	/*状态移动联合会议 原型：(坐标,玩家,评估修正值)*/
	void Push(coor,int);
	/*状态还原联合会议 原型：(坐标,评估修正值)*/
	void Pop(coor,int);
}Conference_minister;
/*财政分配主体*/
class Fiscal_AI{
	public:
		/*期望前15点均摊*/
		void capitation(int sum,Proposal &);
		/*一次分段函数*/
		void function_1(int sum,Proposal &);
		/*分段sigmoid函数*/
		void function_2(int sum,Proposal &);
		/*完全sigmoid函数*/
		void function_3(int sum,Proposal &);
}Fiscal_minister;
/*知识智能主体*/
class Knowledge_AI{
	public:
	/*转移自动机*/
	AC_Auto_Machine AC;
	/*公共知识库*/
	void Default_Library();
	/*进攻知识库*/
	void Attack_Library();
	/*防御知识库*/
	void Defence_Library();
	/*护盾知识库*/
	void Powered_Armor_Library();
	/*建立资料馆*/
	void Build_Library();
	/*全局评估报告 原型(环境)*/
	int Global_Assess(const Environmental&,int);
	/*局部评估报告(行列对角线)*/
	int Local_Assess(const Environmental&,coor,int);
}Knowledge_minister;
/*智囊团*/
class Brain_Trust{
	public:
		Knowledge_AI Adviser;
		void Assess(Environmental &,Proposal &,int);
};
/*开局智能主体*/
class Start_AI{
	public:
		/*主体评估*/
		bool Assess(coor &);
}Start_minister;
/*布阵智能主体*/
Brain_Trust Embattle_minister;
/*进攻智能主体*/
Brain_Trust Attack_minister;
/*防御智能主体*/
Brain_Trust Defense_minister;
/*防御护盾主体*/
Brain_Trust Powered_Armor_minister;
/*博弈决策首脑*/
class Head_AI{
	public:
		/*决策*/
		coor Solve();
		/*博弈树搜索体系(返回搜索是否成功，原型:(参数集合))*/
		bool AI_Game_Tree(Environmental &,Parameter_Collection &); 
}Head_minister;


/*———————————(*智能核心*)————————————*/
int main(int argc,char **argv)
{
	/*外交主体汇报信息*/
	Foreign_minister.Translate(argc,argv,Environmental_minister);
	/*召开会议——全员工作！*/
	Conference_minister.Start_Work();
	//Embattle_minister.Adviser.AC.draw();
	if(switch_4)
	{
		printf("局面评估:%d\n",Environmental_minister.value[0]);
		Proposal List;
		Embattle_minister.Assess(Environmental_minister,List,0);
		for(int i=0;i<List.a.size();i++)
		printf("(%d,%d,%d)",List.a[i].x,List.a[i].y,List.a[i].value);
		system("pause");
	}
	/*coor ans=Head_minister.Solve();
	cout<<ans.x<<' '<<ans.y<<endl;
	return Foreign_minister.Translate_Decision(ans);*/
	/*做出决策*/
	return Foreign_minister.Translate_Decision(Head_minister.Solve());
}




/*————————————函数实现——————————*/
/*工具主体_工具函数*/
int Tools::cint(char *s){
	int ans=0,i,l=strlen(s),offset;
	if(s[0]=='-')offset=-1,i=1;else offset=1,i=0;
	for(;i<l;i++)if(s[i]>=48&&s[i]<=57)ans=ans*10+s[i]-'0';else return -1;
	return ans*offset;
}
bool coor::operator<(const coor &a){
return x<a.x||(x==a.x&&y<a.y);
}
bool coor::operator==(const coor &a){
return x==a.x&&y==a.y;
}
bool Tools::check(const coor &a){
return a.x>=0&&a.y>=0&&a.x<Environmental_minister.size&&a.y<Environmental_minister.size;
}
void class_map::draw(){
for(int i=0;i<Environmental_minister.size;i++){
	for(int j=0;j<Environmental_minister.size;j++)
	cout<<a[i][j];
cout<<endl;}
}
void coor::Relative_Move(const int a){
	x+=relative_move[a][0];
	y+=relative_move[a][1];
	return;
}
coor coor::Relative(const int a){ 
	return coor((*this).x+relative_move[a][0],(*this).y+relative_move[a][1]);
}

/*外交通信_翻译局面*/
void Foreign::Translate(int argc,char **argv,Environmental &E){ 
	int mine=Tools_minister.cint(argv[1]);E.size=Tools_minister.cint(argv[2]);
	if(mine==1)for(int i=0;i<E.size*E.size;i++)
	{
		E.map[0].a[i/E.size][i%E.size]=argv[3][i]-48;
		if(E.map[0].a[i/E.size][i%E.size])
		E.map[1].a[i/E.size][i%E.size]=E.map[0].a[i/E.size][i%E.size]%2+1;
	}
	else for(int i=0;i<E.size*E.size;i++)
	{
		E.map[0].a[i/E.size][i%E.size]=argv[3][i]-48;
		if(E.map[0].a[i/E.size][i%E.size]!=0)
		E.map[0].a[i/E.size][i%E.size]=(E.map[0].a[i/E.size][i%E.size]&1)+1;
		if(E.map[0].a[i/E.size][i%E.size])
		E.map[1].a[i/E.size][i%E.size]=E.map[0].a[i/E.size][i%E.size]%2+1;
	}
	if(switch_1)cout<<"外交局面翻译完成\n";
	return;
}

/*外交通信_翻译决策*/
int Foreign::Translate_Decision(coor a){
	if(switch_1)cout<<"最终决策翻译完成\n";
	return a.x*Environmental_minister.size+a.y;
}

/*环境控制_建立网格矢量*/
void Environmental::Build_Motion_Vector(){
	int i,j,k;
	coor now,start;
	for(start.x=0;start.x<size;start.x++)
		for(start.y=0;start.y<size;start.y++)
			for(k=0;k<4;k++)
			{
				now=start;
				while(Tools_minister.check(now))now.Relative_Move(k<<1);
				particle[start.x][start.y].start[k]=now;
				particle[start.x][start.y].direction[k]=1^(k<<1);
			}
	if(switch_1)cout<<"网络矢量环境建立完成\n";
	return;
}

/*人工智能_边界控制_初始化空间*/
void Border_AI::Start_Work(){
	size=Environmental_minister.size;
	memset(map.a,0,sizeof(map.a));
	list.clear();
	if(switch_1)cout<<"边界控制主体初始化完成\n";
	return; 
}

/*人工智能_边界控制_边界格式化*/
void Border_AI::Clear(){
	memset(map.a,0,sizeof(map));
	size=0;list.clear();
	if(switch_1)cout<<"边界控制主体格式化完成\n";
	return; 
}

/*人工智能_边界控制_意见反馈*/
bool Border_AI::Accepted(const coor &a){
	return Tools_minister.check(a)&&!Environmental_minister.map[0].a[a.x][a.y]&&map.a[a.x][a.y];
}

/*人工智能_边界控制_全局边界更新*/
void Border_AI::Filter_Map(class_map &M){
	coor now;
	for(int i=0;i<size;i++)
		for(int j=0;j<size;j++)
		if(Environmental_minister.map[0].a[i][j])
		{
			now.x=i;now.y=j;
			Filter_Map_Aim(now,1);
		}
	if(switch_1)cout<<"全局边界更新\n";
	return;
}

/*人工智能_边界控制_单点插值边界更新*/
void Border_AI::Filter_Map_Aim(coor a,int value){
	coor now=a,ls;
	map.a[now.x][now.y]+=value;
	if(map.a[now.x][now.y]==value)list.push_back(now);
	else if(map.a[now.x][now.y]==0)Delete(now);
	for(int i=0;i<16;i++)
	{
		ls=now.Relative(i);
		if(Tools_minister.check(ls))
		{ 
			map.a[ls.x][ls.y]+=value;
			if(map.a[ls.x][ls.y]==value)list.push_back(ls);
			else if(map.a[ls.x][ls.y]==0)Delete(ls);
		}
	}
	if(switch_1)printf("单点边界更新\n");
	return;
}

/*人工智能_边界控制_删除决策*/
bool Border_AI::Delete(coor a){
	for(int p=list.size()-1;p>=0;p--)
	if(list[p]==a)
	{
		for(int i=p+1;i<list.size();i++)
		list[i-1]=list[i];
		list.pop_back();
		return true;
	}
	cout<<"删除失败";
	return false;
}

/*人工智能_边界控制_工作汇报*/
void Border_AI::Report(){
	printf("size:%d\n list:",size);
	for(int i=0;i<list.size();i++)
	if(Border_minister.Accepted(list[i]))
	printf("(%d,%d)",list[i].x,list[i].y);
	printf("\nmap:\n");
	map.draw();
}

/*人工智能_会议组织_启动仪式*/
void Conference_AI::Start_Work(){
	srand((unsigned)time(NULL));
	rand();rand();rand();
	/*环境主体启动工作*/
	if(switch_1)cout<<"环境主体启动工作\n";
	Environmental_minister.Build_Motion_Vector();
	/*边界控制主体启动工作*/
	if(switch_1)cout<<"边界控制主体启动工作\n";
	Border_minister.Start_Work();
	Border_minister.Filter_Map(Environmental_minister.map[0]);
	/*知识库主体启动工作*/
	if(switch_1)cout<<"知识库主体启动工作\n";
	Knowledge_minister.Build_Library();
	Embattle_minister.Adviser.Build_Library();
	Attack_minister.Adviser.Attack_Library();
	Attack_minister.Adviser.Build_Library();
	Defense_minister.Adviser.Defence_Library();
	Defense_minister.Adviser.Build_Library();
	Powered_Armor_minister.Adviser.Defence_Library();
	Powered_Armor_minister.Adviser.Powered_Armor_Library();
	Powered_Armor_minister.Adviser.Build_Library();
	if(switch_1)cout<<"资料馆建立完成\n";
	Environmental_minister.value[0]=Knowledge_minister.Global_Assess(Environmental_minister,0); 
	Environmental_minister.value[1]=Knowledge_minister.Global_Assess(Environmental_minister,1); 
	return;
}

/*人工智能_会议组织_落子*/
void Conference_AI::Push(coor coordinates,int player){
	/*边界控制主体自我更新*/
	Border_minister.Filter_Map_Aim(coordinates,1);
	int value[2];
	value[0]=-Knowledge_minister.Local_Assess(Environmental_minister,coordinates,0);
	value[1]=-Knowledge_minister.Local_Assess(Environmental_minister,coordinates,1);
	/*落子*/
	Environmental_minister.map[0].a[coordinates.x][coordinates.y]=player;
	Environmental_minister.map[1].a[coordinates.x][coordinates.y]=(player&1)+1;
	/*修正局面评估*/
	value[0]+=Knowledge_minister.Local_Assess(Environmental_minister,coordinates,0);
	value[1]+=Knowledge_minister.Local_Assess(Environmental_minister,coordinates,1);
	Environmental_minister.value[0]+=value[0];
	Environmental_minister.value[1]+=value[1];
}

/*人工智能_会议组织_提子*/
void Conference_AI::Pop(coor coordinates,int player){
	/*边界控制主体自我更新*/
	Border_minister.Filter_Map_Aim(coordinates,-1);
	int value[2];
	value[0]=-Knowledge_minister.Local_Assess(Environmental_minister,coordinates,0);
	value[1]=-Knowledge_minister.Local_Assess(Environmental_minister,coordinates,1);
	/*提子*/
	Environmental_minister.map[0].a[coordinates.x][coordinates.y]=0;
	Environmental_minister.map[1].a[coordinates.x][coordinates.y]=0;
	/*修正局面评估*/
	value[0]+=Knowledge_minister.Local_Assess(Environmental_minister,coordinates,0);
	value[1]+=Knowledge_minister.Local_Assess(Environmental_minister,coordinates,1);
	Environmental_minister.value[0]+=value[0];
	Environmental_minister.value[1]+=value[1];
}

/*人工智能_知识库_导入公共知识库*/
void Knowledge_AI::Default_Library(){
	if(switch_2==1)
	{
		AC.insert("11111",110000000);
		AC.insert("011110",500000);
		AC.insert("0110",200);
		AC.insert("0111",200);
		AC.insert("1110",200);
		AC.insert("1111",200);
		AC.insert("01110",400);
		AC.insert("01111",400);
		AC.insert("11110",400);
		AC.insert("11111",400);
		AC.insert("010010",100);
		AC.insert("010011",100);
		AC.insert("110010",100);
		AC.insert("110011",100);
		AC.insert("011010",350);
		AC.insert("011011",350);
		AC.insert("111010",350);
		AC.insert("111011",350);
		AC.insert("010110",350);
		AC.insert("010111",350);
		AC.insert("110110",350);
		AC.insert("110111",350);
		AC.insert("0111010",350);
		AC.insert("0111011",350);
		AC.insert("1111010",350);
		AC.insert("1111011",350);
		AC.insert("0110110",350);
		AC.insert("0110111",350);
		AC.insert("1110110",350);
		AC.insert("1110111",350);
		AC.insert("0101110",350);
		AC.insert("0101111",350);
		AC.insert("1101110",350);
		AC.insert("1101111",350);
		AC.insert("0101010",100);
		AC.insert("0101011",100);
		AC.insert("1101010",100);
		AC.insert("1101011",100);
		AC.insert("01011010",300);
		AC.insert("01011011",300);
		AC.insert("11011010",300);
		AC.insert("11011011",300);
		AC.insert("010111010",500000);
		AC.insert("010111011",500000);
		AC.insert("110111010",500000);
		AC.insert("110111011",500000);
		AC.insert("01010",150);
		AC.insert("01011",150);
		AC.insert("11010",150);
		AC.insert("11011",150);//
		AC.insert("22222",-11000000);
		AC.insert("022220",-3800000);
		AC.insert("0220",-200);
		AC.insert("0222",-200);
		AC.insert("2220",-200);
		AC.insert("2222",-200);
		AC.insert("02220",-400);
		AC.insert("02222",-400);
		AC.insert("22220",-400);
		AC.insert("22222",-400);
		AC.insert("020020",-100);
		AC.insert("020022",-100);
		AC.insert("220020",-100);
		AC.insert("220022",-100);
		AC.insert("022020",-350);
		AC.insert("022022",-350);
		AC.insert("222020",-350);
		AC.insert("222022",-350);
		AC.insert("020220",-350);
		AC.insert("020222",-350);
		AC.insert("220220",-350);
		AC.insert("220222",-350);
		AC.insert("0222020",-350);
		AC.insert("0222022",-350);
		AC.insert("2222020",-350);
		AC.insert("2222022",-350);
		AC.insert("0220220",-350);
		AC.insert("0220222",-350);
		AC.insert("2220220",-350);
		AC.insert("2220222",-350);
		AC.insert("0202220",-350);
		AC.insert("0202222",-350);
		AC.insert("2202220",-350);
		AC.insert("2202222",-350);
		AC.insert("0202020",-100);
		AC.insert("0202022",-100);
		AC.insert("2202020",-100);
		AC.insert("2202022",-100);
		AC.insert("02022020",-300);
		AC.insert("02022022",-300);
		AC.insert("22022020",-300);
		AC.insert("22022022",-300);
		AC.insert("020222020",-3800000);
		AC.insert("020222022",-3800000);
		AC.insert("220222020",-3800000);
		AC.insert("220222022",-3800000);
		AC.insert("02020",-150);
		AC.insert("02022",-150);
		AC.insert("22020",-150);
		AC.insert("22022",-150);//
		AC.insert("02222",-3000000);
		AC.insert("20222",-3000000);
		AC.insert("22022",-3000000);
		AC.insert("22202",-3000000);
		AC.insert("22220",-3000000);
		AC.insert("002220",-10000);
		AC.insert("020220",-10000);
		AC.insert("022020",-10000);
		AC.insert("022200",-10000);
		AC.insert("0022200",10000);
	}
	else
	if(switch_2==2)
	{
		AC.insert("11111",110000000);
		AC.insert("011110",500000);
		AC.insert("0110",100);
		AC.insert("0111",100);
		AC.insert("1110",100);
		AC.insert("1111",100);
		//隐冲四 
		AC.insert("211100",200);
		AC.insert("211010",200);
		AC.insert("211001",200);
		AC.insert("210110",200);
		AC.insert("210101",200);
		AC.insert("210011",200);
		AC.insert("001112",200);
		AC.insert("010112",200);
		AC.insert("100112",200);
		AC.insert("011012",200);
		AC.insert("101012",200);
		AC.insert("110012",200);
		AC.insert("2110012",-200);
		AC.insert("2101012",-200);
		AC.insert("2100112",-200);
		//连活三 
		AC.insert("01110",400);
		AC.insert("01111",400);
		AC.insert("11110",400);
		AC.insert("11111",400);
		//活二 
		AC.insert("010010",100);
		AC.insert("010011",100);
		AC.insert("110010",100);
		AC.insert("110011",100);
		//跳活三 
		AC.insert("011010",350);
		AC.insert("011011",350);
		AC.insert("111010",350);
		AC.insert("111011",350);
		AC.insert("010110",350);
		AC.insert("010111",350);
		AC.insert("110110",350);
		AC.insert("110111",350);
		AC.insert("0111010",350);
		AC.insert("0111011",350);
		AC.insert("1111010",350);
		AC.insert("1111011",350);
		AC.insert("0110110",350);
		AC.insert("0110111",350);
		AC.insert("1110110",350);
		AC.insert("1110111",350);
		AC.insert("0101110",350);
		AC.insert("0101111",350);
		AC.insert("1101110",350);
		AC.insert("1101111",350);
		AC.insert("0101010",100);
		AC.insert("0101011",100);
		AC.insert("1101010",100);
		AC.insert("1101011",100);
		AC.insert("01011010",300);
		AC.insert("01011011",300);
		AC.insert("11011010",300);
		AC.insert("11011011",300);
		AC.insert("010111010",500000);
		AC.insert("010111011",500000);
		AC.insert("110111010",500000);
		AC.insert("110111011",500000);
		AC.insert("01010",150);
		AC.insert("01011",150);
		AC.insert("11010",150);
		AC.insert("11011",150);//
		AC.insert("22222",-11000000);
		AC.insert("022220",-3800000);
		AC.insert("0220",-100);
		AC.insert("0222",-100);
		AC.insert("2220",-100);
		AC.insert("2222",-100);
		//隐冲四 
		AC.insert("122200",-200);
		AC.insert("122020",-200);
		AC.insert("122002",-200);
		AC.insert("120220",-200);
		AC.insert("120202",-200);
		AC.insert("120022",-200);
		AC.insert("002221",-200);
		AC.insert("020221",-200);
		AC.insert("200221",-200);
		AC.insert("022021",-200);
		AC.insert("202021",-200);
		AC.insert("220021",-200);
		AC.insert("1220021",200);
		AC.insert("1202021",200);
		AC.insert("1200221",200);
		//连活三 
		AC.insert("02220",-400);
		AC.insert("02222",-400);
		AC.insert("22220",-400);
		AC.insert("22222",-400);
		AC.insert("020020",-100);
		AC.insert("020022",-100);
		AC.insert("220020",-100);
		AC.insert("220022",-100);
		AC.insert("022020",-350);
		AC.insert("022022",-350);
		AC.insert("222020",-350);
		AC.insert("222022",-350);
		AC.insert("020220",-350);
		AC.insert("020222",-350);
		AC.insert("220220",-350);
		AC.insert("220222",-350);
		AC.insert("0222020",-350);
		AC.insert("0222022",-350);
		AC.insert("2222020",-350);
		AC.insert("2222022",-350);
		AC.insert("0220220",-350);
		AC.insert("0220222",-350);
		AC.insert("2220220",-350);
		AC.insert("2220222",-350);
		AC.insert("0202220",-350);
		AC.insert("0202222",-350);
		AC.insert("2202220",-350);
		AC.insert("2202222",-350);
		AC.insert("0202020",-100);
		AC.insert("0202022",-100);
		AC.insert("2202020",-100);
		AC.insert("2202022",-100);
		AC.insert("02022020",-300);
		AC.insert("02022022",-300);
		AC.insert("22022020",-300);
		AC.insert("22022022",-300);
		AC.insert("020222020",-3800000);
		AC.insert("020222022",-3800000);
		AC.insert("220222020",-3800000);
		AC.insert("220222022",-3800000);
		AC.insert("02020",-150);
		AC.insert("02022",-150);
		AC.insert("22020",-150);
		AC.insert("22022",-150);//
		AC.insert("02222",-3000000);
		AC.insert("20222",-3000000);
		AC.insert("22022",-3000000);
		AC.insert("22202",-3000000);
		AC.insert("22220",-3000000);
		AC.insert("002220",-10000);
		AC.insert("020220",-10000);
		AC.insert("022020",-10000);
		AC.insert("022200",-10000);
		AC.insert("0022200",10000);
	}
	if(switch_1)printf("默认知识库导入完成\n");
	return;
}

/*人工智能_知识库_导入进攻知识库*/
void Knowledge_AI::Attack_Library(){
	AC.insert("0110",200);
	AC.insert("0111",100);
	AC.insert("1110",100);
	AC.insert("1111",100);
	AC.insert("011100",300);
	AC.insert("001110",300);
	AC.insert("010010",50);
	AC.insert("010011",50);
	AC.insert("110010",50);
	AC.insert("110011",50);
	AC.insert("011010",200);
	AC.insert("011011",200);
	AC.insert("111010",200);
	AC.insert("111011",200);
	AC.insert("010110",200);
	AC.insert("010111",200);
	AC.insert("110110",200);
	AC.insert("110111",200);
	AC.insert("0111010",200);
	AC.insert("0111011",200);
	AC.insert("1111010",200);
	AC.insert("1111011",200);
	AC.insert("0110110",200);
	AC.insert("0110111",200);
	AC.insert("1110110",200);
	AC.insert("1110111",200);
	AC.insert("0101110",200);
	AC.insert("0101111",200);
	AC.insert("1101110",200);
	AC.insert("1101111",200);
	AC.insert("0101010",50);
	AC.insert("0101011",50);
	AC.insert("1101010",50);
	AC.insert("1101011",50);
	AC.insert("01011010",100);
	AC.insert("01011011",100);
	AC.insert("11011010",100);
	AC.insert("11011011",100);
	AC.insert("01010",50);
	AC.insert("01011",50);
	AC.insert("11010",50);
	AC.insert("11011",50);
}

/*人工智能_知识库_导入防御知识库*/
void Knowledge_AI::Defence_Library(){
	AC.insert("0220",-100);
	AC.insert("0222",-100);
	AC.insert("2220",-100);
	AC.insert("2222",-100);
	AC.insert("02220",-200);
	AC.insert("02222",-200);
	AC.insert("22220",-200);
	AC.insert("22222",-200);
	AC.insert("020020",-50);
	AC.insert("020022",-50);
	AC.insert("220020",-50);
	AC.insert("220022",-50);
	AC.insert("022020",-200);
	AC.insert("022022",-200);
	AC.insert("222020",-200);
	AC.insert("222022",-200);
	AC.insert("020220",-200);
	AC.insert("020222",-200);
	AC.insert("220220",-200);
	AC.insert("220222",-200);
	AC.insert("0222020",-200);
	AC.insert("0222022",-200);
	AC.insert("2222020",-200);
	AC.insert("2222022",-200);
	AC.insert("0220220",-200);
	AC.insert("0220222",-200);
	AC.insert("2220220",-200);
	AC.insert("2220222",-200);
	AC.insert("0202220",-200);
	AC.insert("0202222",-200);
	AC.insert("2202220",-200);
	AC.insert("2202222",-200);
	AC.insert("0202020",-50);
	AC.insert("0202022",-50);
	AC.insert("2202020",-50);
	AC.insert("2202022",-50);
	AC.insert("02022020",-200);
	AC.insert("02022022",-200);
	AC.insert("22022020",-200);
	AC.insert("22022022",-200);
	AC.insert("02020",-100);
	AC.insert("02022",-100);
	AC.insert("22020",-100);
	AC.insert("22022",-100);//
}

/*人工智能_知识库_导入终极护盾知识库*/
void Knowledge_AI::Powered_Armor_Library(){
	AC.insert("022200",-10000);
	AC.insert("020220",-10000);
	AC.insert("022020",-10000);
	AC.insert("002220",-10000);
	AC.insert("0022200",10000);
	AC.insert("01111",20000);
	AC.insert("10111",20000);
	AC.insert("11011",20000);
	AC.insert("11101",20000);
	AC.insert("11110",20000);
}

/*人工智能_知识库_建立资料馆*/
void Knowledge_AI::Build_Library(){
	if(strlen(switch_0)==0)
	{
		//导入公共知识库
		 Default_Library();
	}
	else
	{
		//导入目标知识库
		freopen(switch_0,"r",stdin);
		int n,i,j;
		char s[max_size+2];
		scanf("%d",&n);
		for(i=0;i<n;i++)
		{
			memset(s,0,sizeof(s));
			scanf("%s%d",s,&j);
			AC.insert(s,j);
		}
		fclose(stdin);
		if(switch_1)printf("外部资料库导入完成\n");
	}
	//封装自动机 
	 AC.Build();
	 if(switch_1)printf("AC自动机封装完成\n");
}

/*人工智能_知识库_全局评估*/
int Knowledge_AI::Global_Assess(const Environmental &E,int player){
	if(switch_1)printf("进行全局评估\n");
	bool visit[max_size+4][max_size+4][4];
	memset(visit,0,sizeof(visit));
	int x,y,i,j,k,l,re=0;
	int iterator_1;
	coor now;
	for(x=0;x<E.size;x++)
		for(y=0;y<E.size;y++)
			for(k=0;k<4;k++)
			{
				now=E.particle[x][y].start[k];
				l=E.particle[x][y].direction[k];
				if(!visit[now.x+1][now.y+1][k])
				{
					visit[now.x+1][now.y+1][k]=1;
					iterator_1=0;
					AC.Push(iterator_1,3);
					for(;;)
					{
						now.Relative_Move(l);
						if(Tools_minister.check(now))
						{
							AC.Push(iterator_1,E.map[player].a[now.x][now.y]);
							re+=AC.node[iterator_1].value;
						}
						else
						{
							AC.Push(iterator_1,3);
							re+=AC.node[iterator_1].value;
							break;
						}
					}
				}
			}
	if(switch_1)cout<<"全局评估完成，结果"<<re<<endl;
	return re;
}

/*人工智能_知识库_局部评估*/
int Knowledge_AI::Local_Assess(const Environmental &E,coor start,int player){
	coor now;
	int l,k,re=0;
	int iterator_1;
	for(k=0;k<4;k++)
	{
		now=E.particle[start.x][start.y].start[k];
		l=E.particle[start.x][start.y].direction[k];
		iterator_1=0;
		AC.Push(iterator_1,3);
		for(;;)
		{
			now.Relative_Move(l);
			if(Tools_minister.check(now))
			{
				AC.Push(iterator_1,E.map[player].a[now.x][now.y]);
				re+=AC.node[iterator_1].value;
			}
			else
			{
				AC.Push(iterator_1,3);
				re+=AC.node[iterator_1].value;
				break;
			}
		}
	}
	return re;
}

/*人工智能_财政顾问_前15均摊方法*/
void Fiscal_AI::capitation(int sum,Proposal &P){
	for(int i=0;i<P.a.size();i++)
	if(i<15)P.a[i].Estimated_Cost=sum/15;
	else P.a[i].Estimated_Cost=0;
	return;
}

/*人工智能_财政顾问_分段直线函数*/
void Fiscal_AI::function_1(int sum,Proposal &P){
	/*f(x)=0(x<X)|line(x){(X,Y)->(1,1)}*/
	int Min_i=INF,Max_i=-INF;
	double Sum=0.0,Min,Max,x,y;
	for(int i=0;i<P.a.size()&&P.a[i].value>0;i++)
	{
		Min_i=min(Min_i,P.a[i].value);
		Max_i=max(Max_i,P.a[i].value);
	}
	if(Min_i>0)Min_i--;
	if(Max_i<=0)
	{
		for(int i=0;i<P.a.size();i++)
		P.a[i].Estimated_Cost=sum/P.a.size();
		return;
	}
	Min=(double)Min_i;
	Max=(double)Max_i;
	/*线性函数分段点*/
	double X=-0.5,Y=0.2,k=(1.0-Y)/(1.0-X),b=Y-k*X;
	for(int i=0;i<P.a.size()&&P.a[i].value>0;i++)
	{
		x=2*(P.a[i].value-Min)/(Max-Min)-1;
		if(x<X)y=0.0;
		else y=k*x+b;
		Sum+=y;
	}
	for(int i=0;i<P.a.size();i++)
	{
		x=2*(P.a[i].value-Min)/(Max-Min)-1;
		if(x<X)y=0.0;
		else y=k*x+b;
		P.a[i].Estimated_Cost=(int)(((y/Sum)*sum));
	}	
	return;
}

/*人工智能_财政顾问_分段sigmoid函数*/
void Fiscal_AI::function_2(int sum,Proposal &P){
	/*f(x)=0(x<X)|sigmoid(x){sigmoid((X,Y)->(1,1))∝sigmoid(-1->1))}*/
	int Min_i=INF,Max_i=-INF;
	double Sum=0.0,Min,Max,x,y;
	for(int i=0;i<P.a.size()&&P.a[i].value>0;i++)
	{
		Min_i=min(Min_i,P.a[i].value);
		Max_i=max(Max_i,P.a[i].value);
	}
	if(Min_i>0)Min_i--;
	if(Max_i<=0)
	{
		for(int i=0;i<P.a.size();i++)
		P.a[i].Estimated_Cost=sum/P.a.size();
		return;
	}
	Min=(double)Min_i;
	Max=(double)Max_i;
	/*sigmoid分段点*/
	double X=-0.5,Y=0.2;
	for(int i=0;i<P.a.size()&&P.a[i].value>0;i++)
	{
		x=2.0*(P.a[i].value-Min)/(Max-Min)-1;
		if(x<X)y=0.0;
		else 
		{
			x=(x-X)/(1.0-X)*2.0-1.0;
			y=1.0/(1.0+exp(-x*4.0))*(1.0-Y)+Y;
		}
		Sum+=y;
	}
	for(int i=0;i<P.a.size();i++)
	{
		x=2.0*(P.a[i].value-Min)/(Max-Min)-1;
		if(x<X)y=0.0;
		else
		{
			x=(x-X)/(1.0-X)*2.0-1.0;
			y=1.0/(1.0+exp(-x*4.0))*(1.0-Y)+Y;
		}
		P.a[i].Estimated_Cost=(int)(((y/Sum)*sum));
	}	
	return;
}

/*人工智能_财政顾问_完全sigmoid函数*/
void Fiscal_AI::function_3(int sum,Proposal &P){
	/*f(x)=0(x<X)|sigmoid(x){sigmoid((X,Y)->(1,1))∝sigmoid(-1->1))}*/
	int Min_i=INF,Max_i=-INF,cnt=0;
	double Sum=0.0,Min,Max,x,y;
	for(int i=0;i<P.a.size()&&P.a[i].value>0;i++)
	if(P.a[i].value>0) 
	{
		cnt++;
		Min_i=min(Min_i,P.a[i].value);
		Max_i=max(Max_i,P.a[i].value);
	}
	if(Max_i<=Min_i)
	{
		for(int i=0;i<P.a.size();i++)
		P.a[i].Estimated_Cost=sum/cnt;
		return;
	}
	Min=(double)Min_i;
	Max=(double)Max_i;
	/*sigmoid分段点*/
	double X=-1.0,Y=0.0;
	for(int i=0;i<P.a.size()&&P.a[i].value>0;i++)
	{
		x=2.0*(P.a[i].value-Min)/(Max-Min)-1.0;
		y=1.0/(1.0+exp(-x*4.0));
		Sum+=y;
	}
	for(int i=0;i<P.a.size();i++)
	{
		x=2.0*(P.a[i].value-Min)/(Max-Min)-1.0;
		y=1.0/(1.0+exp(-x*4.0));
		P.a[i].Estimated_Cost=(int)(((y/Sum)*sum));
	}	
	return;
}

/*人工智能_开局决策*/
bool Start_AI::Assess(coor &operate){
	int i,j,k=0;
	for(i=0;i<Environmental_minister.size;i++)
		for(j=0;j<Environmental_minister.size;j++)
		if(Environmental_minister.map[0].a[i][j])
		{
			k++;
			operate.x=i;
			operate.y=j;
		}
	if(k>1)return false;
	if(k==0)
	{
		operate.x=operate.y=Environmental_minister.size>>1;
		return true;
	}
	if(k==1)
	{
		operate.Relative_Move(rand()%8);
		return true;
	}
}

/*人工智能_智囊团_提案*/
void Brain_Trust::Assess(Environmental &E,Proposal &P,int player){
	P.clear();
	int i,j,k;
	coor now;
	/*接受边界主体信息*/
	for(i=0;i<Border_minister.list.size();i++)
	{
		now=Border_minister.list[i];
		if(Border_minister.Accepted(now))
		{
			/*请求自己的顾问评估*/
			k=-Adviser.Local_Assess(E,now,player);
			E.map[player].a[now.x][now.y]=1;
			k+=Adviser.Local_Assess(E,now,player);
			E.map[player].a[now.x][now.y]=0;
			P.push(decision(now,k));
		}
	}
	/*优化提案*/
	P.adjust();
	return;
}

/*人工智能_首脑_决策*/
coor Head_AI::Solve(){
	coor opr;
	if(Start_minister.Assess(opr))return opr;
	Parameter_Collection P;
	P.deep=1;
	P.player=0;
	P.Estimated_Cost=switch_3;
	P.cut_value=0x7fffffff;
	P.value=0x80000000;
	AI_Game_Tree(Environmental_minister,P);
	return P.Decision_Point;
}

/*人工智能_首脑_博弈树核心搜索结构*/
bool Head_AI::AI_Game_Tree(Environmental &E,Parameter_Collection &P){
	if(P.Estimated_Cost==0)return false;
	P.Estimated_Cost--;
	bool B;
	/*提案容器*/
	Proposal List;
	/*参数集合*/
	Parameter_Collection A; 
	/*智囊团三方会谈做出评估*/
	if(E.value[P.player]<switch_5)
	Powered_Armor_minister.Assess(E,List,P.player);
	else if(E.value[P.player]>switch_6)
	Attack_minister.Assess(E,List,P.player);
	else Embattle_minister.Assess(E,List,P.player);
	/*分配财政资源*/
	Fiscal_minister.function_2(P.Estimated_Cost,List);
	
if(switch_4&&P.deep<3)
{
E.map[0].draw();
printf("进入：全局评估:%d 当前深度:%d 执棋:%d 预支:%d a_b上界:%d 评估:%d\n",E.value[P.player],P.deep,P.player,P.Estimated_Cost,P.cut_value,P.value);
printf("决策点(%d):",List.a.size());
for(int i=0;i<List.a.size();i++)
printf("(%d,%d,%d,%d)",List.a[i].x,List.a[i].y,List.a[i].value,List.a[i].Estimated_Cost);
system("pause");
}
	
	A.deep=P.deep+1;
	A.player=P.player^1;
	/*我方执棋*/
	if(P.player==0)
	{
		bool re=false;
		/*边界*/
		if(List.a[0].value>=100000000)
		{
			P.value=100000000+E.value[0]-P.deep*1000000;
			P.Decision_Point=List.a[0];
			return true;
		}
		for(int i=0;i<List.a.size();i++)
		if(List.a[i].Estimated_Cost>1)
		{
			A.Estimated_Cost=List.a[i].Estimated_Cost;
			A.value=0x7fffffff;
			A.cut_value=P.value;
			Conference_minister.Push(List.a[i],P.player+1);
			B=AI_Game_Tree(E,A);
			if(B)re=true;
			Conference_minister.Pop(List.a[i],P.player+1);
			if(B&&A.value>P.value)
			{
				P.value=A.value;
				P.Decision_Point=List.a[i];
				/*α_β剪枝*/ 
				if(P.value>=P.cut_value)return true;
			}
		}
		if(!re)
		{
			Conference_minister.Push(List.a[0],P.player+1);
			P.value=E.value[0];
			P.Decision_Point=List.a[0];
			Conference_minister.Pop(List.a[0],P.player+1);
		}
if(switch_4&&P.deep<3)
{
printf("回溯：当前深度:%d 执棋:%d 决策:(%d,%d)预支:%d a_b上界:%d 评估:%d\n",P.deep,P.player,P.Decision_Point.x,P.Decision_Point.y,P.Estimated_Cost,P.cut_value,P.value);
}
		return true;
	}
	/*敌方执棋*/
	else
	{
		bool re=false;
		/*边界*/
		if(List.a[0].value>=100000000)
		{
			P.value=-100000000+E.value[0]+P.deep*2000000;
			P.Decision_Point=List.a[0];
			return true;
		}
		for(int i=0;i<List.a.size();i++)
		{ 
			/*转入下一层状态*/ 
			if(List.a[i].Estimated_Cost>0)
			{
				re=true;
				A.Estimated_Cost=List.a[i].Estimated_Cost;
				A.value=0x80000000;
				A.cut_value=P.value;
				Conference_minister.Push(List.a[i],P.player+1);
				AI_Game_Tree(E,A);
				Conference_minister.Pop(List.a[i],P.player+1);
				if(A.value<P.value)
				{
					P.value=A.value;
					P.Decision_Point=List.a[i];
					/*α_β剪枝*/ 
					if(P.value<=P.cut_value)return true;
				}
			}
		} 
if(switch_4&&P.deep<3)
{
printf("回溯：当前深度:%d 执棋:%d 决策:(%d,%d)预支:%d a_b上界:%d 评估:%d\n",P.deep,P.player,P.Decision_Point.x,P.Decision_Point.y,P.Estimated_Cost,P.cut_value,P.value);
}
		return re;
	}
}

