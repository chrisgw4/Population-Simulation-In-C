import math
import os
import random
import matplotlib.pyplot as plt
import numpy as np


file = open("population_stats.txt", 'r')
data = file.read()
data = data.split(" ")
data.pop(len(data)-1)

f, ax = plt.subplots(2, 2, figsize=(14, 8))
plt.subplots_adjust(wspace=0.31, hspace=0.2, left=0.06)
plt.locator_params(axis='both', nbins=4) 

ax[0][0].set(xlabel="Iterations", ylabel="Population")

x_array = []
y_array = []


count = 0
for i in data:
    if count == 0:
        x_array.append(int(i)) 
        count += 1
    else:
        y_array.append(int(i))
        count = 0



plt.setp(ax, xticks=[0, x_array[-1]/2, x_array[-1]], yticks=[0, max(y_array)/2, max(y_array)])

ax[0][0].scatter(x_array, y_array, color='r', s=3)



file = open("gene1AA_stats.txt")
data = file.read()
data = data.split(" ")
data.pop(len(data)-1)

ax[0][1].set(xlabel="Iterations", ylabel="Population")




x_array2 = []
y_array2 = []


count = 0
for i in data:
    if count == 0:
        x_array2.append(int(i))
        count+=1
    else:
        y_array2.append(int(i))
        count = 0
    


file = open("gene2Aa_stats.txt")
data = file.read()
data = data.split(" ")
data.pop(len(data)-1)

x_array3 = []
y_array3 = []

count = 0
for i in data:
    if count == 0:
        x_array3.append(int(i))
        count+=1
    else:
        y_array3.append(int(i))
        count = 0



file = open("gene3aa_stats.txt")
data = file.read()
data = data.split(" ")
data.pop(len(data)-1)

x_array4 = []
y_array4 = []

count = 0
for i in data:
    if count == 0:
        x_array4.append(int(i))
        count+=1
    else:
        y_array4.append(int(i))
        count = 0


ax[0][1].plot(x_array, y_array, color='r', label="Population")
ax[0][1].plot(x_array2, y_array2, color='black', label="AA")
ax[0][1].plot(x_array3, y_array3, color='g', label="Aa")
ax[0][1].plot(x_array4, y_array4, color='b', label="aa")

ax[0][1].legend()












plt.savefig(f'graphs00{count}.pdf')
plt.show()
