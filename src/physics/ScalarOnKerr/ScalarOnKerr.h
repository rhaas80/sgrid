/* ScalarOnKerr.h */
/* Wolfgang Tichy  8/2005 */


int ScalarOnKerr_startup(tGrid *grid);
void ScalarOnKerr_evolve(tVarList *unew, tVarList *upre, double dt, 
                         tVarList *ucur);
void set_boundary_ofPi(tVarList *unew, tVarList *upre);
int ScalarOnKerr_analyze(tGrid *grid);
void Kerr(tGrid *grid, int i_x, int i_g, int i_gup, int i_Gam);
void Kerr3d(tGrid *grid, int i_x, int i_alpha, int i_beta, int i_g, 
            int i_K, int i_TrK, int i_gup, int i_Gam, int i_dalpha);