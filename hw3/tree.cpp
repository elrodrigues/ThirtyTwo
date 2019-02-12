int countIncludes(const double a1[], int n1, const double a2[], int n2)
{
    if(n2 <= 0)
      return 1;
    if(n1 < n2)
      return 0;
    if(a1[0] != a2[0])
      return countIncludes(a1 + 1, n1 - 1, a2, n2);
    else
      return (countIncludes(a1 + 1, n1 - 1, a2 + 1, n2 - 1) +
      countIncludes(a1 + 1, n1 - 1, a2, n2));
}

  // Exchange two doubles
void exchange(double& x, double& y)
{
    double t = x;
    x = y;
    y = t;
}

void divide(double a[], int n, double divider,
		    int& firstNotGreater, int& firstLess)
{
  if (n < 0)
    n = 0;

  firstNotGreater = 0;
  firstLess = n;
  int firstUnknown = 0;
  while (firstUnknown < firstLess)
  {
    if (a[firstUnknown] < divider)
    {
      firstLess--;
      exchange(a[firstUnknown], a[firstLess]);
    }
    else
    {
      if(a[firstUnknown] > divider)
      {
        exchange(a[firstNotGreater], a[firstUnknown]);
        firstNotGreater++;
      }
      firstUnknown++;
    }
  }
}

void order(double a[], int n)
{
    if(n <= 1)
      return;
    int pos, dum; // Position, Dummy variable
    divide(a, n, a[0], pos, dum);
    if(pos == 0)
      order(a + 1, n - 1);
    else
      order(a, n);
}
