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

// Use cpp to scan a file and print line numbers.
// Print out each input line read in, then strtok it for
// tokens.

bool yy_flex_debug;
bool yydebug;

const string CPP = "/usr/bin/cpp";
constexpr size_t LINESIZE = 1024;

// Chomp the last character from a buffer if it is delim.
void chomp (char* string, char delim) {
   size_t len = strlen (string);
   if (len == 0) return;
   char* nlpos = string + len - 1;
   if (*nlpos == delim) *nlpos = '\0';
}

// Run cpp against the lines of the file.
void cpplines (FILE* pipe, const char* filename) {
   int linenr = 1;
   char inputname[LINESIZE];
   strcpy (inputname, filename);
   for (;;) {
      char buffer[LINESIZE];
      char* fgets_rc = fgets (buffer, LINESIZE, pipe);
      if (fgets_rc == NULL) break;
      chomp (buffer, '\n');
      //printf ("%s:line %d: [%s]\n", filename, linenr, buffer);
      // http://gcc.gnu.org/onlinedocs/cpp/Preprocessor-Output.html
      int sscanf_rc = sscanf (buffer, "# %d \"%[^\"]\"",
                              &linenr, inputname);
      if (sscanf_rc == 2) {
         //printf("DIRECTIVE: line %d file \"%s\"\n",linenr,inputname);
         continue;
      }
      char* savepos = NULL;
      char* bufptr = buffer;
      for (int tokenct = 1;; ++tokenct) {
         char* token = strtok_r (bufptr, " \t\n", &savepos);
         bufptr = NULL;
         if (token == NULL) break;
         string_set::intern(token);
         //printf ("token %d.%d: [%s]\n",
                // linenr, tokenct, token);
      }
      ++linenr;
   }
}

int main (int argc, char **argv) {
   yy_flex_debug = false;
   yydebug = false;
   bool d_flag = false;
   char *argument;
   char *filename = NULL;
   char *d_args;
   int opt;
   FILE* output_file;
   int exit_status = EXIT_SUCCESS;  
   set_execname (argv[0]);
   // Analyze the flags
   while((opt = getopt(argc, argv, "ly@D:")) != -1) {
      switch(opt)
      {
         case 'l':
            yy_flex_debug = true;
            break;
         case 'y':
            yydebug = true;
            break;
         case '@':
            for (int argi = 1; argi < argc; ++argi){
               if (strstr(argv[argi], "@") != NULL){
                  set_debugflags(argv[argi]);
               }
            }
            break;
         case 'D':
            d_flag = true;
            for (int argi = 1; argi < argc; ++argi){
               if (strstr(argv[argi], "D") != NULL){
                  d_args = argv[argi];
               }
            }
            break; 
         default:
            eprintf("%: - %c: invalid option\n", optopt);
            exit_status = EXIT_FAILURE;
            break;
      }
   }

   for (int argi = 1; argi < argc; ++argi) {
      argument = argv[argi];
      if(strstr(argv[argi], ".oc") != NULL) {
         filename = argv[argi];
         string command;
         if (d_flag){ command = CPP + " " + d_args + " " + filename;}
         else { command = CPP + " " + filename; }
         FILE *pipe = popen (command.c_str(), "r");
         if (pipe == NULL) {
            syserrprintf (command.c_str());
            exit_status = EXIT_FAILURE;
         }else {
            cpplines(pipe, filename);
            int pclose_rc = pclose (pipe);
            eprint_status (command.c_str(), pclose_rc);
            if(pclose_rc) exit_status = EXIT_FAILURE;
         }
         char *extension = strstr(filename, ".oc");
         *extension = '\0';
         char *output_file_name = strcat(filename, ".str");
         output_file = fopen(output_file_name, "w");
         string_set::dump(output_file);
         break;
      }
   }

   if (filename == NULL){
      eprintf("Invalid File Name: %s \n", argument);
      exit_status = EXIT_FAILURE;
  }
   return exit_status;
}
