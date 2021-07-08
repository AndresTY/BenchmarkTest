import numpy as np
import matplotlib.pyplot as plt

tests = ['mmClassic-test-100.txt', 'mmClassic-test-250.txt', 'mmClassic-test-500.txt', 'mmClassic-test-750.txt', 'mmClassic-test-1000.txt',
         'mmClassic-test-1500.txt', 'mmClassic-test-2000.txt', 'mmClassic-test-2500.txt', 'mmClassic-test-3000.txt', 'mmClassic-test-3500.txt', 'mmClassic-test-4000.txt']
x = [i for i in range(30)]

for i in tests:
    files = 'soluciones/'+i
    data = np.loadtxt(files, delimiter='\n').astype(int)
    modi = [x/60000000 for x in data]
    plt.plot(x, modi)
    plt.xlabel('Repeticiones')
    plt.ylabel('Min')
    plt.title('rendimiento '+i+' (Min vs repeticiones)')
    plt.savefig(f'graficoS/{i[0:-4]}.png')
    plt.show()
