#!/usr/bin/env python
# -*- coding: utf-8 -*-

import numpy as np
import matplotlib.pyplot as plt
import colorsys

fileName = "big.dem"

_max = 0

with open(fileName, "r") as f:
    w, h, l = [int(x) for x in f.readline().split()]
    data = []
    for line in f:
        data.append([float(x) for x in line.split()])

print w, h, l
print len(data), len(data[0])

fig = plt.figure()
ax = fig.add_subplot(111)

ax.set_title("strange map")

img = np.zeros((w, h, 3))
for i in range(h):
    for j in range(w):
        hue = (155.0 - data[i][j]) / 395.0
        saturation = 1.0
        value = 1.0
        img[i, j] = colorsys.hsv_to_rgb(hue, saturation, value)

im = ax.imshow(img, aspect="auto")
#im.set_extent([0, 1, 0, 1])
#ax.yaxis.set_visible(False)

plt.show()
