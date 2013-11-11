#!/usr/bin/env python
# -*- coding: utf-8 -*-

import numpy as np
import matplotlib.pyplot as plt
import colorsys
import math

fileName = "big.dem"

with open(fileName, "r") as f:
    w, h, l = [int(x) for x in f.readline().split()]
    data = []
    for line in f:
        data.append([float(x) for x in line.split()])

fig = plt.figure()
ax = fig.add_subplot(111)

l /= 100.0

sx = -400
sy = 150
sz = 0

img = np.zeros((w, h, 3))
for i in range(h):
    for j in range(w):
        hue = (155.0 - data[i][j]) / 395.0
        if (i > 0 and i < h-1 and j > 0 and j < w-1):
            w1x = 2 * l
            w1z = data[i][j+1] - data[i][j-1]
            w1x, w1y, w1z = -w1z, 0.0, w1x

            w2y = 2 * l
            w2z = data[i-1][j] - data[i+1][j]
            w2x, w2y, w2z = 0.0, -w2z, w2y

            wx = (w1x + w2x)
            wy = (w1y + w2y)
            wz = (w1z + w2z)

            if (wx == wy and wx == 0):
                wy = 1
            cosAlpha = (wx*sx + wy*sy) / (math.sqrt(wx*wx + wy*wy) * math.sqrt(sx*sx + sy*sy))
            alpha = math.acos(cosAlpha)

            value = 1.0 - alpha / 28.0
            if (value < 0.86):
                value = 1.0 - alpha / 71.0

            if (alpha < 0.22):
                saturation = alpha * 3.8
            elif (alpha < 1.0):
                saturation = (alpha + 1.0) / 2.0
            else:
                saturation = 1.0

        else:
            saturation = 1.0
            value = 1.0
        img[i, j] = colorsys.hsv_to_rgb(hue, saturation, value)

im = ax.imshow(img, aspect="auto")

plt.show()
