#include <string>
using namespace std;

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <libgen.h>
#include <wait.h>
#include "auxlib.h"
#include "string_set.h"
#include "cppstrtok.h"

// Use cpp to scan a file and print line numbers.
// Print out each input line read in, then strtok it for
// tokens.

bool yy_flex_debug;
bool yydebug;
extern string_set strset;

int main (int argc, char **argv) {
   yy_flex_debug = false;
   yydebug = false;
   bool debug_flag = false;
   char *filename = NULL;
   char *debug_args;
   int opt;
   FILE* output_file;
   int exit_status = EXIT_SUCCESS;  
   set_execname (argv[0]);
   // Analyze the flags
   while((opt = getopt(argc, argv, "ly@:D:")) != -1) {
      switch(opt)
      {
         case 'l':
            yy_flex_debug = true;
            break;
         case 'y':
            yydebug = true;
            break;
         case '@':
            set_debugflags(optarg);
            break;
         case 'D':
            debug_flag = true;
            debug_args = optarg;
            break;
	 case '?':
            eprintf("%: - %c: invalid option\n", optopt);
            exit_status = EXIT_FAILURE;
            break;
         default:
            eprintf("%: - %c: invalid option\n", optopt);
            exit_status = EXIT_FAILURE;
            break;
      }
   }

   if(strstr(argv[optind], ".oc") != NULL) {
      filename = argv[optind];
      string command;
      if (debug_flag){ command = CPP + " -D" + debug_args + " " + filename;}
      else { command = CPP + " " + filename; }
      FILE *pipe = popen (command.c_str(), "r");
      if (pipe == NULL) {
         syserrprintf (command.c_str());
         exit_status = EXIT_FAILURE;
      }
      else {
         cpplines(pipe, filename);
         int pclose_rc = pclose (pipe);
         eprint_status (command.c_str(), pclose_rc);
            if(pclose_rc) exit_status = EXIT_FAILURE;
         }
         char *ext = strstr(filename, ".oc");
         *ext = '\0';
         char *output_file_name = strcat(filename, ".str");
         output_file = fopen(output_file_name, "w");
         strset.dump(output_file);
       }

   if (filename == NULL){
      eprintf("Invalid File Name: %s \n", argv[optind]);
      exit_status = EXIT_FAILURE;
  }
   return exit_status;
}
