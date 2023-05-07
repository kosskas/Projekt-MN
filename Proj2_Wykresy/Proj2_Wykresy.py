import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import math

#int N[10] = { 100, 200, 300, 500, 700, 1000, 2000, 3000, 4000, 5000 };
N = [i for i in [100, 200, 300, 500, 700, 1000, 2000, 3000, 4000, 5000]]
jak = [
    0.0103,
0.0290,
0.0460,
0.1161,
0.2203,
0.4970,
1.9010,
3.6077,
6.3605,
10.6645
    ]

gs = [0.0084,
0.0167,
0.0240,
0.0648,
0.0990,
0.1974,
0.8344,
1.8893,
2.7616,
4.2776]


lu = [0.0030,
0.0116,
0.0423,
0.1656,
0.4516,
1.4587,
10.2391,
34.5864,
78.6752,
151.7079
    ]
plt.plot(N,jak, label="Jakobi")
plt.plot(N,gs, label="GS")
plt.plot(N,lu, label="LU")

plt.title("Czas rozwiązywania równiania w zależności od rozmiarów macierzy dla poszczególnych metod")

#plt.title("Normy błędu rezydualnego w kolejnych iteracjach dla metody Jakobiego")
plt.xlabel("N")
plt.ylabel("Czas [s]")
plt.legend(loc="upper left")
plt.show()