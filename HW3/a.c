#include<stdio.h>
#include<stdlib.h>
typedef long long ll;
ll naive(ll *v,ll size)
{
    ll sum=0;
    ll i;
    ll first,second,third;
    first=v[0];
    second=v[1];
    third=v[2];
    i=3;
    while(i!=size+2)
    {
       sum=sum+(first*second*third);
       second=third;
       third=v[i];
       i++;
    }
    return sum;
}
int main()
{
    ll size,sum;
    ll *v,n,i=0;
    scanf("%lld",&size);
    v=(ll *)malloc(size*sizeof(ll));
    while(scanf("%lld",&n)==1)
    {
        v[i]=n;
        i++;
    }
    sum=naive(v,size);
    printf("%lld\n",sum);
    free(v);
    return 0;
}
