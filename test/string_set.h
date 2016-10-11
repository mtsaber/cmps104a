//This program was completed using pair programming.
//Partner: Muhammad Saber (mtsaber@ucsc.edu)
//Partner: Teghpreet Singh (tsingh8@ucsc.edu)

// $Id: string_set.h,v 1.2 2016-08-18 15:13:48-07 - - $

#ifndef __STRING_SET__
#define __STRING_SET__

#include <string>
#include <unordered_set>
using namespace std;

#include <stdio.h>

struct string_set {
   string_set();
   static unordered_set<string> set;
   static const string* intern (const char*);
   static void dump (FILE*);
};

#endif

