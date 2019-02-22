/*
 * =====================================================================================
 *
 *       Filename:  heap.c
 *
 *    Description:  Solution for Programming Problem: Heap Tunable Branching Factor 
 *
 *        Version:  1.0
 *        Created:  02/21/2019 04:55:05 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Nikhil Kamat nnkamat, Deepali Prasad dprasad2
 *   Organization:  North Carolina State University
 *
 * =====================================================================================
 */
//We have used the following reference
//https://stackoverflow.com/questions/3536153/c-dynamically-growing-array
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
static int bf;
static int count=0;
static int shft=0;
typedef struct{
    int key;
    int value;
}Node;
typedef struct {
  Node *inst;
  size_t used;
  size_t size;
} Heap;

void initHeap(Heap *h, size_t initialSize) {
  h->inst=(Node*)malloc(initialSize*sizeof(Node));
  h->used=0;
  h->size=initialSize;
}
void printHeap(Heap *h)
{
    int i;
    for(i=0;i<(h->used);i++)
    {
        printf("%d %d\n",h->inst[i].key,h->inst[i].value);
    }
}
int parent(int n)
{
      return ((n-1)>>shft);
}
void percolateUp(Heap *h,int n)
{
    if(parent(n)>=0)
    {
        count++;
        if(h->inst[n].key<h->inst[parent(n)].key)
        {
            (h->inst[n].key)^=(h->inst[parent(n)].key)^=(h->inst[n].key)^=(h->inst[parent(n)].key);
            (h->inst[n].value)^=(h->inst[parent(n)].value)^=(h->inst[n].value)^=(h->inst[parent(n)].value);

            percolateUp(h,parent(n));
        }
    }
}

void insertValue(Heap *h, int k, int v)
{
    if(h->used == h->size)
    {
        h->size*=2;
        h->inst=(Node*)realloc(h->inst, h->size * sizeof(Node));
    }
    h->inst[h->used].key=k;
    h->inst[h->used].value=v;

    percolateUp(h,h->used);
    h->used++;
}
void freeHeap(Heap *h) {
  free(h->inst);
  h->inst=NULL;
  h->used=h->size=0;
}
void MinHeapify(Heap *h,int i)
{
    int smaller,j,min;
    if((i<((h->used)>>shft) || i==0 ) && h->used>1 )
    {
        count++;
        min=h->inst[(i<<shft)+1].key;
        smaller=(i<<shft)+1;
        for(j=(i<<shft)+2;j<=(i<<shft)+bf && j<h->used;j++)
        {
            if(h->inst[j].key<min)
            {
                min=h->inst[j].key;
                smaller=j;
            }
        }
        if(h->inst[i].key>h->inst[smaller].key)
        {
            (h->inst[i].key)^=(h->inst[smaller].key)^=(h->inst[i].key)^=(h->inst[smaller].key);
            (h->inst[i].value)^=(h->inst[smaller].value)^=(h->inst[i].value)^=(h->inst[smaller].value);

            MinHeapify(h,smaller);
        }
    }

}
void removeMin(Heap *h,int *k,int *v)
{
    *k=h->inst[0].key;
    *v=h->inst[0].value;

    h->used--;

    (h->inst[0].key)^=(h->inst[h->used].key)^=(h->inst[0].key)^=(h->inst[h->used].key);
    (h->inst[0].value)^=(h->inst[h->used].value)^=(h->inst[0].value)^=(h->inst[h->used].value);

    MinHeapify(h,0);
}

int main(int argc, char** argv)
{
    int j;
    char *c,in[100];
    int key,value;
    int rkey,rvalue;
    Heap h;
    initHeap(&h,2);
    if(argc<2)
        bf=2;
    else if(argc==2)
        bf=(int)strtol(argv[argc-1],&c,10);
    else
    {
        printf("Please check input\n");
        exit(1);
    }
    //Branching Factor Power of 2
    if((bf&(bf-1))!=0)
    {
        printf("Please check Branching Factor \n");
        exit(1);
    }
    else
    {
        j=bf;
        while(j/2!=0)
        {
            shft++;
            j=j/2;
        }
    }
    while(scanf("%s",in)!=EOF)
    {
        if(in[0]!='-')
        {
            key=(int)strtol(in,&c,10);
            scanf("%s",in);
            value=(int)strtol(in,&c,10);
            insertValue(&h,key,value);
        }
        else
        {
            removeMin(&h,&rkey,&rvalue);
            printf("%d %d\n",rkey,rvalue);
        }
    }
    printf("key comparisons: %d\n",count);
    freeHeap(&h);
    return 0;
}
