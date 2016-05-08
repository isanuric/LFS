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
void   do_CIE();
void   calc_CIE();
void   calc_CIE_Lab();
void   print_header();
double do_sqr3(double a);
double get_param();



#endif /* MYHEADER_H */
