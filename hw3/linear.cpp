bool anyTrue(const double a[], int n)
{
    if(n <= 0)
      return false;
    if(somePredicate(a[0]))
      return true;
    return anyTrue(a + 1, n - 1);
}

int countTrue(const double a[], int n)
{
    if(n <= 0) // STOPGAP
      return 0;
    if(somePredicate(a[0]))
      return 1 + countTrue(a + 1, n - 1);
    return countTrue(a + 1, n - 1);

}

int firstTrue(const double a[], int n)
{
  if(n <= 0) // Not Found
    return -1;
  if(somePredicate(a[0])) // Found
    return 0;
  int t = firstTrue(a + 1, n - 1);
  if(t == -1)
    return -1;
  else
    return 1 + t;

}

int positionOfMin(const double a[], int n)
{
    if(n <= 0)
      return -1;
    if(n == 1)
      return 0;
    int t = positionOfMin(a, n - 1);
    if(a[t] <= a[n - 1])
      return t;
    else
      return n - 1;
}

bool includes(const double a1[], int n1, const double a2[], int n2)
{
    if(n2 <= 0)
      return true;
    if(n1 < n2) // Pigeonhole
      return false;

    if(a1[0] != a2[0])
      return includes(a1 + 1, n1 - 1, a2, n2);
    else
      return includes(a1 + 1, n1 - 1, a2 + 1, n2 - 1);
}
