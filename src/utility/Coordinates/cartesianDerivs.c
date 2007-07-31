/* cartesianDerivs.c */
/* Wolfgang Tichy 4/2005 */

#include "sgrid.h"
#include "Coordinates.h"

/* compute du/dx_m  where m=1,2,3 */
/* Note dX_dx[l][m] = dX_l/dx_m where x_m is the mth cartesian coord,
   i.e x_m=(x,y,z) and X_l is e.g. X_l=(rho,phi,z)*/
void cart_partials(tBox *box, double *u, double *u1, double *u2, double *u3)
{
  spec_Deriv1(box, 1, u, u1);
  spec_Deriv1(box, 2, u, u2);
  spec_Deriv1(box, 3, u, u3);

  if( box->x_of_X[1] != NULL )
  {
    int i,m, ind;
    double *Xp = box->v[Ind("X")];
    double *Yp = box->v[Ind("Y")];
    double *Zp = box->v[Ind("Z")];
    double *du[4];
    double dv[4];

    du[1] = u1;        du[2] = u2;        du[3] = u3;

    /* compute cartesian derivs at all points */
    forallpoints(box,ind)
    {
      double X = Xp[ind];
      double Y = Yp[ind];
      double Z = Zp[ind];
    
      /* Transform derivs to cartesian coords */
      for(m=1; m<=3; m++)
      {
        dv[m] = 0.0;
          
        for(i=1; i<=3; i++)
          dv[m] += box->dX_dx[i][m]((void *) box, ind, X,Y,Z)  *  du[i][ind];
      }
      /* copy dv into du */
      for(m=1; m<=3; m++)
        du[m][ind] = dv[m];
    } /* end loop over all points */

    /* set derivs at singular points */
    for(m=1; m<=3; m++)
    {
      if( box->Sing_d_dx[m] != NULL )
        box->Sing_d_dx[m]((void *) box,   (void *) du[m],
                          (void *) du[1], (void *) du[2], (void *) du[3]);
    }
  }
}


/* compute du/dx_m and d^2u/(dx_m dx_n) where m,n=1,2,3 */
/* Note ddX_dxdx[l][m][n] = dX_l/(dx_m dx_n) where x_m is the mth 
   cartesian coord, i.e x_m=(x,y,z) and X_l is e.g. X_l=(rho,phi,z)*/
void cart_partial_all(tBox *box, double *u, double *u1, double *u2, double *u3,
                      double *u11, double *u12, double *u13,
                      double *u22, double *u23, double *u33 )
{
  /* compute spectral derivs and use u1,u2,u3, u11,u12,u13,u22,u23,u33
     as temporal storage */
  spec_allDerivs(box, u, u1,u2,u3, u11,u12,u13,u22,u23,u33);

  /* Note: the pointers x_of_X[i] are NULL if X_l = x_l */
  if( box->x_of_X[1] != NULL )
  {
    int i,j,m,n, ind;
    double *Xp = box->v[Ind("X")];
    double *Yp = box->v[Ind("Y")];
    double *Zp = box->v[Ind("Z")];
    double *du[4];
    double *ddu[4][4];
    double dv[4];
    double ddv[4][4];

    du[1] = u1;        du[2] = u2;        du[3] = u3;
    ddu[1][1] = u11;   ddu[1][2] = u12;   ddu[1][3] = u13;  
    ddu[2][1] = u12;   ddu[2][2] = u22;   ddu[2][3] = u23;  
    ddu[3][1] = u13;   ddu[3][2] = u23;   ddu[3][3] = u33;  

    if( box->ddX_dxdx[1][1][1] == NULL )
      errorexit("cart_partial_all: box->ddX_dxdx[i][m][n] = NULL.");

    /* loop over all points */
    forallpoints(box,ind)
    {
      double X = Xp[ind];
      double Y = Yp[ind];
      double Z = Zp[ind];
      
      /* Transform derivs to cartesian coords */
      /* Note:
      d u / dx^m = (dX^i / dx^m)  (d u / dX^i)
      d^2 u / dx^m dx^n =  (dX^i / dx^m dx^n) (d u / dX^i)
                          + (dX^i / dx^m) (d u / dX^i dx^n)
                        =   (dX^i / dx^m dx^n) (d u / dX^i)
                          + (dX^i / dx^m) (dX^j / dx^n) (d u / dX^i dX^j)  */
      for(m=1; m<=3; m++)
      {
        dv[m] = 0.0;
        
        for(i=1; i<=3; i++)
          dv[m] += box->dX_dx[i][m]((void *) box, ind, X,Y,Z)  *  du[i][ind];

        for(n=m; n<=3; n++)
        {
          ddv[m][n] = 0.0;

          for(i=1; i<=3; i++)
            ddv[m][n] += box->ddX_dxdx[i][m][n]((void *) box, ind, X,Y,Z) * du[i][ind];

          for(i=1; i<=3; i++)
            for(j=1; j<=3; j++)
              ddv[m][n] += box->dX_dx[i][m]((void *) box, ind, X,Y,Z) *
                           box->dX_dx[j][n]((void *) box, ind, X,Y,Z) *
                           ddu[i][j][ind];
        }
      }

      /* copy dv and ddv into du and ddu */
      for(m=1; m<=3; m++)
      {
        du[m][ind] = dv[m];
        
        for(n=m; n<=3; n++)
          ddu[m][n][ind] = ddv[m][n];
      }
    } /* end loop over all points */
  } /* end if */
}

