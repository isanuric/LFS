/*
 * myHeader.h
 *
 *  Created on: May 4, 2016
 *      Author: ehsan
 */
#ifndef MYHEADER_H
#define MYHEADER_H

#include<stdio.h>
#include<stdlib.h>
#include<math.h>


#define  dim 3 // matrix dimension
#define SQRN(m, x) pow(x, 1.0/m)

/*
 *
 */
//void do_CIE(double X1, double Y1, double Z1, double Xn, double Yn, double Zn);
void do_CIE();
double do_sqr3(double a);
double *get_array();
void print_header();


#endif /* MYHEADER_H */
