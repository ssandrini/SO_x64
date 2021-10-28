#ifndef __COMMANDS_H__
#define __COMMANDS_H__
#include <stdfun.h>
#include <stdint.h>
#include <syscalls.h>
#include "filter.h"
#include "loop.h"
#include "wc.h"
#include "phylo.h"
#include "cat.h"
#define COMMANDS_SIZE 19
#define APPS_SIZE 5
#define CASE_CAT 1
#define CASE_GETMEM 3
#define LENGTH_PRINTMEM 8
#define LENGTH_CAT 3
#define SPECIAL_FEATURES_ID 7
#define FEATURES_ID 1

// --------------------------------------COMMANDS------------------------------------------//
void help(); 
void inforeg();
void getMem(char * param);
void getTime();
void exc0Trigger();
void exc6Trigger();
void clear();
void quadratic();
void cpuid();
void celsius();
void fahrenheit();
void polar();
int cat(int argc, char *argv[]);
//-------------------------------------------------------------------------------------------
//---------------------------------------AUX FUNCTIONS---------------------------------------
int checkCommandBuiltIn(char * buffer, char * parameter);
int checkCommandUserApps(char * buffer, char * parameter);
//-------------------------------------------------------------------------------------------

//---------------------------------------ASM FUNCTIONS----------------------------------------
extern int _quadratic(long double * a, long double * b, long double * c, long double * r1, long double * r2);
extern void _exc6Trigger();
extern void _FtoCelcius(long double * deg_f, long double * c1, long double * c2 , long double * res);
extern void _CtoFahren(long double * deg_c, long double * c1, long double * c2 , long double * res);
extern void _rectToPolar(long double * x, long double * y, long double * mod, long double * angle, long double * c);
//---------------------------------------------------------------------------------------------

#endif
    