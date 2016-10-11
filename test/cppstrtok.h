//This program was completed using pair programming.
//Partner: Muhammad Saber (mtsaber@ucsc.edu)
//Partner: Teghpreet Singh (tsingh8@ucsc.edu)

#ifndef __CPPSTRTOK__
#define __CPPSTRTOK__

#include <string>
using namespace std;

#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wait.h>

#include "auxlib.h"

const string CPP = "/usr/bin/cpp";
const size_t LINESIZE = 1024;

void chomp (char *string, char delim);

void cpplines (FILE *pipe, const char *filename);

#endif
