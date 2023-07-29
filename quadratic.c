#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<conio.h>
int main()
{
    double a,b,c,root1,root2;
    printf("add coefficient according to your equation ax^2+bx+c=0\n");
    printf("enter the value of a =\n");          // i did this = printf("enter the value of a = %0.1lf\n",a); and got error because
    scanf("%lf",&a);                             //i was already assigning value in a but no value is given as scanf was after printf and due to that %lf we already get variable assigned but as we had not given any value to that assignment, so it autmatically get 0
    printf("enter the value of b =\n");          //also we should know that when we write scanf(), it also print our written value, so we should not write printf(%d) things
    scanf("%lf",&b);
    printf("enter the value of c =\n");
    scanf("%lf",&c);

    
    if ((b*b) - (4*a*c)<0)
    {
        printf("complex output");
    }
    else if (a==0)
    {
        printf("a can't be 0");
    }
    else
    {
        root1=((-b)+sqrt((b*b)-(4*a*c)))/(2*a) ;
        root2=((-b)-sqrt((b*b)-(4*a*c)))/(2*a);
        printf("your roots are %lf and %lf",root1,root2);
    }
}