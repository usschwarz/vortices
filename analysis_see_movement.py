import xml.etree.ElementTree as ET
import numpy as np
#------------------------------
import matplotlib
matplotlib.use("Agg")
#------------------------------
import matplotlib.pyplot as plt
from matplotlib.path import Path
import matplotlib.patches as patches
import re
import glob
import os
import math
#------------------------------
import matplotlib.animation as manimation
#------------------------------

def natural_sort(l):
    convert = lambda text: int(text) if text.isdigit() else text.lower()
    alphanum_key = lambda key: [ convert(c) for c in re.split('([0-9]+)', key) ]
    return sorted(l, key = alphanum_key)

list_of_files = []
for ii in range(1,10):
    #print ii
    hlist_of_files = glob.glob("Data/"+str(ii)+"*.xml")           # create the list of file
    list_of_files = list_of_files + hlist_of_files

list_of_files = natural_sort(list_of_files)

Min=0.0#farr.min()
Max=5.0#farr.max()
fact=100/(Max-Min)


cmap = plt.cm.get_cmap('jet', 100)
colors=cmap(np.arange(cmap.N))

radius = 0.45
rad = radius*2*0.8*72*6/80

import inputpar as input

#---------------
FFMpegWriter = manimation.writers['ffmpeg']
metadata = dict(title='Sporo Movie', artist='Matplotlib',
                comment='none')
writer = FFMpegWriter(fps=6, metadata=metadata, bitrate=6000)
#---------------

fig = plt.gcf()
ax = fig.add_subplot(111)
itera=0
#---------------
with writer.saving(fig, "Movie.mp4", 100):
#---------------
	for file_name in list_of_files:
		print file_name
		tree = ET.parse(file_name)
		root = tree.getroot()
		#--------------
		count = 0
		itera=itera+1
		#--------------

		for xagent in root.findall('xagent'):
			#--------------
			count = count + 1
			#--------------
			id = xagent.find('id').text
			#print int(id)%6
			FP=int(id)%6#float(xagent.find('FP').text)
			#print FP
			x = xagent.find('coordx').text
			y = xagent.find('coordy').text
			xx = re.findall("[-+]?\d*\.\d+|\d+", x)
			yy = re.findall("[-+]?\d*\.\d+|\d+", y)

			hx = np.array(xx).astype(np.float)
			hy = np.array(yy).astype(np.float) 
			
			
			# ------------------ start plotting ------------------


			for ii in range(0,len(hx)):
				circle = plt.Circle((hx[ii],hy[ii]),input.w,color=cmap(int(FP*fact)))
				ax.add_patch(circle)



		ax.set_aspect('equal')
		ax.set_title('time='+str(itera)+'s')
		ax.set_xlim(-10,input.sidex+10)
		ax.set_ylim(-10,input.sidey+10)
		ax.imshow([colors], extent=[0, input.sidex, input.sidex+8, input.sidex+10])#left, right, bottom, top           


		# ------------------ end plotting ------------------
		verts_rect = [(0,0),(0,input.sidey),(input.sidex,input.sidey),(input.sidex,0),(0,0)]
		codes = [Path.MOVETO,Path.LINETO,Path.LINETO,Path.LINETO,Path.CLOSEPOLY]
		path = Path(verts_rect, codes)
		patch = patches.PathPatch(path, facecolor = 'none', lw=2)
		ax.add_patch(patch)       
		print itera
		# ------------------ start plotting ------------------
		if count>0:
			writer.grab_frame()
			ax.cla()
		# ------------------ end plotting ------------------
