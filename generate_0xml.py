# THIS FILES SETS UP THE INITIAL CONDITION OF THE SYSTEM
# the python script is an automated way to create the file 0.xml fullfilling the requirements described in the FLAME user guide

import xml.etree.cElementTree as ET
import numpy as np
import math
import random as rnd

import inputpar as ipt
# all the parameters defined in inputpar.py are available here, with prefix ipt.

#---------------
rnd.seed(ipt.simseed) #seed for random number
#---------------

#----- SYSTEM DEFINITION

root = ET.Element("states")

itno = ET.SubElement(root, "itno")
itno.text = "0"
# at iteration 0

environment = ET.SubElement(root, "environment")
NNODES = ET.SubElement(environment, "NNODES")
NNODES.text = str(ipt.nodes_tot)

WIDTH = ET.SubElement(environment, "WIDTH")
WIDTH.text = str(ipt.w)

DT = ET.SubElement(environment, "DT")
DT.text = str(ipt.dt)

NOISE = ET.SubElement(environment, "NOISE")
NOISE.text = str(ipt.noise)

DELTA = ET.SubElement(environment, "DELTA")
DELTA.text = str(4*ipt.w)
# delta is the border that must be added to the bounding box in order to compute the neighbour box
# see fig 3.5 in the phd thesis
# see read_states.c

KEL = ET.SubElement(environment, "KEL")
KEL.text = str(ipt.kel)
KI = ET.SubElement(environment, "KI")
KI.text = str(ipt.ki)
KTH = ET.SubElement(environment, "KTH")
KTH.text = str(ipt.kth)

SIDEBOX = ET.SubElement(environment, "SIDEBOX")
SIDEBOX.text = str(ipt.sidebox)
SIDEX = ET.SubElement(environment, "SIDEX")
SIDEX.text = str(ipt.sidex)
SIDEY = ET.SubElement(environment, "SIDEY")
SIDEY.text = str(ipt.sidey)

agents = ET.SubElement(root, "agents")

# for each agent in the simulation
for ii in range(1,ipt.n_sporos+1):

    #k0 = ipt.k0
    k0=rnd.gauss(ipt.k0,ipt.fvar)#guass
    #k0=np.random.gamma(ipt.k0,ipt.kvar)#gamma    

    r0 = (2/k0) * math.sin( 0.5 * ipt.Ltot * k0 / (ipt.nodes_tot - 1) )
    # computes r0, the rest distance between two consecutive nodes
    th0 = math.pi - 2 * math.asin( 0.5 * k0 * r0 )
    # computes th0, the rest angle at each node

    rad = r0 / (2 * np.sin(0.5 * (math.pi - th0)))
    # radius of the circle that fits the sporozoite
    alpha = math.pi - th0
    v0 = [rad, 0, 0]
    #-----------------------------

    xagent = ET.SubElement(agents,"xagent")
    name = ET.SubElement(xagent,"name")
    name.text = "Sporo"
    id = ET.SubElement(xagent,"id")
    id.text = str(ii)
    iter = ET.SubElement(xagent,"iter")
    iter.text = "0"

    FP = ET.SubElement(xagent,"FP")
    #For Force Distribution       
    Fval=rnd.gauss(ipt.fp,ipt.fvar)
    while Fval<0.05:
		Fval=rnd.gauss(ipt.fp,ipt.fvar)
    FP.text = str(Fval)
    
    #For mixed polarity
    
    #FP.text = str(ipt.fp)
    #while rnd.random()<0.5:
	#	FP.text = str(-ipt.fp)        
    
    #------------------------------

    K0 = ET.SubElement(xagent,"K0")
    K0.text = str(k0)
    TH0 = ET.SubElement(xagent,"TH0")
    TH0.text = str(th0)
    R0 = ET.SubElement(xagent,"R0")
    R0.text = str(r0)
    #TAU = ET.SubElement(xagent,"TAU")
    #TAU.text ="{"+str(ipt.tau)+"}"

    ex_coordx = "{";
    ex_coordy = "{";

    rnd_ang = 2 * math.pi * rnd.random()

    if ipt.sidebox==0:
        rnd_x = 0.5*ipt.sidex/ipt.sp_per_side + (ii%ipt.sp_per_side)*ipt.sidex/ipt.sp_per_side
        rnd_y = 0.5*ipt.sidey/ipt.sp_per_side + (ii//ipt.sp_per_side)*ipt.sidey/ipt.sp_per_side
    else:
        rnd_x = 0.5*(float(SIDEX.text)-sidebox) + sidebox*rnd.random()
        rnd_y = 0.5*(float(SIDEY.text)-sidebox) + sidebox*rnd.random()

    # --- start: positioning and orientation of the agents
    # coordinates and velocities
    # computes the center of mass
    v = [0,0,0]
    for jj in range(0,ipt.nodes_tot):
        ang = jj*alpha + rnd_ang
        Mrot = [[np.cos(ang), np.sin(ang), 0],[-np.sin(ang), np.cos(ang), 0],[0, 0, 1]]
        hv = np.dot(Mrot,v0)
        v = v + hv
    
    cm = []
    cm.insert(0,v[0]/(1.0*ipt.nodes_tot))
    cm.insert(1,v[1]/(1.0*ipt.nodes_tot))
    cm.insert(2,0)

    # translates the centre of mass in the box (periodic boundaries)
    for jj in range(0,ipt.nodes_tot):
        ang = jj*alpha + rnd_ang
        Mrot = [[np.cos(ang), np.sin(ang), 0],[-np.sin(ang), np.cos(ang), 0],[0, 0, 1]]
        v = np.dot(Mrot,v0)
                
        v = v - cm + [rnd_x, rnd_y, 0]
        if (jj == ipt.nodes_tot-1):
            ex_coordx = ex_coordx + str(v[0]) + "}"
            ex_coordy = ex_coordy + str(v[1]) + "}"
            #ex_velx = ex_velx + "0}"
            #ex_vely = ex_vely + "0}"
        else:
            ex_coordx = ex_coordx + str(v[0]) + ","
            ex_coordy = ex_coordy + str(v[1]) + ","
    # --- end: positioning and orientation of the agents

    # cartesian coordinates of the nodes
    coordx = ET.SubElement(xagent,"coordx")
    coordx.text = ex_coordx
    coordy = ET.SubElement(xagent,"coordy")
    coordy.text = ex_coordy

    # dynamic arrays for the cartesian coordinates of the neighbours
    for kk in range(0,ipt.nodes_tot):
        nbrx = ET.SubElement(xagent,"xnbr_node" + str(kk))
        nbrx.text = "{0}"

    for kk in range(0,ipt.nodes_tot):
        nbry = ET.SubElement(xagent,"ynbr_node" + str(kk))
        nbry.text = "{0}"

#----- OUTPUT DEFINITION

outputs = ET.SubElement(root,"outputs")

output = ET.SubElement(outputs,"output")

type = ET.SubElement(output,"type")
type.text = "snapshot"

format = ET.SubElement(output,"format")
format.text = "xml"

location = ET.SubElement(output,"location")
location.text = ipt.path_to_outdata

time = ET.SubElement(output,"time")

period = ET.SubElement(time,"period")
period.text = str(ipt.prd)

phase = ET.SubElement(time,"phase")
phase.text = str(ipt.phase)

#---------------------------------------

tree = ET.ElementTree(root)
tree.write("0.xml")

#----- PATCH: there is an empty tag that creates problems
# these lines scan the file to replace the empty tag

# loading file and replacing
def replace_all(text, dic):
    for i, j in dic.iteritems():
        text = text.replace(i, j)
    return text

f_dic = {' />':'></location>'}

with open('0.xml','r') as f:
    text=f.read()
    with open('0.xml','w') as w:
        text=replace_all(text,f_dic)
        w.write(text)
