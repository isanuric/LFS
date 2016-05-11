/*
 * LFS.c
 *
 *  Created on: May 4, 2016
 *      Author: ehsan salmani
 */
#include "header.h"


/*
 *
 */
void do_CIE()
{
	double *input_datas, *inputs2;
	struct_Stars param_star1, param_star2, param_star3, param_star4;
	int num_of_groups = MAX /XYZ;
	int group = 0;
	int each_data_group;
	int j;
	struct_Stars st_arr_results[2];



	print_header();        // print program header
	input_datas  = get_param(); // get user parameters
	inputs2 = input_datas + 3;  // get user parameters

	Xn = input_datas[MAX-3];
	Yn = input_datas[MAX-2];
	Zn = input_datas[MAX-1];


	for ( each_data_group = 0; each_data_group < MAX/XYZ-1; each_data_group++) {
		for ( j = each_data_group; j < MAX-XYZ-3; j+=XYZ) {
			st_arr_results[group] = calc_CIE_Lab(input_datas + j);
			group++;
			if(group % 2 == 0){
				calc_delta_e( st_arr_results[0].arr, st_arr_results[1].arr);
				group = 0;
			}
		}

	}

//	calc_CIE(input_datas[0], input_datas[1], input_datas[2]);  // CIE Normalvalenzsystem
//	param_star1 = calc_CIE_Lab(input_datas);         // CIE-L*a*b* - Farbraumsystem
//	param_star2 = calc_CIE_Lab(inputs2);        // CIE-L*a*b* - Farbraumsystem
//	calc_delta_e(param_star1.arr, param_star2.arr); // delta E
//
//	CIE-Normalvalenzsystem for X=124.900, Y=61.432, Z=30.500
//	x     = 0.576
//	y     = 0.283
//	z     = 0.141
//	x+y+z = 1.000
//
//	CIE-L*a*b* Farbraumsystem for X=124.900, Y=61.432, Z=30.500
//	L*    = 82.610
//	a*    = 96.788
//	b*    = -19.935
//	h_ab  = -11.638
//	C*_ab = 216.833
//
//	CIE-L*a*b* Farbraumsystem for X=135.900, Y=59.309, Z=20.370
//	L*    = 81.461
//	a*    = 116.632
//	b*    = 1.997
//	h_ab  = 0.981
//	C*_ab = 216.833
//
//	ΔE = 29.600

	printf("***********************************************************\n");
	param_star3 = calc_CIE_Luv(input_datas);         // CIE-L*v*u* System
	param_star4 = calc_CIE_Luv(inputs2);        // CIE-L*v*u* System
	calc_delta_e(param_star3.arr, param_star4.arr); // delta E
 }

/*
 * CIE 1931 color space
 * calculates the normalized values x, y and z
 */
void calc_CIE(double X1, double Y1, double Z1)
{
	double x1, y1, z1;
	double denominator;
	denominator = X1 + Y1 + Z1;
	x1 = X1/denominator;  // normalized value x
	y1 = Y1/denominator;  // normalized value y
	z1 = Z1/denominator;  // normalized value z
	printf("CIE-Normalvalenzsystem for X=%.3lf, Y=%.3lf, Z=%.3lf\n", X1, Y1, Z1);
	printf("x     = %.3f \n", x1);
	printf("y     = %.3f \n", y1);
	printf("z     = %.3f \n", z1);
	printf("x+y+z = %.3f \n", x1 + y1 + z1);
	printf("\n");
}

/*
 * CIE-L*a*b* - Farbraumsystem
 */
struct_Stars calc_CIE_Lab(double *param)
{
	double h_ab, C_str_ab;
	double X1_Xn, Y1_Yn, Z1_Zn, limit;
	struct_Stars struct_lab;
	double lab[3];
	X1_Xn = param[0]/Xn;
	Y1_Yn = param[1]/Yn;
	Z1_Zn = param[2]/Zn;
	limit = pow( 6.0/29, 3);  // 0.008856
	struct_lab.arr[2] = 116 * SQRN(3, Y1_Yn) - 16;
	// calculate a* and b*:
	if(X1_Xn > limit || Y1_Yn > limit)	{
		struct_lab.arr[0] = 500 * ( do_sqr3(X1_Xn) - do_sqr3(Y1_Yn) );
		struct_lab.arr[1] = 200 * ( do_sqr3(Y1_Yn) - do_sqr3(Z1_Zn) );
	}else{
		struct_lab.arr[0] = 200 * ( (841/108) * ( X1_Xn - Y1_Yn ) );
		struct_lab.arr[1] = 200 * ( (841/108) * ( Y1_Yn - Z1_Zn ) );
	}
	// calculate h_ab
	h_ab = atan(struct_lab.arr[1]/struct_lab.arr[0]) * (180 / M_PI);  // radians into degree: (180 / M_PI)
	if(lab[0] < 0)	{
		if(struct_lab.arr[1] > 0)
			h_ab += 180;
		else
			h_ab -= 180;
	}
	// C*_ab
	C_str_ab = sqrt( ( pow(lab[0],2) + pow(lab[1], 2) ) );
	printf("CIE-L*a*b* Farbraumsystem for X=%.3lf, Y=%.3lf, Z=%.3lf\n", param[0], param[1], param[2]);
	printf("L*    = %.3f \n", struct_lab.arr[2]);
	printf("a*    = %.3f \n", struct_lab.arr[0]);
	printf("b*    = %.3f  \n", struct_lab.arr[1]);
	printf("h_ab  = %.3f \n", h_ab);
	printf("C*_ab = %.3f \n\n", C_str_ab);
	return struct_lab;
}

/*
 * CIE Luv
 */
struct_Stars calc_CIE_Luv(double *param)
{
	struct_Stars struct_stars;
	struct_Luv luv    = calc_uv(param[0], param[1], param[2]);
	struct_Luv luv_n  = calc_uv(Xn,  Yn,  Zn);
	struct_stars.arr[0] = 13.0 * luv.l * (luv.u - luv_n.u);
	struct_stars.arr[1] = 13.0 * luv.l * (luv.v - luv_n.v);
	struct_stars.arr[2] = luv.l;
	printf("CIE-L*v*u* Farbraumsystem for X=%.3lf, Y=%.3lf, Z=%.3lf\n", param[0], param[1], param[2]);
	printf("L*  = %.3f \n", luv.l);
	printf("u   = %.3f \n", luv.u);
	printf("v   = %.3f \n", luv.v);
	printf("u_n = %.3f \n", luv_n.u);
	printf("v_n = %.3f \n", luv_n.v);
	printf("u*  = %.3f \n", struct_stars.arr[0]);
	printf("v*  = %.3f \n\n", struct_stars.arr[1]);
	return struct_stars;
}

/*
 * delta E
 */
void calc_delta_e(double *param, double *param2)
{	double delta_e;
	delta_e =  sqrt( pow((param[2]-param2[2]), 2) + pow((param[1]-param2[1]), 2) + pow((param[0]-param2[0]), 2) );
	printf("ΔE = %.3f \n\n", delta_e);
}

/*
 * calculate u and v
 */
struct_Luv calc_uv(double a, double b, double c)
{
	struct_Luv retval;
	double denominator;
	denominator = a + 15*b + 3*c;
	retval.l = 116 * SQRN(3, b/Yn) - 16;
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
double *get_param()
{

	int    var;
	int    count;
//	const int size = 30;
	FILE * myFile;
	// get values
	//static double input_array[30] = {-1234};
	static double input_array[MAX];

//	printf("please enter the number of XYZ groups: ");
//	scanf("%d", &MAX);
//	MAX *= 3;
	//scanf("%lf", &input_array[var]);

//	do{
//		count++;
//		printf("%d: \n", input_array[count]);
//	}while (input_array[count] != -1234);
//	MAX = count;
//	count = 0;

	printf("open file:\n");
    myFile = fopen("lfs.txt", "r");
	if(myFile==NULL)	{
		//printf("error!\n");
		return 1;
	}

	for (var = 0; var < MAX; ++var) {
		fscanf(myFile, "%lf", &input_array[var]);
		printf("%.3lf\n", input_array[var]);
		count++;
		if(count == 3)		{
			printf("\n");
			count = 0;
		}

	}
//	MAX = sizeof(input_array) / sizeof(input_array[0]);
//	printf("size of array: %d", MAX);

	fclose(myFile);
	printf("\n");
	return input_array;
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

