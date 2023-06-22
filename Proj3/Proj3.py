import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import math
   
challenger_path = "2018_paths/GlebiaChallengera.csv"
everest_path = "2018_paths/MountEverest.csv"
kanion_path = "2018_paths/WielkiKanionKolorado.csv"
chelm = "2018_paths/chelm.txt"

def odczytaj_plik(nazwa):
    plik = pd.read_csv(nazwa,delimiter=",") #delimiter=" ")         #
    dane = [tuple(linia) for linia in plik.values]   
    x = [linia[0] for linia in dane]
    y = [linia[1] for linia in dane]
    return np.array(x), np.array(y)

def langrange_int(dys_x, dys_y, n):
    # n liczba węzłów
    # n - 1 liczba podprzedziałów
    n-=1 
    x = np.linspace(dys_x[0], dys_x[-1])
    y = []
    krok = int(len(dys_x)/n)
    wezel_x = []
    wezel_y = []
    for xp in x:
        yp = 0
        for i in range(0, len(dys_x), krok):
            f = 1.0
            for j in range(0, len(dys_x), krok):
                if j != i:
                    f*=(xp - dys_x[j])/(dys_x[i] - dys_x[j])
            yp+=dys_y[i]*f
        y.append(yp)
    for i in range(0, len(dys_x), krok):
            wezel_x.append(dys_x[i])
            wezel_y.append(dys_y[i])
    plt.scatter(wezel_x,wezel_y, color = 'red', s = 20)
    return x, y

def spline(dys_x, dys_y, n):
    n_eq = n-1
    idx = np.linspace(0, dys_x.shape[0] - 1, n, dtype='int')
    X = dys_x[idx]
    Y = dys_y[idx]
    h = abs(X[0]-X[1])
    S = np.zeros((4 * n_eq, 4 * n_eq))
    b = np.zeros((4 * n_eq))

    S[0][0] = 1
    b[0] = Y[0]
    S[1][3] = 1

    S[4 * (n - 2), 0:4] = np.array([1, 0, 0, 0]) #S0 = a0
    S[4 * (n - 2) + 1, 0:4] = np.array([0, 0, 1, 0]) #S1(x1) = a1
    S[4 * (n - 2) + 2, (n - 3) * 4 + 4 : (n - 3) * 4 + 8] = np.array([1, h, h ** 2, h ** 3]) #S0' = S1'
    S[4 * (n - 2) + 3, (n - 3) * 4 + 4 : (n - 3) * 4 + 8] = np.array([0, 0, 2, 6 * h]) #S0" = S1"
    b[4 * (n - 2)] = Y[0]
    b[4 * (n - 2) + 1] = 0
    b[4 * (n - 2) + 2] = Y[n - 1]
    b[4 * (n - 2) + 3] = 0

    for i in range(0, n - 2):
        S[4 * i, 4 * i : 4 * i + 4] = np.array([1, h, h ** 2, h ** 3])
        S[4 * i + 1, 4 * i + 4 : 4 * i + 8] = np.array([1, 0, 0, 0])
        S[4 * i + 2, 4 * i : 4 * i + 8] = np.array([0, 1, 2 * h, 3 * (h ** 2), 0, -1, 0, 0])
        S[4 * i + 3, 4 * i : 4 * i + 8] = np.array([0, 0, 2, 6 * h, 0, 0, -2, 0])
        b[4 * i] = Y[i + 1]
        b[4 * i + 1] = Y[i + 1]
        b[4 * i + 2] = 0
        b[4 * i + 3] = 0

    wsp = np.linalg.solve(S, b)

    print(wsp)
    for i in range(n):
        plt.scatter(X[i], Y[i], color = 'red',s = 20)
    for i in range(n-1):     
        sklej_x = np.arange(X[i], X[i+1])
        sklej_y = (wsp[4*i] + (wsp[4*i+1]*(sklej_x - X[i])) + (wsp[4*i+2]*((sklej_x - X[i])**2)) + (wsp[4*i+3]*((sklej_x - X[i])**3)))
        if i == 0:
            plt.plot(sklej_x, sklej_y, color='blue', label='Interpolacja')
        else:
            plt.plot(sklej_x, sklej_y, color='blue')


x, y = odczytaj_plik(everest_path)

wezl = 103
plt.plot(x, y, 'g', label="Dane")#
spline(x,y, wezl)
#ix, iy = langrange_int(x, y, wezl)
#plt.plot(ix, iy, label="Interpolacja")
plt.ylim((min(y)-0.01*min(y),max(y)+0.01*max(y)))
plt.xlabel("Odległość [m]")
plt.ylabel("Wysokość [m]")
plt.legend(loc="upper left")
plt.title(f"Interpolacja funkcjami sklejanymi dla {wezl} węzłów")
#plt.title(f"Interpolacja Lagrange'a dla {wezl} węzłów")
plt.show()
