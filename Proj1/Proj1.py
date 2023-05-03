from dane import *

data, zamkniecie = odczytaj_plik('dane_gieldowe.csv')

wartosci = zamkniecie
macd = MACD(wartosci)
signal = EMA(macd, 9)


wartosci = wyrownaj(wartosci)
macd = wyrownaj(macd)
signal = wyrownaj(signal)
data = wyrownaj(data)
kon = 1000
#kon = 400
poc = 0
#poc = kon - 100
kapital_pocz = 1000
czy_kropki = True
sprawdz(macd, signal, kon)
kupno, sprzedarz, portfel = symuluj(data, wartosci, macd, signal, poc,kon, kapital_pocz)
kupnoL, sprzedarzL, portfel_lepiej = symuluj_lepiej(data, wartosci, macd, signal, poc,kon, kapital_pocz)
#kupno, sprzedarz, portfel = symuluj_lepiej(data, wartosci, macd, signal, poc,kon, kapital_pocz)

#zmienne potrzebne do rysowania wykresow
dane_w = wartosci[poc:kon]
macd_w = macd[poc:kon]
signal_w = signal[poc:kon]
portfel_w = portfel
skala = [i for i in range(poc, kon)]
#Wykres cen akcji
plt.plot(skala, dane_w)
plt.title("Wartości akcji firmy Tesla od {0} do {1}".format(data[poc], data[kon]))
plt.xlabel("Numer dnia")
plt.ylabel("Wartość akcji")
plt.show()


#MACD
plt.plot(skala, macd_w,label="MACD")
plt.plot(skala, signal_w, color="red",label="Signal")
plt.title("Wartości wskaźnika MACD od {0} do {1}".format(data[poc], data[kon]))
plt.xlabel("Numer dnia")
plt.ylabel("Wartość MACD")
plt.legend(loc="center")
plt.show()


#Portfel
plt.plot(skala, portfel_w,label="Zwykły")
plt.plot(skala, portfel_lepiej,label="Ulepszony")
plt.title("Wartość kapitału {0} do {1}".format(data[poc], data[kon]))
plt.xlabel("Numer dnia")
plt.ylabel("Wartość kapitału")
#odnotuj([i for i in range(poc, kon)], portfel_w,0.95, 0.89)
#odnotuj([i for i in range(poc, kon)], portfel_w,0.75, 0.20,355)
plt.ylim(0, 11000)
plt.legend(loc="upper left")
plt.show()

#Combo
fig, axs = plt.subplots(2)
axs[0].plot(skala, dane_w)
if czy_kropki:
    axs[0].scatter(kupno, [wartosci[kupno[i]] for i in range(len(kupno))], color="red",marker="X",label="Kupno")
    axs[0].scatter(sprzedarz, [wartosci[sprzedarz[i]] for i in range(len(sprzedarz))], color="green",marker="o",label="Sprzedaż")
    axs[0].legend(loc="lower left")
axs[0].set_xlabel("Numer dnia")
axs[0].set_ylabel("Wartość akcji")
axs[0].set_ylim(0, 2700)

#bbox_props = dict(boxstyle="square,pad=0.3", fc="w", ec="k", lw=0.72)
#arrowprops=dict(arrowstyle="->",connectionstyle="angle,angleA=0,angleB=60")
#kw = dict(xycoords='data',textcoords="axes fraction",
#              arrowprops=arrowprops, bbox=bbox_props, ha="right", va="top")
#axs[0].annotate("Ominięta sprzedaż", xy=(352, wartosci[351]), xytext=(0.95,0.89), **kw)

#bbox_props = dict(boxstyle="square,pad=0.3", fc="w", ec="k", lw=0.72)
#arrowprops=dict(arrowstyle="->",connectionstyle="angle,angleA=-60,angleB=0")
#kw = dict(xycoords='data',textcoords="axes fraction",
#              arrowprops=arrowprops, bbox=bbox_props, ha="right", va="top")
#axs[0].annotate("Sprzedaż nastąpiła za późno", xy=(353, wartosci[352]), xytext=(0.45,0.39), **kw)

axs[1].plot(skala, macd_w,label="MACD")
axs[1].plot(skala, signal_w, color="red",label="Signal")
axs[1].set_xlabel("Numer dnia")
axs[1].set_ylabel("Wartość MACD")
axs[1].legend(loc="lower left")
fig.suptitle("Wartości akcji firmy Tesla oraz wskaźnika MACD od {0} do {1}".format(data[poc], data[kon]))
plt.show()

#fig, axs1 = plt.subplots()
#axs1.plot([i for i in range(poc, kon)], dane_w, color="green")
#axs2 = axs1.twinx()

#axs2.plot([i for i in range(poc, kon)], macd_w)
#axs2.plot([i for i in range(poc, kon)], signal_w, color="red")

#fig.suptitle(("Od {0} do {1}".format(data[poc], data[kon])))
#plt.show()

