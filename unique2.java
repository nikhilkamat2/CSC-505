import java.util.Scanner;
import java.util.*;
public class unique2
{
    public static void main(String[] args)
    {
        Scanner sc = new Scanner(System.in);
        TreeMap<Integer,Integer> treemap = new TreeMap<Integer,Integer>();
        int i,j,n;
        int count,maxcount=1,index=0;
        int[] a= new int[20000000];
        n = sc.nextInt();
        for(i=0;i<n;i++)
        {
            a[i]=sc.nextInt();
        }
        for(i=0;i<n;i++)
        {
            count=1;
            treemap.put(a[i],1);
            for(j=i+1;j<n;j++)
            {
                if(treemap.containsKey(a[j]))
                {
                    treemap.clear();
                    break;
                }
                else
                {
                   treemap.put(a[j],1); 
                   count++;
                   if(count>maxcount)
                   {
                       maxcount=count;
                       index=i;
                   }
                }
            }
        }
        System.out.println(index+" "+maxcount);

    }
}
