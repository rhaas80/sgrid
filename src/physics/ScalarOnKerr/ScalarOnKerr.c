/* ScalarOnKerr.c */
/* Wolfgang Tichy  8/2007 */

#include "sgrid.h"
#include "ScalarOnKerr.h"



/* initialize ScalarOnKerr */
int ScalarOnKerr_startup(tGrid *grid)
{
  tVarList *ScalarOnKerrvars;
  int b;
  
  printf("Initializing ScalarOnKerr:\n");

  /* set boundary information for ScalarOnKerr evolution: 
     farlimit, falloff, propagation speed   */
  VarNameSetBoundaryInfo("ScalarOnKerr_psi",     0, 1, 1.0);
  VarNameSetBoundaryInfo("ScalarOnKerr_dpsix",   0, 1, 1.0);
  VarNameSetBoundaryInfo("ScalarOnKerr_dpsiy",   0, 1, 1.0);
  VarNameSetBoundaryInfo("ScalarOnKerr_dpsiz",   0, 1, 1.0);
  VarNameSetBoundaryInfo("ScalarOnKerr_ddpsixx", 0, 1, 1.0);
  VarNameSetBoundaryInfo("ScalarOnKerr_ddpsixy", 0, 1, 1.0);
  VarNameSetBoundaryInfo("ScalarOnKerr_ddpsixz", 0, 1, 1.0);
  VarNameSetBoundaryInfo("ScalarOnKerr_ddpsiyy", 0, 1, 1.0);
  VarNameSetBoundaryInfo("ScalarOnKerr_ddpsiyz", 0, 1, 1.0);
  VarNameSetBoundaryInfo("ScalarOnKerr_ddpsizz", 0, 1, 1.0);

  VarNameSetBoundaryInfo("ScalarOnKerr_pi",   0, 1, 1.0);
  VarNameSetBoundaryInfo("ScalarOnKerr_dpix", 0, 1, 1.0);
  VarNameSetBoundaryInfo("ScalarOnKerr_dpiy", 0, 1, 1.0);
  VarNameSetBoundaryInfo("ScalarOnKerr_dpiz", 0, 1, 1.0);

  /* create a variable list for ScalarOnKerr evolutions  */
  ScalarOnKerrvars = vlalloc(grid);
  vlpush(ScalarOnKerrvars, Ind("ScalarOnKerr_psi"));
  vlpush(ScalarOnKerrvars, Ind("ScalarOnKerr_pi"));
  if (0) prvarlist(ScalarOnKerrvars);
  enablevarlist(ScalarOnKerrvars);

  /* register evolved variables */
  evolve_vlregister(ScalarOnKerrvars);
  
  /* register evolution routine */
  evolve_rhsregister(ScalarOnKerr_evolve);

  /* set initial data in boxes */
  forallboxes(grid,b)
  {  
    tBox *box = grid->box[b];
    int i,j,k;
    int n1=box->n1;
    int n2=box->n2;
    int n3=box->n3;
    double *px = box->v[Ind("x")];
    double *py = box->v[Ind("y")];
    double *pz = box->v[Ind("z")];

    double *psi = box->v[Ind("ScalarOnKerr_psi")];
    double *pi  = box->v[Ind("ScalarOnKerr_pi")];

    forallpoints(box,i)
    {
      double x = px[i];
      double y = py[i];
      double z = pz[i];

      psi[i] =  0.0;
      pi[i]  =  0.0;
    }
  }

  //set_boundary_symmetry(level, ScalarOnKerrvars);

  if(Getv("ScalarOnKerr_reset_doubleCoveredPoints", "yes"))
    reset_doubleCoveredPoints(ScalarOnKerrvars);
  
  /* enable all derivative vars */
  enablevar(grid, Ind("ScalarOnKerr_dpsix"));
  enablevar(grid, Ind("ScalarOnKerr_ddpsixx"));
  enablevar(grid, Ind("ScalarOnKerr_dpix"));
  
  /* enable all metric vars */
  enablevar(grid, Ind("ScalarOnKerr_gtt"));
  enablevar(grid, Ind("ScalarOnKerr_guptt"));
  enablevar(grid, Ind("ScalarOnKerr_Gammattt"));

  enablevar(grid, Ind("ScalarOnKerr3d_gxx"));
  enablevar(grid, Ind("ScalarOnKerr3d_alpha"));
  enablevar(grid, Ind("ScalarOnKerr3d_betax"));
  enablevar(grid, Ind("ScalarOnKerr3d_Kxx"));
  enablevar(grid, Ind("ScalarOnKerr3d_TrK"));
  enablevar(grid, Ind("ScalarOnKerr3d_gupxx"));
  enablevar(grid, Ind("ScalarOnKerr3d_Gammaxxx"));
  enablevar(grid, Ind("ScalarOnKerr3d_dalphax"));

  /* enable temp vars */
  if(!Getv("physics", "ADMvars"))
  {
    enablevar(grid, Ind("temp1"));
    enablevar(grid, Ind("temp2"));
    enablevar(grid, Ind("temp3"));
  }


  /* set Kerr metric and Christoffels */
  Kerr(grid, Ind("x"), Ind("ScalarOnKerr_gtt"), Ind("ScalarOnKerr_guptt"),
       Ind("ScalarOnKerr_Gammattt"));

  /* set Kerr in 3+1 */
  Kerr3d(grid, Ind("x"), Ind("ScalarOnKerr3d_alpha"), Ind("ScalarOnKerr3d_betax"),
         Ind("ScalarOnKerr3d_gxx"), Ind("ScalarOnKerr3d_Kxx"),
         Ind("ScalarOnKerr3d_TrK"), Ind("ScalarOnKerr3d_gupxx"),
         Ind("ScalarOnKerr3d_Gammaxxx"), Ind("ScalarOnKerr3d_dalphax"));

  return 0;
}




/* evolve and set boundary points */
void ScalarOnKerr_evolve_old(tVarList *unew, tVarList *upre, double dt, 
                             tVarList *ucur)
{
  tGrid *grid = ucur->grid;
  int b;
  
  forallboxes(grid,b)
  {
    tBox *box = grid->box[b];
    double *cpsi = vlldataptr(ucur, box, 0);
    double *ppsi = vlldataptr(upre, box, 0);
    double *npsi = vlldataptr(unew, box, 0);
    double *cpi = vlldataptr(ucur, box, 1);
    double *ppi = vlldataptr(upre, box, 1);
    double *npi = vlldataptr(unew, box, 1);
    int i;
    int ipsi 	= (ucur)->index[0];
    int ipi = (ucur)->index[1];
    double *psix = box->v[Ind("ScalarOnKerr_dpsix")];
    double *psiy = box->v[Ind("ScalarOnKerr_dpsix")+1];
    double *psiz = box->v[Ind("ScalarOnKerr_dpsix")+2];
    double *psixx = box->v[Ind("ScalarOnKerr_ddpsixx")];
    double *psixy = box->v[Ind("ScalarOnKerr_ddpsixx")+1];
    double *psixz = box->v[Ind("ScalarOnKerr_ddpsixx")+2];
    double *psiyy = box->v[Ind("ScalarOnKerr_ddpsixx")+3];
    double *psiyz = box->v[Ind("ScalarOnKerr_ddpsixx")+4];
    double *psizz = box->v[Ind("ScalarOnKerr_ddpsixx")+5];
    double *pix = box->v[Ind("ScalarOnKerr_dpix")];
    double *piy = box->v[Ind("ScalarOnKerr_dpix")+1];
    double *piz = box->v[Ind("ScalarOnKerr_dpix")+2];
    double *px = box->v[Ind("x")];
    double *py = box->v[Ind("y")];
    double *pz = box->v[Ind("z")];
    int i_gup = Ind("ScalarOnKerr_guptt");
    double *gtt = box->v[i_gup];
    double *gtx = box->v[i_gup+1];
    double *gty = box->v[i_gup+2];
    double *gtz = box->v[i_gup+3];
    double *gxx = box->v[i_gup+4];
    double *gxy = box->v[i_gup+5];
    double *gxz = box->v[i_gup+6];
    double *gyy = box->v[i_gup+7];
    double *gyz = box->v[i_gup+8];
    double *gzz = box->v[i_gup+9];
    int i_Gam = Ind("ScalarOnKerr_Gammattt");
    double *Gamttt = box->v[i_Gam];
    double *Gamttx = box->v[i_Gam+1];
    double *Gamtty = box->v[i_Gam+2];
    double *Gamttz = box->v[i_Gam+3];
    double *Gamtxx = box->v[i_Gam+4];
    double *Gamtxy = box->v[i_Gam+5];
    double *Gamtxz = box->v[i_Gam+6];
    double *Gamtyy = box->v[i_Gam+7];
    double *Gamtyz = box->v[i_Gam+8];
    double *Gamtzz = box->v[i_Gam+9];
    double *Gamxtt = box->v[i_Gam+10];
    double *Gamxtx = box->v[i_Gam+11];
    double *Gamxty = box->v[i_Gam+12];
    double *Gamxtz = box->v[i_Gam+13];
    double *Gamxxx = box->v[i_Gam+14];
    double *Gamxxy = box->v[i_Gam+15];
    double *Gamxxz = box->v[i_Gam+16];
    double *Gamxyy = box->v[i_Gam+17];
    double *Gamxyz = box->v[i_Gam+18];
    double *Gamxzz = box->v[i_Gam+19];
    double *Gamytt = box->v[i_Gam+20];
    double *Gamytx = box->v[i_Gam+21];
    double *Gamyty = box->v[i_Gam+22];
    double *Gamytz = box->v[i_Gam+23];
    double *Gamyxx = box->v[i_Gam+24];
    double *Gamyxy = box->v[i_Gam+25];
    double *Gamyxz = box->v[i_Gam+26];
    double *Gamyyy = box->v[i_Gam+27];
    double *Gamyyz = box->v[i_Gam+28];
    double *Gamyzz = box->v[i_Gam+29];
    double *Gamztt = box->v[i_Gam+30];
    double *Gamztx = box->v[i_Gam+31];
    double *Gamzty = box->v[i_Gam+32];
    double *Gamztz = box->v[i_Gam+33];
    double *Gamzxx = box->v[i_Gam+34];
    double *Gamzxy = box->v[i_Gam+35];
    double *Gamzxz = box->v[i_Gam+36];
    double *Gamzyy = box->v[i_Gam+37];
    double *Gamzyz = box->v[i_Gam+38];
    double *Gamzzz = box->v[i_Gam+39];
            
    /* compute the spatial derivs */
    allDerivsOf_S(box, ipsi,
                  Ind("ScalarOnKerr_dpsix"), Ind("ScalarOnKerr_ddpsixx"));
    FirstDerivsOf_S(box, ipi , Ind("ScalarOnKerr_dpix"));

    /* loop over points and set RHS */
    forallpoints(box, i)
    {
      double rpi, rpsi;
      double x = px[i];
      double y = py[i];
      double z = pz[i];
      double t = grid->time;
      double x0, y0;
      double g_ddpsi, gGt,gGx,gGy,gGz, gG_dpsi;
      /* g is upper metric */
      /* get all terms with less than 2 time derivs in g^ab d_a d_b psi */
      g_ddpsi = 2.0*(gtx[i]*pix[i] +gty[i]*piy[i] +gtz[i]*piz[i]) + 
                gxx[i]*psixx[i] + gyy[i]*psiyy[i] + gzz[i]*psizz[i] +
                2.0*(gxy[i]*psixy[i] + gxz[i]*psixz[i] + gyz[i]*psiyz[i]);

      /* get gG[a] = g^bc Gamma^a_bc */
      gGt = gtt[i]*Gamttt[i] + 
            gxx[i]*Gamtxx[i] + gyy[i]*Gamtyy[i] + gzz[i]*Gamtzz[i] +
            2.0*(gtx[i]*Gamttx[i] + gty[i]*Gamtty[i] + gtz[i]*Gamttz[i] +
                 gxy[i]*Gamtxy[i] + gxz[i]*Gamtxz[i] + gyz[i]*Gamtyz[i]);
      gGx = gtt[i]*Gamxtt[i] + 
            gxx[i]*Gamxxx[i] + gyy[i]*Gamxyy[i] + gzz[i]*Gamxzz[i] +
            2.0*(gtx[i]*Gamxtx[i] + gty[i]*Gamxty[i] + gtz[i]*Gamxtz[i] +
                 gxy[i]*Gamxxy[i] + gxz[i]*Gamxxz[i] + gyz[i]*Gamxyz[i]);
      gGy = gtt[i]*Gamytt[i] + 
            gxx[i]*Gamyxx[i] + gyy[i]*Gamyyy[i] + gzz[i]*Gamyzz[i] +
            2.0*(gtx[i]*Gamytx[i] + gty[i]*Gamyty[i] + gtz[i]*Gamytz[i] +
                 gxy[i]*Gamyxy[i] + gxz[i]*Gamyxz[i] + gyz[i]*Gamyyz[i]);
      gGz = gtt[i]*Gamztt[i] + 
            gxx[i]*Gamzxx[i] + gyy[i]*Gamzyy[i] + gzz[i]*Gamzzz[i] +
            2.0*(gtx[i]*Gamztx[i] + gty[i]*Gamzty[i] + gtz[i]*Gamztz[i] +
                 gxy[i]*Gamzxy[i] + gxz[i]*Gamzxz[i] + gyz[i]*Gamzyz[i]);
      gG_dpsi = gGt*cpi[i] + gGx*psix[i] + gGy*psiy[i] + gGz*psiz[i];

      /* source posistion */
      x0 = 10*cos(0.02*t);
      y0 = 10*sin(0.02*t);

      /* set RHS of psi and pi */
      rpi  = -(g_ddpsi - gG_dpsi)/gtt[i] + 
                exp(-(x-x0)*(x-x0))*exp(-(y-y0)*(y-y0))*exp(-z*z); // source
      rpsi = cpi[i];

      /* set new vars or RHS, depending in which integrator is used */
      if(dt!=0.0)
      {
        npi[i]  = ppi[i]  + dt * rpi;
        npsi[i] = ppsi[i] + dt * rpsi;
      }
      else
      {
        npi[i]  = rpi;
        npsi[i] = rpsi;
      }
    }
  }

  /* set BCs */
  set_boundary(unew, upre, dt, ucur);

  if(Getv("ScalarOnKerr_reset_doubleCoveredPoints", "yes"))
    reset_doubleCoveredPoints(unew);
}


/* evolve and set boundary points */
void ScalarOnKerr_evolve(tVarList *unew, tVarList *upre, double dt, 
                         tVarList *ucur)
{
  tGrid *grid = ucur->grid;
  int b;
  
  forallboxes(grid,b)
  {
    tBox *box = grid->box[b];
    double *cpsi = vlldataptr(ucur, box, 0);
    double *ppsi = vlldataptr(upre, box, 0);
    double *npsi = vlldataptr(unew, box, 0);
    double *cpi = vlldataptr(ucur, box, 1);
    double *ppi = vlldataptr(upre, box, 1);
    double *npi = vlldataptr(unew, box, 1);
    int i;
    int ipsi 	= (ucur)->index[0];
    int ipi = (ucur)->index[1];
    double *psix = box->v[Ind("ScalarOnKerr_dpsix")];
    double *psiy = box->v[Ind("ScalarOnKerr_dpsix")+1];
    double *psiz = box->v[Ind("ScalarOnKerr_dpsix")+2];
    double *psixx = box->v[Ind("ScalarOnKerr_ddpsixx")];
    double *psixy = box->v[Ind("ScalarOnKerr_ddpsixx")+1];
    double *psixz = box->v[Ind("ScalarOnKerr_ddpsixx")+2];
    double *psiyy = box->v[Ind("ScalarOnKerr_ddpsixx")+3];
    double *psiyz = box->v[Ind("ScalarOnKerr_ddpsixx")+4];
    double *psizz = box->v[Ind("ScalarOnKerr_ddpsixx")+5];
    double *pix = box->v[Ind("ScalarOnKerr_dpix")];
    double *piy = box->v[Ind("ScalarOnKerr_dpix")+1];
    double *piz = box->v[Ind("ScalarOnKerr_dpix")+2];
    double *px = box->v[Ind("x")];
    double *py = box->v[Ind("y")];
    double *pz = box->v[Ind("z")];
    int i_alpha = Ind("ScalarOnKerr3d_alpha");
    double *alpha = box->v[i_alpha];
    int i_beta = Ind("ScalarOnKerr3d_betax");
    double *betax = box->v[i_beta];
    double *betay = box->v[i_beta+1];
    double *betaz = box->v[i_beta+2];
    int i_gup = Ind("ScalarOnKerr3d_gupxx");
    double *gxx = box->v[i_gup];
    double *gxy = box->v[i_gup+1];
    double *gxz = box->v[i_gup+2];
    double *gyy = box->v[i_gup+3];
    double *gyz = box->v[i_gup+4];
    double *gzz = box->v[i_gup+5];
    int i_TrK = Ind("ScalarOnKerr3d_TrK");
    double *TrK = box->v[i_TrK];
    int i_Gam = Ind("ScalarOnKerr3d_Gammaxxx");
    double *Gamxxx = box->v[i_Gam];
    double *Gamxxy = box->v[i_Gam+1];
    double *Gamxxz = box->v[i_Gam+2];
    double *Gamxyy = box->v[i_Gam+3];
    double *Gamxyz = box->v[i_Gam+4];
    double *Gamxzz = box->v[i_Gam+5];
    double *Gamyxx = box->v[i_Gam+6];
    double *Gamyxy = box->v[i_Gam+7];
    double *Gamyxz = box->v[i_Gam+8];
    double *Gamyyy = box->v[i_Gam+9];
    double *Gamyyz = box->v[i_Gam+10];
    double *Gamyzz = box->v[i_Gam+11];
    double *Gamzxx = box->v[i_Gam+12];
    double *Gamzxy = box->v[i_Gam+13];
    double *Gamzxz = box->v[i_Gam+14];
    double *Gamzyy = box->v[i_Gam+15];
    double *Gamzyz = box->v[i_Gam+16];
    double *Gamzzz = box->v[i_Gam+17];
    int i_dalpha = Ind("ScalarOnKerr3d_dalphax");
    double *dalphax = box->v[i_dalpha];
    double *dalphay = box->v[i_dalpha+1];
    double *dalphaz = box->v[i_dalpha+2];
            
    /* compute the spatial derivs */
    allDerivsOf_S(box, ipsi,
                  Ind("ScalarOnKerr_dpsix"), Ind("ScalarOnKerr_ddpsixx"));
    FirstDerivsOf_S(box, ipi , Ind("ScalarOnKerr_dpix"));

    /* loop over points and set RHS */
    forallpoints(box, i)
    {
      double rpi, rpsi;
      double x = px[i];
      double y = py[i];
      double z = pz[i];
      double t = grid->time;
      double x0, y0;
      double beta_dpi,ag_ddpsi, gGx,gGy,gGz,agG_dpsi, g_dpsi_da, aKpi, beta_dpsi;
      /* g is upper metric */
      /* terms on RHS of pi eqn */ 
      beta_dpi = betax[i]*pix[i] + betay[i]*piy[i] + betaz[i]*piz[i];
      ag_ddpsi = alpha[i]*
                  (gxx[i]*psixx[i] + gyy[i]*psiyy[i] + gzz[i]*psizz[i] +
                   2.0*(gxy[i]*psixy[i] + gxz[i]*psixz[i] + gyz[i]*psiyz[i]));
      
      /* get alpha gG[i] = alpha g^ik Gamma^i_jk */
      gGx = gxx[i]*Gamxxx[i] + gyy[i]*Gamxyy[i] + gzz[i]*Gamxzz[i] +
             2.0*(gxy[i]*Gamxxy[i] + gxz[i]*Gamxxz[i] + gyz[i]*Gamxyz[i]);
      gGy = gxx[i]*Gamyxx[i] + gyy[i]*Gamyyy[i] + gzz[i]*Gamyzz[i] +
             2.0*(gxy[i]*Gamyxy[i] + gxz[i]*Gamyxz[i] + gyz[i]*Gamyyz[i]);
      gGz = gxx[i]*Gamzxx[i] + gyy[i]*Gamzyy[i] + gzz[i]*Gamzzz[i] +
             2.0*(gxy[i]*Gamzxy[i] + gxz[i]*Gamzxz[i] + gyz[i]*Gamzyz[i]);
      agG_dpsi = alpha[i]*(gGx*psix[i] + gGy*psiy[i] + gGz*psiz[i]);

      /* get g_dpsi_da = g^ik dpsi_i dalpha_k */
      g_dpsi_da = gxx[i]*psix[i]*dalphax[i] + gyy[i]*psiy[i]*dalphay[i] +
                  gzz[i]*psiz[i]*dalphaz[i] +
                  2.0*(gxy[i]*psix[i]*dalphay[i] + 
                       gxz[i]*psix[i]*dalphaz[i] + gyz[i]*psiy[i]*dalphaz[i]);
      aKpi = alpha[i]*(TrK[i]*cpi[i]);

      /* term on RHS of psi eqn */
      beta_dpsi = betax[i]*psix[i] + betay[i]*psiy[i] + betaz[i]*psiz[i];

      /* source posistion */
      x0 = 10*cos(0.02*t);
      y0 = 10*sin(0.02*t);

      /* set RHS of psi and pi */
      rpi  = beta_dpi - ag_ddpsi + agG_dpsi - g_dpsi_da + aKpi  +
              -exp(-(x-x0)*(x-x0))*exp(-(y-y0)*(y-y0))*exp(-z*z); // source
      rpsi = beta_dpsi - alpha[i]*cpi[i];

      /* set new vars or RHS, depending in which integrator is used */
      if(dt!=0.0)
      {
        npi[i]  = ppi[i]  + dt * rpi;
        npsi[i] = ppsi[i] + dt * rpsi;
      }
      else
      {
        npi[i]  = rpi;
        npsi[i] = rpsi;
      }
    }
  }

  /* set BCs */
  set_boundary(unew, upre, dt, ucur);

  if(Getv("ScalarOnKerr_reset_doubleCoveredPoints", "yes"))
    reset_doubleCoveredPoints(unew);
}




/* compute and integrals of rho */
int ScalarOnKerr_analyze(tGrid *grid)
{
  int b;

// if( ! timeforoutput_index(grid, Ind("ScalarOnKerr_rho")) ) return 0;

// do something useful here 

  return 0;
}
