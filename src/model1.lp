\ENCODING=ISO-8859-1
\Problem name: IloCplex

Minimize
 obj: 500 x2 + 517 x3
Subject To
 c1: x1 <= 10000
 c2: x2 <= 10000
 c3: x3 <= 10000
 c4: x1 - x4  = 1482
 c5: x2 + x4 - x5  = 1664
 c6: x3 + x5 - x6  = 0
Bounds
      x1 >= 0
      x2 >= 0
      x3 >= 0
 0 <= x4 <= 9.22337203685478e+18
 0 <= x5 <= 9.22337203685478e+18
 0 <= x6 <= 9.22337203685478e+18
Generals
 x1  x2  x3  x4  x5  x6 
End
