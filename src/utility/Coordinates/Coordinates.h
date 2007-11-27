/* Coordinates.h */
/* Wolfgang Tichy 4/2005 */

/* use we can use long double in some places */
#define LDOUBLE long double


/* from cartesianDerivs.c */
double dX_dx(int i, int m, int dXd,
             tBox *box, int ind, double X, double Y, double Z);
double ddX_dxdx(int i, int m, int n, int ddXdd,
                tBox *box, int ind, double X, double Y, double Z);

/* from doubleCovering.c */
void reset_doubleCoveredPoints_SphericalDF(tBox *box, tVarList *unew);

/* from coordFilters.c */
void coordinateDependentFilter_SphericalDF(tBox *box, tVarList *unew);
void coordinateDependentFilter_Spherical(tBox *box, tVarList *unew);

/* from transfDerivs.c */
void dXdx_from_dxdX(double dXdx[4][4], double dxdX[4][4]);
void ddXdxdx_from_dXdx_ddxdXdX(double ddXdxdx[4][4][4],
                               double dXdx[4][4], double ddxdXdX[4][4][4]);

/* Some trivial functions */
double zero_of_xyz(void *aux, int ind, double X, double Y, double Z);
double one_of_xyz(void *aux, int ind, double X, double Y, double Z);
double x_equals_X(void *aux, int ind, double X, double Y, double Z);
double y_equals_Y(void *aux, int ind, double X, double Y, double Z);
double z_equals_Z(void *aux, int ind, double X, double Y, double Z);

/* Polar coordinates: */
double x_ofPolar(void *aux, int ind, double rho, double phi, double Z);
double y_ofPolar(void *aux, int ind, double rho, double phi, double Z);
double z_ofPolar(void *aux, int ind, double rho, double phi, double Z);
double drho_dx(void *aux, int ind, double rho, double phi, double Z);
double drho_dy(void *aux, int ind, double rho, double phi, double Z);
double dphi_dx(void *aux, int ind, double rho, double phi, double Z);
double dphi_dy(void *aux, int ind, double rho, double phi, double Z);
void set_d_dx_at_rhoEQzero(void *bo, void *va, void *v1,void *v2,void *v3);
void set_d_dy_at_rhoEQzero(void *bo, void *va, void *v1,void *v2,void *v3);
/*
double drho_dxdx(void *aux, int ind, double rho, double phi, double Z);
double drho_dxdy(void *aux, int ind, double rho, double phi, double Z);
double drho_dydy(void *aux, int ind, double rho, double phi, double Z);
double dphi_dxdx(void *aux, int ind, double rho, double phi, double Z);
double dphi_dxdy(void *aux, int ind, double rho, double phi, double Z);
double dphi_dydy(void *aux, int ind, double rho, double phi, double Z);
*/

/* PolarCE coordinates: */
double x_ofPolarCE(void *aux, int ind, double rho, double Y, double Z);
double y_ofPolarCE(void *aux, int ind, double rho, double Y, double Z);
double dYPolarCE_dx(void *aux, int ind, double rho, double Y, double Z);
double dYPolarCE_dy(void *aux, int ind, double rho, double Y, double Z);

/* SphericalDF coordinates: */
double x_ofSphericalDF(void *aux, int ind, double r, double thm, double phi);
double y_ofSphericalDF(void *aux, int ind, double r, double thm, double phi);
double z_ofSphericalDF(void *aux, int ind, double r, double thm, double phi);
double dr_dx(void *aux, int ind, double r, double thm, double phi);
double dr_dy(void *aux, int ind, double r, double thm, double phi);
double dr_dz(void *aux, int ind, double r, double thm, double phi);
double dphiSphericalDF_dx(void *aux, int ind, double r, double thm, double phi);
double dphiSphericalDF_dy(void *aux, int ind, double r, double thm, double phi);
double dthm_dx(void *aux, int ind, double r, double thm, double phi);
double dthm_dy(void *aux, int ind, double r, double thm, double phi);
double dthm_dz(void *aux, int ind, double r, double thm, double phi);
double ddr_SphericalDF_dxdx(void *aux, int ind, double r, double thm, double phi);
double ddr_SphericalDF_dxdy(void *aux, int ind, double r, double thm, double phi);
double ddr_SphericalDF_dxdz(void *aux, int ind, double r, double thm, double phi);
double ddr_SphericalDF_dydy(void *aux, int ind, double r, double thm, double phi);
double ddr_SphericalDF_dydz(void *aux, int ind, double r, double thm, double phi);
double ddr_SphericalDF_dzdz(void *aux, int ind, double r, double thm, double phi);
double ddthm_SphericalDF_dxdx(void *aux, int ind, double r, double thm, double phi);
double ddthm_SphericalDF_dxdy(void *aux, int ind, double r, double thm, double phi);
double ddthm_SphericalDF_dxdz(void *aux, int ind, double r, double thm, double phi);
double ddthm_SphericalDF_dydy(void *aux, int ind, double r, double thm, double phi);
double ddthm_SphericalDF_dydz(void *aux, int ind, double r, double thm, double phi);
double ddthm_SphericalDF_dzdz(void *aux, int ind, double r, double thm, double phi);
double ddphi_SphericalDF_dxdx(void *aux, int ind, double r, double thm, double phi);
double ddphi_SphericalDF_dxdy(void *aux, int ind, double r, double thm, double phi);
double ddphi_SphericalDF_dxdz(void *aux, int ind, double r, double thm, double phi);
double ddphi_SphericalDF_dydy(void *aux, int ind, double r, double thm, double phi);
double ddphi_SphericalDF_dydz(void *aux, int ind, double r, double thm, double phi);
double ddphi_SphericalDF_dzdz(void *aux, int ind, double r, double thm, double phi);

/* compactSphericalDF coordinates: */
double r_of_xi(double xi);
double dxi_dr(double xi);
double x_ofcompactSphericalDF(void *aux, int ind, double xi, double thm, double phi);
double y_ofcompactSphericalDF(void *aux, int ind, double xi, double thm, double phi);
double z_ofcompactSphericalDF(void *aux, int ind, double xi, double thm, double phi);
double dxi_dx(void *aux, int ind, double xi, double thm, double phi);
double dxi_dy(void *aux, int ind, double xi, double thm, double phi);
double dxi_dz(void *aux, int ind, double xi, double thm, double phi);
double dthmcompactSphericalDF_dx(void *aux, int ind, double xi, double thm, double phi);
double dthmcompactSphericalDF_dy(void *aux, int ind, double xi, double thm, double phi);
double dthmcompactSphericalDF_dz(void *aux, int ind, double xi, double thm, double phi);
double dphicompactSphericalDF_dx(void *aux, int ind, double xi, double thm, double phi);
double dphicompactSphericalDF_dy(void *aux, int ind, double xi, double thm, double phi);
double ddxi_compactSphericalDF_dxdx(void *aux, int ind, double xi, double thm, double phi);
double ddxi_compactSphericalDF_dxdy(void *aux, int ind, double xi, double thm, double phi);
double ddxi_compactSphericalDF_dxdz(void *aux, int ind, double xi, double thm, double phi);
double ddxi_compactSphericalDF_dydy(void *aux, int ind, double xi, double thm, double phi);
double ddxi_compactSphericalDF_dydz(void *aux, int ind, double xi, double thm, double phi);
double ddxi_compactSphericalDF_dzdz(void *aux, int ind, double xi, double thm, double phi);
double ddthm_compactSphericalDF_dxdx(void *aux, int ind, double xi, double thm, double phi);
double ddthm_compactSphericalDF_dxdy(void *aux, int ind, double xi, double thm, double phi);
double ddthm_compactSphericalDF_dxdz(void *aux, int ind, double xi, double thm, double phi);
double ddthm_compactSphericalDF_dydy(void *aux, int ind, double xi, double thm, double phi);
double ddthm_compactSphericalDF_dydz(void *aux, int ind, double xi, double thm, double phi);
double ddthm_compactSphericalDF_dzdz(void *aux, int ind, double xi, double thm, double phi);
double ddphi_compactSphericalDF_dxdx(void *aux, int ind, double xi, double thm, double phi);
double ddphi_compactSphericalDF_dxdy(void *aux, int ind, double xi, double thm, double phi);
double ddphi_compactSphericalDF_dxdz(void *aux, int ind, double xi, double thm, double phi);
double ddphi_compactSphericalDF_dydy(void *aux, int ind, double xi, double thm, double phi);
double ddphi_compactSphericalDF_dydz(void *aux, int ind, double xi, double thm, double phi);
double ddphi_compactSphericalDF_dzdz(void *aux, int ind, double xi, double thm, double phi);

/* Spherical coordinates */
double x_ofSpherical(void *aux, int ind, double r, double theta, double phi);
double y_ofSpherical(void *aux, int ind, double r, double theta, double phi);
double z_ofSpherical(void *aux, int ind, double r, double theta, double phi);
double drSpherical_dx(void *aux, int ind, double r, double theta, double phi);
double drSpherical_dy(void *aux, int ind, double r, double theta, double phi);
double drSpherical_dz(void *aux, int ind, double r, double theta, double phi);
double dthetaSpherical_dx(void *aux, int ind, double r, double theta, double phi);
double dthetaSpherical_dy(void *aux, int ind, double r, double theta, double phi);
double dthetaSpherical_dz(void *aux, int ind, double r, double theta, double phi);
double dphiSpherical_dx(void *aux, int ind, double r, double theta, double phi);
double dphiSpherical_dy(void *aux, int ind, double r, double theta, double phi);
double ddr_Spherical_dxdx(void *aux, int ind, double r, double theta, double phi);
double ddr_Spherical_dxdy(void *aux, int ind, double r, double theta, double phi);
double ddr_Spherical_dxdz(void *aux, int ind, double r, double theta, double phi);
double ddr_Spherical_dydy(void *aux, int ind, double r, double theta, double phi);
double ddr_Spherical_dydz(void *aux, int ind, double r, double theta, double phi);
double ddr_Spherical_dzdz(void *aux, int ind, double r, double theta, double phi);
double ddtheta_Spherical_dxdx(void *aux, int ind, double r, double theta, double phi);
double ddtheta_Spherical_dxdy(void *aux, int ind, double r, double theta, double phi);
double ddtheta_Spherical_dxdz(void *aux, int ind, double r, double theta, double phi);
double ddtheta_Spherical_dydy(void *aux, int ind, double r, double theta, double phi);
double ddtheta_Spherical_dydz(void *aux, int ind, double r, double theta, double phi);
double ddtheta_Spherical_dzdz(void *aux, int ind, double r, double theta, double phi);
double ddphi_Spherical_dxdx(void *aux, int ind, double r, double theta, double phi);
double ddphi_Spherical_dxdy(void *aux, int ind, double r, double theta, double phi);
double ddphi_Spherical_dxdz(void *aux, int ind, double r, double theta, double phi);
double ddphi_Spherical_dydy(void *aux, int ind, double r, double theta, double phi);
double ddphi_Spherical_dydz(void *aux, int ind, double r, double theta, double phi);
double ddphi_Spherical_dzdz(void *aux, int ind, double r, double theta, double phi);

/* Spherical2 coordinates */
double x_ofSpherical2(void *aux, int ind, double r, double U, double phi);
double y_ofSpherical2(void *aux, int ind, double r, double U, double phi);
double z_ofSpherical2(void *aux, int ind, double r, double U, double phi);
double dr_Spherical2_dx(void *aux, int ind, double r, double U, double phi);
double dr_Spherical2_dy(void *aux, int ind, double r, double U, double phi);
double dr_Spherical2_dz(void *aux, int ind, double r, double U, double phi);
double dU_Spherical2_dx(void *aux, int ind, double r, double U, double phi);
double dU_Spherical2_dy(void *aux, int ind, double r, double U, double phi);
double dU_Spherical2_dz(void *aux, int ind, double r, double U, double phi);
double dphi_Spherical2_dx(void *aux, int ind, double r, double U, double phi);
double dphi_Spherical2_dy(void *aux, int ind, double r, double U, double phi);
double ddr_Spherical2_dxdx(void *aux, int ind, double r, double U, double phi);
double ddr_Spherical2_dxdy(void *aux, int ind, double r, double U, double phi);
double ddr_Spherical2_dxdz(void *aux, int ind, double r, double U, double phi);
double ddr_Spherical2_dydy(void *aux, int ind, double r, double U, double phi);
double ddr_Spherical2_dydz(void *aux, int ind, double r, double U, double phi);
double ddr_Spherical2_dzdz(void *aux, int ind, double r, double U, double phi);
double ddU_Spherical2_dxdx(void *aux, int ind, double r, double U, double phi);
double ddU_Spherical2_dxdy(void *aux, int ind, double r, double U, double phi);
double ddU_Spherical2_dxdz(void *aux, int ind, double r, double U, double phi);
double ddU_Spherical2_dydy(void *aux, int ind, double r, double U, double phi);
double ddU_Spherical2_dydz(void *aux, int ind, double r, double U, double phi);
double ddU_Spherical2_dzdz(void *aux, int ind, double r, double U, double phi);
double ddphi_Spherical2_dxdx(void *aux, int ind, double r, double U, double phi);
double ddphi_Spherical2_dxdy(void *aux, int ind, double r, double U, double phi);
double ddphi_Spherical2_dxdz(void *aux, int ind, double r, double U, double phi);
double ddphi_Spherical2_dydy(void *aux, int ind, double r, double U, double phi);
double ddphi_Spherical2_dydz(void *aux, int ind, double r, double U, double phi);
double ddphi_Spherical2_dzdz(void *aux, int ind, double r, double U, double phi);

/* Spherical3 coordinates */
double x_ofSpherical3(void *aux, int ind, double r, double U, double phi);
double y_ofSpherical3(void *aux, int ind, double r, double U, double phi);
double z_ofSpherical3(void *aux, int ind, double r, double U, double phi);
double dr_Spherical3_dx(void *aux, int ind, double r, double U, double phi);
double dr_Spherical3_dy(void *aux, int ind, double r, double U, double phi);
double dr_Spherical3_dz(void *aux, int ind, double r, double U, double phi);
double dU_Spherical3_dx(void *aux, int ind, double r, double U, double phi);
double dU_Spherical3_dy(void *aux, int ind, double r, double U, double phi);
double dU_Spherical3_dz(void *aux, int ind, double r, double U, double phi);
double dphi_Spherical3_dx(void *aux, int ind, double r, double U, double phi);
double dphi_Spherical3_dy(void *aux, int ind, double r, double U, double phi);
double ddr_Spherical3_dxdx(void *aux, int ind, double r, double U, double phi);
double ddr_Spherical3_dxdy(void *aux, int ind, double r, double U, double phi);
double ddr_Spherical3_dxdz(void *aux, int ind, double r, double U, double phi);
double ddr_Spherical3_dydy(void *aux, int ind, double r, double U, double phi);
double ddr_Spherical3_dydz(void *aux, int ind, double r, double U, double phi);
double ddr_Spherical3_dzdz(void *aux, int ind, double r, double U, double phi);
double ddU_Spherical3_dxdx(void *aux, int ind, double r, double U, double phi);
double ddU_Spherical3_dxdy(void *aux, int ind, double r, double U, double phi);
double ddU_Spherical3_dxdz(void *aux, int ind, double r, double U, double phi);
double ddU_Spherical3_dydy(void *aux, int ind, double r, double U, double phi);
double ddU_Spherical3_dydz(void *aux, int ind, double r, double U, double phi);
double ddU_Spherical3_dzdz(void *aux, int ind, double r, double U, double phi);
double ddphi_Spherical3_dxdx(void *aux, int ind, double r, double U, double phi);
double ddphi_Spherical3_dxdy(void *aux, int ind, double r, double U, double phi);
double ddphi_Spherical3_dxdz(void *aux, int ind, double r, double U, double phi);
double ddphi_Spherical3_dydy(void *aux, int ind, double r, double U, double phi);
double ddphi_Spherical3_dydz(void *aux, int ind, double r, double U, double phi);
double ddphi_Spherical3_dzdz(void *aux, int ind, double r, double U, double phi);

/* tan_stretch coordinates: */
double x_of_xs(double xs);
double dxs_dx(double xs);
double ddxs_dxdx(double xs);
double x_of_tan_stretch(void *aux, int ind, double xs, double ys, double zs);
double y_of_tan_stretch(void *aux, int ind, double xs, double ys, double zs);
double z_of_tan_stretch(void *aux, int ind, double xs, double ys, double zs);
double dxs_dx_tan_stretch(void *aux, int ind, double xs, double ys, double zs);
double dys_dy_tan_stretch(void *aux, int ind, double xs, double ys, double zs);
double dzs_dz_tan_stretch(void *aux, int ind, double xs, double ys, double zs);
double ddxs_dxdx_tan_stretch(void *aux, int ind, double xs, double ys, double zs);
double ddys_dydy_tan_stretch(void *aux, int ind, double xs, double ys, double zs);
double ddzs_dzdz_tan_stretch(void *aux, int ind, double xs, double ys, double zs);

/* AnsorgNS coordinates:                                               */
/* 4 domains: 0=inside NS+, 1=outside NS+, 2=outside NS-, 3=inside NS- */
double x_of_AnsorgNS0(void *aux, int ind, double A, double B, double phi);
double y_of_AnsorgNS0(void *aux, int ind, double A, double B, double phi);
double z_of_AnsorgNS0(void *aux, int ind, double A, double B, double phi);
double dA_dx_AnsorgNS0(void *aux, int ind, double A, double B, double phi);
double dA_dy_AnsorgNS0(void *aux, int ind, double A, double B, double phi);
double dA_dz_AnsorgNS0(void *aux, int ind, double A, double B, double phi);
double dB_dx_AnsorgNS0(void *aux, int ind, double A, double B, double phi);
double dB_dy_AnsorgNS0(void *aux, int ind, double A, double B, double phi);
double dB_dz_AnsorgNS0(void *aux, int ind, double A, double B, double phi);
double dphi_dx_AnsorgNS0(void *aux, int ind, double A, double B, double phi);
double dphi_dy_AnsorgNS0(void *aux, int ind, double A, double B, double phi);
double dphi_dz_AnsorgNS0(void *aux, int ind, double A, double B, double phi);

double x_of_AnsorgNS1(void *aux, int ind, double A, double B, double phi);
double y_of_AnsorgNS1(void *aux, int ind, double A, double B, double phi);
double z_of_AnsorgNS1(void *aux, int ind, double A, double B, double phi);
double dA_dx_AnsorgNS1(void *aux, int ind, double A, double B, double phi);
double dA_dy_AnsorgNS1(void *aux, int ind, double A, double B, double phi);
double dA_dz_AnsorgNS1(void *aux, int ind, double A, double B, double phi);
double dB_dx_AnsorgNS1(void *aux, int ind, double A, double B, double phi);
double dB_dy_AnsorgNS1(void *aux, int ind, double A, double B, double phi);
double dB_dz_AnsorgNS1(void *aux, int ind, double A, double B, double phi);
double dphi_dx_AnsorgNS1(void *aux, int ind, double A, double B, double phi);
double dphi_dy_AnsorgNS1(void *aux, int ind, double A, double B, double phi);
double dphi_dz_AnsorgNS1(void *aux, int ind, double A, double B, double phi);

double x_of_AnsorgNS2(void *aux, int ind, double A, double B, double phi);
double y_of_AnsorgNS2(void *aux, int ind, double A, double B, double phi);
double z_of_AnsorgNS2(void *aux, int ind, double A, double B, double phi);
double dA_dx_AnsorgNS2(void *aux, int ind, double A, double B, double phi);
double dA_dy_AnsorgNS2(void *aux, int ind, double A, double B, double phi);
double dA_dz_AnsorgNS2(void *aux, int ind, double A, double B, double phi);
double dB_dx_AnsorgNS2(void *aux, int ind, double A, double B, double phi);
double dB_dy_AnsorgNS2(void *aux, int ind, double A, double B, double phi);
double dB_dz_AnsorgNS2(void *aux, int ind, double A, double B, double phi);
double dphi_dx_AnsorgNS2(void *aux, int ind, double A, double B, double phi);
double dphi_dy_AnsorgNS2(void *aux, int ind, double A, double B, double phi);
double dphi_dz_AnsorgNS2(void *aux, int ind, double A, double B, double phi);

double x_of_AnsorgNS3(void *aux, int ind, double A, double B, double phi);
double y_of_AnsorgNS3(void *aux, int ind, double A, double B, double phi);
double z_of_AnsorgNS3(void *aux, int ind, double A, double B, double phi);
double dA_dx_AnsorgNS3(void *aux, int ind, double A, double B, double phi);
double dA_dy_AnsorgNS3(void *aux, int ind, double A, double B, double phi);
double dA_dz_AnsorgNS3(void *aux, int ind, double A, double B, double phi);
double dB_dx_AnsorgNS3(void *aux, int ind, double A, double B, double phi);
double dB_dy_AnsorgNS3(void *aux, int ind, double A, double B, double phi);
double dB_dz_AnsorgNS3(void *aux, int ind, double A, double B, double phi);
double dphi_dx_AnsorgNS3(void *aux, int ind, double A, double B, double phi);
double dphi_dy_AnsorgNS3(void *aux, int ind, double A, double B, double phi);
double dphi_dz_AnsorgNS3(void *aux, int ind, double A, double B, double phi);
void set_d_dy_at_rhoEQzero_AnsorgNS(void *bo, void *va, 
                                    void *v1,void *v2,void *v3);
void set_d_dz_at_rhoEQzero_AnsorgNS(void *bo, void *va, 
                                    void *v1,void *v2,void *v3);

/* generic coord trafos */
void init_dXdx_generic(tBox *box);
void init_ddXdxdx_generic(tBox *box);
double dX_dx_generic(void *aux, int ind, double X, double Y, double Z);
double dX_dy_generic(void *aux, int ind, double X, double Y, double Z);
double dX_dz_generic(void *aux, int ind, double X, double Y, double Z);
double dY_dx_generic(void *aux, int ind, double X, double Y, double Z);
double dY_dy_generic(void *aux, int ind, double X, double Y, double Z);
double dY_dz_generic(void *aux, int ind, double X, double Y, double Z);
double dZ_dx_generic(void *aux, int ind, double X, double Y, double Z);
double dZ_dy_generic(void *aux, int ind, double X, double Y, double Z);
double dZ_dz_generic(void *aux, int ind, double X, double Y, double Z);
double ddX_dxdx_generic(void *aux, int ind, double X, double Y, double Z);
double ddX_dxdy_generic(void *aux, int ind, double X, double Y, double Z);
double ddX_dxdz_generic(void *aux, int ind, double X, double Y, double Z);
double ddX_dydy_generic(void *aux, int ind, double X, double Y, double Z);
double ddX_dydz_generic(void *aux, int ind, double X, double Y, double Z);
double ddX_dzdz_generic(void *aux, int ind, double X, double Y, double Z);
double ddY_dxdx_generic(void *aux, int ind, double X, double Y, double Z);
double ddY_dxdy_generic(void *aux, int ind, double X, double Y, double Z);
double ddY_dxdz_generic(void *aux, int ind, double X, double Y, double Z);
double ddY_dydy_generic(void *aux, int ind, double X, double Y, double Z);
double ddY_dydz_generic(void *aux, int ind, double X, double Y, double Z);
double ddY_dzdz_generic(void *aux, int ind, double X, double Y, double Z);
double ddZ_dxdx_generic(void *aux, int ind, double X, double Y, double Z);
double ddZ_dxdy_generic(void *aux, int ind, double X, double Y, double Z);
double ddZ_dxdz_generic(void *aux, int ind, double X, double Y, double Z);
double ddZ_dydy_generic(void *aux, int ind, double X, double Y, double Z);
double ddZ_dydz_generic(void *aux, int ind, double X, double Y, double Z);
double ddZ_dzdz_generic(void *aux, int ind, double X, double Y, double Z);
