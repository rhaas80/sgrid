/* interpolate.c */
/* Wolfgang Tichy 8/2007 */

#include "sgrid.h"
#include "Spectral.h"



/* get c=c_ijk of var u */
void spec_Coeffs(tBox *box, double *u, double *c)
{
  /* get c_ijk by calling spec_analysis1 in all 3 directions */
  spec_analysis1(box, 1, u, c);
  spec_analysis1(box, 2, c, c);
  spec_analysis1(box, 3, c, c);
}


/* get var u from coeffs c=c_ijk */
void spec_Eval(tBox *box, double *u, double *c)
{
  /* get u by calling spec_synthesis1 in all 3 directions */
  spec_synthesis1(box, 1, u, c);
  spec_synthesis1(box, 2, u, u);
  spec_synthesis1(box, 3, u, u);
}


/* old version of spec_Coeffs */
void spec_Coeffs_old(tBox *box, double *u, double *c)
{
  int n1 = box->n1;
  int n2 = box->n2;
  int n3 = box->n3;
  double *M;
  double *d;
  void (*get_coeffs)(double *,double *, int)=NULL;
  void (*coeffs_of_deriv)(double, double, double *,double *, int)=NULL;
  void (*coeffs_of_2ndderiv)(double, double, double *,double *, int)=NULL;
  void (*coeffs_of_int)(double, double, double *,double *, int)=NULL;
  void (*eval_onPoints)(double *,double *, int)=NULL;
  void (*filter_coeffs)(double *, int, int)=NULL;
  double (*basisfunc)(void *aux, double a, double b, int k, int n1, double X)=NULL;
  int m3 = max3(n1,n2,n3);

  /* memory for matrix and temp var d */
  M = (double*) calloc( m3*m3, sizeof(double) );
  d = (double*) calloc( n1*n2*n3, sizeof(double) );
  if( !(M && d) ) errorexit("spec_Coeffs: out of memory for M, d");
         
  /* get c_ijk by calling spec_analysis1 in all 3 directions */
  /* direction 1 */
  get_spec_functionpointers(box, 1, &get_coeffs, &coeffs_of_deriv,
                            &coeffs_of_2ndderiv, &coeffs_of_int, &eval_onPoints,
                            &filter_coeffs, &basisfunc);
  initMatrix_ForCoeffs(M, n1, get_coeffs);
  spec_analysis1_diffmatrix(box, 1, M, u, c);

  /* direction 2 */
  get_spec_functionpointers(box, 2, &get_coeffs, &coeffs_of_deriv,
                            &coeffs_of_2ndderiv, &coeffs_of_int, &eval_onPoints,
                            &filter_coeffs, &basisfunc);
  initMatrix_ForCoeffs(M, n2, get_coeffs);
  spec_analysis1_diffmatrix(box, 2, M, c, d);

  /* direction 3 */
  get_spec_functionpointers(box, 3, &get_coeffs, &coeffs_of_deriv,
                            &coeffs_of_2ndderiv, &coeffs_of_int, &eval_onPoints,
                            &filter_coeffs, &basisfunc);
  initMatrix_ForCoeffs(M, n3, get_coeffs);
  spec_analysis1_diffmatrix(box, 3, M, d, c);

  /* free memory for matrix M and temp var d */
  free(M);
  free(d);
}


/* use coeffs c to interpolate to X,Y,Z in box 
   (set coeffs c of u by calling spec_Coeffs(box, u, c); */
double spec_interpolate(tBox *box, double *c, double X, double Y, double Z)
{
  double *B1;
  double *B2;
  double *B3;
  int n1 = box->n1;
  int n2 = box->n2;
  int n3 = box->n3;
  int i,j,k;
  double sum=0.0;
  double a1=box->bbox[0];
  double b1=box->bbox[1];
  double a2=box->bbox[2];
  double b2=box->bbox[3];
  double a3=box->bbox[4];
  double b3=box->bbox[5];

  if(box->basis1==NULL || box->basis2==NULL || box->basis3==NULL)
    errorexiti("spec_interpolate: box%d: one box->basis1/2/3 is NULL",box->b);

  /* allocate memory for B1/2/3 */
  B1 = (double*) malloc(n1 * sizeof(double));
  B2 = (double*) malloc(n2 * sizeof(double));
  B3 = (double*) malloc(n3 * sizeof(double));

  /* set basis func values at X,Y,Z */
  for(i = n1-1; i >=0; i--)  B1[i]=box->basis1((void *) box, a1,b1, i,n1, X);
  for(j = n2-1; j >=0; j--)  B2[j]=box->basis2((void *) box, a2,b2, j,n2, Y);
  for(k = n3-1; k >=0; k--)  B3[k]=box->basis3((void *) box, a3,b3, k,n3, Z);

  /* interpolate to X,Y,Z */
  /*  for(k = n3-1; k >=0; k--)
      for(j = n2-1; j >=0; j--)
      for(i = n1-1; i >=0; i--)
        sum += c[Index(i,j,k)] * B1[i] * B2[j] * B3[k]; */
  #pragma omp parallel for reduction(+:sum) 
  for(k = n3-1; k >=0; k--)
  { int j,i;
    for(j = n2-1; j >=0; j--)
    for(i = n1-1; i >=0; i--)
      sum += c[Index(i,j,k)] * B1[i] * B2[j] * B3[k];
  }

  /* free memory for B1/2/3 */
  free(B1);
  free(B2);
  free(B3);

  return sum;
}
