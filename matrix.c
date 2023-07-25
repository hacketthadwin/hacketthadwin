#include<stdio.h>
int main()
{
    double a,b,c,d,e,f,g,h,i,determinant;
    printf("enter value according to this matrix |a b c|\n");
    printf("                                     |d e f|\n");
    printf("                                     |g h i|\n");
    printf("enter value of a=");
    scanf("%lf",&a);
    printf("enter value of b=");
    scanf("%lf",&b);
    printf("enter value of c=");
    scanf("%lf",&c);
    printf("enter value of d=");
    scanf("%lf",&d);
    printf("enter value of e=");
    scanf("%lf",&e);
    printf("enter value of f=");
    scanf("%lf",&f);
    printf("enter value of g=");
    scanf("%lf",&g);
    printf("enter value of h=");
    scanf("%lf",&h);
    printf("enter value of i=");
    scanf("%lf",&i);
    determinant=(a*e*i)+(b*f*g)+(c*d*h)-(c*e*g)-(f*h*a)-(b*d*i);
    printf("the adjoint of this matrix is=\n");
    printf("|%lf %lf %lf     |\n",((e*i)-(f*h)),((f*g)-(d*i)),((d*h)-(g*e)));
    printf("|%lf %lf %lf     |\n",((h*c)-(b*i)),((i*a)-(c*g)),((g*b)-(a*h)));
    printf("|%lf %lf %lf     |\n",((b*f)-(c*e)),((c*d)-(f*a)),((a*e)-(b*d)));

    printf("the inverse of the matrix is=\n");
    printf("|%lf %lf %lf     |\n",((e*i)-(f*h))/determinant,((f*g)-(d*i))/determinant,((d*h)-(g*e))/determinant);
    printf("|%lf %lf %lf     |\n",((h*c)-(b*i))/determinant,((i*a)-(c*g))/determinant,((g*b)-(a*h))/determinant);
    printf("|%lf %lf %lf     |\n",((b*f)-(c*e))/determinant,((c*d)-(f*a))/determinant,((a*e)-(b*d))/determinant);     //copyright:god Hadwin
}