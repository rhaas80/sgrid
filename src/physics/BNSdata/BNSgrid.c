/* grid_setup.c */
/* Wolfgang Tichy 2008 */


#include "sgrid.h"
#include "BNSdata.h"

#define Power pow


/* from Coordinates.c */
extern double  (*Coordinates_AnsorgNS_sigmap)(tBox *box, int ind, double B, double phi);
extern double (*Coordinates_AnsorgNS_dsigmap_dB)(tBox *box, int ind, double B, double phi);
extern double (*Coordinates_AnsorgNS_dsigmap_dphi)(tBox *box, int ind, double B, double phi);
extern double  (*Coordinates_AnsorgNS_sigmam)(tBox *box, int ind, double B, double phi);
extern double (*Coordinates_AnsorgNS_dsigmam_dB)(tBox *box, int ind, double B, double phi);
extern double (*Coordinates_AnsorgNS_dsigmam_dphi)(tBox *box, int ind, double B, double phi);
extern double Coordinates_AnsorgNS_b; /* value of x if A=1 in AnsorgNS0/3 */

/* global vars in this file */
double sigma1; /* initial sigma_+ */
double sigma2; /* initial sigma_- */
double rf_surf1; /* radius of star1 */
double rf_surf2; /* radius of star2 */
double P_core1;  /* core pressure of star1 */
double P_core2;  /* core pressure of star2 */
tBox *BNdata_q_VectorFunc_box;      /* box for BNdata_q_VectorFunc */
double *BNdata_q_VectorFunc_coeffs; /* coeffs for BNdata_q_VectorFunc */
double BNdata_sigp_VectorFunc_sigp_1phi; /* sigp_1phi for BNdata_sigp_VectorFunc */
double BNdata_sigp_VectorFunc_B;         /* B for BNdata_sigp_VectorFunc */
double BNdata_sigp_VectorFunc_X0;        /* X0 for BNdata_sigp_VectorFunc */

/* funs in this file */
void rf_surf1_VectorFunc(int n, double *vec, double *fvec);
void rf_surf2_VectorFunc(int n, double *vec, double *fvec);
void m01_VectorFunc(int n, double *vec, double *fvec);
void m02_VectorFunc(int n, double *vec, double *fvec);




/* functions for sigma_{+-} */
double return_sigma1(tBox *box, int ind, double B, double phi)
{
  return sigma1;
}
double return_dsigma_zero(tBox *box, int ind, double B, double phi)
{
  return 0.0;
}
double return_sigma2(tBox *box, int ind, double B, double phi)
{
  return sigma2;
}

/* setup initial boxsizes */
int set_boxsizes(tGrid *grid)
{
  double m1, Phic1, Psic1;
  double m2, Phic2, Psic2;
  double kappa     = Getd("BNSdata_kappa");
  double BNSdata_n = Getd("BNSdata_n");
  double BNSdata_b = Getd("BNSdata_b");
  double m01 = Getd("BNSdata_m01");
  double m02 = Getd("BNSdata_m02");
  double Gamma     = 1.0 + 1.0/BNSdata_n;
  double vec[2];
  int check;

  printf("set_boxsizes: setting box sizes and coordinates used ...\n");
  
  /* set Coordinates_AnsorgNS_sigmap/m funcs, .s.t. it works with sigma1/2 */
  Coordinates_AnsorgNS_sigmap       = return_sigma1;
  Coordinates_AnsorgNS_dsigmap_dB   = return_dsigma_zero;
  Coordinates_AnsorgNS_dsigmap_dphi = return_dsigma_zero;
  Coordinates_AnsorgNS_sigmam       = return_sigma2;
  Coordinates_AnsorgNS_dsigmam_dB   = return_dsigma_zero;
  Coordinates_AnsorgNS_dsigmam_dphi = return_dsigma_zero;
  Setd("Coordinates_AnsorgNS_b", BNSdata_b);
  Coordinates_AnsorgNS_b = BNSdata_b;

  /* set some box pars */
  if(Getv("BNSdata_grid", "4ABphi_2xyz"))
  {
    Sets("box0_Coordinates", "AnsorgNS0");
    Sets("box0_basis1", "ChebExtrema");
    Sets("box0_min1", "0");
    Sets("box0_basis2", "ChebExtrema");
    Sets("box0_min2", "0");
    Sets("box0_max2", "1");
    Sets("box0_basis3", "Fourier");
    Sets("box0_min3", "0");
    Sets("box0_max3", "2*pi");
        
    Sets("box1_Coordinates", "AnsorgNS1");
    Sets("box1_basis1", "ChebExtrema");
    Sets("box1_min1", "0");
    Sets("box1_max1", "1");
    Sets("box1_basis2", "ChebExtrema");
    Sets("box1_min2", "0");
    Sets("box1_max2", "1");
    Sets("box1_basis3", "Fourier");
    Sets("box1_min3", "0");
    Sets("box1_max3", "2*pi");

    Sets("box2_Coordinates", "AnsorgNS2");
    Sets("box2_basis1", "ChebExtrema");
    Sets("box2_min1", "0");
    Sets("box2_max1", "1");
    Sets("box2_basis2", "ChebExtrema");
    Sets("box2_min2", "0");
    Sets("box2_max2", "1");
    Sets("box2_basis3", "Fourier");
    Sets("box2_min3", "0");
    Sets("box2_max3", "2*pi");
  
    Sets("box3_Coordinates", "AnsorgNS3");
    Sets("box3_basis1", "ChebExtrema");
    Sets("box3_min1", "0");
    Sets("box3_basis2", "ChebExtrema");
    Sets("box3_min2", "0");
    Sets("box3_max2", "1");
    Sets("box3_basis3", "Fourier");
    Sets("box3_min3", "0");
    Sets("box3_max3", "2*pi");

    Sets("box4_Coordinates", "Cartesian");
    Sets("box4_basis1", "ChebExtrema");
    Sets("box4_basis2", "ChebExtrema");
    Sets("box4_basis3", "ChebExtrema");

    Sets("box5_Coordinates", "Cartesian");
    Sets("box5_basis1", "ChebExtrema");
    Sets("box5_basis2", "ChebExtrema");
    Sets("box5_basis3", "ChebExtrema");
  }

  /* find P_core1, s.t. rest mass is m01 */
  vec[1] = 1e-10;   /* initial guess */
  /* do newton_lnsrch iterations: */
  newton_lnsrch(vec, 1, &check, m01_VectorFunc, 
 		Geti("Coordinates_newtMAXITS"),
    		Getd("Coordinates_newtTOLF") );
  if(check) printf(": check=%d\n", check);  
  P_core1 = vec[1];
  printf(" setting: P_core1=%g\n", P_core1);

  /* find P_core2, s.t. rest mass is m02 */
  vec[1] = 1e-10;   /* initial guess */
  /* do newton_lnsrch iterations: */
  newton_lnsrch(vec, 1, &check, m02_VectorFunc, 
 		Geti("Coordinates_newtMAXITS"),
    		Getd("Coordinates_newtTOLF") );
  if(check) printf(": check=%d\n", check);  
  P_core2 = vec[1];
  printf(" setting: P_core2=%g\n", P_core2);

  /* TOV_init yields m01 for a given P_core1 */
  TOV_init(P_core1, kappa, Gamma, 1, &rf_surf1, &m1, &Phic1, &Psic1, &m01);
  printf(" rf_surf1=%g: m1=%g Phic1=%g Psic1=%g m01=%g\n",
         rf_surf1, m1, Phic1, Psic1, m01);
{
double m,P,Phi,Psi,m0;
double rf=rf_surf1;
TOV_m_P_Phi_Psi_m0_OF_rf(rf, rf_surf1, kappa, Gamma,
                      P_core1, Phic1, Psic1,
                      &m, &P, &Phi, &Psi, &m0);
printf(" check rf=%g: r=%g m=%g P=%g Phi=%g Psi=%g m0=%g\n", 
rf,rf*Psi*Psi,m,P,Phi,Psi,m0);
}
  /* TOV_init yields m02 for a given P_core2 */
  TOV_init(P_core2, kappa, Gamma, 1, &rf_surf2, &m2, &Phic2, &Psic2, &m02);
  printf(" rf_surf2=%g: m2=%g Phic2=%g Psic2=%g m02=%g\n",
         rf_surf2, m2, Phic2, Psic2, m02);

  /* find sigma1, s.t. radius is rf_surf1 */
  vec[1] = 1.0/rf_surf1;   /* initial guess */
  /* do newton_lnsrch iterations: */
  newton_lnsrch(vec, 1, &check, rf_surf1_VectorFunc, 
 		Geti("Coordinates_newtMAXITS"),
    		Getd("Coordinates_newtTOLF") );
  if(check) printf(": check=%d\n", check);  
  sigma1 = vec[1];
  printf(" setting: sigma1=%g\n", sigma1);

  /* find sigma2, s.t. radius is rf_surf2 */
  vec[1] = -1.0/rf_surf2;   /* initial guess */
  /* do newton_lnsrch iterations: */
  newton_lnsrch(vec, 1, &check, rf_surf2_VectorFunc, 
 		Geti("Coordinates_newtMAXITS"),
    		Getd("Coordinates_newtTOLF") );
  if(check) printf(": check=%d\n", check);  
  sigma2 = vec[1];
  printf(" setting: sigma2=%g\n", sigma2);

  printf(" => radius of domain0 = %g,   radius of domain3 = %g\n", 
         0.5*(x_of_AnsorgNS0(NULL, -1, 0.0,0.0,0.0)-
              x_of_AnsorgNS0(NULL, -1, 0.0,1.0,0.0)),
         0.5*(x_of_AnsorgNS3(NULL, -1, 0.0,1.0,0.0)-
              x_of_AnsorgNS3(NULL, -1, 0.0,0.0,0.0)) );

  /* set max A inside stars and adjust boxes4/5 accordingly */
  if(Getv("BNSdata_grid", "4ABphi_2xyz"))
  {
    double box0_max1 = 0.85;
    double box3_max1 = 0.85;
    double scal = 1.05; //1.551108723489246  /* make box4/5 5% larger than needed */
    double xr, xp, xm, xmax, xmin;
    double b = Coordinates_AnsorgNS_b;

    Setd("box0_max1", box0_max1);
    xmin = x_of_AnsorgNS0(NULL, -1, box0_max1,1.0,0.0);
    xmax = x_of_AnsorgNS0(NULL, -1, box0_max1,0.0,0.0);
    xr = scal * 0.5*(xmax-xmin);
    xm = scal * (xmin-b);
    xp = scal * (xmax-b);
    Setd("box5_min1", b + xm);
    Setd("box5_max1", b + xp);
    Setd("box5_min2", -xr);
    Setd("box5_max2",  xr);
    Setd("box5_min3", -xr);
    Setd("box5_max3",  xr);

    Setd("box3_max1", box3_max1);
    xmin = x_of_AnsorgNS3(NULL, -1, box3_max1,0.0,0.0);
    xmax = x_of_AnsorgNS3(NULL, -1, box3_max1,1.0,0.0);
    xr = scal * 0.5*(xmax-xmin);
    xm = scal * (xmin+b);
    xp = scal * (xmax+b);
    Setd("box4_min1", -b + xm);
    Setd("box4_max1", -b + xp);
    Setd("box4_min2", -xr);
    Setd("box4_max2",  xr);
    Setd("box4_min3", -xr);
    Setd("box4_max3",  xr);
  }

  return 0;
}

/* funtion to be passed into newton_lnsrch by ??? */
void rf_surf1_VectorFunc(int n, double *vec, double *fvec)
{
  double xmax, xmin;
  
  sigma1 = vec[1];
  xmax = x_of_AnsorgNS0(NULL, -1, 0.0,0.0,0.0);
  xmin = x_of_AnsorgNS0(NULL, -1, 0.0,1.0,0.0);
  fvec[1] = (xmax-xmin)-2.0*rf_surf1;
}

/* funtion to be passed into newton_lnsrch by ??? */
void rf_surf2_VectorFunc(int n, double *vec, double *fvec)
{
  double xmax, xmin;

  sigma2 = vec[1];
  xmin = x_of_AnsorgNS3(NULL, -1, 0.0,0.0,0.0);
  xmax = x_of_AnsorgNS3(NULL, -1, 0.0,1.0,0.0);
  fvec[1] = (xmax-xmin)-2.0*rf_surf2;
}


/* setup initial Coordinates_AnsorgNS_sigma_pm vars */
int set_sigma_pm_vars(tGrid *grid)
{
  int sigma_pm, dsigma_pm_dB, dsigma_pm_dphi;
  int boxindex;

  printf("set_sigma_pm_vars: setting Coordinates_AnsorgNS_sigma_pm ...\n");

  /* make sure that Coordinates uses Coordinates_AnsorgNS_sigma_pm */
  if(!Getv("Coordinates_AnsorgNS_sigma_pm_vars", "yes"))
    errorexit("you need: Coordinates_AnsorgNS_sigma_pm_vars = yes");

  sigma_pm = Ind("Coordinates_AnsorgNS_sigma_pm");  
  dsigma_pm_dB = Ind("Coordinates_AnsorgNS_dsigma_pm_dB");
  dsigma_pm_dphi = Ind("Coordinates_AnsorgNS_dsigma_pm_dphi");
  enablevar(grid, Ind("Temp1")); /* for coeffs in interpolation */

  /* set Coordinates_AnsorgNS_sigma_pm */
  for(boxindex=0; boxindex <=3; boxindex++)
  {
    tBox *box = grid->box[boxindex];
    int ind;

    enablevar_inbox(box, sigma_pm); 
    enablevar_inbox(box, dsigma_pm_dB);
    enablevar_inbox(box, dsigma_pm_dphi);
    forallpoints(box, ind)
    {
      if(boxindex<2) box->v[sigma_pm][ind] = sigma1;
      else           box->v[sigma_pm][ind] = sigma2;
    }
  }
  return 0;
}

/* funtion to be passed into newton_lnsrch to find P_core1 from m01 */
void m01_VectorFunc(int n, double *vec, double *fvec)
{
  double kappa     = Getd("BNSdata_kappa");
  double BNSdata_n = Getd("BNSdata_n");
  double m01       = Getd("BNSdata_m01");
  double Gamma     = 1.0 + 1.0/BNSdata_n;
  double Pc, m, Phic, Psic, m0;

  Pc = vec[1];
  TOV_init(Pc, kappa, Gamma, 0, &rf_surf1, &m, &Phic, &Psic, &m0);
  fvec[1] = m0-m01;
}
/* funtion to be passed into newton_lnsrch to find P_core2 from m02 */
void m02_VectorFunc(int n, double *vec, double *fvec)
{
  double kappa     = Getd("BNSdata_kappa");
  double BNSdata_n = Getd("BNSdata_n");
  double m02       = Getd("BNSdata_m02");
  double Gamma     = 1.0 + 1.0/BNSdata_n;
  double Pc, m, Phic, Psic, m0;

  Pc = vec[1];
  TOV_init(Pc, kappa, Gamma, 0, &rf_surf2, &m, &Phic, &Psic, &m0);
  fvec[1] = m0-m02;
}


/*************************************/
/* functions to adjust star surfaces */
/*************************************/

/* get q at A by interpolation */
void BNdata_q_VectorFunc(int n, double *vec, double *fvec)
{
  tBox *box = BNdata_q_VectorFunc_box;
  double *c = BNdata_q_VectorFunc_coeffs;
  int i;
  double B1;
  double a1=box->bbox[0];
  double b1=box->bbox[1];
  int n1 = box->n1;
  double q = 0.0;
  double A = vec[1];  

  /* interpolate q onto point A */
  for(i=n1-1; i>=0; i--)
  {
    B1 = box->basis1((void *) box, a1,b1, i,n1, A);
    q += c[i] * B1;
  }   
  fvec[1] = q;
}

/* get q at A by interpolation */
void BNdata_sigp_VectorFunc(int n, double *vec, double *fvec)
{
  double sigp_Bphi = vec[1];
  double sigp_1phi = BNdata_sigp_VectorFunc_sigp_1phi;
  double B         = BNdata_sigp_VectorFunc_B;
  double X0        = BNdata_sigp_VectorFunc_X0;
  double AbsCp_Bphi = sqrt( Abstanh(0.25*sigp_Bphi, 0.25*PI*B) );
  double ArgCp_Bphi = 0.5 * Argtanh(0.25*sigp_Bphi, 0.25*PI*B);
  double ReCp_Bphi = AbsCp_Bphi * cos(ArgCp_Bphi);
  /* double ImCp_Bphi = AbsCp_Bphi * sin(ArgCp_Bphi); */
  double AbsCp_1phi = sqrt( Abstanh(0.25*sigp_1phi, 0.25*PI) );
  double ArgCp_1phi = 0.5 * Argtanh(0.25*sigp_1phi, 0.25*PI);
  double ReCp_1phi = AbsCp_1phi * cos(ArgCp_1phi);
  /* double ImCp_1phi = AbsCp_1phi * sin(ArgCp_1phi); */
  double X,R;

  /* use Eq. (22), (23) or (24) at A=0 */
  X = ReCp_Bphi - B*ReCp_1phi + B*cos(ArgCp_1phi);
  /* R = ImCp_Bphi - B*ImCp_1phi + B*sin(ArgCp_1phi); */

  fvec[1] = (X-X0);
}

// THIS DOES NOT WORK IF 0<B<1 :
/* reset sigma such that the zeros in BNSdata_q are at A=0 */
void reset_Coordinates_AnsorgNS_sigma_pm(tGrid *grid, tGrid *gridnew,
                                         int innerdom,  int outerdom)
{
  int iq = Ind("BNSdata_q");
  int ic = Ind("Temp1"); /* we store coeffs of BNSdata_q in Temp1 */
  int iX = Ind("X");
  int iY = Ind("Y");
  int iZ = Ind("Z");
  int isigma      = Ind("Coordinates_AnsorgNS_sigma_pm");
  int isigma_dB   = Ind("Coordinates_AnsorgNS_dsigma_pm_dB");
  int isigma_dphi = Ind("Coordinates_AnsorgNS_dsigma_pm_dphi");
  double *q_in = grid->box[innerdom]->v[iq];
  double *q_out= grid->box[outerdom]->v[iq];  
  double *c_in = grid->box[innerdom]->v[ic];
  double *c_out= grid->box[outerdom]->v[ic];  
  int n1 = grid->box[innerdom]->n1;
  int n2 = grid->box[innerdom]->n2;
  int n3 = grid->box[innerdom]->n3;
  int i,j,k;
  int ineg_in;  /* q_in<0  at i=ineg_in (and q_in>=0 i=ineg_in+1) */
  int ipos_out; /* q_out>0 at i=ipos_out (and q_out<0 i=ipos_out+1) */
  int i1, i2, dom; /* zero occurs between index i1 and i2 in domain dom */
  double A1, A2;   /* zero occurs between A=A1 and A=A2 in domain dom */
  double A0;      /* q=0 at A=A0 in domain dom */
  double q1, q2;
  double X0, R0;  /* value of X,R at A=A0 */
  double B,phi, x,y,z;
  double ArgCp_1phi, AbsCp_1phi, ReCp_1phi, ImCp_1phi;
  double ArgCp_Bphi, ReCp_Bphi, ImCp_Bphi;
  double sigp_1phi, sigp_Bphi;
  int itmax = Geti("Coordinates_newtMAXITS");
  double tol = Getd("Coordinates_newtTOLF");
  double vec[2];
  int check;

  /* set coeffs of BNSdata_q */
  spec_analysis1(grid->box[innerdom], 1, grid->box[innerdom]->Mcoeffs1, 
                 q_in, c_in);
  spec_analysis1(grid->box[outerdom], 1, grid->box[outerdom]->Mcoeffs1, 
                 q_out, c_out);

  /* loop over j,k i.e. B,phi. 
     NOTE: we assue that n1,n2,n3 are the same in both domains */
  for(j=n2-1; j>=0; j--)
  {
    for(k=0; k<n3; k++)
    {
      /* find indices where q_in and q_out switch sign */
      for(i=n1-1; i>=0; i--) if(q_in[Index(i,j,k)]<0.0) break;
      ineg_in=i;
      for(i=n1-1; i>=0; i--) if(q_out[Index(i,j,k)]>0.0) break;
      ipos_out=i;

      /* if ipos_out=>0, q has zero in outer domain */
      if(ipos_out>=0) { i1=ipos_out; i2=ipos_out+1; dom=outerdom; }
      /* if ineg_in=>0, q has zero in inner domain */
      else if(ineg_in>=0) { i1=ineg_in; i2=ineg_in+1; dom=innerdom; }
           else           { i1=0;       i2=1;         dom=innerdom; }
      A1 = grid->box[dom]->v[iX][i1];
      A2 = grid->box[dom]->v[iX][i2];
      q1 = grid->box[dom]->v[iq][i1];
      q2 = grid->box[dom]->v[iq][i2];
      B   = grid->box[dom]->v[iY][Index(i1,j,k)];
      phi = grid->box[dom]->v[iZ][Index(i1,j,k)];

      /* find zero in q between A1 and A2 */
      if( fabs(q1) < tol || (ineg_in<0 && dom==innerdom) ) A0=A1;
      else if( fabs(q2) < tol ) A0=A2;
      else /* use root finder */
      {
        A0 = A1 - q1*(A2-A1)/(q2-q1); /* initial guess */
        /* use newton_lnsrch to find A0 */
        BNdata_q_VectorFunc_box    = grid->box[dom];
        BNdata_q_VectorFunc_coeffs = grid->box[dom]->v[ic]+Index(0,j,k);
        vec[1] = A0;
        newton_lnsrch(vec, 1, &check, BNdata_q_VectorFunc, itmax, tol);
        if(check)
          printf("reset_Coordinates_AnsorgNS_sigma_pm: check=%d\n", check);  
        A0 = vec[1];
        if(A0<A1 || A0>A2) errorexit("reset_Coordinates_AnsorgNS_sigma_pm: "
                                     "newton_lnsrch failed!");
      }
printf("\nA0,B,phi=%g,%g,%g  dom=%d A1=%g A2=%g q1=%g q2=%g\n", A0,B,phi, dom, A1,A2, q1,q2);

      /* compute values of X0,R0 at A=A0 */
      xyz_of_AnsorgNS(grid->box[dom], -1, dom, A0,B,phi, &x,&y,&z, &X0,&R0);
printf("old X0=%g R0=%g\n", X0,R0);      
      /* get Cp and sigp at B=1  */
      if(j==n2-1 && k==0) /* B=1 case, but compute it only for phi=0 */
      {
        ArgCp_1phi = acos(X0);
        /* 2 ArgCp = ArcTan[Sin[Pi B/2]/Sinh[sigma/2]] */
        /* Tan[2 ArgCp] = Sin[Pi B/2]/Sinh[sigma/2] */
        sigp_1phi = 2.0 * asinh( (1.0/tan(2.0*ArgCp_1phi)) );
        AbsCp_1phi = sqrt( Abstanh(0.25*sigp_1phi, 0.25*PI) );
        ReCp_1phi = AbsCp_1phi * cos(ArgCp_1phi);
        ImCp_1phi = AbsCp_1phi * sin(ArgCp_1phi);
        sigp_Bphi = sigp_1phi;
printf("ArgCp_1phi=%g ReCp_1phi=%g ImCp_1phi=%g\n",ArgCp_1phi, ReCp_1phi,ImCp_1phi);
      }
      if( (j==0 && k==0) || j>0 && j<n2-1 ) /* B<1 case */
      {                              /* if B=0 compute only for phi=0 */
        ReCp_Bphi = X0 + B*(ReCp_1phi - cos(ArgCp_1phi));
        ImCp_Bphi = R0 + B*(ImCp_1phi - sin(ArgCp_1phi));
        ArgCp_Bphi = Arg(ReCp_Bphi, ImCp_Bphi);
        if(B==0) /* Cp_Bphi^2 = tanh(0.25*sigp_Bphi) */
          sigp_Bphi = 4.0 * atanh(ReCp_Bphi*ReCp_Bphi-ImCp_Bphi*ImCp_Bphi);
        else
        {
          sigp_Bphi = 2.0 * asinh( sin(PIh*B)/tan(2.0*ArgCp_Bphi) );
          //or
          /* Note: |tanh(x+iy)|^2 = K  =>  2x =acosh( cos(2y)*(1+K)/(1-K) ) */
          //double AbsCpSqr_Bphi=ReCp_Bphi*ReCp_Bphi+ImCp_Bphi*ImCp_Bphi;
          //double K=AbsCpSqr_Bphi*AbsCpSqr_Bphi;
          //sigp_Bphi = 2.0 * acosh( cos(PIh*B)*(1.0+K)/(1.0-K) );
          //sigp_Bphi = sigp_Bphi*0.5 + acosh( cos(PIh*B)*(1.0+K)/(1.0-K) ) ;
        }
        /* use newton_lnsrch to find better sigp_Bphi */
        BNdata_sigp_VectorFunc_sigp_1phi = sigp_1phi;
        BNdata_sigp_VectorFunc_B         = B;
        BNdata_sigp_VectorFunc_X0        = X0;
        vec[1] = sigp_Bphi;
        newton_lnsrch(vec, 1, &check, BNdata_sigp_VectorFunc, itmax, tol);
        if(check)
          printf("reset_Coordinates_AnsorgNS_sigma_pm: check=%d\n", check);  
        sigp_Bphi = vec[1];
printf("ReCp_Bphi*ReCp_Bphi+ImCp_Bphi*ImCp_Bphi=%g ", 
sqrt(ReCp_Bphi*ReCp_Bphi+ImCp_Bphi*ImCp_Bphi));
printf("ArgCp_Bphi=%g ReCp_Bphi=%g ImCp_Bphi=%g\n",ArgCp_Bphi, ReCp_Bphi,ImCp_Bphi);
{
double AbsCp_Bphi = sqrt( Abstanh(0.25*sigp_Bphi, 0.25*PI*B) );
double ArgCp_Bphi = 0.5 * Argtanh(0.25*sigp_Bphi, 0.25*PI*B);
double ReCp_Bphi = AbsCp_Bphi * cos(ArgCp_Bphi);
double ImCp_Bphi = AbsCp_Bphi * sin(ArgCp_Bphi);

double AbsCp_1phi = sqrt( Abstanh(0.25*sigp_1phi, 0.25*PI) );
double ArgCp_1phi = 0.5 * Argtanh(0.25*sigp_1phi, 0.25*PI);
double ReCp_1phi = AbsCp_1phi * cos(ArgCp_1phi);
double ImCp_1phi = AbsCp_1phi * sin(ArgCp_1phi);
//double Ap=0;
double X = (ReCp_Bphi - B*ReCp_1phi) + B*cos(ArgCp_1phi);
double R = (ImCp_Bphi - B*ImCp_1phi) + B*sin(ArgCp_1phi);
printf("new X=%g R=%g  ArgCp_Bphi=%g AbsCp_Bphi=%g ReCp_Bphi=%g ImCp_Bphi=%g\n", 
X,R, ArgCp_Bphi, AbsCp_Bphi, ReCp_Bphi,ImCp_Bphi); 
}
      }
printf("sigp_Bphi=%g sigp_1phi=%g\n", sigp_Bphi, sigp_1phi);

      /* set Coordinates_AnsorgNS_sigma_pm = sigp_Bphi in both domains */
      for(i=0; i<n1; i++)
      {
        gridnew->box[innerdom]->v[isigma][Index(i,j,k)] = sigp_Bphi;
        gridnew->box[outerdom]->v[isigma][Index(i,j,k)] = sigp_Bphi;
      }
    } /* end for k */
  } /* end for j */
  
  /* compute derivs of sigma */
  spec_Deriv1(gridnew->box[innerdom], 2, gridnew->box[innerdom]->v[isigma],
              gridnew->box[innerdom]->v[isigma_dB]);
  spec_Deriv1(gridnew->box[outerdom], 3, gridnew->box[outerdom]->v[isigma],
              gridnew->box[outerdom]->v[isigma_dphi]);
}

// WE NEED FO FIND sigp_Bphi at B,phi such that q(sigp_Bphi; A=0, B, phi)=0
/*
q_of_sigp_Bphi_forgiven_Bphi(int n, double *vec, double *fvec)
{
  spec_interpolate(tBox *box, double *c, double X, double Y, double Z);
}
*/
