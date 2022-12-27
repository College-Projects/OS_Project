#pragma once
#include <string.h>
#include <stdbool.h>
typedef struct PCB {
    char name;
    int structure;
    int ArrivalTime;         
    int FinishTime;            
    int ResidualTime;         
    float I_O;
    int nofI_O;
    int ResidualI_O;
    int cpu ;
    char *state_now;
    int str_now;
    int timer ;
	//int ServiceTime;          

} process,PCB;

typedef struct Node {
	process data;
	struct Node *next;
}Node;
typedef struct Queue {
	Node *front;
	Node *rear;
	size_t size;			
}Queue;


typedef struct linkList {
    Node *head;
    Node *tail;
    size_t size;
}linkList;

void QueueInit(Queue *pqueue) {			
	assert(pqueue != NULL);
	pqueue->front = NULL;
	pqueue->rear = NULL;
	pqueue->size = 0;
}

void QueueDestroy(Queue *pqueue)
{
	Node *cur = NULL;
	Node *del = NULL;
	assert(pqueue != NULL);
	if (pqueue->front == NULL) {
		return;
	}
	cur = pqueue->front;
	while (cur != NULL) {
		del = cur;
		cur = cur->next;
		free(del);
		del = NULL;
	}
}

Node *CreatNode(process data)
{
	Node *ret = (Node *)malloc(sizeof(Node));
	if (ret == NULL) {
		exit(0);
	}
	ret->data = data;
	ret->next = NULL;
	return ret;
}

void QueuePush(Queue *pqueue, process data)
{
	assert(pqueue != NULL);
	Node *temp = CreatNode(data);
	if (pqueue->front == NULL) {
		pqueue->front = temp;
		pqueue->rear = temp;
		//pqueue->rear = pqueue->front = temp;
		++pqueue->size;
	}
	else {
		pqueue->rear->next = temp;
		pqueue->rear = pqueue->rear->next;
		//pqueue->rear = temp;
		++pqueue->size;
	}
}

void QueuePop(Queue *pqueue)
{
	assert(pqueue != NULL);
	Node *del = NULL;
	assert(pqueue != NULL);
	if (pqueue->front == NULL) {
		return;
	}
	del = pqueue->front;
	pqueue->front = pqueue->front->next;
	free(del);
	del = NULL;
	--pqueue->size;
    
}

Node *QueueTop(Queue *pqueue)
{
	return pqueue->front;
}


bool QueueEmpty(const Queue *pqueue)
{
	return pqueue->front == NULL;
}

size_t QueueSize(const Queue *pqueue)
{
    return pqueue->size;
}




void linkListInitialize(linkList *plinklist)
{
    assert(plinklist != NULL);
    plinklist->head = NULL;
    plinklist->tail = NULL;
    plinklist->size = 0;    

}

void linkListDestroy(linkList *plinklist)
{
	Node *cur = NULL;
	Node *del = NULL;
	assert(plinklist != NULL);
	if (plinklist->head == NULL) {
		return;
	}
	cur = plinklist->head;
	while (cur != NULL) {
		del = cur;
		cur = cur->next;
		free(del);
		del = NULL;
	}
}

void linkListPushBack(linkList *plinklist, process data)
{
	assert(plinklist != NULL);
	Node *temp = CreatNode(data);
	if (plinklist->head == NULL) {
		plinklist->head = temp;
		plinklist->tail = temp;
		//pqueue->rear = pqueue->front = temp;
		++plinklist->size;
	}
	else {
		plinklist->tail->next = temp;
		plinklist->tail = plinklist->tail->next;
		//pqueue->rear = temp;
		++plinklist->size;
	}
}

void linkListErase(linkList *plinklist, Node *pnode)
{
	Node *cur = NULL;
	assert(plinklist != NULL);
	cur = plinklist->head;
	if ((plinklist->head) == NULL) {
		return;
	}
    if ((plinklist->head) == pnode) {
        plinklist->head = pnode->next;
        free(pnode);
        pnode = NULL;
        --(plinklist->size);
        return;
    }
    while(cur != NULL) {
        if (cur->next == pnode) {
            cur->next = pnode->next;
            free(pnode);
            pnode = NULL;
            --(plinklist->size);
            break;
        }
        cur = cur->next;
    } 
}

bool linkListEmpty(const linkList *plinkList)
{
	return plinkList->head == NULL;
}

size_t linkListSize(const linkList *plinklist)
{
    return plinklist->size;
}
