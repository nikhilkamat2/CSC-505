#include<stdio.h>
#include<stdlib.h>
typedef long long ll;
ll naive(ll *p,ll size)
{
    ll sum=0;
    ll i;
    ll first,second,third;
    first=p[0];
    second=p[1];
    third=p[2];
    i=3;
    while(i!=size+2)
    {
       sum=sum+(first*second*third);
       second=third;
       if(i<size+1)
           third=p[i];
       i++;
    }
    return sum;
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
    sum=naive(p,size);
    printf("%lld\n",sum);
    free(p);
    return 0;
}
