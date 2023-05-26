import numpy as np
import matplotlib.pyplot as plt

# cdf_curve.py was used for the HPC cluster to plot the accurate CDF curve (actual)

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


file_name = fb_path
key_type = np.int64

if file_name == lognormal_path or file_name == test_lognormal_path or file_name == ycsb_path or file_name == test_ycsb_path:
    key_type = np.int_
elif file_name == longlat_path or file_name == test_longlat_path or file_name == test_longitudes_path or file_name == longitudes_path:
    key_type = np.float

# uncomment for binary file
# with open(file_name, 'rb') as file:
#     arr = np.fromfile(file, dtype=key_type)

# uncomment for text files
arr = np.loadtxt(file_name)


print("length of arr: ", len(arr))

# Create a plot of the CDF
ecdf = np.arange(1, len(arr)+1) / len(arr)
arr.sort()

# Plot the eCDF (empirical CDF)
print("plotting cdf...")
plt.step(arr, ecdf, linewidth=3)

# Plot customization
title_font = {'family' : 'sans-serif',
        'fontweight' : 'medium',
        'size'   : 33}

x_axis_font = {'family' : 'sans-serif',
        'fontweight' : 'medium',
        'size'   : 23}
y_axis_font = {'family' : 'sans-serif',
        'fontweight' : 'medium',
        'size'   : 28}

plt.title("fb-wall", **title_font)
plt.xlabel('Key', **x_axis_font)
plt.ylabel('CDF', **y_axis_font)

# add left=0.17 if adding y label
plt.subplots_adjust(bottom=0.15)
locs, labels = plt.yticks()
plt.yticks(np.arange(0, 1.1, step=0.25), fontsize=15)
plt.xticks(fontsize=15)

# use the below line if xticks overlap, switch between 4 and 6
# plt.locator_params( axis='x', nbins=6)

plt.grid(axis = 'y')

# saving figure and showing
print("saving plot...")
plt.savefig("fb-wall_ecdf.png")
print(file_name[23:], "plot created")
# plt.show()
