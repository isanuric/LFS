/*
 * matrix.c
 *
 *  Created on: May 4, 2016
 *      Author: ehsan
 */
#include "header.h"
double X1,  Y1,  Z1,  X2,  Y2,  Z2, Xn,  Yn,  Zn;
double L_str;



//void do_CIE(double X1, double Y1, double Z1, double Xn, double Yn, double Zn)
void do_CIE()
{

	print_header();		// print program header
	get_param();		// get user parameters
	calc_CIE();			// CIE Normalvalenzsystem
	calc_CIE_Lab();		// CIE-L*a*b* - Farbraumsystem
	calc_CIE_Luv();		// CIE-L*v*u* System
 }

/*
 * CIE Normalvalenzsystem
 */
void calc_CIE()
{
	double x1, y1,z1;
	double denominator;
	denominator = X1 + Y1 + Z1;
	x1 = X1/denominator;
	y1 = Y1/denominator;
	z1 = Z1/denominator;
	printf("CIE-Normalvalenzsystem:\n");
	printf("x     = %.3f \n", x1);
	printf("y     = %.3f \n", y1);
	printf("z     = %.3f \n", z1);
	printf("x+y+z = %.3f \n", x1 + y1 + z1);
	printf("\n");
}

/*
 * CIE-L*a*b* - Farbraumsystem
 */
void calc_CIE_Lab()
{
	double a_str, b_str, h_ab, C_str_ab;
	double X1_Xn, Y1_Yn, Z1_Zn, limit;
	Y1_Yn = Y1/Yn;
	X1_Xn = X1/Xn;
	Z1_Zn = Z1/Zn;
	limit = pow( 6.0/29, 3);  // 0.008856
	L_str = 116 * SQRN(3, Y1_Yn) - 16;
	// calculate a* and b*:
	if(X1_Xn > limit || Y1_Yn > limit)	{
		a_str = 500 * ( do_sqr3(X1_Xn) - do_sqr3(Y1_Yn) );
		b_str = 200 * ( do_sqr3(Y1_Yn) - do_sqr3(Z1_Zn) );
	}else{
		a_str = 200 * ( (841/108) * ( X1_Xn - Y1_Yn ) );
		b_str = 200 * ( (841/108) * ( Y1_Yn - Z1_Zn ) );
	}
	// calculate h_ab
	h_ab = atan(b_str/a_str) * (180 / M_PI);  // radians into degree: (180 / M_PI)
	if(a_str < 0)	{
		if(b_str > 0)
			h_ab += 180;
		else
			h_ab -= 180;
	}
	// C*_ab
	C_str_ab = sqrt( ( pow(a_str,2) + pow(b_str, 2) ) );
	printf("CIE-L*a*b* Farbraumsystem:\n");
	printf("L*    = %.3f \n", L_str);
	printf("a*    = %.3f \n", a_str);
	printf("b*    = %.3f  \n", b_str);
	printf("h_ab  = %.3f \n", h_ab);
	printf("C*_ab = %.3f \n\n", C_str_ab);
}

void calc_CIE_Luv()
{
	double u_str, v_str, delta_E;
	mystruct uv   = calc_uv(X1, Y1, Z1);
	mystruct uv_n = calc_uv(Xn, Yn, Zn);
	u_str = 13.0 * L_str * (uv.u - uv_n.u);
 	v_str = 13.0 * L_str * (uv.v - uv_n.v);
 	delta_E =
	printf("CIE-L*v*u* Farbraumsystem:\n");
	printf("L* = %.3f \n", L_str);
	printf("u  = %.3f \n", uv.u);
	printf("v  = %.3f \n", uv.v);
	printf("u_n  = %.3f \n", uv_n.u);
	printf("v_n  = %.3f \n", uv_n.v);
	printf("u* = %.3f \n", u_str);
	printf("v* = %.3f \n", v_str);
}

mystruct calc_uv(double a, double b, double c)
{
	mystruct retval;
	double denominator;
	denominator = a + 15*b + 3*c;
	retval.u = 4*a / denominator;
	retval.v = 6*b / denominator;
	retval.v *= 1.5;
	return retval;
}

/*
 * dritte Wurzel
 */
double do_sqr3(double a)
{
	return SQRN(3, a);
}

/*
 * get the user parameters
 */
double get_param()
{
	int var;
	// get values
	static double input_array[6];
	printf("Please enter X Y Z Xn Yn Zn: ");

	for (var = 0; var < 6; ++var) {
		scanf("%lf", &input_array[var]);
	}
	printf("\n");
	X1 = input_array[0];
	Y1 = input_array[1];
	Z1 = input_array[2];
	Xn = input_array[3];
	Yn = input_array[4];
	Zn = input_array[5];
}

/*
 * print the program header in terminal
 */
void print_header(){
	printf("***********************************************************\n"
			"			   LFS\n"
			" This program calculates the CIE and CIE-L*a*b* parameters\n"
			"\n"
			"                   created by: isan slam\n"
			"***********************************************************\n");
}

