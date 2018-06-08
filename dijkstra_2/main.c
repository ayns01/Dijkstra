//
//  main.c
//  dijkstra_2
//
//  Created by 酒井綾菜 on 2018-06-02.
//  Copyright © 2018 Ayana Sakai. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INF 10000
#define TRUE 1
#define FALSE 0
#define MAX_STR 256
#define MAX_WORD 128
int getNumberOfNodes(){ return 6; }
int getNumberOfEdges(){ return 9; }
char* getNodes(){ return "A,B,C,D,E,F,G"; }
char* getEdges(){ return "A_B_1,A_C_2,B_C_4,B_D_5,C_E_3,D_E_2,D_F_4,E_B_2,E_F_6"; }

struct Edge
{
    int distance;
    struct Node *to;
    struct Edge *next;
};

struct Node
{
    int visited;
    char name;
    int cost;
    struct Node *from;
    struct Edge *edge;
    struct Node *next;
};

int main(int argc, const char * argv[]) {
    
    char source;
    char distination;
    printf("Enter source alphabet\n");
    scanf(" %c", &source);
    printf("\nEnter distination alphabet\n");
    scanf(" %c", &distination);
    
    char *strnode = getNodes();
    char *gnode = NULL;
    //split(strnode, gnode);
    char *here = NULL;
    char *str_work = NULL;
    int last = 0;
    int visitednum = 0;
    int totalcost = 0;
    
    str_work = (char *)malloc(sizeof(char) * (strlen(strnode) + 1));
    strcpy(str_work, strnode);
    gnode = (char *)malloc(sizeof(char) * MAX_WORD);
    here = gnode;
    while (1) {
        if(last == 0) {
            str_work = strtok(str_work, ",");
            *gnode = *str_work;
            gnode++;
        }else {
            str_work = strtok(NULL, ",");
            if(str_work == NULL) {break;}
            *gnode = *str_work;
            gnode++;
        }
        last++;
    }
    gnode = NULL;
    gnode = here;
    
    /*
     sprit path
    */
    char *stredges = getEdges();
    char *first = NULL;
    char *second = NULL;
    int  *value = 0;
    //splitpath(stredges, first, second, value);
    char *str_copy = NULL;
    char *here_f = NULL;
    char *here_s = NULL;
    int *here_v = 0;
    int sum=1;
    
    str_copy = (char *)malloc(sizeof(char) * (strlen(stredges) + 1));
    strcpy(str_copy, stredges);
    first = (char *)malloc(sizeof(char) * MAX_WORD);
    second = (char *)malloc(sizeof(char) * MAX_WORD);
    value = (int *)malloc(sizeof(int));
    here_f = first;
    here_s = second;
    here_v = value;
    last = 0;
    /*
     split given path
    */
    while (1) {
        if(last == 0) {
            str_copy = strtok(str_copy, "_,");
            *first = *str_copy;
            first++;
        }else {
            str_copy = strtok(NULL, "_,");
            if(str_copy == NULL) {break;}
            if(last % 3 == 0) {
                *first = *str_copy;
                first++;
            }else if(last == sum) {
                *second = *str_copy;
                second++;
                sum = sum + 3;
            }else {
                *value = atoi(str_copy);
                value++;
            }
        }
        last++;
    }
    first = NULL;
    second = NULL;
    value = NULL;
    first = here_f;
    second = here_s;
    value = here_v;
    
    int e = getNumberOfEdges();
    int n = getNumberOfNodes();
    
    int i = 0;
    struct Node *p, *head;
    p = malloc(sizeof(struct Node));
    for(i=0; i<n; i++) {
        if(i == 0) {
            head = p;
        }else {
            p->next = malloc(sizeof(struct Node));
            p = p->next;
        }
        p->name = gnode[i];
        p->cost = INF;
        if(source == p->name) {
            p->cost = 0;
        }
        p->visited = 0;
        p->from = NULL;
        p->edge = NULL;
        p->next = NULL;
    }
    
    for(i=0;i<e;i++) {
        p = head;
        
        while(p != NULL)
        {
            if(p->name == first[i]) {
                if(p->edge == NULL) {
                    p->edge = malloc(sizeof(struct Edge));
                    struct Node *p2;
                    p2 = head;
                    while(second[i] != p2->name)
                    {
                        p2 = p2->next;
                    }
                    p->edge->to = p2;
                    p->edge->distance = value[i];
                    p->edge->next = NULL;
                    break;
                }else {
                    /*
                     check the end of edge
                    */
                    while (p->edge->next != NULL)
                    {
                        p->edge = p->edge->next;
                    }
                    struct Node *p2;
                    p2 = head;
                    while(second[i] != p2->name)
                    {
                        p2 = p2->next;
                    }
                    p->edge->next = malloc(sizeof(struct Edge));
                    p->edge->next->to = p2;
                    p->edge->next->distance = value[i];
                    p->edge->next->next = NULL;   //Add
                    break;
                }
            }
            p = p->next;
        }
    }
    
    /*
     Implement dijkstra
    */
    struct Node *pp;
    pp = head;
    while (1) {
        int min = INF;
        /*
         targetnode : Keep node of minimum cost
        */
        struct Node *targetnode;
        targetnode = NULL;
        pp = head;
        while(pp != NULL) {
            if (pp->cost < min && pp->visited==0) {
                min = pp->cost;
                targetnode = pp;
            }
            pp = pp->next;
        }

        if(targetnode == NULL) {
            pp = head;
            while(1) {
                if (pp->visited==0) {
                    targetnode = pp;
                    break;
                }
                pp = pp->next;
            }
        }
        
        /*
         calculate cost of each node and assgin 'from' node
        */
        if(targetnode->edge != NULL) {
            if(targetnode->edge->to->cost > targetnode->cost + targetnode->edge->distance) {
                targetnode->edge->to->cost = targetnode->cost + targetnode->edge->distance;
                targetnode->edge->to->from = targetnode;
            }
            /*
             If having next edge
            */
            struct Edge *pptemp;
            pptemp = targetnode->edge->next;
            while(pptemp != NULL)
            {
                if(pptemp->to->cost > targetnode->cost + pptemp->distance) {
                    pptemp->to->cost = targetnode->cost + pptemp->distance;
                    pptemp->to->from = targetnode;
                }
                pptemp = pptemp->next;
            }
        }
        /*
         change visited flag
        */
        targetnode->visited = 1;
        visitednum++;
        if(visitednum == n) {
            break;
        }
    }
    
    /*
     find distination node
    */
    struct Node *chkfrom;
    struct Node *temp;
    temp = NULL;
    chkfrom = head;
    printf("The shortest path is ");
    while (chkfrom != NULL) {
        if(chkfrom->name == distination) {
            printf("%c←", chkfrom->name);
            temp = chkfrom;
            totalcost = chkfrom->cost;
            break;
        }
        chkfrom = chkfrom->next;
    }
    
    while (temp->from != NULL) {
        if(temp->from->name == source) {
            break;
        }
        printf("%c←", temp->from->name);
        temp = temp->from;
    }
    printf("%c\n", source);
    printf("Total cost : %d\n", totalcost);
    
    /*
     free memory
    */
    free(str_work);
    free(str_copy);
    first = here_f;
    second = here_s;
    value = here_v;
//    for(i=0; i<e; i++) {
//        free(first);
//        free(second);
//        free(value);
//        first++;
//        second++;
//        value++;
//    }
    
    struct Node *q;
    struct Edge *r;
    p = head;
    while (p != NULL) {
        q = p->next;
        while (p->edge != NULL) {
            r = p->edge->next;
            free(p->edge);
            p->edge = r;
        }
        free(p);
        p = q;
    }
    
    return 0;
}