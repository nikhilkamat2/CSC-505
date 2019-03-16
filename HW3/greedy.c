#include<stdio.h>
#include<stdlib.h>
typedef long long ll;
ll split(ll *p, ll left, ll right)
{
    ll i,index,min;
    min=p[left+1]; 
    index=left+1;
    for(i=left+2;i<right;i++)
    {
        if(min>p[i])
        {
            min=p[i];
            index=i;
        }
    }
    return index;
}
ll gdy(ll *p,ll left, ll right)
{
    ll index;
    if((right-left)>1)
    {
        index=split(p,left,right);
        return((p[left]*p[index]*p[right])+gdy(p,left,index)+gdy(p,index,right));
    }
    else
        return 0;
}
int main()
{
    ll size,sum;
    ll *p,n,i=0;
    scanf("%lld",&size);
    p=(ll *)malloc(size*sizeof(ll));
    while(scanf("%lld",&n)==1)
    {
        p[i]=n;
        i++;
    }
    sum=gdy(p,0,size);
    printf("%lld\n",sum);
    free(p);
    return 0;
}
