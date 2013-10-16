#!/usr/bin/env python
# -*- coding: utf-8 -*-

import matplotlib.pyplot as plt
import numpy as np
import csv

"""
class Wiersz:
	def __init__(self, p1, p2, gen):
		self.p1 = p1
		self.p

w = Wiersz(1,2,(1,2,3))
w.p1

class Wiersz2:
	pass
"""

def import_data(fileName, algorithmName):
	f = open("data/"+fileName+".csv", "rb")
	data = { "data": [], "last": [] }

	try:
		reader = csv.reader(f)
		for number,row in enumerate(reader):
			cur_data = {}
			if number > 0:
				cur_data["name"] = algorithmName
				cur_data["generation"] = int(row[0])
				cur_data["effort"] = float(row[1])
				cur_data["values"] = map(lambda x: float(x), row[2:])
				
				data["data"].append(cur_data)
				data["last"] = map(lambda x: float(x), row[2:])
	finally:
		f.close()

	return data

def draw_plot(data, marker, color):
	args = []
	values = []
	
	for i in range(len(data["data"])):
		args.append(data["data"][i]["effort"]/1000.0)
		values.append( np.mean(data["data"][i]["values"])*100.0 ) 
		#print args[i], values[i]

	plt.plot(args, values, marker+color+'-', label = data["data"][i]["name"], markersize=7, markevery=25)

fileNames = [ "rsel", "cel-rs", "2cel-rs", "cel", "2cel" ]
algorithmNames = [ "1-Evol-RS", "1-Coev-RS", "2-Coev-RS", "1-Coev", "2-Coev" ]
colors = [ "b", "g", "r", "k", "m" ]
markers = [ "o", "v", "D", "s", "d" ]

files = []
for fileName,algorithmName in zip(fileNames,algorithmNames):
	files.append(import_data(fileName,algorithmName))

plt.rc("font", family="serif")
plt.rc("font", serif="Times New Roman")

# line plot
fig = plt.figure(1)
linePlot = plt.subplot(1,2,1)
for i in range(len(files)):
	draw_plot(files[i], markers[i], colors[i])

plt.ylabel("Odsetek wygranych gier [%]")
plt.xlabel("Rozegranych gier (x1000)")

plt.tick_params(labeltop="on")

plt.grid(True, color="0.75")

legend = plt.legend(loc = "lower right", numpoints=2, shadow=False, fancybox=True, prop={"size":12})
legend.get_frame().set_alpha(0.55)

plt.xlim(0,500)
plt.twiny()
plt.xticks(range(0,201,40))
plt.xlim(0,200)
plt.xlabel("Pokolenie")

plt.ylim(60,100)

# box plot
plt.subplot(1,2,2)
plt.grid(True, color="0.80")
plt.tick_params(labelleft="off", labelright="on")

box_values = []
for i in range(len(files)):
	box_values.append(map(lambda x: x*100.0, files[i]["last"]))

plt.boxplot(box_values, notch=True)
plt.scatter(range(len(box_values)+1)[1:], map(lambda x: np.mean(x), box_values))
plt.xticks(range(len(box_values)+1)[1:], algorithmNames, rotation=22)
plt.ylim(60,100)

#plt.show()
plt.savefig("myplot.pdf")
plt.close()

