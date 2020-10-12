from matplotlib import pyplot as plt
import pandas as pd
res1=pd.read_csv("res1.dat",sep="    ",header=None)
plt.plot(res1[0],res1[1],color="red",label="p1")

res2=pd.read_csv("res2.dat",sep="    ",header=None)
plt.plot(res2[0],res2[1],color="green",label="p2")

res3=pd.read_csv("res3.dat",sep="    ",header=None)
plt.plot(res3[0],res3[1],color="blue",label="p3")

plt.legend()
plt.show()