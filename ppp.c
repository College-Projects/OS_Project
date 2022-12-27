#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include "common.h"
#include "common_threads.h"
#include <pthread.h>
#include <assert.h>
#include <string.h>
#include "hea.h"
#include <string.h>
#include <stdbool.h>

int done = 0;
typedef struct {
    Queue *queue;
    Queue *waitingqueue;
    process *pcbs;
    int processnumber;
} st_pr;
typedef struct {
    linkList *linkList;
    linkList *linkListwaiting;
    process *pcbs;
    int processnumber;
} st_pr2;
typedef struct {
    Queue *queue;
    Queue *waitingqueue;
    process *pcbs;
    int processnumber;
    int q;
} st_pr3;

void *FIFO(void *arg) {  
     st_pr *allstr=(st_pr *) arg; 
    int pcbnumber = allstr->processnumber;
	int time1 = 0;
    srand(time(NULL)); 
	while (1) {
        if (!QueueEmpty(allstr->waitingqueue)){
            Node *current = allstr->waitingqueue->front;
            while (current!=NULL)
            {
                current->data.timer--;
                if(current->data.timer==0){
                    QueueTop(allstr->waitingqueue)->data.state_now="ready";
                    printf("%c is ready now ",QueueTop(allstr->waitingqueue)->data.name);

                    QueuePush(allstr->queue,QueueTop(allstr->waitingqueue)->data);
                    QueuePop(allstr->waitingqueue);  
                }
                    current=current->next;

            }
            
        }
        for (int i=0; i < pcbnumber; i++) {
            if (allstr->pcbs[i].ArrivalTime == time1) {
                allstr->pcbs[i].nofI_O=(allstr->pcbs[i].structure)*(allstr->pcbs[i].I_O) -1 ;
                allstr->pcbs[i].state_now="ready";
                allstr->pcbs[i].timer=3;
                QueuePush(allstr->queue, allstr->pcbs[i]);
                printf("%c is ready \n",allstr->pcbs[i].name);
			    printf("PCB %c ARRIVAL TIME IS %d\n", allstr->pcbs[i].name, time1);
                
            }
        }
        if (QueueEmpty(allstr->queue) && pcbnumber == 0) {
			printf("finsh all！\n");
			break;
		}
       
        if (!QueueEmpty(allstr->queue) && QueueTop(allstr->queue)->data.ResidualTime == 0) {
            printf("PCB %c FINISH TIME IS %d\n", QueueTop(allstr->queue)->data.name, time1);
            QueuePop(allstr->queue);
            pcbnumber--;
        }
        if (!QueueEmpty(allstr->queue)) {
            QueueTop(allstr->queue)->data.ResidualTime--;
        int ra =rand()%QueueTop(allstr->queue)->data.structure ;
        printf("ra =%d  ",ra);
        if(ra<=QueueTop(allstr->queue)->data.nofI_O){
        QueueTop(allstr->queue)->data.state_now="waiting";
        QueueTop(allstr->queue)->data.timer=3;
        printf("%c is waiting now \n",QueueTop(allstr->queue)->data.name);
        QueuePush(allstr->waitingqueue, QueueTop(allstr->queue)->data);
        QueuePop(allstr->queue);
		
        }
        else{
            printf("%c is running now with residual time =%d\n",QueueTop(allstr->queue)->data.name,QueueTop(allstr->queue)->data.ResidualTime);
            QueueTop(allstr->queue)->data.state_now ="running";
		
        }
    }
    time1++;
    printf("time=%d\n",time1);
       
        Spin(1);
    }
    

    return NULL;
}
void *SPF(void *arg) {  
     st_pr2 *allstr=(st_pr2 *) arg; 
    int pcbnumber = allstr->processnumber;
	int time1 = 0;
    srand(time(NULL)); 
	
    Node *curnode = NULL;
    Node *minnode = NULL;
    

    while (1) {
        
        if (!linkListEmpty(allstr->linkListwaiting)){
            Node *current = allstr->linkListwaiting->head;
            while (current!=NULL)
            {
                current->data.timer--;
                if(current->data.ResidualTime==0){
                    printf("PCB %c FINISH TIME IS %d\n", (current->data).name, time1);
                }
                if(current->data.timer==0&&current->data.ResidualTime!=0){
                    current->data.state_now="ready";
                    printf("%c is ready now ",current->data.name);

                    linkListPushBack(allstr->linkList,current->data);
                    linkListErase(allstr->linkListwaiting,current);  
                }
                    current=current->next;
            }
            
        }
        for (int i = 0; i < pcbnumber; i++) {
            if (allstr->pcbs[i].ArrivalTime == time1) {
            allstr->pcbs[i].nofI_O=(allstr->pcbs[i].structure)*(allstr->pcbs[i].I_O) -1 ;
                allstr->pcbs[i].state_now="ready";
                allstr->pcbs[i].timer=3;
                allstr->pcbs[i].cpu=allstr->pcbs[i].structure - allstr->pcbs[i].nofI_O +1;
                linkListPushBack(allstr->linkList, allstr->pcbs[i]);
                printf("PCB %c ARRIVAL TIME IS %d\n", allstr->pcbs[i].name, time1);
            }
        }

        if (linkListEmpty(allstr->linkList) && pcbnumber == 0) {
			printf("FINISHED！\n");
			break;
		}
	
        if (!linkListEmpty(allstr->linkList) && minnode == NULL) {
            minnode = NULL;
            curnode = allstr->linkList->head;
            while(curnode != NULL) {
                if (minnode == NULL || curnode->data.cpu < minnode->data.cpu) {
                    minnode = curnode;
                }
                curnode = curnode->next;
            }
        }
        
        if (minnode != NULL && (minnode->data).ResidualTime == 0) {
            printf("PCB %c FINISH TIME IS %d\n", (minnode->data).name, time1);
            linkListErase(allstr->linkList, minnode);
            minnode = NULL;
            pcbnumber--;
        }

        
    if (!linkListEmpty(allstr->linkList)&&minnode != NULL) {
        --(minnode->data).ResidualTime;    
        int ra =rand()%minnode->data.structure ;
        printf("ra =%d  ",ra);
        if(ra<=minnode->data.nofI_O){
            --(minnode->data).ResidualI_O;
        minnode->data.state_now="waiting";
        minnode->data.timer=3;
        printf("%c is waiting now \n",minnode->data.name);
        linkListPushBack(allstr->linkListwaiting,minnode->data);
        linkListErase(allstr->linkList, minnode);
        minnode = NULL;
        }
        else{
            --(minnode->data).cpu;
            printf("%c is running now with residual time =%d\n",minnode->data.name,minnode->data.ResidualTime);
            minnode->data.state_now ="running";
            
            
        }
    }
time1++;
            printf("time=%d\n",time1);
        Spin(1);
       
       
    }
    return NULL;
}void *STCF(void *arg) {  
     st_pr2 *allstr=(st_pr2 *) arg; 
    int pcbnumber = allstr->processnumber;
	int time1 = 0;
    srand(time(NULL)); 
	
    Node *curnode = NULL;
    Node *minnode = NULL;
     
    while (1) {
        if (!linkListEmpty(allstr->linkListwaiting)){
            Node *current = allstr->linkListwaiting->head;
            while (current!=NULL)
            {
                current->data.timer--;
                if(current->data.timer==0){
                    current->data.state_now="ready";
                    printf("%c is ready now ",current->data.name);

                    linkListPushBack(allstr->linkList,current->data);
                    linkListErase(allstr->linkListwaiting,current);  
                    minnode = NULL;
                }
                    current=current->next;
            }
            
        }
        for (int i = 0; i < pcbnumber; i++) {
            if (allstr->pcbs[i].ArrivalTime == time1) {
            allstr->pcbs[i].nofI_O=(allstr->pcbs[i].structure)*(allstr->pcbs[i].I_O)  ;
                allstr->pcbs[i].ResidualI_O=0;
                allstr->pcbs[i].cpu=0;
                allstr->pcbs[i].state_now="ready";
                allstr->pcbs[i].timer=3;


                linkListPushBack(allstr->linkList, allstr->pcbs[i]);
                
                printf("PCB %c ARRIVAL TIME IS %d\n", allstr->pcbs[i].name, time1);
                
                
            }
        }

        if (linkListEmpty(allstr->linkList) && pcbnumber == 0) {
			printf("FINISHED！\n");
			break;
		}

        if (!linkListEmpty(allstr->linkList) ) {
            minnode = NULL;
            curnode = allstr->linkList->head;
            while(curnode != NULL) {
                if (minnode == NULL || curnode->data.ResidualTime < minnode->data.ResidualTime) {
                    minnode = curnode;
                }
                curnode = curnode->next;
            }
        }
       
if (minnode != NULL && (minnode->data).ResidualTime == 0) {
            printf("PCB %c FINISH TIME IS %d\n", (minnode->data).name, time1);
            linkListErase(allstr->linkList, minnode);
            minnode = NULL;
            pcbnumber--;
        }
        


    if (!linkListEmpty(allstr->linkList)&&minnode != NULL) {
        --(minnode->data).ResidualTime;    
        int ra =rand()%minnode->data.structure ;
        printf("ra =%d  ",ra);
        if(ra<=minnode->data.nofI_O){
            ++(minnode->data).ResidualI_O;
        minnode->data.state_now="waiting";
        minnode->data.timer=3;
        printf("%c is waiting now and run %d io\n",minnode->data.name,minnode->data.ResidualI_O);
        linkListPushBack(allstr->linkListwaiting,minnode->data);
        linkListErase(allstr->linkList, minnode);
        minnode = NULL;
        
        }
        else{
            ++(minnode->data).cpu;
            printf("%c is running now with run in cpu =%d\n",minnode->data.name,minnode->data.cpu);
            minnode->data.state_now ="running";
            
            
        }
    }
        time1++;
        printf("time=%d\n",time1);
        Spin(1);
       
    
    }
    return NULL;
}

void MFCFS(Queue *pqueue, PCB pcbs[], int size, int t)
{
    int pcbnumber = size;
	int time = 0;


	while (1) {

        for (int i = 0; i < size; i++) {
            if (pcbs[i].ArrivalTime == time) {
                QueuePush(pqueue, pcbs[i]);
			    printf("PCB %c ARRIVAL TIME IS %d\n", pcbs[i].name, time);
                pcbnumber--;
            }
        }
        if (QueueEmpty(pqueue) && pcbnumber == 0) {
			printf("Finished！\n");
			break;
		}

        if (!QueueEmpty(pqueue) && QueueTop(pqueue)->data.ResidualTime == 0) {
            printf("PCB %c FINISH TIME IS %d\n", QueueTop(pqueue)->data.name, (time+t));
            QueuePop(pqueue);
        }

        if (!QueueEmpty(pqueue)) {
            QueueTop(pqueue)->data.ResidualTime--;
        }

		time++;
	}
}


void MLFQ(Queue *pqueue, PCB Q1[], int size)
{ int r=0, k=0, flag=0, time=0, tq1=5, tq2=8;

 PCB Q2[10];
 PCB Q3[10];
 int n=size;

struct PCB temp;
for(int i=0;i<n;i++)
{
    for(int j=i+1;j<n;j++)
        {
            if(Q1[i].ArrivalTime>Q1[j].ArrivalTime)
                {
                    temp=Q1[i];
                    Q1[i]=Q1[j];
                    Q1[j]=temp;

                }
        }
}

time=Q1[0].ArrivalTime;
printf("Process in first queue following RR with qt=5");
printf("\nProcess\t\tAT\t\tFT\t\t");

for(int i=0;i<n;i++)
{

    if(Q1[i].ResidualTime<=tq1)
    {
        time+=Q1[i].ResidualTime;
        Q1[i].ResidualTime=0;
        Q1[i].FinishTime=time;
        printf("\n%c\t\t%d\t\t%d\t\t",Q1[i].name,Q1[i].ArrivalTime,Q1[i].FinishTime);
    }
    else
        {
            time+=tq1;
            Q1[i].ResidualTime-=tq1;
            Q2[k].structure=Q1[i].ResidualTime;
            Q2[k].ResidualTime=Q2[k].structure;
            Q2[k].name=Q1[i].name;
            Q2[k].ArrivalTime=Q1[i].ArrivalTime;
            k=k+1;
            flag=1;
        }
}

if(flag==1)
{
    printf("\nProcess in second queue following RR with qt=8");
    printf("\nProcess\t\tAT\t\tFT\t\t");
}
for(int i=0;i<k;i++)
{
    if(Q2[i].ResidualTime<=tq2)
    {
        time+=Q2[i].ResidualTime;
        Q2[i].ResidualTime=0;
        Q2[i].FinishTime=time;
        printf("\n%c\t\t%d\t\t%d\t\t",Q2[i].name,Q2[i].ArrivalTime,Q2[i].FinishTime);

    }
    else
    {
        time+=tq2;
        Q2[i].ResidualTime-=tq2;
        Q3[r].structure=Q2[i].ResidualTime;
        Q3[r].ResidualTime=Q3[r].structure;
        Q3[r].name=Q2[i].name;
        Q3[r].ArrivalTime=Q2[i].ArrivalTime;
        r=r+1;
        flag=2;
    }
}

if(flag==2){
printf("\nProcess in third queue following FCFS\n ");
        Queue queue;
        QueueInit(&queue);
        MFCFS(&queue, Q3, r,time);
        QueueDestroy(&queue);
}

}

void *RR(void *arg)
{

    st_pr3 *allstr=(st_pr3 *) arg; 
    int pcbnumber = allstr->processnumber;
    int q = allstr->q;
	int time1 = 0;
    int lessq = 0;
	srand(time(NULL)); 


	while (1) {
		if(!QueueEmpty(allstr->waitingqueue)){
            Node *current = allstr->waitingqueue->front;
            while (current!=NULL)
            {
                current->data.timer--;
                if(current->data.timer==0){
                    QueueTop(allstr->waitingqueue)->data.state_now="ready";
                    printf("%c is ready now ",QueueTop(allstr->waitingqueue)->data.name);

                    QueuePush(allstr->queue,QueueTop(allstr->waitingqueue)->data);
                    QueuePop(allstr->waitingqueue);  
                }
                    current=current->next;
            }
        }
        for (int i=0; i < pcbnumber; i++) {
            if (allstr->pcbs[i].ArrivalTime == time1) {
                allstr->pcbs[i].nofI_O=(allstr->pcbs[i].structure)*(allstr->pcbs[i].I_O) -1 ;
                allstr->pcbs[i].state_now="ready";
                allstr->pcbs[i].timer=3;
                QueuePush(allstr->queue, allstr->pcbs[i]);
                printf("%c is ready \n",allstr->pcbs[i].name);
			    printf("PCB %c ARRIVAL TIME IS %d\n", allstr->pcbs[i].name, time1);
                
            }
        }
        if (QueueEmpty(allstr->queue) && pcbnumber == 0) {
			printf("finsh all！\n");
			break;
		}
    

        if (lessq == 0) {
            if (!QueueEmpty(allstr->queue) && QueueTop(allstr->queue)->data.ResidualTime == 0) {
                printf("PCB %c FINISH TIME IS %d\n", QueueTop(allstr->queue)->data.name, time1);

                QueuePop(allstr->queue);
                pcbnumber--;
            }
            else {
                if (!QueueEmpty(allstr->queue) &&
                    (QueueTop(allstr->queue)->data.structure !=
                    QueueTop(allstr->queue)->data.ResidualTime)) {
                    allstr->queue->rear->next = allstr->queue->front;
                    allstr->queue->rear = allstr->queue->rear->next;
                    allstr->queue->front = allstr->queue->front->next;
                    allstr->queue->rear->next = NULL;
                }
            }
            lessq = q;
		}else {
            if (!QueueEmpty(allstr->queue) && QueueTop(allstr->queue)->data.ResidualTime == 0) {
                printf("PCB %c FINISH TIME IS %d\n", QueueTop(allstr->queue)->data.name, time1);

                QueuePop(allstr->queue);
                pcbnumber--;
                lessq = q;
            }
        }
        if (!QueueEmpty(allstr->queue)) {
            QueueTop(allstr->queue)->data.ResidualTime--;
        int ra =rand()%QueueTop(allstr->queue)->data.structure ;
        printf("ra =%d  ",ra);
        if(ra<=QueueTop(allstr->queue)->data.nofI_O){
        QueueTop(allstr->queue)->data.state_now="waiting";
        QueueTop(allstr->queue)->data.timer=3;
        printf("%c is waiting now \n",QueueTop(allstr->queue)->data.name);
        QueuePush(allstr->waitingqueue, QueueTop(allstr->queue)->data);
        QueuePop(allstr->queue);
		
        }
        else{
            printf("%c is running now with residual time =%d\n",QueueTop(allstr->queue)->data.name,QueueTop(allstr->queue)->data.ResidualTime);
            QueueTop(allstr->queue)->data.state_now ="running";
        }
        
    }
        time1++;
        printf("time=%d\n",time1);
        lessq--;
        Spin(1);
        
	}
    return NULL;
}



int main(int argc, char *argv[]) {
int processnumber = 0,q=0;
pthread_t f1;
	Queue queue;
    Queue waitingqueue;

    linkList linklist;
        linkList linkListwaiting;


    printf("please input process number: ");
    scanf("%d", &processnumber);
    process *pcbs = (process *) malloc (sizeof(process) * processnumber);
    memset(pcbs, 0, sizeof(process) * processnumber);
    for (int i = 0; i < processnumber; i++) {
        printf("process->name: ");
        scanf(" %c",&(pcbs[i].name));
        printf("process->ArrivalTime: ");
        scanf("%d",&(pcbs[i].ArrivalTime));
        printf("process->structure: ");
        scanf("%d",&(pcbs[i].structure));
        pcbs[i].ResidualTime = pcbs[i].structure;
        printf("process->I_O: ");
        scanf("%e",&(pcbs[i].I_O));
    }
	int input = 0;
    QueueInit(&queue);
    QueueInit(&waitingqueue);
    linkListInitialize(&linklist);
    linkListInitialize(&linkListwaiting);
    st_pr st={&queue,&waitingqueue,pcbs,processnumber};
    st_pr2 st2={&linklist,&linkListwaiting,pcbs,processnumber};
    
	
    

	printf("1. FCFS   2.RR   3.SjF   4.STCF   5.MLFQ\n");
	printf("Please input number: ");
	scanf("%d", &input);
    int p_1 ;
    
	switch (input)
	{
	case 1:
		
		p_1 = pthread_create(&f1, NULL, FIFO, &st);
		break;
    case 2:
        printf("please input q:");
        scanf("%d", &q);
        st_pr3 st3={&queue,&waitingqueue,pcbs,processnumber,q};
        p_1 = pthread_create(&f1, NULL, RR, &st3);
		break;
    case 3:
        p_1 = pthread_create(&f1, NULL, SPF, &st2);
		break;
    case 4:
        p_1 = pthread_create(&f1, NULL,STCF , &st2);
        break;
    case 5:
    for(int i=0;i<processnumber;i++){
        pcbs[i].ResidualTime = pcbs[i].structure;
    }
        MLFQ(&queue, pcbs, processnumber);
		break;
	default:
        printf("wrong choice");
		break;
	}
    
    
    assert(p_1 == 0);

    pthread_join(f1, NULL);
    
    printf("done\n");
    linkListDestroy(&linklist);
    linkListDestroy(&linkListwaiting);

	QueueDestroy(&queue);
    QueueDestroy(&waitingqueue);
    return 0;
}

