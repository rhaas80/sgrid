/* BNS_CTS.c */
/* Copyright (C) 2005 Wolfgang Tichy & Bernd Bruegmann, 21.2.2008 */
/* Produced with Mathematica */

#include "sgrid.h"
#include "BNSdata.h"

#define Power(x,y) (pow((double) (x), (double) (y)))
#define Log(x)     (log((double) (x)))
#define pow2(x)    ((x)*(x))
#define pow2inv(x) (1.0/((x)*(x)))
#define Cal(x,y,z) ((x)?(y):(z))




void BNS_CTS(tVarList *vlFu, tVarList *vlu,       tVarList *vlJdu, tVarList *vldu, tVarList *vlduDerivs,      int nonlin)
{
double n = Getd("BNSdata_n");
double kappa = Getd("BNSdata_kappa");
double Omega = Getd("BNSdata_Omega");
double xCM = Getd("BNSdata_x_CM");

tGrid *grid = vlu->grid;
int bi;

forallboxes(grid,bi)
{
tBox *box = grid->box[bi];
int ijk;



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
int index_BNSdata_Psix = Ind("BNSdata_Psix");
double *dPsi1 = box->v[index_BNSdata_Psix + 0];
double *dPsi2 = box->v[index_BNSdata_Psix + 1];
double *dPsi3 = box->v[index_BNSdata_Psix + 2];
int index_BNSdata_Psixx = Ind("BNSdata_Psixx");
double *ddPsi11 = box->v[index_BNSdata_Psixx + 0];
double *ddPsi12 = box->v[index_BNSdata_Psixx + 1];
double *ddPsi13 = box->v[index_BNSdata_Psixx + 2];
double *ddPsi22 = box->v[index_BNSdata_Psixx + 3];
double *ddPsi23 = box->v[index_BNSdata_Psixx + 4];
double *ddPsi33 = box->v[index_BNSdata_Psixx + 5];
int index_BNSdata_Bxx = Ind("BNSdata_Bxx");
double *dB11 = box->v[index_BNSdata_Bxx + 0];
double *dB12 = box->v[index_BNSdata_Bxx + 1];
double *dB13 = box->v[index_BNSdata_Bxx + 2];
double *dB21 = box->v[index_BNSdata_Bxx + 3];
double *dB22 = box->v[index_BNSdata_Bxx + 4];
double *dB23 = box->v[index_BNSdata_Bxx + 5];
double *dB31 = box->v[index_BNSdata_Bxx + 6];
double *dB32 = box->v[index_BNSdata_Bxx + 7];
double *dB33 = box->v[index_BNSdata_Bxx + 8];
int index_BNSdata_Bxxx = Ind("BNSdata_Bxxx");
double *ddB111 = box->v[index_BNSdata_Bxxx + 0];
double *ddB112 = box->v[index_BNSdata_Bxxx + 1];
double *ddB113 = box->v[index_BNSdata_Bxxx + 2];
double *ddB122 = box->v[index_BNSdata_Bxxx + 3];
double *ddB123 = box->v[index_BNSdata_Bxxx + 4];
double *ddB133 = box->v[index_BNSdata_Bxxx + 5];
double *ddB211 = box->v[index_BNSdata_Bxxx + 6];
double *ddB212 = box->v[index_BNSdata_Bxxx + 7];
double *ddB213 = box->v[index_BNSdata_Bxxx + 8];
double *ddB222 = box->v[index_BNSdata_Bxxx + 9];
double *ddB223 = box->v[index_BNSdata_Bxxx + 10];
double *ddB233 = box->v[index_BNSdata_Bxxx + 11];
double *ddB311 = box->v[index_BNSdata_Bxxx + 12];
double *ddB312 = box->v[index_BNSdata_Bxxx + 13];
double *ddB313 = box->v[index_BNSdata_Bxxx + 14];
double *ddB322 = box->v[index_BNSdata_Bxxx + 15];
double *ddB323 = box->v[index_BNSdata_Bxxx + 16];
double *ddB333 = box->v[index_BNSdata_Bxxx + 17];
int index_BNSdata_alphaPx = Ind("BNSdata_alphaPx");
double *dalphaP1 = box->v[index_BNSdata_alphaPx + 0];
double *dalphaP2 = box->v[index_BNSdata_alphaPx + 1];
double *dalphaP3 = box->v[index_BNSdata_alphaPx + 2];
int index_BNSdata_alphaPxx = Ind("BNSdata_alphaPxx");
double *ddalphaP11 = box->v[index_BNSdata_alphaPxx + 0];
double *ddalphaP12 = box->v[index_BNSdata_alphaPxx + 1];
double *ddalphaP13 = box->v[index_BNSdata_alphaPxx + 2];
double *ddalphaP22 = box->v[index_BNSdata_alphaPxx + 3];
double *ddalphaP23 = box->v[index_BNSdata_alphaPxx + 4];
double *ddalphaP33 = box->v[index_BNSdata_alphaPxx + 5];
int index_BNSdata_Sigmax = Ind("BNSdata_Sigmax");
double *dSigma1 = box->v[index_BNSdata_Sigmax + 0];
double *dSigma2 = box->v[index_BNSdata_Sigmax + 1];
double *dSigma3 = box->v[index_BNSdata_Sigmax + 2];
int index_BNSdata_Sigmaxx = Ind("BNSdata_Sigmaxx");
double *ddSigma11 = box->v[index_BNSdata_Sigmaxx + 0];
double *ddSigma12 = box->v[index_BNSdata_Sigmaxx + 1];
double *ddSigma13 = box->v[index_BNSdata_Sigmaxx + 2];
double *ddSigma22 = box->v[index_BNSdata_Sigmaxx + 3];
double *ddSigma23 = box->v[index_BNSdata_Sigmaxx + 4];
double *ddSigma33 = box->v[index_BNSdata_Sigmaxx + 5];
int index_x = Ind("x");
double *x = box->v[index_x + 0];
int index_y = Ind("y");
double *y = box->v[index_y + 0];
int index_alpha = Ind("alpha");
double *alpha = box->v[index_alpha + 0];
int index_betax = Ind("betax");
double *beta1 = box->v[index_betax + 0];
double *beta2 = box->v[index_betax + 1];
double *beta3 = box->v[index_betax + 2];
int index_BNSdata_q = Ind("BNSdata_q");
double *q = box->v[index_BNSdata_q + 0];
int index_BNSdata_vRSx = Ind("BNSdata_vRSx");
double *vRS1 = box->v[index_BNSdata_vRSx + 0];
double *vRS2 = box->v[index_BNSdata_vRSx + 1];
double *vRS3 = box->v[index_BNSdata_vRSx + 2];
int index_BNSdata_qx = Ind("BNSdata_qx");
double *dq1 = box->v[index_BNSdata_qx + 0];
double *dq2 = box->v[index_BNSdata_qx + 1];
double *dq3 = box->v[index_BNSdata_qx + 2];
int index_BNSdata_vRSxx = Ind("BNSdata_vRSxx");
double *dvRS11 = box->v[index_BNSdata_vRSxx + 0];
double *dvRS12 = box->v[index_BNSdata_vRSxx + 1];
double *dvRS13 = box->v[index_BNSdata_vRSxx + 2];
double *dvRS21 = box->v[index_BNSdata_vRSxx + 3];
double *dvRS22 = box->v[index_BNSdata_vRSxx + 4];
double *dvRS23 = box->v[index_BNSdata_vRSxx + 5];
double *dvRS31 = box->v[index_BNSdata_vRSxx + 6];
double *dvRS32 = box->v[index_BNSdata_vRSxx + 7];
double *dvRS33 = box->v[index_BNSdata_vRSxx + 8];


double alpha2;
double alphaP2;
double alphaP3;
double dalpha1;
double dalpha2;
double dalpha3;
double dbeta11;
double dbeta12;
double dbeta13;
double dbeta21;
double dbeta22;
double dbeta23;
double dbeta31;
double dbeta32;
double dbeta33;
double dlalpha1;
double dlalpha2;
double dlalpha3;
double dLnalphaP1;
double dLnalphaP2;
double dLnalphaP3;
double dLnalphaPsim61;
double dLnalphaPsim62;
double dLnalphaPsim63;
double dLnPsi1;
double dLnPsi2;
double dLnPsi3;
double dLnrho01;
double dLnrho02;
double dLnrho03;
double dLnuzerosqr1;
double dLnuzerosqr2;
double dLnuzerosqr3;
double doouzerosqr1;
double doouzerosqr2;
double doouzerosqr3;
double duzerosqr1;
double duzerosqr2;
double duzerosqr3;
double dvR11;
double dvR12;
double dvR13;
double dvR21;
double dvR22;
double dvR23;
double dvR31;
double dvR32;
double dvR33;
double gdB;
double gdlB;
double j1;
double j2;
double j3;
double lalpha;
double LB11;
double LB12;
double LB13;
double LB22;
double LB23;
double LB33;
double LBdo11;
double LBdo12;
double LBdo13;
double LBdo21;
double LBdo22;
double LBdo23;
double LBdo31;
double LBdo32;
double LBdo33;
double LBLB;
double ldLnalphaPsim61;
double ldLnalphaPsim62;
double ldLnalphaPsim63;
double ldoouzerosqr1;
double ldoouzerosqr2;
double ldoouzerosqr3;
double lduzerosqr1;
double lduzerosqr2;
double lduzerosqr3;
double lj1;
double lj2;
double lj3;
double LlB11;
double LlB12;
double LlB13;
double LlB22;
double LlB23;
double LlB33;
double LlBdo11;
double LlBdo12;
double LlBdo13;
double LlBdo21;
double LlBdo22;
double LlBdo23;
double LlBdo31;
double LlBdo32;
double LlBdo33;
double LlBLlB;
double loouzerosqr;
double lrho;
double lS;
double luzerosqr;
double lvR1;
double lvR2;
double lvR3;
double OmegaCrossR1;
double OmegaCrossR2;
double OmegaCrossR3;
double oouzerosqr;
double P;
double Psi2;
double Psi3;
double Psi4;
double Psi5;
double Psi6;
double Psi7;
double rho;
double rho0;
double rhoE;
double S;
double uzerosqr;
double vecLapB1;
double vecLapB2;
double vecLapB3;
double vecLaplB1;
double vecLaplB2;
double vecLaplB3;
double vR1;
double vR2;
double vR3;
double vRI1;
double vRI2;
double vRI3;



/* Jetzt geht's los! */


/* conditional */
if (nonlin) {


FirstAndSecondDerivsOf_S(box, index_Psi,     Ind("BNSdata_Psix"), Ind("BNSdata_Psixx")); 


FirstAndSecondDerivsOf_Sa(box, index_B1,    Ind("BNSdata_Bxx"), Ind("BNSdata_Bxxx")); 


FirstAndSecondDerivsOf_S(box, index_alphaP,    Ind("BNSdata_alphaPx"), Ind("BNSdata_alphaPxx")); 


FirstAndSecondDerivsOf_S(box, index_Sigma,    Ind("BNSdata_Sigmax"), Ind("BNSdata_Sigmaxx")); 


} else { /* if (!nonlin) */


FirstAndSecondDerivsOf_S(box, index_lPsi,       index_dlPsi1, index_ddlPsi11); 


FirstAndSecondDerivsOf_Sa(box, index_lB1,      index_dlB11, index_ddlB111); 


FirstAndSecondDerivsOf_S(box, index_lalphaP,      index_dlalphaP1, index_ddlalphaP11); 


FirstAndSecondDerivsOf_S(box, index_lSigma,      index_dlSigma1, index_ddlSigma11); 

}
/* if (nonlin) */



FirstDerivsOf_Sa(box, Ind("BNSdata_vRSx"),       Ind("BNSdata_vRSxx")); 


FirstDerivsOf_S(box,  Ind("BNSdata_q"),                     Ind("BNSdata_qx")); 


forallpoints(box, ijk) { 

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

beta1[ijk]
=
OmegaCrossR1 + B1[ijk]
;

beta2[ijk]
=
OmegaCrossR2 + B2[ijk]
;

beta3[ijk]
=
OmegaCrossR3 + B3[ijk]
;

gdB
=
dB11[ijk] + dB22[ijk] + dB33[ijk]
;

LB11
=
-0.66666666666666666667*gdB + 2.*dB11[ijk]
;

LB12
=
dB12[ijk] + dB21[ijk]
;

LB13
=
dB13[ijk] + dB31[ijk]
;

LB22
=
-0.66666666666666666667*gdB + 2.*dB22[ijk]
;

LB23
=
dB23[ijk] + dB32[ijk]
;

LB33
=
-0.66666666666666666667*gdB + 2.*dB33[ijk]
;

LBdo11
=
LB11
;

LBdo12
=
LB12
;

LBdo13
=
LB13
;

LBdo21
=
LB12
;

LBdo22
=
LB22
;

LBdo23
=
LB23
;

LBdo31
=
LB13
;

LBdo32
=
LB23
;

LBdo33
=
LB33
;

LBLB
=
LB11*LBdo11 + LB12*(LBdo12 + LBdo21) + LB22*LBdo22 + 
  LB13*(LBdo13 + LBdo31) + LB23*(LBdo23 + LBdo32) + LB33*LBdo33
;

alpha[ijk]
=
alphaP[ijk]/Psi[ijk]
;

alpha2
=
pow2(alpha[ijk])
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

Psi5
=
Psi4*Psi[ijk]
;

vRI1
=
dSigma1[ijk]
;

vRI2
=
dSigma2[ijk]
;

vRI3
=
dSigma3[ijk]
;

vR1
=
vRI1 + vRS1[ijk]
;

vR2
=
vRI2 + vRS2[ijk]
;

vR3
=
vRI3 + vRS3[ijk]
;

oouzerosqr
=
alpha2 - Psi4*(2.*(vR1*beta1[ijk] + vR2*beta2[ijk] + vR3*beta3[ijk]) + 
     pow2(vR1) + pow2(vR2) + pow2(vR3) + pow2(beta1[ijk]) + 
     pow2(beta2[ijk]) + pow2(beta3[ijk]))
;

uzerosqr
=
1./oouzerosqr
;

rho0
=
Power(q[ijk]/kappa,n)
;

P
=
rho0*q[ijk]
;

rhoE
=
rho0 + n*rho0*q[ijk]
;

rho
=
alpha2*rhoE*uzerosqr + P*(-1. + alpha2*uzerosqr)
;

j1
=
(P + rhoE)*uzerosqr*alpha[ijk]*(vR1 + beta1[ijk])
;

j2
=
(P + rhoE)*uzerosqr*alpha[ijk]*(vR2 + beta2[ijk])
;

j3
=
(P + rhoE)*uzerosqr*alpha[ijk]*(vR3 + beta3[ijk])
;

S
=
3.*P + rho - rhoE
;

dLnalphaPsim61
=
dalphaP1[ijk]/alphaP[ijk] - (7.*dPsi1[ijk])/Psi[ijk]
;

dLnalphaPsim62
=
dalphaP2[ijk]/alphaP[ijk] - (7.*dPsi2[ijk])/Psi[ijk]
;

dLnalphaPsim63
=
dalphaP3[ijk]/alphaP[ijk] - (7.*dPsi3[ijk])/Psi[ijk]
;



/* conditional */
if (bi == 0 || bi == 3 || bi == 4 || bi == 5) {

dLnrho01
=
(n*dq1[ijk]*Power(q[ijk]/kappa,-1. + n))/kappa
;

dLnrho02
=
(n*dq2[ijk]*Power(q[ijk]/kappa,-1. + n))/kappa
;

dLnrho03
=
(n*dq3[ijk]*Power(q[ijk]/kappa,-1. + n))/kappa
;

dalpha1
=
-((alphaP[ijk]*dPsi1[ijk])/Psi2) + dalphaP1[ijk]/Psi[ijk]
;

dalpha2
=
-((alphaP[ijk]*dPsi2[ijk])/Psi2) + dalphaP2[ijk]/Psi[ijk]
;

dalpha3
=
-((alphaP[ijk]*dPsi3[ijk])/Psi2) + dalphaP3[ijk]/Psi[ijk]
;

dbeta11
=
dB11[ijk]
;

dbeta12
=
-Omega + dB12[ijk]
;

dbeta13
=
dB13[ijk]
;

dbeta21
=
Omega + dB21[ijk]
;

dbeta22
=
dB22[ijk]
;

dbeta23
=
dB23[ijk]
;

dbeta31
=
dB31[ijk]
;

dbeta32
=
dB32[ijk]
;

dbeta33
=
dB33[ijk]
;

dvR11
=
ddSigma11[ijk] + dvRS11[ijk]
;

dvR12
=
ddSigma12[ijk] + dvRS12[ijk]
;

dvR13
=
ddSigma13[ijk] + dvRS13[ijk]
;

dvR21
=
ddSigma12[ijk] + dvRS21[ijk]
;

dvR22
=
ddSigma22[ijk] + dvRS22[ijk]
;

dvR23
=
ddSigma23[ijk] + dvRS23[ijk]
;

dvR31
=
ddSigma13[ijk] + dvRS31[ijk]
;

dvR32
=
ddSigma23[ijk] + dvRS32[ijk]
;

dvR33
=
ddSigma33[ijk] + dvRS33[ijk]
;

doouzerosqr1
=
2.*(dalpha1*alpha[ijk] - Psi4*((dbeta11 + dvR11)*(vR1 + beta1[ijk]) + 
        (dbeta21 + dvR21)*(vR2 + beta2[ijk]) + 
        (dbeta31 + dvR31)*(vR3 + beta3[ijk]))) - 
  Psi3*dPsi1[ijk]*(8.*(vR1*beta1[ijk] + vR2*beta2[ijk] + vR3*beta3[ijk]) + 
     4.*(pow2(vR1) + pow2(vR2) + pow2(vR3) + pow2(beta1[ijk]) + 
        pow2(beta2[ijk]) + pow2(beta3[ijk])))
;

doouzerosqr2
=
2.*(dalpha2*alpha[ijk] - Psi4*((dbeta12 + dvR12)*(vR1 + beta1[ijk]) + 
        (dbeta22 + dvR22)*(vR2 + beta2[ijk]) + 
        (dbeta32 + dvR32)*(vR3 + beta3[ijk]))) - 
  Psi3*dPsi2[ijk]*(8.*(vR1*beta1[ijk] + vR2*beta2[ijk] + vR3*beta3[ijk]) + 
     4.*(pow2(vR1) + pow2(vR2) + pow2(vR3) + pow2(beta1[ijk]) + 
        pow2(beta2[ijk]) + pow2(beta3[ijk])))
;

doouzerosqr3
=
2.*(dalpha3*alpha[ijk] - Psi4*((dbeta13 + dvR13)*(vR1 + beta1[ijk]) + 
        (dbeta23 + dvR23)*(vR2 + beta2[ijk]) + 
        (dbeta33 + dvR33)*(vR3 + beta3[ijk]))) - 
  Psi3*dPsi3[ijk]*(8.*(vR1*beta1[ijk] + vR2*beta2[ijk] + vR3*beta3[ijk]) + 
     4.*(pow2(vR1) + pow2(vR2) + pow2(vR3) + pow2(beta1[ijk]) + 
        pow2(beta2[ijk]) + pow2(beta3[ijk])))
;

duzerosqr1
=
-(doouzerosqr1*pow2(uzerosqr))
;

duzerosqr2
=
-(doouzerosqr2*pow2(uzerosqr))
;

duzerosqr3
=
-(doouzerosqr3*pow2(uzerosqr))
;

dLnuzerosqr1
=
duzerosqr1/uzerosqr
;

dLnuzerosqr2
=
duzerosqr2/uzerosqr
;

dLnuzerosqr3
=
duzerosqr3/uzerosqr
;

dLnalphaP1
=
dalphaP1[ijk]/alphaP[ijk]
;

dLnalphaP2
=
dalphaP2[ijk]/alphaP[ijk]
;

dLnalphaP3
=
dalphaP3[ijk]/alphaP[ijk]
;

dLnPsi1
=
dPsi1[ijk]/Psi[ijk]
;

dLnPsi2
=
dPsi2[ijk]/Psi[ijk]
;

dLnPsi3
=
dPsi3[ijk]/Psi[ijk]
;

}
/* if (bi == 0 || bi == 3 || bi == 4 || bi == 5) */




/* conditional */
if (nonlin) {

vecLapB1
=
1.3333333333333333333*ddB111[ijk] + ddB122[ijk] + ddB133[ijk] + 
  0.33333333333333333333*(ddB212[ijk] + ddB313[ijk])
;

vecLapB2
=
ddB211[ijk] + 1.3333333333333333333*ddB222[ijk] + ddB233[ijk] + 
  0.33333333333333333333*(ddB112[ijk] + ddB323[ijk])
;

vecLapB3
=
0.33333333333333333333*(ddB113[ijk] + ddB223[ijk]) + ddB311[ijk] + 
  ddB322[ijk] + 1.3333333333333333333*ddB333[ijk]
;

FPsi[ijk]
=
Psi5*((0.03125*LBLB)/alpha2 + 6.283185307179586477*rho) + ddPsi11[ijk] + 
  ddPsi22[ijk] + ddPsi33[ijk]
;

FB1[ijk]
=
-(dLnalphaPsim61*LB11) - dLnalphaPsim62*LB12 - dLnalphaPsim63*LB13 + 
  vecLapB1 - 50.26548245743669182*j1*Psi4*alpha[ijk]
;

FB2[ijk]
=
-(dLnalphaPsim61*LB12) - dLnalphaPsim62*LB22 - dLnalphaPsim63*LB23 + 
  vecLapB2 - 50.26548245743669182*j2*Psi4*alpha[ijk]
;

FB3[ijk]
=
-(dLnalphaPsim61*LB13) - dLnalphaPsim62*LB23 - dLnalphaPsim63*LB33 + 
  vecLapB3 - 50.26548245743669182*j3*Psi4*alpha[ijk]
;

FalphaP[ijk]
=
-(Psi4*((0.21875*LBLB)/alpha2 + 3.1415926535897932385*(2.*rho + 4.*S))*
     alphaP[ijk]) + ddalphaP11[ijk] + ddalphaP22[ijk] + ddalphaP33[ijk]
;



/* conditional */
if (bi == 0 || bi == 3 || bi == 4 || bi == 5) {

FSigma[ijk]
=
ddSigma11[ijk] + ddSigma22[ijk] + ddSigma33[ijk] + 
  (dLnalphaP1 + 5.*dLnPsi1 + dLnrho01 + 0.5*dLnuzerosqr1)*
   (dSigma1[ijk] + vRS1[ijk]) + 
  (dLnalphaP2 + 5.*dLnPsi2 + dLnrho02 + 0.5*dLnuzerosqr2)*
   (dSigma2[ijk] + vRS2[ijk]) + 
  (dLnalphaP3 + 5.*dLnPsi3 + dLnrho03 + 0.5*dLnuzerosqr3)*
   (dSigma3[ijk] + vRS3[ijk])
;


} else { /* if (!bi == 0 || bi == 3 || bi == 4 || bi == 5) */

FSigma[ijk]
=
Sigma[ijk]
;

}
/* if (bi == 0 || bi == 3 || bi == 4 || bi == 5) */



} else { /* if (!bi == 0 || bi == 3 || bi == 4 || bi == 5) */

alphaP2
=
pow2(alphaP[ijk])
;

alphaP3
=
alphaP2*alphaP[ijk]
;

Psi6
=
Psi2*Psi4
;

Psi7
=
Psi3*Psi4
;

gdlB
=
dlB11[ijk] + dlB22[ijk] + dlB33[ijk]
;

LlB11
=
-0.66666666666666666667*gdlB + 2.*dlB11[ijk]
;

LlB12
=
dlB12[ijk] + dlB21[ijk]
;

LlB13
=
dlB13[ijk] + dlB31[ijk]
;

LlB22
=
-0.66666666666666666667*gdlB + 2.*dlB22[ijk]
;

LlB23
=
dlB23[ijk] + dlB32[ijk]
;

LlB33
=
-0.66666666666666666667*gdlB + 2.*dlB33[ijk]
;

LlBdo11
=
LlB11
;

LlBdo12
=
LlB12
;

LlBdo13
=
LlB13
;

LlBdo21
=
LlB12
;

LlBdo22
=
LlB22
;

LlBdo23
=
LlB23
;

LlBdo31
=
LlB13
;

LlBdo32
=
LlB23
;

LlBdo33
=
LlB33
;

LlBLlB
=
LlB11*LlBdo11 + LlB12*(LlBdo12 + LlBdo21) + LlB22*LlBdo22 + 
  LlB13*(LlBdo13 + LlBdo31) + LlB23*(LlBdo23 + LlBdo32) + LlB33*LlBdo33
;

vecLaplB1
=
1.3333333333333333333*ddlB111[ijk] + ddlB122[ijk] + ddlB133[ijk] + 
  0.33333333333333333333*(ddlB212[ijk] + ddlB313[ijk])
;

vecLaplB2
=
ddlB211[ijk] + 1.3333333333333333333*ddlB222[ijk] + ddlB233[ijk] + 
  0.33333333333333333333*(ddlB112[ijk] + ddlB323[ijk])
;

vecLaplB3
=
0.33333333333333333333*(ddlB113[ijk] + ddlB223[ijk]) + ddlB311[ijk] + 
  ddlB322[ijk] + 1.3333333333333333333*ddlB333[ijk]
;

lvR1
=
dlSigma1[ijk]
;

lvR2
=
dlSigma2[ijk]
;

lvR3
=
dlSigma3[ijk]
;

lalpha
=
-((alphaP[ijk]*lPsi[ijk])/Psi2) + lalphaP[ijk]/Psi[ijk]
;

loouzerosqr
=
2.*(lalpha*alpha[ijk] - Psi4*((vR1 + beta1[ijk])*(lvR1 + lB1[ijk]) + 
        (vR2 + beta2[ijk])*(lvR2 + lB2[ijk]) + 
        (vR3 + beta3[ijk])*(lvR3 + lB3[ijk]))) - 
  Psi3*lPsi[ijk]*(8.*(vR1*beta1[ijk] + vR2*beta2[ijk] + vR3*beta3[ijk]) + 
     4.*(pow2(vR1) + pow2(vR2) + pow2(vR3) + pow2(beta1[ijk]) + 
        pow2(beta2[ijk]) + pow2(beta3[ijk])))
;

luzerosqr
=
-(loouzerosqr*pow2(uzerosqr))
;

lrho
=
(P + rhoE)*(alpha2*luzerosqr + 2.*lalpha*uzerosqr*alpha[ijk])
;

lj1
=
luzerosqr*alpha[ijk]*((P + rhoE)*vR1 + P*beta1[ijk]) + 
  rhoE*((lalpha*uzerosqr + luzerosqr*alpha[ijk])*beta1[ijk] + 
     uzerosqr*alpha[ijk]*lB1[ijk]) + 
  uzerosqr*(lalpha*((P + rhoE)*vR1 + P*beta1[ijk]) + 
     alpha[ijk]*(lvR1*(P + rhoE) + P*lB1[ijk]))
;

lj2
=
luzerosqr*alpha[ijk]*((P + rhoE)*vR2 + P*beta2[ijk]) + 
  rhoE*((lalpha*uzerosqr + luzerosqr*alpha[ijk])*beta2[ijk] + 
     uzerosqr*alpha[ijk]*lB2[ijk]) + 
  uzerosqr*(lalpha*((P + rhoE)*vR2 + P*beta2[ijk]) + 
     alpha[ijk]*(lvR2*(P + rhoE) + P*lB2[ijk]))
;

lj3
=
luzerosqr*alpha[ijk]*((P + rhoE)*vR3 + P*beta3[ijk]) + 
  rhoE*((lalpha*uzerosqr + luzerosqr*alpha[ijk])*beta3[ijk] + 
     uzerosqr*alpha[ijk]*lB3[ijk]) + 
  uzerosqr*(lalpha*((P + rhoE)*vR3 + P*beta3[ijk]) + 
     alpha[ijk]*(lvR3*(P + rhoE) + P*lB3[ijk]))
;

lS
=
lrho
;

ldLnalphaPsim61
=
dlalphaP1[ijk]/alphaP[ijk] - (dalphaP1[ijk]*lalphaP[ijk])/alphaP2 + 
  (7.*dPsi1[ijk]*lPsi[ijk])/Psi2 - (7.*dlPsi1[ijk])/Psi[ijk]
;

ldLnalphaPsim62
=
dlalphaP2[ijk]/alphaP[ijk] - (dalphaP2[ijk]*lalphaP[ijk])/alphaP2 + 
  (7.*dPsi2[ijk]*lPsi[ijk])/Psi2 - (7.*dlPsi2[ijk])/Psi[ijk]
;

ldLnalphaPsim63
=
dlalphaP3[ijk]/alphaP[ijk] - (dalphaP3[ijk]*lalphaP[ijk])/alphaP2 + 
  (7.*dPsi3[ijk]*lPsi[ijk])/Psi2 - (7.*dlPsi3[ijk])/Psi[ijk]
;

FlPsi[ijk]
=
((0.0625*(LBdo11*LlB11 + (LBdo12 + LBdo21)*LlB12 + 
          (LBdo13 + LBdo31)*LlB13 + LBdo22*LlB22 + 
          (LBdo23 + LBdo32)*LlB23 + LBdo33*LlB33))/alpha2 + 
     6.283185307179586477*lrho)*Psi5 + ddlPsi11[ijk] + ddlPsi22[ijk] + 
  ddlPsi33[ijk] + 31.415926535897932385*Psi4*rho*lPsi[ijk] + 
  LBLB*((-0.0625*Psi7*lalphaP[ijk])/alphaP3 + 
     (0.21875*Psi6*lPsi[ijk])/alphaP2)
;

FlB1[ijk]
=
-(LB11*ldLnalphaPsim61) - LB12*ldLnalphaPsim62 - LB13*ldLnalphaPsim63 - 
  dLnalphaPsim61*LlB11 - dLnalphaPsim62*LlB12 - dLnalphaPsim63*LlB13 + 
  vecLaplB1 - 3.1415926535897932385*
   (16.*(lj1*Psi4*alpha[ijk] + j1*Psi3*lalphaP[ijk]) + 
     48.*j1*Psi2*alphaP[ijk]*lPsi[ijk])
;

FlB2[ijk]
=
-(LB12*ldLnalphaPsim61) - LB22*ldLnalphaPsim62 - LB23*ldLnalphaPsim63 - 
  dLnalphaPsim61*LlB12 - dLnalphaPsim62*LlB22 - dLnalphaPsim63*LlB23 + 
  vecLaplB2 - 3.1415926535897932385*
   (16.*(lj2*Psi4*alpha[ijk] + j2*Psi3*lalphaP[ijk]) + 
     48.*j2*Psi2*alphaP[ijk]*lPsi[ijk])
;

FlB3[ijk]
=
-(LB13*ldLnalphaPsim61) - LB23*ldLnalphaPsim62 - LB33*ldLnalphaPsim63 - 
  dLnalphaPsim61*LlB13 - dLnalphaPsim62*LlB23 - dLnalphaPsim63*LlB33 + 
  vecLaplB3 - 3.1415926535897932385*
   (16.*(lj3*Psi4*alpha[ijk] + j3*Psi3*lalphaP[ijk]) + 
     48.*j3*Psi2*alphaP[ijk]*lPsi[ijk])
;

FlalphaP[ijk]
=
ddlalphaP11[ijk] + ddlalphaP22[ijk] + ddlalphaP33[ijk] + 
  ((0.21875*LBLB*Psi6)/alphaP2 - 12.566370614359172954*Psi4*S)*
   lalphaP[ijk] - Psi4*(((0.4375*
           (LBdo11*LlB11 + (LBdo12 + LBdo21)*LlB12 + 
             (LBdo13 + LBdo31)*LlB13 + LBdo22*LlB22 + 
             (LBdo23 + LBdo32)*LlB23 + LBdo33*LlB33))/alpha2 + 
        3.1415926535897932385*(2.*lrho + 4.*lS))*alphaP[ijk] + 
     6.283185307179586477*rho*lalphaP[ijk]) - 
  ((1.3125*LBLB*Psi5)/alphaP2 + 3.1415926535897932385*Psi3*(8.*rho + 16.*S))*
   alphaP[ijk]*lPsi[ijk]
;



/* conditional */
if (bi == 0 || bi == 3 || bi == 4 || bi == 5) {

dlalpha1
=
(2.*alphaP[ijk]*dPsi1[ijk]*lPsi[ijk])/Psi3 - 
  (alphaP[ijk]*dlPsi1[ijk] + dPsi1[ijk]*lalphaP[ijk] + 
     dalphaP1[ijk]*lPsi[ijk])/Psi2 + dlalphaP1[ijk]/Psi[ijk]
;

dlalpha2
=
(2.*alphaP[ijk]*dPsi2[ijk]*lPsi[ijk])/Psi3 - 
  (alphaP[ijk]*dlPsi2[ijk] + dPsi2[ijk]*lalphaP[ijk] + 
     dalphaP2[ijk]*lPsi[ijk])/Psi2 + dlalphaP2[ijk]/Psi[ijk]
;

dlalpha3
=
(2.*alphaP[ijk]*dPsi3[ijk]*lPsi[ijk])/Psi3 - 
  (alphaP[ijk]*dlPsi3[ijk] + dPsi3[ijk]*lalphaP[ijk] + 
     dalphaP3[ijk]*lPsi[ijk])/Psi2 + dlalphaP3[ijk]/Psi[ijk]
;

ldoouzerosqr1
=
2.*(dalpha1*lalpha + dlalpha1*alpha[ijk] - 
     Psi4*((vR1 + beta1[ijk])*(ddlSigma11[ijk] + dlB11[ijk]) + 
        (vR2 + beta2[ijk])*(ddlSigma12[ijk] + dlB21[ijk]) + 
        (vR3 + beta3[ijk])*(ddlSigma13[ijk] + dlB31[ijk]) + 
        (dbeta11 + dvR11)*(dlSigma1[ijk] + lB1[ijk]) + 
        (dbeta21 + dvR21)*(dlSigma2[ijk] + lB2[ijk]) + 
        (dbeta31 + dvR31)*(dlSigma3[ijk] + lB3[ijk]))) - 
  Psi2*dPsi1[ijk]*lPsi[ijk]*(24.*
      (vR1*beta1[ijk] + vR2*beta2[ijk] + vR3*beta3[ijk]) + 
     12.*(pow2(vR1) + pow2(vR2) + pow2(vR3) + pow2(beta1[ijk]) + 
        pow2(beta2[ijk]) + pow2(beta3[ijk]))) - 
  Psi3*(8.*(dPsi1[ijk]*((vR1 + beta1[ijk])*(dlSigma1[ijk] + lB1[ijk]) + 
           (vR2 + beta2[ijk])*(dlSigma2[ijk] + lB2[ijk]) + 
           (vR3 + beta3[ijk])*(dlSigma3[ijk] + lB3[ijk])) + 
        ((dbeta11 + dvR11)*(vR1 + beta1[ijk]) + 
           (dbeta21 + dvR21)*(vR2 + beta2[ijk]) + 
           (dbeta31 + dvR31)*(vR3 + beta3[ijk]))*lPsi[ijk]) + 
     dlPsi1[ijk]*(8.*(vR1*beta1[ijk] + vR2*beta2[ijk] + vR3*beta3[ijk]) + 
        4.*(pow2(vR1) + pow2(vR2) + pow2(vR3) + pow2(beta1[ijk]) + 
           pow2(beta2[ijk]) + pow2(beta3[ijk]))))
;

ldoouzerosqr2
=
2.*(dalpha2*lalpha + dlalpha2*alpha[ijk] - 
     Psi4*((vR1 + beta1[ijk])*(ddlSigma12[ijk] + dlB12[ijk]) + 
        (vR2 + beta2[ijk])*(ddlSigma22[ijk] + dlB22[ijk]) + 
        (vR3 + beta3[ijk])*(ddlSigma23[ijk] + dlB32[ijk]) + 
        (dbeta12 + dvR12)*(dlSigma1[ijk] + lB1[ijk]) + 
        (dbeta22 + dvR22)*(dlSigma2[ijk] + lB2[ijk]) + 
        (dbeta32 + dvR32)*(dlSigma3[ijk] + lB3[ijk]))) - 
  Psi2*dPsi2[ijk]*lPsi[ijk]*(24.*
      (vR1*beta1[ijk] + vR2*beta2[ijk] + vR3*beta3[ijk]) + 
     12.*(pow2(vR1) + pow2(vR2) + pow2(vR3) + pow2(beta1[ijk]) + 
        pow2(beta2[ijk]) + pow2(beta3[ijk]))) - 
  Psi3*(8.*(dPsi2[ijk]*((vR1 + beta1[ijk])*(dlSigma1[ijk] + lB1[ijk]) + 
           (vR2 + beta2[ijk])*(dlSigma2[ijk] + lB2[ijk]) + 
           (vR3 + beta3[ijk])*(dlSigma3[ijk] + lB3[ijk])) + 
        ((dbeta12 + dvR12)*(vR1 + beta1[ijk]) + 
           (dbeta22 + dvR22)*(vR2 + beta2[ijk]) + 
           (dbeta32 + dvR32)*(vR3 + beta3[ijk]))*lPsi[ijk]) + 
     dlPsi2[ijk]*(8.*(vR1*beta1[ijk] + vR2*beta2[ijk] + vR3*beta3[ijk]) + 
        4.*(pow2(vR1) + pow2(vR2) + pow2(vR3) + pow2(beta1[ijk]) + 
           pow2(beta2[ijk]) + pow2(beta3[ijk]))))
;

ldoouzerosqr3
=
2.*(dalpha3*lalpha + dlalpha3*alpha[ijk] - 
     Psi4*((vR1 + beta1[ijk])*(ddlSigma13[ijk] + dlB13[ijk]) + 
        (vR2 + beta2[ijk])*(ddlSigma23[ijk] + dlB23[ijk]) + 
        (vR3 + beta3[ijk])*(ddlSigma33[ijk] + dlB33[ijk]) + 
        (dbeta13 + dvR13)*(dlSigma1[ijk] + lB1[ijk]) + 
        (dbeta23 + dvR23)*(dlSigma2[ijk] + lB2[ijk]) + 
        (dbeta33 + dvR33)*(dlSigma3[ijk] + lB3[ijk]))) - 
  Psi2*dPsi3[ijk]*lPsi[ijk]*(24.*
      (vR1*beta1[ijk] + vR2*beta2[ijk] + vR3*beta3[ijk]) + 
     12.*(pow2(vR1) + pow2(vR2) + pow2(vR3) + pow2(beta1[ijk]) + 
        pow2(beta2[ijk]) + pow2(beta3[ijk]))) - 
  Psi3*(8.*(dPsi3[ijk]*((vR1 + beta1[ijk])*(dlSigma1[ijk] + lB1[ijk]) + 
           (vR2 + beta2[ijk])*(dlSigma2[ijk] + lB2[ijk]) + 
           (vR3 + beta3[ijk])*(dlSigma3[ijk] + lB3[ijk])) + 
        ((dbeta13 + dvR13)*(vR1 + beta1[ijk]) + 
           (dbeta23 + dvR23)*(vR2 + beta2[ijk]) + 
           (dbeta33 + dvR33)*(vR3 + beta3[ijk]))*lPsi[ijk]) + 
     dlPsi3[ijk]*(8.*(vR1*beta1[ijk] + vR2*beta2[ijk] + vR3*beta3[ijk]) + 
        4.*(pow2(vR1) + pow2(vR2) + pow2(vR3) + pow2(beta1[ijk]) + 
           pow2(beta2[ijk]) + pow2(beta3[ijk]))))
;

lduzerosqr1
=
-2.*doouzerosqr1*luzerosqr*uzerosqr - ldoouzerosqr1*pow2(uzerosqr)
;

lduzerosqr2
=
-2.*doouzerosqr2*luzerosqr*uzerosqr - ldoouzerosqr2*pow2(uzerosqr)
;

lduzerosqr3
=
-2.*doouzerosqr3*luzerosqr*uzerosqr - ldoouzerosqr3*pow2(uzerosqr)
;

FlSigma[ijk]
=
ddlSigma11[ijk] + ddlSigma22[ijk] + ddlSigma33[ijk] + 
  (dLnalphaP1 + 5.*dLnPsi1 + dLnrho01 + 0.5*dLnuzerosqr1)*dlSigma1[ijk] + 
  (dLnalphaP2 + 5.*dLnPsi2 + dLnrho02 + 0.5*dLnuzerosqr2)*dlSigma2[ijk] + 
  (dLnalphaP3 + 5.*dLnPsi3 + dLnrho03 + 0.5*dLnuzerosqr3)*dlSigma3[ijk] + 
  (dSigma1[ijk] + vRS1[ijk])*(dlalphaP1[ijk]/alphaP[ijk] - 
     (dalphaP1[ijk]*lalphaP[ijk])/alphaP2 - 
     (5.*dPsi1[ijk]*lPsi[ijk])/Psi2 + (5.*dlPsi1[ijk])/Psi[ijk] + 
     0.5*(lduzerosqr1/uzerosqr - duzerosqr1*luzerosqr*pow2inv(uzerosqr))) + 
  (dSigma2[ijk] + vRS2[ijk])*(dlalphaP2[ijk]/alphaP[ijk] - 
     (dalphaP2[ijk]*lalphaP[ijk])/alphaP2 - 
     (5.*dPsi2[ijk]*lPsi[ijk])/Psi2 + (5.*dlPsi2[ijk])/Psi[ijk] + 
     0.5*(lduzerosqr2/uzerosqr - duzerosqr2*luzerosqr*pow2inv(uzerosqr))) + 
  (dSigma3[ijk] + vRS3[ijk])*(dlalphaP3[ijk]/alphaP[ijk] - 
     (dalphaP3[ijk]*lalphaP[ijk])/alphaP2 - (5.*dPsi3[ijk]*lPsi[ijk])/Psi2 + 
     (5.*dlPsi3[ijk])/Psi[ijk] + 
     0.5*(lduzerosqr3/uzerosqr - duzerosqr3*luzerosqr*pow2inv(uzerosqr)))
;


} else { /* if (!bi == 0 || bi == 3 || bi == 4 || bi == 5) */

FlSigma[ijk]
=
lSigma[ijk]
;

}
/* if (bi == 0 || bi == 3 || bi == 4 || bi == 5) */


}
/* if (bi == 0 || bi == 3 || bi == 4 || bi == 5) */



} /* end of points loop */ 

} /* end of boxes */


}  /* end of function */

/* BNS_CTS.c */
/* nvars = 166, n* = 700,  n/ = 119,  n+ = 786, n = 1605, O = 1 */
