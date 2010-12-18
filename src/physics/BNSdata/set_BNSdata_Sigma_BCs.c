/* set_BNSdata_Sigma_BCs.c */
/* Copyright (C) 2005-2008 Wolfgang Tichy, 18.12.2010 */
/* Produced with Mathematica */

#include "sgrid.h"
#include "BNSdata.h"

#define Power(x,y) (pow((double) (x), (double) (y)))
#define Log(x)     (log((double) (x)))
#define pow2(x)    ((x)*(x))
#define pow2inv(x) (1.0/((x)*(x)))
#define Cal(x,y,z) ((x)?(y):(z))




void set_BNSdata_Sigma_BC(tVarList *vlFu, tVarList *vlu,  
		   tVarList *vlJdu, tVarList *vldu, tVarList *vlduDerivs, 		   int nonlin)
{
int corot1 = Getv("BNSdata_rotationstate1","corotation");
int corot2 = Getv("BNSdata_rotationstate2","corotation");
int RegularityOnAxis = Getv("BNSdata_Sigma_surface_BCs","RegularityOnAxis");
int SigmaZeroAtA0B0 = Getv("BNSdata_Sigma_surface_BCs","ZeroAt00");
int AddInnerVolIntToBC = Getv("BNSdata_Sigma_surface_BCs","AddInnerVolIntToBC");
int InnerVolIntZero = Getv("BNSdata_Sigma_surface_BCs","InnerVolIntZero");
int AddInnerSumToBC = Getv("BNSdata_Sigma_surface_BCs","AddInnerSumToBC");
int InnerSumZero = Getv("BNSdata_Sigma_surface_BCs","InnerSumZero");
int SigmaZeroInOuterBoxAtA0B0 = Getv("BNSdata_Sigma_surface_BCs","ZeroInOuterBoxAt00");
int SigmaZeroInOuterBoxes = Getv("BNSdata_Sigma_surface_BCs","ZeroInOuterBoxes");
int noBCs = Getv("BNSdata_Sigma_surface_BCs","none");
int UniqueSigmaAtPoles = 0; //Getv("BNSdata_Sigma_surface_BCs","UniqueSigmaAtPoles");
double n = Getd("BNSdata_n");
double kappa = Getd("BNSdata_kappa");
double Omega = Getd("BNSdata_Omega");
double xCM = Getd("BNSdata_x_CM");
double VolAvSigma, VolAvlSigma;

tGrid *grid = vlu->grid;
int bi;


/* do nothing if noBCs, i.e. BNSdata_Sigma_surface_BCs = none */
if(noBCs) return;


forallboxes(grid,bi)
{
tBox *box = grid->box[bi];
int ijk;

int n1 = box->n1;
int n2 = box->n2;
int n3 = box->n3;
int i,j,k, pln;



double *FPsi = vlldataptr(vlFu, box, 0);
double *FB1 = vlldataptr(vlFu, box, 1);
double *FB2 = vlldataptr(vlFu, box, 2);
double *FB3 = vlldataptr(vlFu, box, 3);
double *FalphaP = vlldataptr(vlFu, box, 4);
double *FSigma = vlldataptr(vlFu, box, 5);
double *Psi = vlldataptr(vlu, box, 0);
double *B1 = vlldataptr(vlu, box, 1);
double *B2 = vlldataptr(vlu, box, 2);
double *B3 = vlldataptr(vlu, box, 3);
double *alphaP = vlldataptr(vlu, box, 4);
double *Sigma = vlldataptr(vlu, box, 5);
double *FlPsi = vlldataptr(vlJdu, box, 0);
double *FlB1 = vlldataptr(vlJdu, box, 1);
double *FlB2 = vlldataptr(vlJdu, box, 2);
double *FlB3 = vlldataptr(vlJdu, box, 3);
double *FlalphaP = vlldataptr(vlJdu, box, 4);
double *FlSigma = vlldataptr(vlJdu, box, 5);
double *lPsi = vlldataptr(vldu, box, 0);
double *lB1 = vlldataptr(vldu, box, 1);
double *lB2 = vlldataptr(vldu, box, 2);
double *lB3 = vlldataptr(vldu, box, 3);
double *lalphaP = vlldataptr(vldu, box, 4);
double *lSigma = vlldataptr(vldu, box, 5);
double *dlPsi1 = vlldataptr(vlduDerivs, box, 0);
double *dlPsi2 = vlldataptr(vlduDerivs, box, 1);
double *dlPsi3 = vlldataptr(vlduDerivs, box, 2);
double *ddlPsi11 = vlldataptr(vlduDerivs, box, 3);
double *ddlPsi12 = vlldataptr(vlduDerivs, box, 4);
double *ddlPsi13 = vlldataptr(vlduDerivs, box, 5);
double *ddlPsi22 = vlldataptr(vlduDerivs, box, 6);
double *ddlPsi23 = vlldataptr(vlduDerivs, box, 7);
double *ddlPsi33 = vlldataptr(vlduDerivs, box, 8);
double *dlB11 = vlldataptr(vlduDerivs, box, 9);
double *dlB12 = vlldataptr(vlduDerivs, box, 10);
double *dlB13 = vlldataptr(vlduDerivs, box, 11);
double *dlB21 = vlldataptr(vlduDerivs, box, 12);
double *dlB22 = vlldataptr(vlduDerivs, box, 13);
double *dlB23 = vlldataptr(vlduDerivs, box, 14);
double *dlB31 = vlldataptr(vlduDerivs, box, 15);
double *dlB32 = vlldataptr(vlduDerivs, box, 16);
double *dlB33 = vlldataptr(vlduDerivs, box, 17);
double *ddlB111 = vlldataptr(vlduDerivs, box, 18);
double *ddlB112 = vlldataptr(vlduDerivs, box, 19);
double *ddlB113 = vlldataptr(vlduDerivs, box, 20);
double *ddlB122 = vlldataptr(vlduDerivs, box, 21);
double *ddlB123 = vlldataptr(vlduDerivs, box, 22);
double *ddlB133 = vlldataptr(vlduDerivs, box, 23);
double *ddlB211 = vlldataptr(vlduDerivs, box, 24);
double *ddlB212 = vlldataptr(vlduDerivs, box, 25);
double *ddlB213 = vlldataptr(vlduDerivs, box, 26);
double *ddlB222 = vlldataptr(vlduDerivs, box, 27);
double *ddlB223 = vlldataptr(vlduDerivs, box, 28);
double *ddlB233 = vlldataptr(vlduDerivs, box, 29);
double *ddlB311 = vlldataptr(vlduDerivs, box, 30);
double *ddlB312 = vlldataptr(vlduDerivs, box, 31);
double *ddlB313 = vlldataptr(vlduDerivs, box, 32);
double *ddlB322 = vlldataptr(vlduDerivs, box, 33);
double *ddlB323 = vlldataptr(vlduDerivs, box, 34);
double *ddlB333 = vlldataptr(vlduDerivs, box, 35);
double *dlalphaP1 = vlldataptr(vlduDerivs, box, 36);
double *dlalphaP2 = vlldataptr(vlduDerivs, box, 37);
double *dlalphaP3 = vlldataptr(vlduDerivs, box, 38);
double *ddlalphaP11 = vlldataptr(vlduDerivs, box, 39);
double *ddlalphaP12 = vlldataptr(vlduDerivs, box, 40);
double *ddlalphaP13 = vlldataptr(vlduDerivs, box, 41);
double *ddlalphaP22 = vlldataptr(vlduDerivs, box, 42);
double *ddlalphaP23 = vlldataptr(vlduDerivs, box, 43);
double *ddlalphaP33 = vlldataptr(vlduDerivs, box, 44);
double *dlSigma1 = vlldataptr(vlduDerivs, box, 45);
double *dlSigma2 = vlldataptr(vlduDerivs, box, 46);
double *dlSigma3 = vlldataptr(vlduDerivs, box, 47);
double *ddlSigma11 = vlldataptr(vlduDerivs, box, 48);
double *ddlSigma12 = vlldataptr(vlduDerivs, box, 49);
double *ddlSigma13 = vlldataptr(vlduDerivs, box, 50);
double *ddlSigma22 = vlldataptr(vlduDerivs, box, 51);
double *ddlSigma23 = vlldataptr(vlduDerivs, box, 52);
double *ddlSigma33 = vlldataptr(vlduDerivs, box, 53);
int index_Psi = (vlu)->index[0];
int index_B1 = (vlu)->index[1];
int index_B2 = (vlu)->index[2];
int index_B3 = (vlu)->index[3];
int index_alphaP = (vlu)->index[4];
int index_Sigma = (vlu)->index[5];
int index_lPsi = (vldu)->index[0];
int index_lB1 = (vldu)->index[1];
int index_lB2 = (vldu)->index[2];
int index_lB3 = (vldu)->index[3];
int index_lalphaP = (vldu)->index[4];
int index_lSigma = (vldu)->index[5];
int index_dlPsi1 = (vlduDerivs)->index[0];
int index_dlPsi2 = (vlduDerivs)->index[1];
int index_dlPsi3 = (vlduDerivs)->index[2];
int index_ddlPsi11 = (vlduDerivs)->index[3];
int index_ddlPsi12 = (vlduDerivs)->index[4];
int index_ddlPsi13 = (vlduDerivs)->index[5];
int index_ddlPsi22 = (vlduDerivs)->index[6];
int index_ddlPsi23 = (vlduDerivs)->index[7];
int index_ddlPsi33 = (vlduDerivs)->index[8];
int index_dlB11 = (vlduDerivs)->index[9];
int index_dlB12 = (vlduDerivs)->index[10];
int index_dlB13 = (vlduDerivs)->index[11];
int index_dlB21 = (vlduDerivs)->index[12];
int index_dlB22 = (vlduDerivs)->index[13];
int index_dlB23 = (vlduDerivs)->index[14];
int index_dlB31 = (vlduDerivs)->index[15];
int index_dlB32 = (vlduDerivs)->index[16];
int index_dlB33 = (vlduDerivs)->index[17];
int index_ddlB111 = (vlduDerivs)->index[18];
int index_ddlB112 = (vlduDerivs)->index[19];
int index_ddlB113 = (vlduDerivs)->index[20];
int index_ddlB122 = (vlduDerivs)->index[21];
int index_ddlB123 = (vlduDerivs)->index[22];
int index_ddlB133 = (vlduDerivs)->index[23];
int index_ddlB211 = (vlduDerivs)->index[24];
int index_ddlB212 = (vlduDerivs)->index[25];
int index_ddlB213 = (vlduDerivs)->index[26];
int index_ddlB222 = (vlduDerivs)->index[27];
int index_ddlB223 = (vlduDerivs)->index[28];
int index_ddlB233 = (vlduDerivs)->index[29];
int index_ddlB311 = (vlduDerivs)->index[30];
int index_ddlB312 = (vlduDerivs)->index[31];
int index_ddlB313 = (vlduDerivs)->index[32];
int index_ddlB322 = (vlduDerivs)->index[33];
int index_ddlB323 = (vlduDerivs)->index[34];
int index_ddlB333 = (vlduDerivs)->index[35];
int index_dlalphaP1 = (vlduDerivs)->index[36];
int index_dlalphaP2 = (vlduDerivs)->index[37];
int index_dlalphaP3 = (vlduDerivs)->index[38];
int index_ddlalphaP11 = (vlduDerivs)->index[39];
int index_ddlalphaP12 = (vlduDerivs)->index[40];
int index_ddlalphaP13 = (vlduDerivs)->index[41];
int index_ddlalphaP22 = (vlduDerivs)->index[42];
int index_ddlalphaP23 = (vlduDerivs)->index[43];
int index_ddlalphaP33 = (vlduDerivs)->index[44];
int index_dlSigma1 = (vlduDerivs)->index[45];
int index_dlSigma2 = (vlduDerivs)->index[46];
int index_dlSigma3 = (vlduDerivs)->index[47];
int index_ddlSigma11 = (vlduDerivs)->index[48];
int index_ddlSigma12 = (vlduDerivs)->index[49];
int index_ddlSigma13 = (vlduDerivs)->index[50];
int index_ddlSigma22 = (vlduDerivs)->index[51];
int index_ddlSigma23 = (vlduDerivs)->index[52];
int index_ddlSigma33 = (vlduDerivs)->index[53];
int index_BNSdata_Sigmax = Ind("BNSdata_Sigmax");
double *dSigma1 = box->v[index_BNSdata_Sigmax + 0];
double *dSigma2 = box->v[index_BNSdata_Sigmax + 1];
double *dSigma3 = box->v[index_BNSdata_Sigmax + 2];
int index_x = Ind("x");
double *x = box->v[index_x + 0];
int index_y = Ind("y");
double *y = box->v[index_y + 0];
int index_BNSdata_q = Ind("BNSdata_q");
double *q = box->v[index_BNSdata_q + 0];
int index_BNSdata_wBx = Ind("BNSdata_wBx");
double *wB1 = box->v[index_BNSdata_wBx + 0];
double *wB2 = box->v[index_BNSdata_wBx + 1];
double *wB3 = box->v[index_BNSdata_wBx + 2];
int index_BNSdata_qx = Ind("BNSdata_qx");
double *dq1 = box->v[index_BNSdata_qx + 0];
double *dq2 = box->v[index_BNSdata_qx + 1];
double *dq3 = box->v[index_BNSdata_qx + 2];
int index_BNSdata_temp3 = Ind("BNSdata_temp3");
double *ddSigmadA2 = box->v[index_BNSdata_temp3 + 0];
int index_BNSdata_temp4 = Ind("BNSdata_temp4");
double *ddlSigmadA2 = box->v[index_BNSdata_temp4 + 0];


double alpha;
double alpha2;
double beta1;
double beta2;
double beta3;
double dlq1;
double dlq2;
double dlq3;
double dlSigmaUp1;
double dlSigmaUp2;
double dlSigmaUp3;
double dlwB11;
double dlwB12;
double dlwB13;
double dlwB21;
double dlwB22;
double dlwB23;
double dlwB31;
double dlwB32;
double dlwB33;
double dSigmaUp1;
double DSigmaUp1;
double dSigmaUp2;
double DSigmaUp2;
double dSigmaUp3;
double DSigmaUp3;
double h;
double h2;
double L2;
double lalpha;
double lh;
double lhuzeroPsi4beta1;
double lhuzeroPsi4beta2;
double lhuzeroPsi4beta3;
double lL2;
double lLnh;
double lq;
double lSig0;
double luzero;
double luzerosqr;
double lwB1;
double lwB2;
double lwB3;
double OmegaCrossR1;
double OmegaCrossR2;
double OmegaCrossR3;
double Psi2;
double Psi3;
double Psi4;
double Psim2;
double Psim3;
double Psim4;
double Psim5;
double Psim6;
double Psim7;
double Psim8;
double Psim9;
double Sig0;
double uzero;
double uzerosqr;
double w1;
double w2;
double w3;
double wBDown1;
double wBDown2;
double wBDown3;
double wDown1;
double wDown2;
double wDown3;



/* Jetzt geht's los! */


/* conditional */
if (bi >= 4) {


continue; 

}
/* if (bi >= 4) */




/* conditional */
if (((bi == 0 || bi == 1) && corot1) || ((bi == 2 || bi == 3) && corot2)) {



/* conditional */
if (nonlin) {


forplane1(i,j,k, n1,n2,n3, 0){ ijk=Index(i,j,k); 

FSigma[ijk]
=
Sigma[ijk]
;


} /* end forplane1 */ 


forplane1(i,j,k, n1,n2,n3, n1-1){ ijk=Index(i,j,k); 

FSigma[ijk]
=
Sigma[ijk]
;


} /* end forplane1 */ 


} else { /* if (!nonlin) */


forplane1(i,j,k, n1,n2,n3, 0){ ijk=Index(i,j,k); 

FlSigma[ijk]
=
lSigma[ijk]
;


} /* end forplane1 */ 


forplane1(i,j,k, n1,n2,n3, n1-1){ ijk=Index(i,j,k); 

FlSigma[ijk]
=
lSigma[ijk]
;


} /* end forplane1 */ 

}
/* if (nonlin) */



continue; /* for corot we are done with this box */ 

}
/* if (nonlin) */




/* conditional */
if ((bi == 1 || bi == 2) && 1) {


int    ind0, ind0in, biin, n1in,n2in,n3in; 


double Sig, Sigin, lSig, lSigin; 


double *Sigmain; 


double *lSigmain; 


if(bi==1) biin=0; else biin=3; 


n1in = grid->box[biin]->n1;                                          
                     n2in = grid->box[biin]->n2;
                     n3in = grid->box[biin]->n3;

                     Sigmain     = grid->box[biin]->v[index_Sigma];
                     lSigmain    = grid->box[biin]->v[index_lSigma];
if(n2in!=n2 || n3in!=n3) errorexit("we need n2in=n2 and n3in=n3"); 



/* conditional */
if (nonlin) {


spec_Deriv2(box, 1, Sigma, ddSigmadA2); 


for(j=0; j<n2-1; j=j+n2-1) 


for(k=0; k<n3; k++) 


for(i=1; i<n1; i++){ ijk=Index(i,j,k); 

FSigma[ijk]
=
ddSigmadA2[ijk]
;


} /* endfor */ 


forplane1(i,j,k, n1,n2,n3, 1){ ijk=Index(i,j,k); 


ind0   = Ind_n1n2(0,j,k,n1,n2);                            
                       ind0in = Ind_n1n2(0,j,k,n1in,n2in);
                       Sig    = Sigma[ind0];
                       Sigin  = Sigmain[ind0in];FSigma[ijk]
=
Sig - Sigin
;


} /* endfor */ 


} else { /* if (!nonlin) */


spec_Deriv2(box, 1, lSigma, ddlSigmadA2); 


for(j=0; j<n2-1; j=j+n2-1) 


for(k=0; k<n3; k++) 


for(i=1; i<n1; i++){ ijk=Index(i,j,k); 

FlSigma[ijk]
=
ddlSigmadA2[ijk]
;


} /* endfor */ 


forplane1(i,j,k, n1,n2,n3, 1){ ijk=Index(i,j,k); 


ind0   = Ind_n1n2(0,j,k,n1,n2);                            
                       ind0in = Ind_n1n2(0,j,k,n1in,n2in);
                       lSig   = lSigma[ind0];
                       lSigin = lSigmain[ind0in];FlSigma[ijk]
=
lSig - lSigin
;


} /* endfor */ 

}
/* if (nonlin) */


}
/* if (nonlin) */




/* conditional */
if (bi == 0 || bi == 3) {


FirstDerivsOf_S(box,  Ind("BNSdata_q"), 			                 Ind("BNSdata_qx")); 



/* conditional */
if (nonlin) {


FirstDerivsOf_S(box, index_Sigma,                                        Ind("BNSdata_Sigmax")); 



/* conditional */
if (AddInnerVolIntToBC || InnerVolIntZero) {


VolAvSigma =                                                  
          VolumeIntegral_inBNSgridBox(grid, bi, index_Sigma);
}
/* if (AddInnerVolIntToBC || InnerVolIntZero) */




/* conditional */
if (AddInnerSumToBC || InnerSumZero) {


VolAvSigma = 0.0; 


forallpoints(box, ijk) { 


VolAvSigma += Sigma[ijk]; 


} /* endfor */ 

}
/* if (AddInnerSumToBC || InnerSumZero) */



//printf("VolAvSigma=%g\n",VolAvSigma); 


forplane1(i,j,k, n1,n2,n3, 0){ ijk=Index(i,j,k); 

OmegaCrossR1
=
-(Omega*y[ijk])
;

OmegaCrossR2
=
Omega*(-xCM + x[ijk])
;

OmegaCrossR3
=
0
;

beta1
=
OmegaCrossR1 + B1[ijk]
;

beta2
=
OmegaCrossR2 + B2[ijk]
;

beta3
=
OmegaCrossR3 + B3[ijk]
;

Psi2
=
pow2(Psi[ijk])
;

Psi4
=
pow2(Psi2)
;

Psim2
=
1/Psi2
;

Psim4
=
pow2(Psim2)
;

Psim6
=
Psim2*Psim4
;

alpha
=
alphaP[ijk]/Psi[ijk]
;

alpha2
=
pow2(alpha)
;

h
=
1. + q[ijk] + n*q[ijk]
;

h2
=
pow2(h)
;

DSigmaUp1
=
Psim4*dSigma1[ijk]
;

DSigmaUp2
=
Psim4*dSigma2[ijk]
;

DSigmaUp3
=
Psim4*dSigma3[ijk]
;

dSigmaUp1
=
dSigma1[ijk]
;

dSigmaUp2
=
dSigma2[ijk]
;

dSigmaUp3
=
dSigma3[ijk]
;

w1
=
Psim6*wB1[ijk]
;

w2
=
Psim6*wB2[ijk]
;

w3
=
Psim6*wB3[ijk]
;

wBDown1
=
wB1[ijk]
;

wBDown2
=
wB2[ijk]
;

wBDown3
=
wB3[ijk]
;

wDown1
=
Psim2*wBDown1
;

wDown2
=
Psim2*wBDown2
;

wDown3
=
Psim2*wBDown3
;

L2
=
h2 + (DSigmaUp1 + w1)*(wDown1 + dSigma1[ijk]) + 
  (DSigmaUp2 + w2)*(wDown2 + dSigma2[ijk]) + 
  (DSigmaUp3 + w3)*(wDown3 + dSigma3[ijk])
;

uzerosqr
=
L2/(alpha2*h2)
;

uzero
=
sqrt(uzerosqr)
;

FSigma[ijk]
=
(dSigmaUp1 - beta1*h*Psi4*uzero)*dq1[ijk] + 
  (dSigmaUp2 - beta2*h*Psi4*uzero)*dq2[ijk] + 
  (dSigmaUp3 - beta3*h*Psi4*uzero)*dq3[ijk]
;

FSigma[ijk]
=
FSigma[ijk] + Psim2*(dq1[ijk]*wB1[ijk] + dq2[ijk]*wB2[ijk] + 
     dq3[ijk]*wB3[ijk])
;



/* conditional */
if (AddInnerVolIntToBC || AddInnerSumToBC) {

FSigma[ijk]
=
VolAvSigma + FSigma[ijk]
;

}
/* if (AddInnerVolIntToBC || AddInnerSumToBC) */



} /* end forplane1 */ 



/* conditional */
if (RegularityOnAxis) {


                                                                       
        /* Be careful: this func overwrites BNSdata_temp1/2/3/4 which 
	   also contain A-derivs of Sigma and lSigma for box1/2 */
        BNSdata_RegularityConditions_for_Var_at_rho_eq_0(box, FSigma,
                        Sigma, dSigma1,dSigma2,dSigma3);}
/* if (RegularityOnAxis) */




/* conditional */
if (SigmaZeroAtA0B0) {


i=0;  j=0; 


for(k=0; k<n3; k++){ ijk=Index(i,j,k); 

FSigma[ijk]
=
Sigma[ijk]
;


} /* end for k  */ 

}
/* if (SigmaZeroAtA0B0) */




/* conditional */
if (InnerVolIntZero || InnerSumZero) {


i=0;  j=0;  k=0; 


ijk=Index(i,j,k); 

FSigma[ijk]
=
VolAvSigma
;

}
/* if (InnerVolIntZero || InnerSumZero) */




/* conditional */
if (UniqueSigmaAtPoles) {


for(i=0; i<n1; i=i+n1-1) 


for(j=0; j<n2; j=j+n2-1) 


for(k=0; k<n3; k++){ ijk=Index(i,j,k); 



/* conditional */
if (k == 0) {

Sig0
=
Sigma[ijk]
;


} else { /* if (!k == 0) */

FSigma[ijk]
=
-Sig0 + Sigma[ijk]
;

}
/* if (k == 0) */



} /* end for k  */ 

}
/* if (k == 0) */



} else { /* if (!k == 0) */


FirstDerivsOf_S(box, index_lSigma, index_dlSigma1); 



/* conditional */
if (AddInnerVolIntToBC || InnerVolIntZero) {


VolAvlSigma =                                                  
          VolumeIntegral_inBNSgridBox(grid, bi, index_lSigma);
}
/* if (AddInnerVolIntToBC || InnerVolIntZero) */




/* conditional */
if (AddInnerSumToBC || InnerSumZero) {


VolAvlSigma = 0.0; 


forallpoints(box, ijk) { 


VolAvlSigma += lSigma[ijk]; 


} /* endfor */ 

}
/* if (AddInnerSumToBC || InnerSumZero) */



//printf("VolAvlSigma=%g\n",VolAvlSigma); 


forplane1(i,j,k, n1,n2,n3, 0){ ijk=Index(i,j,k); 

OmegaCrossR1
=
-(Omega*y[ijk])
;

OmegaCrossR2
=
Omega*(-xCM + x[ijk])
;

OmegaCrossR3
=
0
;

beta1
=
OmegaCrossR1 + B1[ijk]
;

beta2
=
OmegaCrossR2 + B2[ijk]
;

beta3
=
OmegaCrossR3 + B3[ijk]
;

Psi2
=
pow2(Psi[ijk])
;

Psi3
=
Psi2*Psi[ijk]
;

Psi4
=
pow2(Psi2)
;

Psim2
=
1/Psi2
;

Psim3
=
1/Psi3
;

Psim4
=
pow2(Psim2)
;

Psim6
=
Psim2*Psim4
;

Psim8
=
pow2(Psim4)
;

Psim5
=
Psim6*Psi[ijk]
;

Psim7
=
Psim2*Psim5
;

Psim9
=
Psim2*Psim7
;

h
=
1. + q[ijk] + n*q[ijk]
;

h2
=
pow2(h)
;

DSigmaUp1
=
Psim4*dSigma1[ijk]
;

DSigmaUp2
=
Psim4*dSigma2[ijk]
;

DSigmaUp3
=
Psim4*dSigma3[ijk]
;

dSigmaUp1
=
dSigma1[ijk]
;

dSigmaUp2
=
dSigma2[ijk]
;

dSigmaUp3
=
dSigma3[ijk]
;

w1
=
Psim6*wB1[ijk]
;

w2
=
Psim6*wB2[ijk]
;

w3
=
Psim6*wB3[ijk]
;

wBDown1
=
wB1[ijk]
;

wBDown2
=
wB2[ijk]
;

wBDown3
=
wB3[ijk]
;

wDown1
=
Psim2*wBDown1
;

wDown2
=
Psim2*wBDown2
;

wDown3
=
Psim2*wBDown3
;

L2
=
h2 + (DSigmaUp1 + w1)*(wDown1 + dSigma1[ijk]) + 
  (DSigmaUp2 + w2)*(wDown2 + dSigma2[ijk]) + 
  (DSigmaUp3 + w3)*(wDown3 + dSigma3[ijk])
;

uzerosqr
=
L2/(alpha2*h2)
;

uzero
=
sqrt(uzerosqr)
;

alpha
=
alphaP[ijk]/Psi[ijk]
;

alpha2
=
pow2(alpha)
;

lq
=
0
;

dlq1
=
0
;

dlq2
=
0
;

dlq3
=
0
;

lh
=
0
;

lLnh
=
0
;

lwB1
=
0
;

lwB2
=
0
;

lwB3
=
0
;

dlwB11
=
0
;

dlwB12
=
0
;

dlwB13
=
0
;

dlwB21
=
0
;

dlwB22
=
0
;

dlwB23
=
0
;

dlwB31
=
0
;

dlwB32
=
0
;

dlwB33
=
0
;

lalpha
=
-((alphaP[ijk]*lPsi[ijk])/Psi2) + lalphaP[ijk]/Psi[ijk]
;

dlSigmaUp1
=
dlSigma1[ijk]
;

dlSigmaUp2
=
dlSigma2[ijk]
;

dlSigmaUp3
=
dlSigma3[ijk]
;

lL2
=
4.*h2*lLnh - lPsi[ijk]*(8.*Psim5*
      (dSigmaUp1*dSigma1[ijk] + dSigmaUp2*dSigma2[ijk] + 
        dSigmaUp3*dSigma3[ijk]) + 
     (16.*Psim9*wBDown1 + 24.*Psim7*dSigma1[ijk])*wB1[ijk] + 
     (16.*Psim9*wBDown2 + 24.*Psim7*dSigma2[ijk])*wB2[ijk] + 
     (16.*Psim9*wBDown3 + 24.*Psim7*dSigma3[ijk])*wB3[ijk]) + 
  2.*(Psim8*(lwB1*wBDown1 + lwB2*wBDown2 + lwB3*wBDown3) + 
     Psim4*(dSigmaUp1*dlSigma1[ijk] + dSigmaUp2*dlSigma2[ijk] + 
        dSigmaUp3*dlSigma3[ijk]) + 
     Psim6*(lwB1*dSigma1[ijk] + lwB2*dSigma2[ijk] + lwB3*dSigma3[ijk] + 
        dlSigma1[ijk]*wB1[ijk] + dlSigma2[ijk]*wB2[ijk] + 
        dlSigma3[ijk]*wB3[ijk]))
;

luzerosqr
=
(lL2 - 2.*L2*(lalpha/alpha + lLnh))/(alpha2*h2)
;

luzero
=
(0.5*luzerosqr)/uzero
;

lhuzeroPsi4beta1
=
h*Psi4*uzero*lB1[ijk] + beta1*(Psi4*(h*luzero + lh*uzero) + 
     4.*h*Psi3*uzero*lPsi[ijk])
;

lhuzeroPsi4beta2
=
h*Psi4*uzero*lB2[ijk] + beta2*(Psi4*(h*luzero + lh*uzero) + 
     4.*h*Psi3*uzero*lPsi[ijk])
;

lhuzeroPsi4beta3
=
h*Psi4*uzero*lB3[ijk] + beta3*(Psi4*(h*luzero + lh*uzero) + 
     4.*h*Psi3*uzero*lPsi[ijk])
;

FlSigma[ijk]
=
dlq1*(dSigmaUp1 - beta1*h*Psi4*uzero) + 
  dlq2*(dSigmaUp2 - beta2*h*Psi4*uzero) + 
  dlq3*(dSigmaUp3 - beta3*h*Psi4*uzero) + 
  (dlSigmaUp1 - lhuzeroPsi4beta1)*dq1[ijk] + 
  (dlSigmaUp2 - lhuzeroPsi4beta2)*dq2[ijk] + 
  (dlSigmaUp3 - lhuzeroPsi4beta3)*dq3[ijk]
;

FlSigma[ijk]
=
FlSigma[ijk] + Psim2*(lwB1*dq1[ijk] + lwB2*dq2[ijk] + lwB3*dq3[ijk] + 
     dlq1*wB1[ijk] + dlq2*wB2[ijk] + dlq3*wB3[ijk]) - 
  2.*Psim3*lPsi[ijk]*(dq1[ijk]*wB1[ijk] + dq2[ijk]*wB2[ijk] + 
     dq3[ijk]*wB3[ijk])
;



/* conditional */
if (AddInnerVolIntToBC || AddInnerSumToBC) {

FlSigma[ijk]
=
VolAvlSigma + FlSigma[ijk]
;

}
/* if (AddInnerVolIntToBC || AddInnerSumToBC) */



} /* end forplane1 */ 



/* conditional */
if (RegularityOnAxis) {


                                                                       
        /* Be careful: this func overwrites BNSdata_temp1/2/3/4 which 
	   also contain A-derivs of Sigma and lSigma for box1/2 */
        BNSdata_RegularityConditions_for_Var_at_rho_eq_0(box, FlSigma,
                        lSigma, dlSigma1,dlSigma2,dlSigma3);}
/* if (RegularityOnAxis) */




/* conditional */
if (SigmaZeroAtA0B0) {


i=0;  j=0; 


for(k=0; k<n3; k++){ ijk=Index(i,j,k); 

FlSigma[ijk]
=
lSigma[ijk]
;


} /* end for k  */ 

}
/* if (SigmaZeroAtA0B0) */




/* conditional */
if (InnerVolIntZero || InnerSumZero) {


i=0;  j=0;  k=0; 


ijk=Index(i,j,k); 

FlSigma[ijk]
=
VolAvlSigma
;

}
/* if (InnerVolIntZero || InnerSumZero) */




/* conditional */
if (UniqueSigmaAtPoles) {


for(i=0; i<n1; i=i+n1-1) 


for(j=0; j<n2; j=j+n2-1) 


for(k=0; k<n3; k++){ ijk=Index(i,j,k); 



/* conditional */
if (k == 0) {

lSig0
=
lSigma[ijk]
;


} else { /* if (!k == 0) */

FlSigma[ijk]
=
-lSig0 + lSigma[ijk]
;

}
/* if (k == 0) */



} /* end for k  */ 

}
/* if (k == 0) */


}
/* if (k == 0) */


}
/* if (k == 0) */




/* conditional */
if ((bi == 1 || bi == 2) && SigmaZeroInOuterBoxes) {



/* conditional */
if (nonlin) {


forallpoints(box, ijk) { 

FSigma[ijk]
=
Sigma[ijk]
;


} /* endfor */ 


} else { /* if (!nonlin) */


forallpoints(box, ijk) { 

FlSigma[ijk]
=
lSigma[ijk]
;


} /* endfor */ 

}
/* if (nonlin) */


}
/* if (nonlin) */



/* end all */ 

} /* end of boxes */


}  /* end of function */

/* set_BNSdata_Sigma_BCs.c */
/* nvars = 92, n* = 432,  n/ = 197,  n+ = 250, n = 879, O = 1 */
