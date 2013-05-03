/* main.c */
/* Wolfgang Tichy, April 2005 */

#include "sgrid.h"
#include "main.h"


/* global var that contains time (in s) at the start of sgrid */
extern double time_in_s_at_sgrid_start;


/* initialize libraries 
   the automatically generated file calls the initializers for each module
*/
void initialize_libraries(void)
{
  prdivider(0);
  printf("Initializing libraries\n");

#include "sgrid_automatic_initialize.c"
}




/**************************************************************************/
/* main */
int main(int argc, char **argv) 
{
  tGrid *g;

  time_in_s_at_sgrid_start = getTimeIn_s();
  read_command_line(argc, argv);
  parse_parameter_file(Gets("parameterfile"));
  parse_command_line_options();
  make_output_directory();
  initialize_libraries();

  while (iterate_parameters()) {
    RunFun(PRE_GRID, 0);  /* hook for special grid preparation */
    g = make_grid(1);
    RunFun(POST_GRID, g); /* hook for special treatment after grid creation */
    initialize_grid(g);
    evolve_grid(g);
    finalize_grid(g);
    RunFun(POST_FINALIZE_GRID, g); /* hook after finalize_grid, e.g. for special cleanup */
    makeparameter("outdir_previous_iteration", "", "outdir of previous iteration");
    Sets("outdir_previous_iteration", Gets("outdir"));
  }
  return 0;
}




/* read command line */
int read_command_line(int argc, char **argv)
{
  int i; 

  if (0) 
    for (i = 0; i < argc; i++)
      printf("argv[%d] = %s\n", i, argv[i]);

  if (argc < 2)
  {
    printf("Welcome to sgrid.\n");
    printf("Usage:  sgrid name.par\n");
    printf("or:     sgrid name.par options and extra arguments\n");
    printf("\n");
    printf("options: --keep_previous           do not touch name_previous\n");
    printf("         --modify-par:\"P=v\"        set par P to value v\n");
    printf(" all options must start with --\n"); 
    exit(0);
  }

  prdivider(0);
  printf("Welcome to sgrid.\n");
  prdivider(0);

  /* got two parameters */
  if (argc >= 2)
  {
    int nopts, nargs;
    char argi[1000];
    char descr[1000];
    char options[1000];
    char *parfile = (char *) calloc(strlen(argv[1])+40, sizeof(char));
    char *outdir  = (char *) calloc(strlen(argv[1])+40, sizeof(char));
    int parnamelen;

    /* determine name of parameter file and output directory */
    strcpy(parfile, argv[1]);
    parnamelen = strlen(parfile);
    if(!strstr(parfile, ".par") || parnamelen<5) strcat(parfile, ".par");
    parnamelen = strlen(parfile);
    if(!strstr(parfile+parnamelen-4, ".par")) strcat(parfile, ".par");
    strcpy(outdir, parfile);
    outdir[strlen(outdir)-4]=0; /* remove .par */

    /* first parameter initializes parameter data base */
    printf("Adding command line parameters\n");
    AddPar("outdir", outdir, "output directory");
    AddPar("parameterfile", parfile, 
	   "name of parameter file given on command line");
    // AddPar("trace_memory", "no", "enable memory tracing");

    /* add other args */
    nargs=nopts=0;
    options[0]=0;
    for(i=2; i<argc; i++)
    {
      if(argv[i][0]=='-')
      {
        if(strlen(argv[i])==1) errorexit("- is not a valid option");
        if(argv[i][1]!='-') errorexit("all options must start with --");
        /* save all options in options */
        if(nopts>0) strncat(options, " ", 999);
        strncat(options, argv[i], 999);
        nopts++;
      }
      else
      {
        snprintf(argi, 999, "sgrid_arg%d", nargs+2);
        snprintf(descr, 999, "sgrid command line argument%d", nargs+2);
        AddPar(argi, argv[i], descr);
        nargs++;
      }
    }
    /* add sgrid command line options */
    if(nopts>0) AddPar("sgrid_options", options, "sgrid command line options");

    free(parfile);
    free(outdir);
  }

  /* more initialization */

  return 0;
}

int make_output_directory(void)
{
  char *outdir  = Gets("outdir");
  char *outdirp = (char *) calloc(strlen(outdir)+40, sizeof(char));

  /* set outdirp to outdir_previous */
  strcpy(outdirp, outdir);
  strcat(outdirp, "_previous");

  /* check if we remove outdir_previous */
  if(!GetvLax("sgrid_options", "--keep_previous"))
  {
    /* remove outdir_previous and move outdir to outdir_previous */
    system2("rm -rf", outdirp);
    system3("mv", outdir, outdirp);
  }

  /* make output directory, save parfile */
  system2("mkdir", outdir);
  system3("cp", Gets("parameterfile"), outdir);

  free(outdirp);
  return 0;
}


/* go through options and act accordingly */
int parse_command_line_options(void)
{
  char *optionstr;
  char *str1;
  char *str2;
  char *par;
  char *val;
  
  /* get length of sgrid_options string */
  if(GetsLax("sgrid_options")==0) return 0;
  printf("Parsing command line options\n");

  /* parse for all --modify-par: */
  optionstr = strdup((GetsLax("sgrid_options")));
  str1 = optionstr;
  while( (str1=strstr(str1, "--modify-par:"))!=NULL )
  {
    str1+=13;
    str2=strstr(str1, "=");
    if(str2==NULL) break;
    str2[0]=0; /* replace = with 0 */
    par=str1;
    val=str2+1;
    str2=strstr(val, " --");
    if(str2!=NULL) str2[0]=0; /* now val is 0 terminated for sure */
    if(0) printf("par=%s|val=%s|\n", par,val);
    
    /* set par to new value */
    AddOrModifyPar(par, val, "set with --modify-par option");

    /* move forward in str1 */
    if(str2!=NULL) str2[0]=' '; /* restore space before -- */
    str1=val;
    if(0) printf("str1=%s|\n", str1);
  }
  free(optionstr);
  return 0;
} 


/* initialize grid */
int initialize_grid(tGrid *g)
{
  if (1) {
    prdivider(0);
    printf("Initializing grid\n");
  }

  /* some things need to be done before initial data */
  RunFun(PRE_INITIALDATA, g); 

  /* compute initial data */
  RunFun(INITIALDATA, g); 

  /* some things need to be done after initial data */
  RunFun(POST_INITIALDATA, g); 

  /* initial data is just another new time slice */
  RunFun(PRE_POST_EVOLVE, g);
  RunFun(POST_EVOLVE, g); 

  /* initial data complete */
  prdivider(0);
  printf("Done with initialization\n");
  printf(" iteration %d, time=%g\n", g->iteration, g->time);

  /* analyze initial data */
  RunFun(ANALYZE, g);

  /* output for permanent variables */
  RunFun(OUTPUT, g);

  /* post output */
  RunFun(POST_OUTPUT, g);

  /* checkpoint, just in case we need it here already */
  checkpoint(g);

  return 0;
}




/* evolve grid */
int evolve_grid(tGrid *grid)
{
  int iterationmax = Geti("iterations");
  double timemax = Getd("finaltime");

  prdivider(0);

  if (timemax > 0)
    iterationmax = timemax/grid->dt + 0.5;

  if (iterationmax > 0) 
    printf("Evolving grid for %d top grid iterations to time %.3f\n", 
	   iterationmax, iterationmax * grid->dt);

  if (iterationmax <= 0) return 0;

  /* outer most evolution loop */
  while (grid->iteration < iterationmax)
  { 
    /* pre evolve */
    RunFun(PRE_EVOLVE, grid); 

    /* evolve */
    RunFun(EVOLVE, grid); 

    /* post evolve */
    RunFun(PRE_POST_EVOLVE, grid);
    RunFun(POST_EVOLVE, grid); 

    /* evolution step complete */
    grid->iteration++;
    grid->time = grid->iteration * grid->dt;

    /* print info */
    printf(" iteration %d, time=%g\n", grid->iteration, grid->time);
    fflush(stdout); 

    /* analyze */
    RunFun(ANALYZE, grid);

    /* output for permanent variables */
    RunFun(OUTPUT, grid);

    /* post output */
    RunFun(POST_OUTPUT, grid);

    /* checkpoint */
    checkpoint(grid);
  
    /* update since this may change during evolution, say when checkpointing */
    timemax= Getd("finaltime");
    iterationmax= (timemax > 0) ? timemax/grid->dt + 0.5 : Geti("iterations");
  }
  return 0;
}




/* finalize grid */
int finalize_grid(tGrid *g)
{
  prdivider(0);
  free_grid(g);
  return 0;
}
