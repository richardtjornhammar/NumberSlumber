//#include <stdlib.h>
//#include <stdio.h>
#include "parser.h"

const char *argp_program_version =
"NumberSlumber 1.0";

const char *argp_program_bug_address =
"<richard.tjornhammar@gmail.com>";
//
// OPTIONS.  Field 1 in ARGP.
// Order of fields: {NAME, KEY, ARG, FLAGS, DOC}.
//
static struct argp_option options[] =
{
  {"verbose", 'v', 0, 0, "Produce verbose output"} ,
  {"add", '+', "1515", 0,
   "addition with this number"} ,
  {"multiply", '*', "1337", 0,
   "multiplication with this number"} ,
  {"inputbase", 'i', "0123456789", 0,
   "input base character set"} ,
  {"ibase" ,  'n' , "10" , 0,
   "convert from base n using generic set"} ,
  {"outputbase", 'o', "01", 0,
   "output base character set"} ,
  {"obase" ,  'm',  "2" , 0,
   "convert to base m using generic set"} ,
  {0}
};
//
// PARSER. Field 2 in ARGP.
// Order of parameters: KEY, ARG, STATE.
//
static error_t
parse_opt (int key, char *arg, struct argp_state *state)
{
  t_arguments pargs = state->input;

  switch (key)
    {
    case 'v':
      pargs->verbose = 1;
      break;
    case 'i':
      pargs -> inputbase = arg;
      break;
    case 'o':
      pargs -> outputbase = arg;
      break;
    case '+':
      pargs -> add = arg;
      break;
    case '*':
      pargs -> mult = arg;
      break;
    case 'n':
      pargs->ibase = atoi(arg);
      break;
    case 'm':
      pargs->obase = atoi(arg);
      break;
    case ARGP_KEY_ARG:
      if (state->arg_num >= 1) {
          argp_usage(state);
      }
      pargs->args[state->arg_num] = arg;
      break;
    case ARGP_KEY_END:
      if (state->arg_num < 1)
        {
          argp_usage (state);
        }
      break;
    default:
      return ARGP_ERR_UNKNOWN;
    }
  return 0;
}
//
// ARGS_DOC. Field 3 in ARGP.
// A description of the non-option command-line arguments
// that are acceptable.
//
static char args_doc[] = "NUMBER";
static char doc[] = "\nNumberSlumber 2021 Richard Tjörnhammar LICENSE : GNUv3\n\n\tThis program is free software: you can redistribute it and/or modify\n\tit under the terms of the GNU General Public License as published by\n\tthe Free Software Foundation, either version 3 of the License, or\n\t(at your option) any later version.\n\n\tThis program is distributed in the hope that it will be useful,\n\tbut WITHOUT ANY WARRANTY; without even the implied warranty of\n\tMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n\tGNU General Public License for more details.\n\tYou should have received a copy of the GNU General Public License\n\talong with this program.  If not, see <http://www.gnu.org/licenses/>\n" ;
static struct argp argp = {options, parse_opt, args_doc, doc};

int parse (int argc, char **argv, t_arguments pargs )
{
  // DEFAULTS
  pargs->outputbase = NULL;
  pargs->inputbase  = NULL;
  pargs->verbose =  0;
  pargs->ibase   = 10;
  pargs->obase   =  2;
  pargs->add     = NULL;
  pargs->mult    = NULL;
  // PARSER
  argp_parse (&argp, argc, argv, 0, 0, pargs);
  return(0);
}
