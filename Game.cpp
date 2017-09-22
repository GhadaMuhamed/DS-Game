#include <iostream>
#include <fstream>
#include "Header.h"
using namespace std;
void calculate_priority(node* &head, float C1, float C2, float C3)
{
	node* ptrA = head;
	while (ptrA != NULL)
	{
		ptrA->data.priority = (ptrA->data.Fire_power / (ptrA->data.Distance))*C1 + C2 / (ptrA->data.remaining_time + 1) + ptrA->data.Health*C3;
		ptrA = ptrA->next;
	}
	head = MergeSort(head);

}
void Calculations(list L, Tower &T, int &Paved, int timestep)
{
	node* tmp2;
	tmp2 = L.head;
	while (tmp2 != NULL)
	{
		if (tmp2->data.Type != PVR)
		{
			if (tmp2->data.remaining_time == tmp2->data.Reload_period)
			{
				if (60.0 - tmp2->data.Distance < Paved)
					tmp2->data.Distance--;
				T.Health -= (1.0 / (tmp2->data.Distance))*tmp2->data.Fire_power;
			}
			else {
				if (60.0 - tmp2->data.Distance < Paved)
					tmp2->data.Distance--;
			}
		}
		else {
			if (tmp2->data.remaining_time == tmp2->data.Reload_period) // paving 
			{
				if (60.0 - tmp2->data.Distance < Paved)
					tmp2->data.Distance--;
				else {
					if (Paved + tmp2->data.Fire_power <= 58)
					{
						Paved += tmp2->data.Fire_power;
						tmp2->data.Distance -= tmp2->data.Fire_power;
					}
					else Paved = 58;
				}//==paved
			}
			else {
				if (60.0 - tmp2->data.Distance < Paved) // walking
					tmp2->data.Distance--;
			}
		}


		tmp2 = tmp2->next;
	}
}

void transport(list &active_sh, list  &active, list & destnation_SH, list & destnation_Ac, int Paved_d, float C1, float C2, float C3,char k)
{
	node* temp;
	while (active_sh.count != 0)
	{
		temp = active_sh.head;
		if (60.0 - temp->data.Distance > Paved_d)
			temp->data.Distance = 60.0-Paved_d;
		if (k == 'A')
			temp->data.Region = A_REG;
		else if (k=='B')
			temp->data.Region = B_REG;
		else if (k=='C')
			temp->data.Region = C_REG;
		else temp->data.Region = D_REG;

		insert_at_end(destnation_SH, temp->data);
		delete_from_list(active_sh, temp);
	}
	calculate_priority(destnation_SH.head, C1, C2, C3);

	while (active.count != 0)
	{
		temp = active.head;
		if (60.0 - temp->data.Distance > Paved_d)
			temp->data.Distance = 60.0-Paved_d;
		if (k == 'A')
			temp->data.Region = A_REG;
		else if (k == 'B')
			temp->data.Region = B_REG;
		else if (k == 'C')
			temp->data.Region = C_REG;
		else temp->data.Region = D_REG;
		insert_at_end(destnation_Ac, temp->data);
		delete_from_list(active, temp);
	}
	active.head = MergeSort(active.head);
}
void calculate_FD(list L, double &FD)

{
	node* temp = L.head;

	while (temp != NULL)
	{
		if (temp->data.first_shot)
			FD += temp->data.FD;
		temp = temp->next;
	}

}
void Damage_to_enemy(list &sheld, list &active, Tower T, int time_step, Queue &Killed_Enemy, list &aux, int &counterA, int & counterB, int &counterC, int &counterD, int &counterAT, int & counterBT, int &counterCT, int &counterDT)
{
	int i = 0;

	node* ptr1 = sheld.head;
	node* tmp2;
	while (ptr1 != NULL && i<T.number_of_enemies)
	{
		if (!(ptr1->data.first_shot))
		{
			ptr1->data.FD = time_step - ptr1->data.Time_stamp;
			ptr1->data.first_shot = true;
		}
		ptr1->data.Health -= (1.0 / (ptr1->data.Distance))*T.fire_power*(1.0 / 2.0);
		if (ptr1->data.Health <= 0)
		{
			ptr1->data.KD = time_step - (ptr1->data.FD + ptr1->data.Time_stamp);
			ptr1->data.FT = ptr1->data.FD + ptr1->data.KD;
			insert_at_end(aux, ptr1->data);
			if (ptr1->data.Region == A_REG) { counterA++; counterAT++; }
			if (ptr1->data.Region == B_REG) { counterB++; counterBT++; }
			if (ptr1->data.Region == C_REG) { counterC++; counterCT++; }
			if (ptr1->data.Region == D_REG) { counterD++; counterDT++; }
            tmp2 = ptr1->next;
			delete_from_list(sheld, ptr1);
			ptr1 = tmp2;
		}
		i++;
		if (ptr1 != NULL)
			ptr1 = ptr1->next;
	}
	ptr1 = active.head;
	while (i<T.number_of_enemies && ptr1 != NULL)
	{
		if (!(ptr1->data.first_shot))
		{
			ptr1->data.FD = time_step - ptr1->data.Time_stamp;
			ptr1->data.first_shot = true;
		}
		ptr1->data.Health -= (1.0 / (ptr1->data.Distance))*T.fire_power;
		if (ptr1->data.Health <= 0)
		{
			ptr1->data.KD = time_step - (ptr1->data.FD + ptr1->data.Time_stamp);
			ptr1->data.FT = ptr1->data.FD + ptr1->data.KD;
			insert_at_end(aux, ptr1->data);
			if (ptr1->data.Region == A_REG) { counterA++; counterAT++;}
			if (ptr1->data.Region == B_REG) { counterB++; counterBT++;}
			if (ptr1->data.Region == C_REG) { counterC++; counterCT++;}
			if (ptr1->data.Region == D_REG) { counterD++; counterDT++;}

			tmp2 = ptr1->next;
			delete_from_list(active, ptr1);
			ptr1 = tmp2;

		}
		i++;
		if (ptr1 != NULL)
			ptr1 = ptr1->next;
	}
}
void war(Queue &Inactive_Enemy, Queue &Killed_Enemy, list &Active_A, list &Active_B, list &Active_C, list &Active_D, list &Sheld_A, list &Sheld_B, list &Sheld_C, list &Sheld_D, float C1, float C2, float C3, Tower T1, Tower T2, Tower T3, Tower T4, list &aux)
{
	node* ptr;
	castle ct;
	ct.Xstrt = CastleXStrt;
	ct.Ystrt = CastleYStrt;
	ct.W = CastleWidth;
	ct.L = CastleLength;
	int Paved_A = 30, Paved_B = 30, Paved_C = 30, Paved_D = 30;
	int time_step = 1;
	bool win;
	int counterAT = 0, counterBT = 0, counterCT = 0, counterDT = 0;
	while ((T1.Health > 0 || T2.Health > 0 || T3.Health > 0 || T4.Health > 0) && (Active_A.count != 0 || Active_B.count != 0 || Active_C.count != 0 || Active_D.count != 0 || Sheld_A.count != 0 || Sheld_B.count != 0 || Sheld_C.count != 0 || Sheld_D.count != 0 || Inactive_Enemy.count != 0))
	{
		ct.towers[0] = T1;
		ct.towers[1] = T2;
		ct.towers[2] = T3;
		ct.towers[3] = T4;
		DrawCastle(ct, time_step);
		DrawRegions(ct);
		int n = Active_A.count + Active_B.count + Active_C.count + Active_D.count + Sheld_A.count + Sheld_B.count + Sheld_C.count + Sheld_D.count;
		enemy **e = new enemy*[n];
		arr_ptr(e,n, Active_A, Active_B, Active_C, Active_D, Sheld_A, Sheld_B, Sheld_C, Sheld_D);
		DrawEnemies(e, n);
		int counterA = 0, counterB = 0, counterC = 0, counterD = 0;
		while (Inactive_Enemy.front != NULL && Inactive_Enemy.front->data.Time_stamp == time_step)
		{
			if (Inactive_Enemy.front->data.Region == A_REG)
			{
				if (T1.Health > 0)
				{
					if (Inactive_Enemy.front->data.Type == SHLD_FITR)
						insert_at_end(Sheld_A, Inactive_Enemy.front->data);
					else insert_at_end(Active_A, Inactive_Enemy.front->data);
				}
				else if (T2.Health > 0)
				{
					Inactive_Enemy.front->data.Region = B_REG;
					if (Inactive_Enemy.front->data.Type == SHLD_FITR)
						insert_at_end(Sheld_B, Inactive_Enemy.front->data);
					else insert_at_end(Active_B, Inactive_Enemy.front->data);
				}
				else if (T3.Health > 0)
				{
					Inactive_Enemy.front->data.Region = C_REG;
                    if (Inactive_Enemy.front->data.Type == SHLD_FITR)
						insert_at_end(Sheld_C, Inactive_Enemy.front->data);
					else insert_at_end(Active_C, Inactive_Enemy.front->data);
				}
				else if (T4.Health>0)
				{
					Inactive_Enemy.front->data.Region = D_REG;

					if (Inactive_Enemy.front->data.Type == SHLD_FITR)
						insert_at_end(Sheld_D, Inactive_Enemy.front->data);
					else insert_at_end(Active_D, Inactive_Enemy.front->data);
				}
			}
			else if (Inactive_Enemy.front->data.Region == B_REG)
			{
				if (T2.Health > 0)
				{
					if (Inactive_Enemy.front->data.Type == SHLD_FITR)
						insert_at_end(Sheld_B, Inactive_Enemy.front->data);
					else insert_at_end(Active_B, Inactive_Enemy.front->data);
				}
				else if (T3.Health>0)
				{
					Inactive_Enemy.front->data.Region = C_REG;

					if (Inactive_Enemy.front->data.Type == SHLD_FITR)
						insert_at_end(Sheld_C, Inactive_Enemy.front->data);
					else insert_at_end(Active_C, Inactive_Enemy.front->data);
				}
				else if (T4.Health>0)
				{
					Inactive_Enemy.front->data.Region = D_REG;

					if (Inactive_Enemy.front->data.Type == SHLD_FITR)
						insert_at_end(Sheld_D, Inactive_Enemy.front->data);
					else insert_at_end(Active_D, Inactive_Enemy.front->data);
				}
				else if (T1.Health>0)
				{
					Inactive_Enemy.front->data.Region = A_REG;

					if (Inactive_Enemy.front->data.Type == SHLD_FITR)
						insert_at_end(Sheld_A, Inactive_Enemy.front->data);
					else insert_at_end(Active_A, Inactive_Enemy.front->data);
				}
			}
			else if (Inactive_Enemy.front->data.Region == C_REG)
			{
				if (T3.Health > 0)
				{
					if (Inactive_Enemy.front->data.Type == SHLD_FITR)
						insert_at_end(Sheld_C, Inactive_Enemy.front->data);
					else insert_at_end(Active_C, Inactive_Enemy.front->data);
				}
				else if (T4.Health>0)
				{
					Inactive_Enemy.front->data.Region = D_REG;

					if (Inactive_Enemy.front->data.Type == SHLD_FITR)
						insert_at_end(Sheld_D, Inactive_Enemy.front->data);
					else insert_at_end(Active_D, Inactive_Enemy.front->data);
				}
				else if (T1.Health>0)
				{
					Inactive_Enemy.front->data.Region = A_REG;

					if (Inactive_Enemy.front->data.Type == SHLD_FITR)
						insert_at_end(Sheld_A, Inactive_Enemy.front->data);
					else insert_at_end(Active_A, Inactive_Enemy.front->data);
				}
				else if (T2.Health>0)
				{
					Inactive_Enemy.front->data.Region = B_REG;

					if (Inactive_Enemy.front->data.Type == SHLD_FITR)
						insert_at_end(Sheld_B, Inactive_Enemy.front->data);
					else insert_at_end(Active_B, Inactive_Enemy.front->data);
				}
			}
			else
			{
				if (T4.Health > 0)
				{
					if (Inactive_Enemy.front->data.Type == SHLD_FITR)
						insert_at_end(Sheld_D, Inactive_Enemy.front->data);
					else insert_at_end(Active_D, Inactive_Enemy.front->data);
				}
				else if (T1.Health>0)
				{
					Inactive_Enemy.front->data.Region = A_REG;

					if (Inactive_Enemy.front->data.Type == SHLD_FITR)
						insert_at_end(Sheld_A, Inactive_Enemy.front->data);
					else insert_at_end(Active_A, Inactive_Enemy.front->data);
				}
				else if (T2.Health>0)
				{
					Inactive_Enemy.front->data.Region = B_REG;

					if (Inactive_Enemy.front->data.Type == SHLD_FITR)
						insert_at_end(Sheld_B, Inactive_Enemy.front->data);
					else insert_at_end(Active_B, Inactive_Enemy.front->data);
				}
				else if (T3.Health>0)
				{
					Inactive_Enemy.front->data.Region = C_REG;

					if (Inactive_Enemy.front->data.Type == SHLD_FITR)
						insert_at_end(Sheld_C, Inactive_Enemy.front->data);
					else insert_at_end(Active_C, Inactive_Enemy.front->data);
				}
			}
			dequeue(Inactive_Enemy);

		}


		Calculations(Active_A, T1, Paved_A, time_step);
		Calculations(Sheld_A, T1, Paved_A, time_step);
		Calculations(Active_B, T2, Paved_B, time_step);
		Calculations(Sheld_B, T2, Paved_B, time_step);
		Calculations(Active_C, T3, Paved_C, time_step);
		Calculations(Sheld_C, T3, Paved_C, time_step);
		Calculations(Active_D, T4, Paved_D, time_step);
		Calculations(Sheld_D, T4, Paved_D, time_step);

		calculate_priority(Sheld_A.head, C1, C2, C3);
		calculate_priority(Sheld_B.head, C1, C2, C3);
		calculate_priority(Sheld_C.head, C1, C2, C3);
		calculate_priority(Sheld_D.head, C1, C2, C3);



		if (T1.Health <= 0)
		{
			if (T2.Health > 0)
				transport(Sheld_A, Active_A, Sheld_B, Active_B, Paved_B, C1, C2, C3,'B');
			else if (T3.Health > 0)
				transport(Sheld_A, Active_A, Sheld_C, Active_C, Paved_C, C1, C2, C3,'C');
			else if (T4.Health > 0)
				transport(Sheld_A, Active_A, Sheld_D, Active_D, Paved_D, C1, C2, C3,'D');
		}
		if (T2.Health <= 0)
		{
			if (T3.Health > 0)
				transport(Sheld_B, Active_B, Sheld_C, Active_C, Paved_C, C1, C2, C3,'C');
			else if (T4.Health > 0)
				transport(Sheld_B, Active_B, Sheld_D, Active_D, Paved_D, C1, C2, C3,'D');
			else if (T1.Health > 0)
				transport(Sheld_B, Active_B, Sheld_A, Active_A, Paved_A, C1, C2, C3,'A');
		}
		if (T3.Health <= 0)
		{
			if (T4.Health > 0)
				transport(Sheld_C, Active_C, Sheld_D, Active_D, Paved_D, C1, C2, C3,'D');
			else if (T1.Health > 0)
				transport(Sheld_C, Active_C, Sheld_A, Active_A, Paved_A, C1, C2, C3,'A');
			else if (T2.Health > 0)
				transport(Sheld_C, Active_C, Sheld_B, Active_B, Paved_B, C1, C2, C3,'B');
		}
		if (T4.Health <= 0)
		{
			if (T1.Health > 0)
				transport(Sheld_D, Active_D, Sheld_A, Active_A, Paved_A, C1, C2, C3,'A');
			else if (T2.Health > 0)
				transport(Sheld_D, Active_D, Sheld_B, Active_B, Paved_B, C1, C2, C3,'B');
			else if (T3.Health > 0)
				transport(Sheld_D, Active_D, Sheld_C, Active_C, Paved_C, C1, C2, C3,'C');
		}
		Damage_to_enemy(Sheld_A, Active_A, T1, time_step, Killed_Enemy, aux, counterA, counterB, counterC, counterD, counterAT, counterBT, counterCT, counterDT);
		Damage_to_enemy(Sheld_B, Active_B, T2, time_step, Killed_Enemy, aux, counterA, counterB, counterC, counterD, counterAT, counterBT, counterCT, counterDT);
		Damage_to_enemy(Sheld_C, Active_C, T3, time_step, Killed_Enemy, aux, counterA, counterB, counterC, counterD, counterAT, counterBT, counterCT, counterDT);
		Damage_to_enemy(Sheld_D, Active_D, T4, time_step, Killed_Enemy, aux, counterA, counterB, counterC, counterD, counterAT, counterBT, counterCT, counterDT);
		if (aux.count > 1)
			aux.head = MergeSort(aux.head);
		while (aux.count != 0)
		{
			enqueue(Killed_Enemy, aux.head->data);
			delete_from_list(aux, aux.head);
		}
		dec_time(Active_A);
		dec_time(Active_B);
		dec_time(Active_C);
		dec_time(Active_D);
		dec_time(Sheld_A);
		dec_time(Sheld_B);
		dec_time(Sheld_C);
		dec_time(Sheld_D);
		PrintMsg("Total current eneimes  A =");
		char cc[100];
		_itoa_s(Active_A.count + Sheld_A.count, cc, 10);
		PrintMsg(cc);
		PrintMsg("\tB =");
		_itoa_s(Active_B.count + Sheld_B.count, cc, 10);
		PrintMsg(cc);
		PrintMsg("\tC = ");
		_itoa_s(Active_C.count + Sheld_C.count, cc, 10);
		PrintMsg(cc);
		PrintMsg("\tD =");
		_itoa_s(Active_D.count + Sheld_D.count, cc, 10);
		PrintMsg(cc);
		PrintMsg("\n");

		PrintMsg("Last time step kill    A = ");
		_itoa_s(counterA, cc, 10);
		PrintMsg(cc);
		PrintMsg("\tB =");
		_itoa_s(counterB, cc, 10);
		PrintMsg(cc);
		PrintMsg("\tC = ");
		_itoa_s(counterC, cc, 10);
		PrintMsg(cc);
		PrintMsg("\tD =");
		_itoa_s(counterD, cc, 10);
		PrintMsg(cc);
		PrintMsg("\n");

		PrintMsg("Total killed by        A =");
		_itoa_s(counterAT, cc, 10);
		PrintMsg(cc);
		PrintMsg("\tB =");
		_itoa_s(counterBT, cc, 10);
		PrintMsg(cc);
		PrintMsg("\tC = ");
		_itoa_s(counterCT, cc, 10);
		PrintMsg(cc);
		PrintMsg("\tD =");
		_itoa_s(counterDT, cc, 10);
		PrintMsg(cc);
		PrintMsg("\n");

		PrintMsg("Unpaved distance      A = ");
		_itoa_s(60 - Paved_A, cc, 10);
		PrintMsg(cc);
		PrintMsg("\tB =");
		_itoa_s(60 - Paved_B, cc, 10);
		PrintMsg(cc);
		PrintMsg("\tC = ");
		_itoa_s(60 - Paved_C, cc, 10);
		PrintMsg(cc);
		PrintMsg("\tD =");
		_itoa_s(60 - Paved_D, cc, 10);
		PrintMsg(cc);
		PrintMsg("\n");

		time_step++;
		Sleep(1000);

	}
	double FD = 0;
	double counter = 0;
	if (T1.Health <= 0 && T2.Health <= 0 && T3.Health <= 0 && T4.Health <= 0)
	{
		win = false;
		calculate_FD(Active_A, FD);
		calculate_FD(Active_B, FD);
		calculate_FD(Active_C, FD);
		calculate_FD(Active_D, FD);
		calculate_FD(Sheld_A, FD);
		calculate_FD(Sheld_B, FD);
		calculate_FD(Sheld_C, FD);
		calculate_FD(Sheld_D, FD);
		counter = Active_A.count + Active_B.count + Active_C.count + Active_D.count + Sheld_A.count + Sheld_B.count + Sheld_C.count + Sheld_D.count;
	}
	else win = true;
	outputfile(Killed_Enemy, counter, FD, T1, T2, T3, T4, Paved_A, Paved_B, Paved_C, Paved_D, win);
}
void file_loading(Queue &Inactive_Enemy, list &Active_A, list &Active_B, list &Active_C, list &Active_D, list &Sheld_A, list &Sheld_B, list &Sheld_C, list &Sheld_D, Queue &Killed_Enemy, list &aux)
{
	ifstream fin;
	fin.open("input.txt");
	Tower T1;
	Tower T2;
	Tower T3;
	Tower T4;
	fin >> T1.Health >> T1.number_of_enemies >> T1.fire_power;
	T1.initial_Health = T1.Health;
	T2.Health = T1.Health;
	T2.initial_Health = T2.Health;

	T2.fire_power = T1.fire_power;
	T2.number_of_enemies = T1.number_of_enemies;
	T3.Health = T1.Health;
	T3.initial_Health = T3.Health;

	T3.fire_power = T1.fire_power;
	T3.number_of_enemies = T1.number_of_enemies;
	T4.Health = T1.Health;
	T4.fire_power = T1.fire_power;
	T4.number_of_enemies = T1.number_of_enemies;
	T4.initial_Health = T4.Health;

	float C1, C2, C3;
	fin >> C1 >> C2 >> C3;
	int x;
	fin >> x;
	while (x != -1)
	{
		node * read = new node;
		read->data.ID = x;
		int tp;
		char reg;
		fin >> tp >> read->data.Time_stamp >> read->data.Health >> read->data.Fire_power >> read->data.Reload_period >> reg;
		read->data.Distance = 60;
		read->data.remaining_time = read->data.Reload_period;
		if (tp == 0)
			read->data.Type = PVR;
		else if (tp == 1)
			read->data.Type = FITR;
		else read->data.Type = SHLD_FITR;
		if (reg == 'A')
			read->data.Region = A_REG;
		else if (reg == 'B')
			read->data.Region = B_REG;
		else if (reg == 'C')
			read->data.Region = C_REG;
		else read->data.Region = D_REG;
		read->data.first_shot = false;
		enqueue(Inactive_Enemy, read->data);
		fin >> x;
	}
	fin.close();
	war(Inactive_Enemy, Killed_Enemy, Active_A, Active_B, Active_C, Active_D, Sheld_A, Sheld_B, Sheld_C, Sheld_D, C1, C2, C3, T1, T2, T3, T4, aux);
}
void outputfile(Queue killed, double counter, double FD, Tower T1, Tower T2, Tower T3, Tower T4, int Paved_A, int Paved_B, int Paved_C, int Paved_D, bool win)
{
	ofstream fout;
	fout.open("game.txt");
	fout << "T" << "        " << " S" << "       " << "FD" << "       " << "KD" << "       " << "FT" << endl;
	node* p = killed.front;
	double fight_delay = 0;
	double kill_delay = 0;

	while (p != NULL)
	{
		int T = p->data.KD + p->data.FD + p->data.Time_stamp;
		fight_delay += p->data.FD;
		kill_delay += p->data.KD;

		fout << T << "        " << p->data.ID << "        " << p->data.FD << "       " << p->data.KD << "       " << p->data.FT << endl;
		p = p->next;
	}


	fout << "T1_Total_Damage" << " " << "T2_Total_Damage" << " " << "T3_Total_Damage" << " " << "T4_Total_Damage" << endl;
	if (T1.Health <= 0)
		fout << T1.initial_Health << "           ";
	else fout << T1.initial_Health - T1.Health << "           ";
	if (T2.Health <= 0)
		fout << T2.initial_Health << "           ";
	else fout << T2.initial_Health - T2.Health << "           ";
	if (T3.Health <= 0)
		fout << T3.initial_Health << "           ";
	else fout << T3.initial_Health - T3.Health << "           ";
	if (T1.Health <= 0)
		fout << T4.initial_Health << "           " << endl << endl;
	else fout << T4.initial_Health - T4.Health << endl << endl;


	fout << "R1_Distance" << " " << "R2_Distance" << " " << "R3_Distance" << " " << "R4_Distance" << endl;
	fout << 60 - Paved_A << "            " << 60 - Paved_B << "            " << 60 - Paved_C << "            " << 60 - Paved_D << endl << endl;
	if (!win)
	{
		fout << "Game Is Lose " << endl << endl;

		fout << "Number of killed Enemies = " << killed.count << endl << endl;

		fout << "Number of alive Enemies = " << counter << endl << endl;

	}
	else
	{
		fout << "Game Is WIN " << endl << endl;

		fout << "Total Enemies = " << killed.count << endl << endl;
	}
	fout << "Fight Delay Average = " << (fight_delay + FD) / (counter + killed.count) << endl << endl;

	fout << "Kill Delay Average = " << (kill_delay) / (killed.count) << endl << endl;
}

void dec_time(list &l)
{
	node* p = l.head;
	while (p != NULL)
	{
		p->data.remaining_time--;
		if (p->data.remaining_time == 0)
			p->data.remaining_time = p->data.Reload_period;
		p = p->next;
	}

}

