from matplotlib import pyplot as plt
import pandas as pd
plt.subplot(1,2,1)
res1=pd.read_csv("res1.dat",sep="    ",header=None)
plt.plot(res1[0],res1[1],color="red",label="q1")

res2=pd.read_csv("res2.dat",sep="    ",header=None)
plt.plot(res2[0],res2[1],color="green",label="q2")

res3=pd.read_csv("res3.dat",sep="    ",header=None)
plt.plot(res3[0],res3[1],color="blue",label="q3")
plt.title("waitingTime")

plt.subplot(1,2,2)
len1=pd.read_csv("len1.dat",sep="    ",header=None)
plt.plot(len1[0],len1[1],color="red",label="q1")

len2=pd.read_csv("len2.dat",sep="    ",header=None)
plt.plot(len2[0],len2[1],color="green",label="q2")

len3=pd.read_csv("len3.dat",sep="    ",header=None)
plt.plot(len3[0],len3[1],color="blue",label="q3")
plt.title("queueLength")

plt.legend()
plt.show()