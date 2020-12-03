import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
import numpy as np


def func(x, a, b):
    return a*x**b

x = np.array([4,8,16,32,64,128,256,512,1024,2048])
y = np.array([4.9E-06,2.285E-05,0.00012715,0.0008821,0.0060839,0.0427608,0.30005135,2.0899459,14.674798,103.4980487])
yerror = np.array([4.47213595499958E-07, 5.871429486124E-07, 4.28307562880989E-06, 4.45278150798967E-05, 0.000199407253207, 0.001167657871666, 0.004495698169244, 0.00835788175695, 0.027409546135761, 0.937289632938851])


plt.rc('pgf', texsystem='pdflatex') 

plt.plot(x, y, 'bo', markersize=3, label='punkty eksperymentalne')
plt.errorbar(x, y, yerr=yerror, fmt='o', markersize=3, capsize=6, color='blue')

popt, pcov = curve_fit(func, x, y)
errors = np.sqrt(np.diag(pcov))
print(popt)
print(errors)

x_fit = np.arange(0.0, 2064, 0.1)

plt.plot(x_fit, func(x_fit, *popt), 'r', label='$y = ax^b$, a = %.2E, b = %f' % (popt[0], popt[1]))

plt.xlabel("N")
plt.ylabel("Średni czas trwania [s]")
plt.legend()

plt.show()

#plt.savefig('strassen_plot.pgf')
