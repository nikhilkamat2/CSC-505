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
 *         Author:  Nikhil Kamat nnkamat 
 *
 *   Organization:  North Carolina State University
 *
 * =====================================================================================
 */
//Have used the following reference
//https://stackoverflow.com/questions/3536153/c-dynamically-growing-array
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
static int bf;
static int count=0;
static int shft=0;
static int *heapPositions;
typedef struct{
    int key;
    int value;
}Node;
typedef struct {
  Node *inst;
  size_t used;
  size_t size;
} Heap;


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  swap
 *  Description:  Swaps 2 Integers
 * =====================================================================================
 */
void swap( int *a, int *b )
{
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  initHeap
 *  Description:  Initialises a Dynamic Array to Create a Heap
 * =====================================================================================
 */
void initHeap(Heap *h, size_t initialSize) {
  h->inst=(Node*)malloc(initialSize*sizeof(Node));
  heapPositions=(int *)malloc(initialSize*sizeof(int));
  h->used=0;
  h->size=initialSize;
}
/*
 * ===  FUNCTION  ======================================================================
 *         Name:  printHeap
 *  Description:  Prints all elements of the Heap
 * =====================================================================================
 */
void printHeap(Heap *h)
{
    int i;
    for(i=0;i<(h->used);i++)
    {
        printf("%d %d\n",h->inst[i].key,h->inst[i].value);
    }
}
/*
 * ===  FUNCTION  ======================================================================
 *         Name:  parent
 *  Description:  Returns the parent of a node
 * =====================================================================================
 */
int parent(int n)
{
      return ((n-1)>>shft);
}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  percolateUp
 *  Description:  Percolates a Node up the Heap
 * =====================================================================================
 */
void percolateUp(Heap *h,int n)
{
    if(parent(n)>=0)
    {
        count++;
        if(h->inst[n].key<h->inst[parent(n)].key)
        {
            swap(&h->inst[n].key, &h->inst[parent(n)].key);
            swap(&h->inst[n].value, &h->inst[parent(n)].value);

            percolateUp(h,parent(n));
        }
    }
}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  insertValue
 *  Description:  Inserts a Value into the Heap
 * =====================================================================================
 */
void insertValue(Heap *h, int k, int v)
{
    if(h->used == h->size)
    {
        h->size*=2;
        h->inst=(Node*)realloc(h->inst, h->size * sizeof(Node));
        heapPositions=(int*)realloc(heapPositions,h->size*sizeof(int));
    }
    h->inst[h->used].key=k;
    h->inst[h->used].value=v;

    percolateUp(h,h->used);
    h->used++;
}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  freeHeap
 *  Description:  Release the memory allocated for the Heap
 * =====================================================================================
 */
void freeHeap(Heap *h) {
  free(h->inst);
  h->inst=NULL;
  h->used=h->size=0;
}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  MinHeapify
 *  Description:  Build an Min-Heap from an array
 * =====================================================================================
 */
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
            count++;
            if(h->inst[j].key<min)
            {
                min=h->inst[j].key;
                smaller=j;
            }
        }
        if(h->inst[i].key>h->inst[smaller].key)
        {
            swap(&h->inst[i].key, &h->inst[smaller].key);
            swap(&h->inst[i].value, &h->inst[smaller].value);

            MinHeapify(h,smaller);
        }
    }
}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  removeMin
 *  Description:  Removes the Minimum Value from the Heap
 * =====================================================================================
 */
void removeMin(Heap *h,int *k,int *v)
{
    *k=h->inst[0].key;
    *v=h->inst[0].value;

    h->used--;

    swap(&h->inst[0].key, &h->inst[h->used].key);
    swap(&h->inst[0].value, &h->inst[h->used].value);

    MinHeapify(h,0);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  For Handling Input and Function Calls
 * =====================================================================================
 */
int main(int argc, char** argv)
{
    int j;
    char *c,in[100];
    int key,value;
    int rkey,rvalue;
    Heap h;
    initHeap(&h,2);
    //Valid Input
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
    if(((bf&(bf-1))!=0) || (bf<2) )
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
    //Heap Operations
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
