/* BNSdata.h */
/* Wolfgang Tichy, 2007 */


/* main functions */
int BNSdata_startup(tGrid *grid);
int BNSdata_analyze(tGrid *grid);
int BNSdata_solve(tGrid *grid);
int setBNSdata(tGrid *grid);
void setADMvars(tGrid *grid);

/* funtions from mathematica */
void BNS_CTS(tVarList *vlFu, tVarList *vlu, tVarList *vlJdu, 
             tVarList *vldu, tVarList *vlduDerivs, int nonlin);

/* for solving all ell. eqns together */
void F_BNSdata(tVarList *vlFu, tVarList *vlu,
               tVarList *vluDerivs, tVarList *vlc2);
void J_BNSdata(tVarList *vlJdu, tVarList *vldu,
               tVarList *vlduDerivs, tVarList *vlu);

/* for solving the ell. eqns sequentially */
void F_oneComp(tVarList *vlFw, tVarList *vlw,
               tVarList *vlwDerivs, tVarList *vlc2);
void J_oneComp(tVarList *vlJdw, tVarList *vldw,
               tVarList *vldwDerivs, tVarList *vlw);
