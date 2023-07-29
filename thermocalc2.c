#include <stdio.h>
#include <math.h>

int main() {
    double a, b, c, n, T, Vf, Vi, Wisothermale, Wisothermalc, d, Ti, Tf, cpcv, Pi, Pf, P;

    printf("[WELCOME TO THERMODYNAMIC CALCULATOR]\n");
    printf("choose one of the following:\n");
    printf("1- ISOTHERMAL\n");
    printf("2- ADIABATIC\n");
    printf("3- ISOCHORIC\n");
    printf("4- ISOBARIC\n");
    printf("press the number for the desired process\n");
    scanf("%lf", &a);

    printf("choose one of the following:\n");
    printf("1- REVERSIBLE\n");
    printf("2- IRREVERSIBLE\n");
    printf("press the number for the desired process\n");
    scanf("%lf", &b);

    printf("choose one of the following:\n");
    printf("1- EXPANSION\n");
    printf("2- COMPRESSION\n");
    printf("press the number for the desired process\n");
    scanf("%lf", &c);

    if (a == 1 && b == 1 && c == 1) {
        printf("enter the value of the following;\n");
        printf("number of moles=");
        scanf("%lf", &n);
        printf("Temperature (if not given then put 298)=");
        scanf("%lf", &T);
        printf("initial volume (in m^3)=");
        scanf("%lf", &Vi);
        printf("final volume (in m^3)=");
        scanf("%lf", &Vf);
        Wisothermale = n * 8.314 * log(Vf / Vi);
        printf("Work done=%lf", Wisothermale);
    } else if (a == 1 && b == 1 && c == 2) {
        printf("enter the value of the following;\n");
        printf("number of moles=");
        scanf("%lf", &n);
        printf("Temperature (if not given then put 298)=");
        scanf("%lf", &T);
        printf("initial volume (in m^3)=");
        scanf("%lf", &Vi);
        printf("final volume (in m^3)=");
        scanf("%lf", &Vf);
        Wisothermalc = -n * 8.314 * log(Vf / Vi);
        printf("Work done=%lf", Wisothermalc);
    } else if (a == 1 && b == 2 && (c == 1 || c == 2)) {
        printf("no mathematical relation possible between P,T,V, Hence we can't calculate the work done\n");
        printf("if the question given any relation between P,T,V, then it is surely reversible\n");
    } else if (a == 2 && b == 1 && c == 1) {
        printf("choose between these according to data you have:\n");
        printf("1- Initial and final temperature\n");
        printf("2- Initial and final volume\n");
        scanf("%lf", &d);
        if (d == 1) {
            printf("enter initial temperature=\n");
            scanf("%lf", &Ti);
            printf("enter final temperature=\n");
            scanf("%lf", &Tf);
            printf("enter number of moles=\n");
            scanf("%lf", &n);

            printf("choose between the following:\n");
            printf("1- molar heat capacity of monoatomic\n");
            printf("2- molar heat capacity of diatomic or polyatomic linear\n");
            printf("3- molar heat capacity of polyatomic\n");
            scanf("%lf", &cpcv);
            if (cpcv == 1) {
                printf("work done= %lf", n * 12.471 * (Tf - Ti));
            } else if (cpcv == 2) {
                printf("work done= %lf", n * 20.785 * (Tf - Ti));
            } else if (cpcv == 3) {
                printf("work done= %lf", n * 24.942 * (Tf - Ti));
            }
        } else if (d == 2) {
            printf("enter initial volume=\n");
            scanf("%lf", &Vi);
            printf("enter final volume=\n");
            scanf("%lf", &Vf);
            printf("enter initial pressure=\n");
            scanf("%lf", &Pi);
            printf("enter final pressure=\n");
            scanf("%lf", &Pf);

            printf("choose between the following:\n");
            printf("1- molar heat capacity of monoatomic\n");
            printf("2- molar heat capacity of diatomic or polyatomic linear\n");
            printf("3- molar heat capacity of polyatomic\n");
            scanf("%lf", &cpcv);
            if (cpcv == 1) {
                printf("work done= %lf", -(Pf * Vf - Pi * Vi) / ((5.0 / 3.0) - 1.0));
            } else if (cpcv == 2) {
                printf("work done= %lf", -(Pf * Vf - Pi * Vi) / ((7.0 / 5.0) - 1.0));
            } else if (cpcv == 3) {
                printf("work done= %lf", -(Pf * Vf - Pi * Vi) / ((4.0 / 3.0) - 1.0));
            }
        }
    } else if (a == 2 && b == 1 && c == 2) {
        printf("choose between these according to data you have:\n");
        printf("1- Initial and final temperature\n");
        printf("2- Initial and final volume\n");
        scanf("%lf", &d);
        if (d == 1) {
            printf("enter initial temperature=\n");
            scanf("%lf", &Ti);
            printf("enter final temperature=\n");
            scanf("%lf", &Tf);
            printf("enter number of moles=\n");
            scanf("%lf", &n);

            printf("choose between the following:\n");
            printf("1- molar heat capacity of monoatomic\n");
            printf("2- molar heat capacity of diatomic or polyatomic linear\n");
            printf("3- molar heat capacity of polyatomic\n");
            scanf("%lf", &cpcv);
            if (cpcv == 1) {
                printf("work done= %lf", -n * 12.471 * (Tf - Ti));
            } else if (cpcv == 2) {
                printf("work done= %lf", -n * 20.785 * (Tf - Ti));
            } else if (cpcv == 3) {
                printf("work done= %lf", -n * 24.942 * (Tf - Ti));
            }
        } else if (d == 2) {
            printf("enter initial volume=\n");
            scanf("%lf", &Vi);
            printf("enter final volume=\n");
            scanf("%lf", &Vf);
            printf("enter initial pressure=\n");
            scanf("%lf", &Pi);
            printf("enter final pressure=\n");
            scanf("%lf", &Pf);

            printf("choose between the following:\n");
            printf("1- molar heat capacity of monoatomic\n");
            printf("2- molar heat capacity of diatomic or polyatomic linear\n");
            printf("3- molar heat capacity of polyatomic\n");
            scanf("%lf", &cpcv);
            if (cpcv == 1) {
                printf("work done= %lf", (Pf * Vf - Pi * Vi) / ((5.0 / 3.0) - 1.0));
            } else if (cpcv == 2) {
                printf("work done= %lf", (Pf * Vf - Pi * Vi) / ((7.0 / 5.0) - 1.0));
            } else if (cpcv == 3) {
                printf("work done= %lf", (Pf * Vf - Pi * Vi) / ((4.0 / 3.0) - 1.0));
            }
        }
    } else if ((a == 2 && b == 2 && c == 1) || (a == 2 && b == 2 && c == 2)) {
        printf("in an adiabatic irreversible process, the value of gamma may not be constant throughout the process, and the system might not follow a simple equation of state.\n");
        printf("Therefore, the work done formula for an adiabatic irreversible process is not as straightforward and would require detailed knowledge of the specific process and the behavior of the gas.\n");
    } else if ((a == 3 && b == 1 && c == 1) || (a == 3 && b == 2 && c == 2) || (a == 3 && b == 1 && c == 2) || (a == 3 && b == 2 && c == 1)) {
        printf("work done= 0\n");
    } else if ((a == 4 && c == 1 && b == 1) || (a == 4 && c == 1 && b == 2)) {
        printf("enter initial volume=\n");
        scanf("%lf", &Vi);
        printf("enter final volume=\n");
        scanf("%lf", &Vf);
        printf("enter pressure=\n");
        scanf("%lf", &P);

        printf("work done= %lf", P * (Vf - Vi));
    } else if ((a == 4 && c == 2 && b == 1) || (a == 4 && c == 2 && b == 2)) {
        printf("enter initial volume=\n");
        scanf("%lf", &Vi);
        printf("enter final volume=\n");
        scanf("%lf", &Vf);
        printf("enter pressure=\n");
        scanf("%lf", &P);

        printf("work done= %lf", -P * (Vf - Vi));
    } else {
        printf("enter correct process\n");
    }
}
