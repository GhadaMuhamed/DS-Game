#include <iostream>
#include "Header.h"
using namespace std;
int main()
{
	SetWindow();

	Queue Inactive_Enemy = create_queue();

	Queue Killed_Enemy = create_queue();
	list Active_A = create_list();
	list Active_B = create_list();
	list Active_C = create_list();
	list Active_D = create_list();
	list Sheld_A = create_list();
	list Sheld_B = create_list();
	list Sheld_C = create_list();
	list Sheld_D = create_list();
	list aux = create_list();
	file_loading(Inactive_Enemy, Active_A, Active_B, Active_C, Active_D, Sheld_A, Sheld_B, Sheld_C, Sheld_D, Killed_Enemy, aux);
}
Queue create_queue()
{
	Queue q;
	q.front = NULL;
	q.rear = NULL;
	q.count = 0;
	return q;
}
list create_list()
{
	list L;
	L.head = NULL;
	L.count = 0;
	return L;
}
node* Split(node* my_node)
{
	node* secondNode;

	if (my_node == NULL) return NULL;
	else if (my_node->next == NULL) return NULL;
	else {
		secondNode = my_node->next;
		my_node->next = secondNode->next;
		secondNode->next = Split(secondNode->next);
		return secondNode;
	}
}
node* Merge(node* firstNode, node* secondNode)
{
	if (firstNode == NULL) return secondNode;
	else if (secondNode == NULL) return firstNode;
	else {
		if (firstNode->data.Health <= 0 && secondNode->data.Health <= 0)
		{
			if (firstNode->data.FD <= secondNode->data.FD)
			{
				firstNode->next = Merge(firstNode->next, secondNode);
				return firstNode;
			}
			else {
				secondNode->next = Merge(firstNode, secondNode->next);
				return secondNode;
			}
		}
		else if (firstNode->data.Type == SHLD_FITR)
		{
			if (firstNode->data.priority >= secondNode->data.priority)
			{
				firstNode->next = Merge(firstNode->next, secondNode);
				return firstNode;
			}
			else {
				secondNode->next = Merge(firstNode, secondNode->next);
				return secondNode;
			}
		}
		else {
			if (firstNode->data.Time_stamp <= secondNode->data.Time_stamp)
			{
				firstNode->next = Merge(firstNode->next, secondNode);
				return firstNode;
			}
			else {
				secondNode->next = Merge(firstNode, secondNode->next);
				return secondNode;
			}
		}
	}
}
node* MergeSort(node *my_node)
{
	node *secondNode;

	if (my_node == NULL)
		return NULL;
	else if (my_node->next == NULL)
		return my_node;
	else
	{
		secondNode = Split(my_node);
		return Merge(MergeSort(my_node), MergeSort(secondNode));
	}
}

void enqueue(Queue&q1, enemy data)
{
	node* Eq = new node;
	Eq->data = data;
	Eq->next = NULL;
	if (q1.count == 0)
		q1.front = Eq;
	else
		q1.rear->next = Eq;

	q1.rear = Eq;
	q1.count++;

}
void dequeue(Queue &q2)
{
	node* temp;
	if (q2.count == 0) return;
	else {
		temp = q2.front;
		q2.front = q2.front->next;
		q2.count--;
	}
	delete temp;
}
void insert_at_end(list &L1, enemy data)
{
	node* p = new node;

	if (L1.count != 0)
	{
		node* p1 = L1.head;
		while (p1->next != NULL)
			p1 = p1->next;
		p1->next = p;

	}
	else
	{
		L1.head = p;
	}
	p->data = data;
	p->next = NULL;
	L1.count++;
}
void delete_from_list(list &L, node* del)
{
	if (L.count == 0) return;
	node* tmp1;
	if (L.head->data.ID == del->data.ID)
	{
		tmp1 = L.head;
		L.head = L.head->next;
		delete tmp1;
	}

	else {
		node* ptr = L.head;
		while (ptr->next != NULL)
		{
			if (ptr->next->data.ID == del->data.ID)
			{
				tmp1 = ptr->next;
				ptr->next = ptr->next->next;
				delete tmp1;
			}
			else
				ptr = ptr->next;
		}
	}
	L.count--;
}