#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
typedef long long ll;
ll dp(ll *p,ll size)
{
    ll sum=0,i,j,k,l,cost;
    ll **T = (ll **)malloc(size*sizeof(ll));
    for(i=0;i<=size;i++)
        T[i]=(ll *)malloc(size*sizeof(ll));

    for(i=0;i<=size;i++)
        T[i][i]=0;

    for(l=2;l<=size;l++)
    {
        for(i=1;i<=size-l+1;i++)
        {
            j=i+l-1;
            T[i][j]=LLONG_MAX;
            for(k=i;k<=j-1;k++)
            {
                cost=T[i][k]+T[k+1][j]+(p[i-1]*p[k]*p[j]);
                if(cost<=T[i][j])
                    T[i][j]=cost;
            }
        }
    }
    sum=T[1][size];
    free(T);
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
    sum=dp(p,size);
    printf("%lld\n",sum);
    free(p);
    return 0;
}
