#include <iostream>
using namespace std;
#include <conio.h>
#include <windows.h>
//The four regions: A, B, C , and D
enum REGION {
	A_REG,
	B_REG,
	C_REG,
	D_REG
};

//Enemy types: Paver, Fighter, and Shielded Fighter
enum Etype {
	PVR,
	FITR,
	SHLD_FITR
};



//constants
#define CmdWidth		150
#define CmdHeight		50
#define CastleWidth		30
#define CastleLength	20
#define CastleXStrt		(CmdWidth/2-(CastleWidth/2))
#define CastleYStrt		(CmdHeight/2-(CastleLength/2))
#define TowerWidth      7
#define TowerLength     3

#define EnemyShape		219  //ASCII code of enemy char shape 



//Tower
struct Tower
{
	int TW; //Tower width
	int TL; //Tower Height
	double Health;
	int number_of_enemies;
	double fire_power;
	double initial_Health;
};

struct castle
{
	//starting x,y
	int Xstrt;
	int Ystrt;
	int W;	//width
	int L;  //Height
	Tower towers[4];	//Castle has 4 towers
};

struct enemy
{

	int ID;			//Each enemy has a unique ID (sequence number)
	REGION Region;	//Region of this enemy
	float Distance;	//Distance to the castle
	double Health;	//Enemy health
	Etype Type;		//PVR, FITR, SHLD_FITR
	double Fire_power;
	int Time_stamp;
	int Reload_period;
	float priority;
	float remaining_time;
	int FD;
	int KD;
	int FT;
	bool first_shot;//calculate FD
};
struct node {
	enemy data;
	node* next;
};
struct Queue {
	node* front;
	node* rear;
	int count;
	int count_at_Tstep;
};
struct list {
	node* head;
	int count;
};


//Functions Prototype

/*A function to set the position of cursor on the screen*/
void gotoxy(int x, int y);

/*A function to set the command window lenght and height for the game specification*/
void SetWindow();

/*A function to color the cmd text*/
void color(int thecolor);

/*A function to partition the castle into regions (A,B,C,D)*/
void DrawRegions(const castle & C);

/*A function to draw the castle and the towers*/
void DrawCastle(const castle & C, int SimulationTime);

/*A function to draw a single enemy using its distance from the castle*/
void DrawEnemy(const enemy& E, int Ypos = 0);

/*A function to draw a list of enemies exist in all regions and ensure there is no overflow in the drawing*/
void DrawEnemies(enemy* enemies[], int size);

/*Student use this function to print his/her message*/
void PrintMsg(char*msg);

void enqueue(Queue &q1, enemy data);
void dequeue(Queue &q2);
void sort(node &head);
void file_loading(Queue &q3);
node* Split(node* my_node);
node* Merge(node* firstNode, node* secondNode);
node* MergeSort(node* my_node);
void war(Queue &Inactive_Enemy, Queue &Killed_Enemy, list &Active_A, list &Active_B, list &Active_C, list &Active_D, list &Sheld_A, list &Sheld_B, list &Sheld_C, list &Sheld_D, float C1, float C2, float C3, Tower T1, Tower T2, Tower T3, Tower T4, list &aux);
void calculate_priority(node* &head, float C1, float C2, float C3);
void insert_at_end(list &L1, enemy data);
void delete_from_list(list &L, node* del);
void file_loading(Queue &Inactive_Enemy, list &Active_A, list &Active_B, list &Active_C, list &Active_D, list &Sheld_A, list &Sheld_B, list &Sheld_C, list &Sheld_D, Queue &Killed_Enemy, list &aux);
list create_list();
void Calculations(list L, Tower &T, int &Paved, int timestep);
void Damage_to_enemy(list &sheld, list* &active, Tower T, int time_step, Queue &Killed_Enemy, list &aux, int &counterA, int & counterB, int &counterC, int &counterD, int &counterAT, int & counterBT, int &counterCT, int &counterDT);
void transport(list &active_sh, list  &active, list & destnation_SH, list & destnation_Ac, int Paved_d, float C1, float C2, float C3, char k);
Queue create_queue();
void calculate_FD(list L, double &FD);
void outputfile(Queue killed, double counter, double FD, Tower T1, Tower T2, Tower T3, Tower T4, int Paved_A, int Paved_B, int Paved_C, int Paved_D, bool win);
void arr_ptr(enemy* * & e, int n, list Active_A, list Active_B, list Active_C, list Active_D, list sheld_A, list sheld_B, list sheld_C, list sheld_D);
void dec_time(list &l);












