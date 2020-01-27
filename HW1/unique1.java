/*
 *CSC505  Homework 1
 *Nikhil Kamat nnkamat 200261513
 */
import java.util.Scanner;
public class unique1
{
    public static void main(String[] args)
    {
        Scanner sc = new Scanner(System.in);
        int i,j,k,n;
        int count,flag,maxcount,index;
        int[] a= new int[20000000];
        n = sc.nextInt();
        for(i=0;i<n;i++)
        {
            a[i]=sc.nextInt();
        }
        maxcount=1;
        index=0;
        for(i=0;i<n;i++)
        {
            count=1;
            for(j=i+1;j<n;j++)
            {
                flag=0;
                for(k=i;k<j;k++)
                {
                    if(a[k]==a[j])
                    {
                        flag=1;
                        break;
                    }

                }
                if(flag!=1)
                {
                    count++;
                    if(count>maxcount)
                    {
                        maxcount=count;
                        index=i;
                    }
                }
                else
                {
                    break;
                }
            }
        }
        System.out.println(index+" "+maxcount);

    }
}
