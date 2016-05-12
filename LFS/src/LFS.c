/*
 * LFS.c
 *
 *  Created on: May 4, 2016
 *      Author: ehsan salmani
 */
#include "header.h"
int size;
int b_print_titel = 1;
int b_create_base = 1;
int group = 0;


/*
 *
 */
void do_CIE()
{
	int i;
	print_header();    // print program header
	for ( i = 0; i < 3; ++i)
		execute_color_system(i);
 }

/*
 * execute color systems
 */
void execute_color_system(int func_numb)
{
	struct_Stars st_arr_results[2];
	int i;
	int compare=1;
	double **arr;

	if(b_create_base){
		arr = read_file(); // get user parameters
		Xn  = arr[size-1][0];
		Yn  = arr[size-1][1];
		Zn  = arr[size-1][2];
		b_create_base = 0;
	}

	for ( i = 0; i < size-1 ; i++) {

		if(group == 0 && (func_numb != 0) && b_print_titel !=1 )
			printf("Group %d:\n", compare++);

		switch(func_numb)
		{
			case 0: /* CIE-Normalvalenzsystem */
				if(b_print_titel)
					print_sys_title(func_numb);
				calc_CIE(arr[i]);
				break;

			case 1: /* CIE-L*a*b* Farbraumsystem */
				if(b_print_titel)
					print_sys_title(func_numb);
				st_arr_results[group] = calc_CIE_Lab(arr[i]);
				group++;
				break;

			case 2: /* CIE-L*v*u* Farbraumsystem */
				if(b_print_titel)
					print_sys_title(func_numb);
				st_arr_results[group] = calc_CIE_Luv(arr[i]);
				group++;
				break;
		}
		if(group % 2 == 0 && (func_numb != 0) ){
			calc_delta_e( st_arr_results[0].arr, st_arr_results[1].arr);
			group = 0;

		}

	}b_print_titel = 1;
}

/*
 * CIE 1931 color space
 * calculates the normalized values x, y and z
 */
void calc_CIE(double *param)
{
	double x1, y1, z1;
	double denominator;

	denominator = param[0] + param[1] + param[2];
	x1 = param[0]/denominator;  // normalized value x
	y1 = param[1]/denominator;  // normalized value y
	z1 = param[2]/denominator;  // normalized value z

	printf("X=%.3lf, Y=%.3lf, Z=%.3lf\n", param[0], param[1], param[2]);
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
	//printf("----------------------> %lf \n", param[0]);
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
	printf("X=%.3lf, Y=%.3lf, Z=%.3lf\n", param[0], param[1], param[2]);
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
	printf("X=%.3lf, Y=%.3lf, Z=%.3lf\n", param[0], param[1], param[2]);
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
{
	double delta_e;
	delta_e =  sqrt( pow((param[2]-param2[2]), 2) +
			         pow((param[1]-param2[1]), 2) +
					 pow((param[0]-param2[0]), 2) );
	printf("ΔE = %.3f \n", delta_e);
	printf("***********************\n\n");
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
	FILE * myFile;
	// get values
	static double input_array[MAX];

//	printf("please enter the number of XYZ groups: ");
//	scanf("%d", &MAX);

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
 * read file in a dynamic two dimensional array
 */
double **read_file()
{
	int i = 0;
	int j = 0;
	int var;
	int row;
	double d;
	int count=0;
	static double **arr_input;
	FILE *file = fopen("lfs.txt", "r");

	if(file==NULL)	{
		fprintf(stderr, "error! unable to open file.\n");
		return -1;
	}

	/* number of xyz data groups in file*/
	while(fscanf(file, "%lf", &d)==1){
		count++;
		if(count%3 == 0) size++;
	}

	/* dynamic array allocation */
	arr_input = malloc(size*sizeof(int*));
	for (row = 0; row < size; ++row)
		arr_input[row] = malloc(3*sizeof(int));

	/* add XYZ parametes into array*/
	file = fopen("lfs.txt", "r");
	while( fscanf(file, "%lf", &d)==1 ){
		arr_input[i][j] = d;
		j++;
		//printf("---> %g ", arr_input[i][j++]);
		if(j == 3){
			i++;
			j=0;
		}
	}

	printf("open file:\n");
	for ( var = 0; var < size; ++var){
		printf("XYZ %d: ", var);
		for ( j = 0; j < column; ++j)
					printf("%.3lf ", arr_input[var][j]);
		printf("\n");
	}
	printf("\n");

	fclose(file);
	return arr_input;
}

/*
 * print the program header in terminal
 */
void print_header(){
	printf("***********************************************************\n");
	printf("***********************************************************\n"
			"			   LFS\n"
			" This program calculates the CIE and CIE-L*a*b* parameters\n"
			"\n"
			"                created by: ehsan salmani\n"
			"***********************************************************\n");
	printf("***********************************************************\n");
}

void print_sys_title(int system) {
	char *title;
	switch (system)
	{
		case 0:
			title = "CIE-Normalvalenzsystem";
			break;
		case 1:
			title = "CIE-L*a*b* Farbraumsystem";
			break;
		case 2:
			title = "CIE-L*v*u* Farbraumsystem";
			break;
	}
	printf("***********************************************************\n");
	printf("                  %s\n", title);
	printf("***********************************************************\n");
	b_print_titel = 0;
}
