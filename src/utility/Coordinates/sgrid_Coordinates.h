/* sgrid_Coordinates.h */
/* Wolfgang Tichy, 4/2005 */

#include "Coordinates.h"

/* Coordinates.c */
int init_CoordTransform_And_Derivs(tGrid *grid);
int compute_xyz_dXYZdxyz_ddXYZddxyz(tGrid *grid);

/* cartesianDerivs.c */
void cart_partials(tBox *box, double *u, double *u1, double *u2, double *u3);
void cart_partial_all(tBox *box, double *u, double *u1, double *u2, double *u3,
                      double *u11, double *u12, double *u13,
                      double *u22, double *u23, double *u33 );

/* doubleCovering.c */
void reset_doubleCoveredPoints(tVarList *unew);
void copy_to_doubleCoveredPoints_SphericalDF(tBox *box, int vind);

/* coordFilters.c */
void coordinateDependentFilter(tVarList *unew);

/* from ComplexFunctions.c */
double Arg(double x, double y);
double Arg_plus(double x, double y);
double dArgdx(double x, double y);
double dArgdy(double x, double y);
double ddArgdxdx(double x, double y);
double ddArgdxdy(double x, double y);
double ddArgdydy(double x, double y);
double Retanh(double x, double y);
double Imtanh(double x, double y);
double Argtanh(double x, double y);
double Abstanh(double x, double y);
double Argsech(double x, double y);
double Abssech(double x, double y);

/* from findXYZ_of_xyz.c */
int XYZ_of_xyz(tBox *box, double *X, double *Y, double *Z,
               double x, double y, double z);
int b_XYZ_of_xyz(tGrid *grid, double *X, double *Y, double *Z,
                 double x, double y, double z);
int b_XYZ_of_xyz_inboxlist(tGrid *grid, int *blist, int nb,
                           double *X, double *Y, double *Z,
                           double x, double y, double z);
double nearestXYZ_of_xyz(tBox *box, int *ind, double *X, double *Y, double *Z,
                         double x, double y, double z);
double nearest_b_XYZ_of_xyz(tGrid *grid,  int *b, int *ind,
                            double *X, double *Y, double *Z,
                            double x, double y, double z);
double nearest_b_XYZ_of_xyz_inboxlist(tGrid *grid, int *blist, int nb, 
                            int *b, int *ind,
                            double *X, double *Y, double *Z,
                            double x, double y, double z);
double nearestXYZ_of_xyz_inplane(tBox *box, int *ind, 
                                 double *X, double *Y, double *Z,
                                 double x, double y, double z,
                                 int plane, int pind);
int X_of_x_forgiven_YZ(tBox *box, double *X, double x, double Y, double Z);
int b_X_of_x_forgiven_YZ(tGrid *grid, double *X, double x, double Y, double Z);
int b_X_of_x_forgiven_YZ_inboxlist(tGrid *grid, int *blist, int nb, 
                                   double *X, double x, double Y, double Z);
