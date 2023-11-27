import pylab
import matplotlib.pyplot as plt
a = [pow(10, i) for i in range(10)]
fig = plt.figure()
ax = fig.add_subplot(2, 1, 1)

line, = ax.plot(a, color='blue', lw=2)

ax.set_yscale('log')

pylab.show()
