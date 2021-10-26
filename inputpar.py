# DECLARATION FILE FOR THE PARAMETERS THAT DEFINE THE SYSTEM AND/OR ARE COMMON TO ALL THE AGENTS
# it is *convenient* to define those parameters here, but not mandatory
# everything can also be directly defined in generate_0xml.py and generate_more_sporos_xml.py, but it can be redundant

sp_per_side = 6
# number of sporozoite per side of the simulation box
# sporozoites are initially arranged with their centres of mass distributed on a square lattice in the simulation box
# orientations are randomly sampled
# this is implemented in generate_0xml

n_sporos = int(pow(sp_per_side,2))
# total number of sporozoites in the simulation

nodes_tot = 11
# number of nodes composing each sporozoite

Ltot = 10
# total length of the sporozoite, in micrometers

w = 0.45
# width of a sporozoite, in micrometers

sidex = 60
sidey = 60
# side of the simulation box, in micrometers
# determines the periodic boundaries 

sidebox = 0
# it is possible to define a box inside the simulation box
# this is useful to study situations of confinement, bypassing the periodic boundaries (see f_forces_conf_F in forces.c)

simseed = 9040
# seed for the random number generator
# important to choose a different one for each simulation

fp = 1 #Force
fvar=0 #Variance of force
# propulsion force associated to each node

kel = 30
# elastic constant of the spring connecting two consecutive nodes

kth = 4
# elastic constant of the spring regulating the bending at each node

ki = 0.02
# height of the LJ potential associated to each node

k0 = 0.2
kvar = 0.0
# rest curvature of each agent and its variance
# this parameter is used in generate_0xml.py to compute angles and distances between the nodes

dt = 0.01
# integration time step

noise = 0.0001

prd = 100
phase = 1
# phase and period of output production
# xml output is produced at prd*n+phase, with n positive integer

# ----- WRITE OUTPUT FILES SOMEWHERE ELSE -----

import os

name_outdata = "Data"

if not os.path.exists(name_outdata):
    os.makedirs(name_outdata)

path_to_outdata = name_outdata + "/"
