#!/usr/bin/python
# -*- coding: utf-8 -*-
# import seaborn, pyplot (for plotting), and pandas (to build the dataframe)

import seaborn as sns
import pandas as pd
import matplotlib.pyplot as plt
import pylab

# Dictionary of data

data = {
    'Exp setup': ['ALEX', 'STX-B+ Tree', 'VCSR', 'PCSR', 'BAL'],
    'Amazon': [1.95, 1.869, 0.552, 1.473, 0.750],
    'Cit-Patents': [15.80, 20.08, 5.82, 12.75, 9.19],
    'enron': [0.207, 0.082, 0.067, 0.159, 0.125],
    'fb-wall': [0.094, 0.064, 0.025, 0.079, 0.067],
    }

data = pd.DataFrame(data, columns=['Exp setup', 'Amazon', 'Cit-Patents'
                    , 'enron', 'fb-wall'])

data_M = pd.melt(data, id_vars='Exp setup', var_name='Graph Insert Time'
                 , value_name='Insert Time (Seconds)')


# set style for seaborn plot

sns.set(style='whitegrid', font='CMU Sans Serif')

# create a color palette (we only have three different colors for the three different tests T1...T3)

pal = sns.color_palette(n_colors=5)
(f, ax2) = plt.subplots()

# ax1 = sns.barplot(x="Graph Insert Time", y="Time",
 #               hue="Exp setup", data=data_M, palette=pal, ax=ax1)

ax2 = sns.barplot(
    x='Graph Insert Time',
    y='Insert Time (Seconds)',
    hue='Exp setup',
    data=data_M,
    palette=pal,
    ax=ax2,
    )

ax2.bar_label(
    ax2.containers[0],
    label_type='edge',
    rotation=90,
    color='black',
    fontsize=10,
    padding=5,
    style='italic',
    )

# ax1.bar_label(ax1.containers[0], label_type='edge', rotation=90, color='black', fontsize = 10, padding= 5, style = 'italic')

ax2.bar_label(
    ax2.containers[1],
    label_type='edge',
    rotation=90,
    color='black',
    fontsize=10,
    padding=5,
    style='italic',
    )

# ax1.bar_label(ax1.containers[1], label_type='edge', rotation=90, color='black', fontsize = 10, padding= 5, style = 'italic')

ax2.bar_label(
    ax2.containers[2],
    label_type='edge',
    rotation=90,
    color='black',
    fontsize=10,
    padding=5,
    style='italic',
    )

# ax1.bar_label(ax1.containers[2], label_type='edge', rotation=90, color='black', fontsize = 10, padding= 5, style = 'italic')

ax2.bar_label(
    ax2.containers[3],
    label_type='edge',
    rotation=90,
    color='black',
    fontsize=10,
    padding=5,
    style='italic',
    )

# ax1.bar_label(ax1.containers[3], label_type='edge', rotation=90, color='black', fontsize = 10, padding= 5, style = 'italic')

ax2.bar_label(
    ax2.containers[4],
    label_type='edge',
    rotation=90,
    color='black',
    fontsize=9,
    padding=5,
    style='italic',
    )

# ax1.bar_label(ax1.containers[4], label_type='edge', rotation=90, color='black', fontsize = 10, padding= 5, style = 'italic')

# ax1.set_ylim(2.5, 25)

ax2.set_ylim(0, 25)

# ax1.get_xaxis().set_visible(False)
# ax1.set_ylabel("")
# ax2.set_ylabel("")
# f.text(0.04, 0.55, "Time", va="center", rotation="vertical")
# ax1.get_legend().remove()

ax2.get_legend().remove()
plt.rcParams.update({'font.size': 22})

# plt.axhline(y = 1, color = 'black', linestyle = 'dashed')
# Creating legend

ax2.legend(loc=(-.034, 1.1), ncol=len(ax2.lines), prop={'size': 11.5})
ax2.grid(False)

# ax1.xaxis.tick_top()
# ax2.xaxis.tick_bottom()

# (this just moves everything, best to try and iterate)

f.subplots_adjust(left=0.15, right=.8, bottom=0.15, top=0.85)

plt.show()
d = .01  # how big to make the diagonal lines in axes coordinates

# arguments to pass to plot, just so we don't keep repeating them

kwargs = dict(transform=ax2.transAxes, color='k', clip_on=False)

# ax1.plot((-d, +d), (-d, +d), **kwargs)        # top-left diagonal
# ax1.plot((1 - d, 1 + d), (-d, +d), **kwargs)  # top-right diagonal

kwargs.update(transform=ax2.transAxes)  # switch to the bottom axes
ax2.plot((-d, +d), (1 - d, 1 + d), **kwargs)  # bottom-left diagonal
ax2.plot((1 - d, 1 + d), (1 - d, 1 + d), **kwargs)  # bottom-right diagonal

# display new plot again
# https://stackoverflow.com/questions/50452455/plt-show-does-nothing-when-used-for-the-second-time

from IPython.display import display
display(f)  # Shows plot again
