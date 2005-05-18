/* boundary.c */
/* Wolfgang Tichy 5/2005 */

#include "sgrid.h"
#include "boundary.h"

/* here we use:
   tPointList *radiativeBoundaryPointList;
   tPointList *ExcisionBoundaryPointList; */
/* initialize our Boundary PointLists */
int initialize_BoundaryPointLists(tGrid *grid)
{
  char str[100];
  int bi;
  int i,j,k;
  int n1,n2,n3;
  int nboxes = grid->nboxes;
  
  radiativeBoundaryPointList = AllocatePointList(grid);
  constantBoundaryPointList = AllocatePointList(grid);
  selectedconstantBoundaryPointList = AllocatePointList(grid);
  ExcisionBoundaryPointList = AllocatePointList(grid);
  boxBoundaryPointList = AllocatePointList(grid);

  printf("boundary: initialize_BoundaryPointLists:\n");

  /* make PointList containing all boundaries of all boxes */
  forallboxes(grid,bi)
  {
     n1=grid->box[bi]->n1;
     n2=grid->box[bi]->n2;
     n3=grid->box[bi]->n3;

     snprintf(str, 99, "box%d_basis1", bi);
     if( !(Getv(str, "Fourier") || Getv(str, "fd2_periodic")) )
     {
       forplane1(i,j,k, n1,n2,n3, 0)
         AddToPointList(boxBoundaryPointList, bi, Index(i,j,k));

       forplane1(i,j,k, n1,n2,n3, n1-1)
         AddToPointList(boxBoundaryPointList, bi, Index(i,j,k));
     }
     snprintf(str, 99, "box%d_basis2", bi);
     if( !(Getv(str, "Fourier") || Getv(str, "fd2_periodic")) )
     {
       forplane2(i,j,k, n1,n2,n3, 0)
         AddToPointList(boxBoundaryPointList, bi, Index(i,j,k));

       forplane2(i,j,k, n1,n2,n3, n2-1)
         AddToPointList(boxBoundaryPointList, bi, Index(i,j,k));
     }
     snprintf(str, 99, "box%d_basis3", bi);
     if( !(Getv(str, "Fourier") || Getv(str, "fd2_periodic")) )
     {
       forplane3(i,j,k, n1,n2,n3, 0)
         AddToPointList(boxBoundaryPointList, bi, Index(i,j,k));

       forplane3(i,j,k, n1,n2,n3, n3-1)
         AddToPointList(boxBoundaryPointList, bi, Index(i,j,k));
     }
  }
  printf("boxBoundaryPointList:\n");
  prPointList(boxBoundaryPointList);

  
  /* radiative boundary condition */
  if( Getv("boundary", "radiative") )
  {
    bi=nboxes-1;
    snprintf(str, 99, "box%d_Coordinates", bi);
    if(Getv(str, "SphericalDF") || Getv(str, "compactSphericalDF"))
    {
       n1=grid->box[bi]->n1;
       n2=grid->box[bi]->n2;
       n3=grid->box[bi]->n3;
        
       forplane1(i,j,k, n1,n2,n3, n1-1)
         AddToPointList(radiativeBoundaryPointList, bi, Index(i,j,k));
    }
    else
    {
       n1=grid->box[bi]->n1;
       n2=grid->box[bi]->n2;
       n3=grid->box[bi]->n3;
        
       forplane1(i,j,k, n1,n2,n3, 0)
         AddToPointList(radiativeBoundaryPointList, bi, Index(i,j,k));

       forplane1(i,j,k, n1,n2,n3, n1-1)
         AddToPointList(radiativeBoundaryPointList, bi, Index(i,j,k));

       forplane2(i,j,k, n1,n2,n3, 0)
         AddToPointList(radiativeBoundaryPointList, bi, Index(i,j,k));

       forplane2(i,j,k, n1,n2,n3, n2-1)
         AddToPointList(radiativeBoundaryPointList, bi, Index(i,j,k));

       forplane3(i,j,k, n1,n2,n3, 0)
         AddToPointList(radiativeBoundaryPointList, bi, Index(i,j,k));

       forplane3(i,j,k, n1,n2,n3, n3-1)
         AddToPointList(radiativeBoundaryPointList, bi, Index(i,j,k));
    }
    printf("radiativeBoundaryPointList:\n");
    prPointList(radiativeBoundaryPointList);
  }

  /* constant boundary condition */
  if( Getv("boundary", "constant") )
  {
    bi=nboxes-1;
    snprintf(str, 99, "box%d_Coordinates", bi);
    if(Getv(str, "SphericalDF") || Getv(str, "compactSphericalDF"))
    {
       n1=grid->box[bi]->n1;
       n2=grid->box[bi]->n2;
       n3=grid->box[bi]->n3;
        
       forplane1(i,j,k, n1,n2,n3, n1-1)
         AddToPointList(constantBoundaryPointList, bi, Index(i,j,k));
    }
    else
    {
       n1=grid->box[bi]->n1;
       n2=grid->box[bi]->n2;
       n3=grid->box[bi]->n3;
        
       forplane1(i,j,k, n1,n2,n3, 0)
         AddToPointList(constantBoundaryPointList, bi, Index(i,j,k));

       forplane1(i,j,k, n1,n2,n3, n1-1)
         AddToPointList(constantBoundaryPointList, bi, Index(i,j,k));

       forplane2(i,j,k, n1,n2,n3, 0)
         AddToPointList(constantBoundaryPointList, bi, Index(i,j,k));

       forplane2(i,j,k, n1,n2,n3, n2-1)
         AddToPointList(constantBoundaryPointList, bi, Index(i,j,k));

       forplane3(i,j,k, n1,n2,n3, 0)
         AddToPointList(constantBoundaryPointList, bi, Index(i,j,k));

       forplane3(i,j,k, n1,n2,n3, n3-1)
         AddToPointList(constantBoundaryPointList, bi, Index(i,j,k));
    }
    printf("constantBoundaryPointList:\n");
    prPointList(constantBoundaryPointList);
  }
  
  /* constant Excision boundary condition */
  if( Getv("boundary", "constantExcision") )
  {
    bi=0;
    snprintf(str, 99, "box%d_Coordinates", bi);
    if(Getv(str, "SphericalDF") || Getv(str, "compactSphericalDF"))
    {
       n1=grid->box[bi]->n1;
       n2=grid->box[bi]->n2;
       n3=grid->box[bi]->n3;
        
       forplane1(i,j,k, n1,n2,n3, 0)
         AddToPointList(constantBoundaryPointList, bi, Index(i,j,k));
    }
    printf("constantBoundaryPointList:\n");
    prPointList(constantBoundaryPointList);
  }

  /* simple Excision boundary condition */
  if( Getv("boundary", "simpleExcision") )
  {
    bi=0;
    snprintf(str, 99, "box%d_Coordinates", bi);
    if(Getv(str, "SphericalDF") || Getv(str, "compactSphericalDF"))
    {
       n1=grid->box[bi]->n1;
       n2=grid->box[bi]->n2;
       n3=grid->box[bi]->n3;
        
       forplane1(i,j,k, n1,n2,n3, 0)
         AddToPointList(ExcisionBoundaryPointList, bi, Index(i,j,k));
    }
    printf("ExcisionBoundaryPointList:\n");
    prPointList(ExcisionBoundaryPointList);
  }

  /* const Excision boundary for some slected variables */
  if( Getv("boundary", "selectedconstantExcision") )
  {
    bi=0;
    snprintf(str, 99, "box%d_Coordinates", bi);
    if(Getv(str, "SphericalDF") || Getv(str, "compactSphericalDF"))
    {
       n1=grid->box[bi]->n1;
       n2=grid->box[bi]->n2;
       n3=grid->box[bi]->n3;
        
       forplane1(i,j,k, n1,n2,n3, 0)
         AddToPointList(selectedconstantBoundaryPointList, bi, Index(i,j,k));
    }
    printf("selectedconstantBoundaryPointList:\n");
    prPointList(selectedconstantBoundaryPointList);
  }

  return 0;
}


/* apply all active boundary conditions after a standard evolution step */
void set_boundary(tVarList *unew, tVarList *upre, double c, tVarList *ucur)
{
  tGrid *grid = unew->grid;
  int i,j;
  //double harmonic_f = 1;
  double v0, var0, varfalloff;
  
  /* radiative boundary condition */
  if( Getv("boundary", "radiative") )
  {
    /* monopole fall off rate: not implemented yet */
    varfalloff = 0;

    /* for all variables */
    for (j = 0; j < unew->n; j++)
    {
      i    = unew->index[j];
      v0   = VarPropSpeed(i);
      var0 = VarFarLimit(i);

      /* keep this variable constant, just copy it */
      if (Getv("boundary_radconstant", VarName(i))) v0 = 0;

      /* do it */
      set_boundary_radiative(grid, 
	unew->index[j], upre->index[j], c, ucur->index[j], var0, v0);
    }
  }

  /* excision boundary condition */
  if( Getv("boundary", "constant") || Getv("boundary", "constantExcision") )
  {
    /* for all variables */
    for (j = 0; j < unew->n; j++)
      set_boundary_constant(constantBoundaryPointList,
                            unew->index[j], upre->index[j]);
  }

  /* simple excision boundary condition */
  if( Getv("boundary", "simpleExcision") )
  {
    /* for all variables */
    for (j = 0; j < unew->n; j++)
      set_boundary_simpleExcision(grid, unew->index[j], upre->index[j]);
  }

  /* selectedconstantExcision excision boundary condition */
  if( Getv("boundary", "selectedconstantExcision") )
  {
    /* for all variables */
    for (j = 0; j < unew->n; j++)
    {
      i = unew->index[j];
      if (Getv("boundary_selectedconstantExcision", VarName(i)))
        set_boundary_constant(selectedconstantBoundaryPointList,
                              unew->index[j], upre->index[j]);
    }
  }

  /* symmetry boundary 
     has to be after radiative and excision
     can be before or after synchronization, but before is more efficient
  */
  //set_boundary_symmetry(level, unew); 
}


/* constant boundary condition */
void set_boundary_constant(tPointList *PL, int unew, int upre)
{
  tGrid *grid = PL->grid;
  int bi, pi, ijk;
  
  forallboxes(grid,bi)
  {
    tBox *box=grid->box[bi];
    double *varnew = box->v[unew];
    double *varpre = box->v[upre];

    forPointList_inbox(PL, box, pi , ijk)
      varnew[ijk] =  varpre[ijk];
  }
}
