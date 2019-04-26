/*
 * =====================================================================================
 *
 *       Filename:  johnsons.c
 *
 *    Description:  Implementation of Johnson's Algorithm for Assignment 5
 *
 *        Version:  1.0
 *        Created:  04/26/2019 06:57:03 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Nikhil Kamat nnkamat
 *                  Deepali Prasad dprasad2
 *   Organization:  
 *
 * =====================================================================================
 */
#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<string.h>

//Globals
static int *heapPositions;
static int *distance;
static int shft=0;
static int bf;
static int *T;

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

//Graph Functions
void initgraph(Graph *graph,int vertices,int edges)
{
    int i,j; 
    graph->vertices=vertices;
    graph->edges=edges;

    graph->list=(Alist *)malloc(vertices*sizeof(Alist));

    for(i=0;i<vertices;i++)
        graph->list[i].start=NULL;

    heapPositions=(int *)malloc(graph->vertices*sizeof(int));

    T=(int *)malloc(vertices*vertices*sizeof(int));
    for(i=0;i<vertices;i++)
        for(j=0;j<vertices;j++)
            T[i*vertices+j]=INT_MAX;

}

void addEdge(Graph *graph,int u, int v, int weight)
{
    Node *node1 = (Node *)malloc(sizeof(Node));
    node1->next=graph->list[u].start;
    node1->vertex=v;
    node1->weight=weight;
    graph->list[u].start=node1;
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

    heapPositions[h->inst[0].value]=-1;

    h->used--;

    (h->inst[0].key)^=(h->inst[h->used].key)^=(h->inst[0].key)^=(h->inst[h->used].key);
    (h->inst[0].value)^=(h->inst[h->used].value)^=(h->inst[0].value)^=(h->inst[h->used].value);

    heapPositions[h->inst[0].value]=0;

    MinHeapify(h,0);
}

void decreaseKey(Heap *h,int vertex,int key)
{
    h->inst[heapPositions[vertex]].key=key;
    percolateUp(h,heapPositions[vertex]);
}

void djikstra(Graph *graph,int src)
{
    int i,rkey,rvertex,u,v,ver;
    ver=graph->vertices;

    Heap heap;
    initHeap(&heap,bf);

    heapPositions=(int *)malloc((graph->vertices)*sizeof(int));

    for(i=0;i<graph->vertices;i++)
    {
        insertValue(&heap,INT_MAX,i);
    }

    decreaseKey(&heap,src,0);
    T[src*ver+src]=0;

    while(heap.used>0)
    {
        removeMin(&heap,&rkey,&rvertex);
        Node *ptr = graph->list[rvertex].start;
        if(rkey!=INT_MAX)
        {
        while(ptr!=NULL)
        {
            u=rvertex;
            v=ptr->vertex;
            if((heapPositions[v]!=-1) && (T[src*ver+v]>T[src*ver+u]+ptr->weight) && (u!=ver-1))
            {
                T[src*ver+v]=T[src*ver+u]+ptr->weight;
                decreaseKey(&heap,v,T[src*ver+v]);
            }
           ptr=ptr->next;
        }
        }
    }

    freeHeap(&heap);
    free(heapPositions);
}


void BF(Graph *graph, int src)
{
    int i,j;
    Node *ptr;
    distance=(int *)malloc(graph->vertices*sizeof(int));

    for(i=0;i<graph->vertices;i++)
        distance[i]=INT_MAX;
    distance[src]=0;

    for(i=0;i<graph->vertices;i++)
    {
        for(j=0;j<graph->vertices;j++)
        {
            ptr=graph->list[j].start;
            while(ptr)
            {
                if(distance[j]!=INT_MAX && distance[ptr->vertex]>distance[j]+ptr->weight)
                {
                    distance[ptr->vertex]=distance[j]+ptr->weight;
                }
                ptr=ptr->next;
            }
        }
    }

    for(j=0;j<graph->vertices;j++)
    {
        ptr=graph->list[j].start;
        while(ptr)
        {
            if(distance[j]!=INT_MAX && distance[ptr->vertex]>distance[j]+ptr->weight)
            {
                printf("Negative edge weight cycle\n");
                exit(0);
            }
            ptr=ptr->next;
        }
    }
}

void johnson(Graph *graph)
{
    int i,j,v;
    Node *ptr;

    //Add S for Bellman Ford
    for(i=0;i<graph->vertices;i++)
    {
        addEdge(graph,graph->vertices-1,i,0);
    }
    //Bellman Ford
    BF(graph,graph->vertices-1);

    for(i=0;i<graph->vertices-1;i++)
    {
        ptr=graph->list[i].start;
        while(ptr)
        {
            ptr->weight+=distance[i]-distance[ptr->vertex];
            ptr=ptr->next;
        }
    }

    for(i=0;i<graph->vertices-1;i++)
    {
        djikstra(graph,i);
    }

    v=graph->vertices;
    for(i=0;i<v;i++)
        for(j=0;j<v;j++)
        {
            if(T[i*v+j]!=INT_MAX)
            {
                T[i*v+j]+=distance[j]-distance[i];
            }
        }

}

void query(Graph *graph, int src,int dest) 
{
    int v;
    v=graph->vertices;
    if(T[src*v+dest]!=INT_MAX)
        printf("%d -> %d = %d\n",src,dest,T[src*v+dest]);
    else
        printf("%d -> %d = x\n",src,dest);
}

//Main
int main(int argv, char** argc)
{
    Graph graph;
    int v,e,numqueries,src,dest;
    int x,y,weight;
    int i,j;
    scanf("%d %d",&v,&e);
    initgraph(&graph,v+1,e);
    for(i=0;i<graph.edges;i++)
    {
        scanf("%d %d %d",&x,&y,&weight);
        addEdge(&graph,x,y,weight);
    }

    bf=2;
    j=bf;
    while(j/2!=0)
    {
        shft++;
        j=j/2;
    }

    johnson(&graph);

    scanf("%d",&numqueries);
    while(numqueries--)
    {
        scanf("%d %d",&src,&dest);
        query(&graph,src,dest);
    }

    return 0;
}
