#!/usr/bin/env python
# -*- coding: utf-8 -*-
from __future__ import division
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from matplotlib import rc
import numpy as np

from matplotlib import colors
import colorsys

import math

def plot_color_gradients(gradients, names):
    # For pretty latex fonts
    # rc('text', usetex=True)
    rc("font", family="serif", size=10)
    rc('legend', fontsize=10)

    column_width_pt = 400         # Show in latex using \the\linewidth
    pt_per_inch = 72
    size = column_width_pt / pt_per_inch

    fig, axes = plt.subplots(nrows=len(gradients), sharex=True, figsize=(size, 0.75 * size))
    fig.subplots_adjust(top=1.00, bottom=0.05, left=0.25, right=0.95)

    for ax, gradient, name in zip(axes, gradients, names):
        # Create image with two lines and draw gradient on it
        img = np.zeros((2, 1024, 3))
        for i, v in enumerate(np.linspace(0, 1, 1024)):
            img[:, i] = gradient(v)

        im = ax.imshow(img, aspect='auto')
        im.set_extent([0, 1, 0, 1])
        ax.yaxis.set_visible(False)

        pos = list(ax.get_position().bounds)
        x_text = pos[0] - 0.25
        y_text = pos[1] + pos[3] / 2.
        fig.text(x_text, y_text, name, va='center', ha='left', fontsize=10)

    fig.savefig('my-gradients.pdf')

def gradient_rgb_bw(v):
    return (v, v, v)


def gradient_rgb_gbr(v):
	x = [0, 0.5, 1]
	yr = [0, 0, 1]
	yg = [1, 0, 0]
	yb = [0, 1, 0]
	return (np.interp(v, x, yr), np.interp(v, x, yg), np.interp(v, x, yb))


def gradient_rgb_gbr_full(v):
	x = [0, 0.33, 0.5, 0.67, 1]
	yr = [0, 0, 0, 1, 1]
	yg = [1, 1, 0, 0, 0]
	yb = [0, 1, 1, 1, 0]
	return (np.interp(v, x, yr), np.interp(v, x, yg), np.interp(v, x, yb))


def gradient_rgb_wb_custom(v):
	period = math.pi
	r = math.sin(v * period)# + math.cos(v * period)
	g = math.fabs(math.cos(v * period))# + math.cos(v * period)
	b = math.fabs(math.sin(v * period - 0.25))# + math.cos(v * period)
	return (1-math.sqrt(v), 1-v, 1-v*v)

def gradient_hsv_bw(v):
	return colorsys.hsv_to_rgb(0.0, 0.0, v)


def gradient_hsv_bgr(v):
	x = [0, 0.5, 1]
	hue = [0.33, 0.66, 1]
	saturation = [1, 1, 1]
	value = [1, 1, 1]
	return colorsys.hsv_to_rgb(np.interp(v, x, hue), np.interp(v, x, saturation), np.interp(v, x, value))


def gradient_hsv_unknown(v):
	x = [0, 0.5, 1]
	hue = [0.33, 0.16, 0]
	saturation = [0.7, 0.7, 0.7]
	value = [1, 1, 1]
	return colorsys.hsv_to_rgb(np.interp(v, x, hue), np.interp(v, x, saturation), np.interp(v, x, value))


def gradient_hsv_custom(v):
    #print colorsys.rgb_to_hsvhsv(1, 1, 0)
    x = [0, 0.5, 1]
    hue = [0, 0.26, 1]
    saturation = [1, 0.8, 1]
    value = [1, 1, 0.5]
    return colorsys.hsv_to_rgb(np.interp(v, x, hue), np.interp(v, x, saturation), np.interp(v, x, value))


if __name__ == '__main__':
    def toname(g):
        return g.__name__.replace('gradient_', '').replace('_', '-').upper()

    gradients = (gradient_rgb_bw, gradient_rgb_gbr, gradient_rgb_gbr_full, gradient_rgb_wb_custom,
                 gradient_hsv_bw, gradient_hsv_bgr, gradient_hsv_unknown, gradient_hsv_custom)

    plot_color_gradients(gradients, [toname(g) for g in gradients])
