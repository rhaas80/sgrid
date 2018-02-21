/* find_extrema.c */
/* Wolfgang Tichy 2/2018 */

#include "sgrid.h"
#include "Coordinates.h"



/* par type used for box_extremum_of_F */
typedef struct T_box_c1c2c3_struct {
  tBox *box;   /* box */
  double *c1;  /* coeffs of fvec1 */
  double *c2;  /* coeffs of fvec2 */
  double *c3;  /* coeffs of fvec3 */
  int status;  /* status code */
} t_box_c1c2c3_struct;



/* get fvec from coeffs, e.g. coeffs could come from dF/dX of F */
void f3vec_from_c1c2c3_VectorFuncP(int n, double *vec, double *fvec, void *p)
{
  t_box_c1c2c3_struct *par = (t_box_c1c2c3_struct *) p;
  tBox *box  = par->box;
  double *cX = par->c1;
  double *cY = par->c2;
  double *cZ = par->c3;
  double X = vec[1];
  double Y = vec[2];
  double Z = vec[3];
  double X0 = box->bbox[0];
  double X1 = box->bbox[1];
  double Y0 = box->bbox[2];
  double Y1 = box->bbox[3];
  double Z0 = box->bbox[4];
  double Z1 = box->bbox[5];
  double f1, f2, f3;
  int ox,oy,oz, ux,uy,uz; /* overrun & underrun flags */

  /* set overrun & underrun flags */
  ox=oy=oz=ux=uy=uz=0;
  if(X<X0) { X=X0; ux=1; }
  if(Y<Y0) { Y=Y0; uy=2; }
  if(Z<Z0) { Z=Z0; uz=4; }
  if(X>X1) { X=X1; ox=8; }
  if(Y>Y1) { Y=Y1; oy=16; }
  if(Z>Z1) { Z=Z1; oz=32; }

  /* interpolate */
  fvec[1] = f1 = spec_interpolate(box, cX, X,Y,Z);
  fvec[2] = f2 = spec_interpolate(box, cY, X,Y,Z);
  fvec[3] = f3 = spec_interpolate(box, cZ, X,Y,Z);

  /* put zero on boundary and continue linearly if over- or underrun */
  if(ux) fvec[1] =  f1 * (vec[1]-X0);
  if(uy) fvec[2] =  f2 * (vec[2]-Y0);
  if(uz) fvec[3] =  f3 * (vec[3]-Z0);
  if(ox) fvec[1] = -f1 * (vec[1]-X1);
  if(oy) fvec[2] = -f2 * (vec[2]-Y1);
  if(oz) fvec[3] = -f3 * (vec[3]-Z1);
  par->status = ox | oy | oz | ux | uy | uz;
//printf("vec[1],vec[2],vec[3]=%.15g %.15g %.15g  ", vec[1],vec[2],vec[3]);
//printf("X,Y,Z=%.15g %.15g %.15g\n", X,Y,Z);
//printf("  => fvec[1],fvec[2],fvec[3] = %.15g %.15g %.15g\n", fvec[1],fvec[2],fvec[3]);
}

/* Find min/max of function F on the grid in a box */
/* Note:  
   dF/dx = dF/dX dX/dx = 0  | assume Jacobian matrix dX/dx is invertible
     <==> dF/dX = 0  */
int box_extremum_of_F(tBox *box, int Fi,
                      double *X, double *Y, double *Z, double *Fextr)
{
  double X0 = box->bbox[0];
  double X1 = box->bbox[1];
  double Y0 = box->bbox[2];
  double Y1 = box->bbox[3];
  double Z0 = box->bbox[4];
  double Z1 = box->bbox[5];
  int ox,oy,oz, ux,uy,uz; /* overrun & underrun flags */
  int N = box->nnodes;
  double *cx = dmalloc(N);
  double *cy = dmalloc(N);
  double *cz = dmalloc(N);
  double *F  = box->v[Fi];
  double Xvec[4];
  t_box_c1c2c3_struct par[1];
  int check, stat, status;

  /* get all 3 derivs */
  spec_Deriv1(box, 1, F, cx);
  spec_Deriv1(box, 2, F, cy);
  spec_Deriv1(box, 3, F, cz);
  /* transform derivs to coeffs, can be done in place */
  spec_Coeffs(box, cx, cx);
  spec_Coeffs(box, cy, cy);
  spec_Coeffs(box, cz, cz);

  /* set Xvec */
  Xvec[1] = *X;
  Xvec[2] = *Y;
  Xvec[3] = *Z;

  /* use newton_linesrch_itsP to find max, use par for parameters */
  par->box = box;
  par->c1  = cx;
  par->c2  = cy;
  par->c3  = cz;
  stat = newton_linesrch_itsP(Xvec, 3, &check, f3vec_from_c1c2c3_VectorFuncP,
                              (void *) par, 1000, dequaleps);
  if(check || stat<0) printf("  --> check=%d stat=%d\n", check, stat);
  if(check || stat<0) printf("  --> Xvec=(%.15g,%.15g,%.15g)\n",
                               Xvec[1],Xvec[2],Xvec[3]);
  *X = Xvec[1];
  *Y = Xvec[2];
  *Z = Xvec[3];

  /* check if we are outside box, set overrun & underrun flags */
  ox=oy=oz=ux=uy=uz=0;
  if(*X<X0) { *X=X0; ux=1; }
  if(*Y<Y0) { *Y=Y0; uy=2; }
  if(*Z<Z0) { *Z=Z0; uz=4; }
  if(*X>X1) { *X=X1; ox=8; }
  if(*Y>Y1) { *Y=Y1; oy=16; }
  if(*Z>Z1) { *Z=Z1; oz=32; }
  status = -(ox | oy | oz | ux | uy | uz);
  if(status==0) status = stat;

  /* set coeffs of F in cx, and set Fextr at rhis point */
  spec_Coeffs(box, F, cx);
  *Fextr = spec_interpolate(box, cx, *X,*Y,*Z);

  free(cz);
  free(cy);
  free(cx);
  return status;
}