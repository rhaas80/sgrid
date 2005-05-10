/* ADMvars.h */
/* Bernd Bruegmann, 6/02 */

void ADMconstraints(tVarList *u);
int computeADMconstraints(tGrid *grid);
int allocateADMvars(tGrid *grid);
int CheckIfFinite(tGrid* grid, char *varname);
int ExitIfNAN(tGrid* grid);
