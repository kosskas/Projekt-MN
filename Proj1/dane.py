import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import math
   
def odczytaj_plik(nazwa):
    plik = pd.read_csv(nazwa, delimiter=",")
    dane = [tuple(linia) for linia in plik.values]   
    data = [linia[0] for linia in dane]
    zamkniecie = [linia[4] for linia in dane]
    return data, zamkniecie

def wyrownaj(dane):
    n = len(dane)
    return dane[n-1001:n]

def EMA(dane, okres):
    ema = []
    alfa = (2 / (okres + 1))   
    for i in range(len(dane)):
        licz = 0
        mian = 0
        for j in range(okres+1):  #+1??
            licz+=dane[i-j]*(1-alfa)**j
            mian+=(1-alfa)**j
        ema.append(licz/mian)
    return ema

def MACD(dane):
    ema12 = EMA(dane, 12)
    ema26 = EMA(dane, 26)
    #print(len(ema12), len(ema26))
    macd = []
    for i in range(len(ema12)):
        macd.append(ema12[i] - ema26[i])
    return macd

def sprawdz(macd, signal, len):
    for i in range(len):
        if macd[i-1] < signal[i-1] and macd[i] > signal[i]:
            print("Kup ",i)
        if macd[i-1] > signal[i-1] and macd[i] < signal[i]:
            print("Sprzedaj ",i)

def symuluj(data, akcje, macd, signal, poc, kon, kapital):
    print("Sumulacja od {0} do {1}. $$$ {2}".format(data[poc], data[kon], kapital))
    portfel = []
    suma = kapital
    kupil = False
    max_sprzed = 0
    licz_akcji = 0
    max_kap = 0
    test = 0
    kupno = []
    sprzed = []
    for i in range(poc, kon):
        if i > 340 and i < 355:
            print("Stan na {0}: {1} {2}".format(data[i],akcje[i], suma+licz_akcji*akcje[i]))
        portfel.append(suma+licz_akcji*akcje[i])
        if test == 100:
            print("Stan na {0}: {1}".format(data[i], suma+licz_akcji*akcje[i]))
            test=0
        if macd[i] > signal[i] and macd[i-1] < signal[i-1] and suma - licz_akcji*akcje[i] > 0 and kupil == False: #Kup
            kupno.append(i)
            licz_akcji+=suma/akcje[i]
            suma=0
            kupil = True
        if macd[i] < signal[i] and macd[i-1] > signal[i-1] and kupil == True:#Sprzedaj
            sprzed.append(i)
            suma+=licz_akcji*akcje[i]          
            licz_akcji=0
            kupil = False
        max_kap = max(max_kap, suma+licz_akcji*akcje[i])
        test+=1
    print("Stan na {0}, mam: {1}".format(data[kon], suma+licz_akcji*akcje[kon]))
    print("Maksymalny kapital", max_kap)
    return kupno, sprzed, portfel

def symuluj_lepiej(data, akcje, macd, signal, poc, kon, kapital):
    print("Sumulacja od {0} do {1}. $$$ {2}".format(data[poc], data[kon], kapital))
    portfel = []
    suma = kapital
    kupil = False
    max_sprzed = 0
    licz_akcji = 0
    max_kap = 0
    test = 0
    kupno = []
    sprzed = []
    for i in range(poc, kon):
        portfel.append(suma+licz_akcji*akcje[i])
        if test == 100:
            print("Stan na {0}: {1}".format(data[i], suma+licz_akcji*akcje[i]))
            test=0
        if macd[i] > signal[i] and macd[i-1] < signal[i-1] and suma - licz_akcji*akcje[i] > 0 and kupil == False: #Kup
            if suma >3*kapital:
                licz_akcji+=(suma*1/2)/akcje[i]
                suma = suma*1/2
            else:           
                licz_akcji+=suma/akcje[i]
                suma=0
            kupno.append(i)
            kupil = True
            #print("Kupuje w {0} za {1}, mam: {2} i tyle akcji {3}".format(data[i],akcje[i],suma, licz_akcji))
        if macd[i] < signal[i] and macd[i-1] > signal[i-1] and kupil == True:#Sprzedaj
            if licz_akcji*akcje[i] > 1.5*kapital:
                suma+=(2/3 * licz_akcji)*akcje[i]          
                licz_akcji= 1/3 * licz_akcji
            else:
                suma+=licz_akcji*akcje[i]          
                licz_akcji=0
            sprzed.append(i)
            kupil = False
           # print("Sprzedaje w {0} za {1}, mam: {2} i tyle akcji {3}".format(data[i],akcje[i],suma,licz_akcji))
        max_kap = max(max_kap, suma+licz_akcji*akcje[i])
        test+=1
    print("Stan na {0}, mam: {1}".format(data[kon], suma+licz_akcji*akcje[kon]))
    print("Maksymalny kapital", max_kap)
    return kupno, sprzed, portfel
#if math.isclose(macd[i], signal[i], rel_tol=0.05,abs_tol=0):
    #if macd[i-1] > macd[i]:
#https://stackoverflow.com/questions/43374920/how-to-automatically-annotate-maximum-value-in-pyplot
def odnotuj(x,y, posX, posY,nr = None,text = None, ax=None):
    if not nr:
        xmax = x[np.argmax(y)]
        ymax = max(y)
    else:
        xmax = nr
        ymax = y[nr]
    if not ax:
        text= "$={:.2f}".format(ymax)
    if not ax:
        ax=plt.gca()
    bbox_props = dict(boxstyle="square,pad=0.3", fc="w", ec="k", lw=0.72)
    arrowprops=dict(arrowstyle="->",connectionstyle="angle,angleA=0,angleB=60")
    kw = dict(xycoords='data',textcoords="axes fraction",
              arrowprops=arrowprops, bbox=bbox_props, ha="right", va="top")
    ax.annotate(text, xy=(xmax, ymax), xytext=(posX,posY), **kw)
