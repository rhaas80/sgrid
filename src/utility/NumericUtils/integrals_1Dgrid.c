/* integrals_1Dgrid.c */
/* Wolfgang Tichy, 5/2010 */



/* integrate with Simpson rule */
double integrate_simpson_1Dgrid(double* f, double dx, int i1, int i2)
{
  double sum;
  int i, ioff;


  sum  = 0;
  ioff = 0;

  /* If total number of points is even, use 3/8 rule for first 4 points */
  if((i2-i1+1) % 2 == 0)
  {
    sum += 3 * dx * (f[i1+0] + 3 * (f[i1+1] + f[i1+2]) + f[i1+3]) / 8;
    ioff = 3;
  }

  for(i = i1+ioff; i < i2-1; i += 2)
    sum += dx / 3 * (f[i] + 4 * f[i+1] + f[i+2]);

  return sum;
}


/* integrate with trapez rule */
double integrate_trapez_1Dgrid(double* f, double dx, int i1, int i2)
{
  double sum;
  int i;

  sum = 0;
  for(i = i1; i < i2; i++)
    sum += dx/2 * (f[i] + f[i+1]);

  return sum;
}