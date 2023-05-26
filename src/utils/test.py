import numpy as np
import matplotlib.pyplot as plt

# test.py is used for simple, quick plotting for CDF/frequency of test datasets (approximation)

# Test Paths
sequential_path = "datasets/sequential-dataset.txt"
test_lognormal_path = "datasets/lognormal-190M.bin.data"
test_longlat_path = "datasets/longlat-200M.bin.data"
test_longitudes_path = "datasets/longitudes-200M.bin.data"
test_ycsb_path = "datasets/ycsb-200M.bin.data"
test_amazon_path = "datasets/amazon.out.text"
test_enron_path = "datasets/enron.out"
test_cit_path = "datasets/cit-patents.out"
test_fb_path = "datasets/fb-wall.out"

# True Paths used in cluster (switch these for the proper filepath for user)
lognormal_path = "/users/ipatel9/dataset/lognormal-190M.bin.data"
longlat_path = "/users/ipatel9/dataset/longlat-200M.bin.data"
longitudes_path = "/users/ipatel9/dataset/longitudes-200M.bin.data"
ycsb_path = "/users/ipatel9/dataset/ycsb-200M.bin.data"
enron_path = "/users/ipatel9/dataset/enron.out"
amazon_path = "/users/ipatel9/dataset/amazon.out.text"
cit_path = "/users/ipatel9/dataset/cit-patents.out"
fb_path = "/users/ipatel9/dataset/fb-wall.out"

file_name = test_ycsb_path
key_type = np.int

# switch datatypes for certain files
if file_name == lognormal_path or file_name == test_lognormal_path or file_name == ycsb_path or file_name == test_ycsb_path:
    key_type = np.int_
elif file_name == longlat_path or file_name == test_longlat_path or file_name == test_longitudes_path or file_name == longitudes_path:
    key_type = np.float

# uncomment for binary files
with open(file_name, 'rb') as file:
    arr = np.fromfile(file, dtype=key_type)

# uncomment for text files
# arr = np.loadtxt(file_name)

# using numpy histogram method to approximate the CDF/frequency of each dataset
counts, bin_edges = np.histogram(arr, bins='auto')

# calculate the bin centers
bin_centers = 0.5*(bin_edges[1:] + bin_edges[:-1])

# plot the PDF as a line chart
plt.plot(bin_centers, counts, linewidth=3)

# plot customization
title_font = {'family' : 'sans-serif',
        'fontweight' : 'medium',
        'size'   : 33}

x_axis_font = {'family' : 'sans-serif',
        'fontweight' : 'medium',
        'size'   : 23}
y_axis_font = {'family' : 'sans-serif',
        'fontweight' : 'medium',
        'size'   : 28}

plt.title("YCSB", **title_font)
plt.xlabel('Key', **x_axis_font)
plt.ylabel('Frequency', **y_axis_font)
# plt.subplots_adjust(bottom=0.15, left=0.15)
plt.subplots_adjust(bottom=0.15)
plt.locator_params(axis='x', nbins=6)
plt.locator_params(axis='y', nbins=8)

locs, labels = plt.yticks()  # Get the current locations and labels.
plt.yticks(np.arange(0, 500000, step=100000), fontsize=15)
plt.ticklabel_format(axis="y", style="sci", scilimits=(0,0))


plt.xticks(fontsize=15)
plt.grid(axis = 'y')

plt.savefig("YCSB_freq.png")
plt.show()


# YCSB - 20 bins? (magnitude of 1e-18, no 0.0 min on y; in theory needs 2 * 10^19 bins for proper y-axis)
# lognormal - 1000000 bins? (magnitude of 1e-8 on y, curve exceeds 1.0, no 0.0 min on y; in theory needs 10^14 bins for proper y-axis)
# longitudes - 357 bins
# longlat - 62500 bins

# notes:
# YCSB x-axis goes up to 1, in theory should go past 1
# YCSB only one that doesn't start at 0.0 for y
