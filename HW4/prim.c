/*
 * =====================================================================================
 *
 *       Filename:  prim.c
 *
 *    Description:  Implementation of Prim's MST Algorithm as per HW4
 *
 *        Version:  1.0
 *        Created:  04/11/2019 09:46:41 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author: Nikhil Kamat nnkamat
 *   Organization: North Carolina State Univertsity
 *
 * =====================================================================================
 */
#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

//Globals
static int *heapPositions;
static int shft=0;
static int bf;
static int numtrees=1;
static int mstweight;

//Graph Declarations
struct node {
    int vertex;
    int weight;
    struct node *next;
}typedef Node;

typedef struct {
    Node *start;
}Alist;

typedef struct {
    int vertices;
    int edges;
    Alist *list;
}Graph;

//Heap Declarations
typedef struct{
    int key;
    int value;
}HeapNode;

typedef struct {
  HeapNode *inst;
  size_t used;
  size_t size;
} Heap;

void swap( int *a, int *b )
{
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

//Graph Functions
void initgraph(Graph *graph,int vertices,int edges)
{
    int i;

    graph->vertices=vertices;
    graph->edges=edges;

    graph->list=(Alist *)malloc(vertices*sizeof(Alist));

    for(i=0;i<vertices;i++)
        graph->list[i].start=NULL;

    heapPositions=(int *)malloc(graph->vertices*sizeof(int));
}

void addEdge(Graph *graph,int u, int v, int weight)
{
    Node *node1 = (Node *)malloc(sizeof(Node));
    node1->next=graph->list[u].start;
    node1->vertex=v;
    node1->weight=weight;
    graph->list[u].start=node1;

    Node *node2 = (Node *)malloc(sizeof(Node));
    node2->next=graph->list[v].start;
    node2->vertex=u;
    node2->weight=weight;
    graph->list[v].start=node2;
}

void printAlist(Graph *graph)
{
    int i;
    Node *ptr;
    for(i=0;i<graph->vertices;i++)
    {
        ptr=graph->list[i].start;
        printf("\n Adjacency list of vertex %d\n start", i);
        while(ptr)
        {
            printf("-> %d(w:%d)", ptr->vertex,ptr->weight);
            ptr=ptr->next;
        }
        printf("\n");
    }

}

void freeGraph(Graph *graph) {
    free(graph->list);
    graph->list=NULL;
    graph->vertices=graph->edges=0;
}

//Heap Functions
void initHeap(Heap *h, size_t initialSize) {
  h->inst=(HeapNode*)malloc(initialSize*sizeof(HeapNode));
  h->used=0;
  h->size=initialSize;
}

void printHeap(Heap *h,int V)
{
    int i;
    printf("Heap\n");
    for(i=0;i<(h->used);i++)
    {
       printf("%d %d\n",h->inst[i].key,h->inst[i].value);
    }
    printf("Heap Positions\n");
    for(i=0;i<V;i++)
    {
       printf("%d %d\n",i,heapPositions[i] );
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
       if(h->inst[n].key<h->inst[parent(n)].key)
       {
          heapPositions[h->inst[parent(n)].value]=n;
          heapPositions[h->inst[n].value]=parent(n);

          swap(&h->inst[n].key, &h->inst[parent(n)].key);
          swap(&h->inst[n].value, &h->inst[parent(n)].value);

          percolateUp(h,parent(n));
       }
    }
}

void insertValue(Heap *h, int k, int v)
{
    if(h->used == h->size)
    {
       h->size*=2;
       h->inst=(HeapNode*)realloc(h->inst, h->size * sizeof(HeapNode));
    }
    h->inst[h->used].key=k;
    h->inst[h->used].value=v;
    heapPositions[h->inst[h->used].value]=h->used;

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
          heapPositions[h->inst[i].value]=smaller;
          heapPositions[h->inst[smaller].value]=i;

          swap(&h->inst[i].key, &h->inst[smaller].key);
          swap(&h->inst[i].value, &h->inst[smaller].value);

          MinHeapify(h,smaller);
        }
    }
}

void removeMin(Heap *h,int *k,int *v)
{
    *k=h->inst[0].key;
    *v=h->inst[0].value;

    heapPositions[h->inst[0].value]=-1;

    h->used--;

    swap(&h->inst[0].key, &h->inst[h->used].key);
    swap(&h->inst[0].value, &h->inst[h->used].value);

    heapPositions[h->inst[0].value]=0;

    MinHeapify(h,0);
}

void decreaseKey(Heap *h,int vertex,int key)
{
    h->inst[heapPositions[vertex]].key=key;
    percolateUp(h,heapPositions[vertex]);
}

void prim(Graph *graph)
{

    int i,rkey,rvertex;
    int v;

    Heap heap;
    initHeap(&heap,bf);

    heapPositions=(int *)malloc((graph->vertices)*sizeof(int));

    for(i=0;i<graph->vertices;i++)
    {
        insertValue(&heap,INT_MAX,i);
    }

    decreaseKey(&heap,0,0);

    mstweight=0;
    while(heap.used>0)
    {
        removeMin(&heap,&rkey,&rvertex);
        if(rkey!=INT_MAX)
            mstweight+=rkey;
        else
            numtrees++;

        Node *ptr = graph->list[rvertex].start;
        while(ptr!=NULL)
        {
           v = ptr->vertex;
           if((heapPositions[v]!=-1) && (ptr->weight<(heap.inst[heapPositions[v]].key)))
           {
               decreaseKey(&heap,v,ptr->weight);
           }
           ptr=ptr->next;
        }
    }

}

//Main
int main(int argv, char** argc)
{
    Graph graph;
    int v,e;
    int x,y,weight;
    int i,j;
    scanf("%d %d",&v,&e);
    initgraph(&graph,v,e);
    for(i=0;i<graph.edges;i++)
    {
        scanf("%d %d %d",&x,&y,&weight);
        addEdge(&graph,x,y,weight);
    }

    bf=2;
    j=2;
    while(1)
    {
        if(j*v>e)
        {
            bf=j;
            break;
        }
        else
           j=j*2;
    }

    j=bf;
    while(j/2!=0)
    {
        shft++;
        j=j/2;
    }

    prim(&graph);
    printf("%d %d %d\n",bf,numtrees,mstweight);

    return 0;
}
