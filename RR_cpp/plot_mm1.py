from matplotlib import pyplot as plt
import pandas as pd
plt.subplot(1,2,1)
res=pd.read_csv("res_mm1.dat",sep="    ",header=None)
plt.plot(res[0],res[1])
plt.title("waitingTime")

plt.subplot(1,2,2)
res=pd.read_csv("len_mm1.dat",sep="    ",header=None)
plt.plot(res[0],res[1])
plt.title("queueLength")

plt.show()