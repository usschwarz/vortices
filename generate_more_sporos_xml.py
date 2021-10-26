# THIS FILES DESCRIBES THE SYSTEM
# the python script is an automated way to create the file more_sporos.xml fullfilling the requirements described in the FLAME user guide
# for the description of the various parts in this file see the FLAME user guide
# basically here are defined: the system parameters, the agents and their variables, the messages that the agents must transmit and receive, the sequence of functions each agent goes through at each iteration

import xml.etree.cElementTree as ET
import inputpar as ipt

################## BEGIN

root = ET.Element("xmodel")
root.set("version","2")

name = ET.SubElement(root, "name")
name.text = "More Spos"

version = ET.SubElement(root, "version")
version.text = "01"

################## environment

environment = ET.SubElement(root, "environment")

################## environment -> constants

constants = ET.SubElement(environment,"constants")

variable = ET.SubElement(constants,"variable")
type = ET.SubElement(variable,"type")
type.text = "int"
name = ET.SubElement(variable,"name")
name.text = "NNODES"

variable = ET.SubElement(constants,"variable")
type = ET.SubElement(variable,"type")
type.text = "float"
name = ET.SubElement(variable,"name")
name.text = "WIDTH"

variable = ET.SubElement(constants,"variable")
type = ET.SubElement(variable,"type")
type.text = "float"
name = ET.SubElement(variable,"name")
name.text = "DT"

variable = ET.SubElement(constants,"variable")
type = ET.SubElement(variable,"type")
type.text = "float"
name = ET.SubElement(variable,"name")
name.text = "NOISE"

variable = ET.SubElement(constants,"variable")
type = ET.SubElement(variable,"type")
type.text = "float"
name = ET.SubElement(variable,"name")
name.text = "DELTA"

variable = ET.SubElement(constants,"variable")
type = ET.SubElement(variable,"type")
type.text = "float"
name = ET.SubElement(variable,"name")
name.text = "KEL"

variable = ET.SubElement(constants,"variable")
type = ET.SubElement(variable,"type")
type.text = "float"
name = ET.SubElement(variable,"name")
name.text = "KI"

variable = ET.SubElement(constants,"variable")
type = ET.SubElement(variable,"type")
type.text = "float"
name = ET.SubElement(variable,"name")
name.text = "KTH"

variable = ET.SubElement(constants,"variable")
type = ET.SubElement(variable,"type")
type.text = "float"
name = ET.SubElement(variable,"name")
name.text = "SIDEBOX"

variable = ET.SubElement(constants,"variable")
type = ET.SubElement(variable,"type")
type.text = "int"
name = ET.SubElement(variable,"name")
name.text = "SIDEX"

variable = ET.SubElement(constants,"variable")
type = ET.SubElement(variable,"type")
type.text = "int"
name = ET.SubElement(variable,"name")
name.text = "SIDEY"

################## environment -> functionFiles

functionFiles = ET.SubElement(environment,"functionFiles")

file = ET.SubElement(functionFiles,"file")
file.text = "write_state.c"

file = ET.SubElement(functionFiles,"file")
file.text = "read_states.c"

file = ET.SubElement(functionFiles,"file")
file.text = "forcesLJ.c"

################## agents

agents = ET.SubElement(root,"agents")

################## agents -> xagents

xagent = ET.SubElement(agents,"xagent")

name = ET.SubElement(xagent,"name")
name.text = "Sporo"

memory = ET.SubElement(xagent,"memory")

variable = ET.SubElement(memory,"variable")
type = ET.SubElement(variable,"type")
type.text = "int"
name = ET.SubElement(variable,"name")
name.text = "id"

variable = ET.SubElement(memory,"variable")
type = ET.SubElement(variable,"type")
type.text = "int"
name = ET.SubElement(variable,"name")
name.text = "iter"

variable = ET.SubElement(memory,"variable")
type = ET.SubElement(variable,"type")
type.text = "float"
name = ET.SubElement(variable,"name")
name.text = "FP"

variable = ET.SubElement(memory,"variable")
type = ET.SubElement(variable,"type")
type.text = "float"
name = ET.SubElement(variable,"name")
name.text = "K0"

variable = ET.SubElement(memory,"variable")
type = ET.SubElement(variable,"type")
type.text = "float"
name = ET.SubElement(variable,"name")
name.text = "TH0"

variable = ET.SubElement(memory,"variable")
type = ET.SubElement(variable,"type")
type.text = "float"
name = ET.SubElement(variable,"name")
name.text = "R0"

#variable = ET.SubElement(memory,"variable")
#type = ET.SubElement(variable,"type")
#type.text = "float"
#name = ET.SubElement(variable,"name")
#name.text = "TAU[1]"#array of 1 length

variable = ET.SubElement(memory,"variable")
type = ET.SubElement(variable,"type")
type.text = "float"
name = ET.SubElement(variable,"name")
name.text = "coordx[" + str(ipt.nodes_tot) + "]"

variable = ET.SubElement(memory,"variable")
type = ET.SubElement(variable,"type")
type.text = "float"
name = ET.SubElement(variable,"name")
name.text = "coordy[" + str(ipt.nodes_tot) + "]"

#variable = ET.SubElement(memory,"variable")
#type = ET.SubElement(variable,"type")
#type.text = "float"
#name = ET.SubElement(variable,"name")
#name.text = "velx[" + str(ipt.nodes_tot) + "]"

#variable = ET.SubElement(memory,"variable")
#type = ET.SubElement(variable,"type")
#type.text = "float"
#name = ET.SubElement(variable,"name")
#name.text = "vely[" + str(ipt.nodes_tot) + "]"

#dynamic arrays to store the coordinates of the neighbours one for each node in x and y
for ii in range(0,ipt.nodes_tot):
    variable = ET.SubElement(memory,"variable")
    type = ET.SubElement(variable,"type")
    type.text = "float_array"
    name = ET.SubElement(variable,"name")
    name.text = "xnbr_node" + str(ii)

for ii in range(0,ipt.nodes_tot):
    variable = ET.SubElement(memory,"variable")
    type = ET.SubElement(variable,"type")
    type.text = "float_array"
    name = ET.SubElement(variable,"name")
    name.text = "ynbr_node" + str(ii)

################# functions

functions = ET.SubElement(xagent,"functions")

function = ET.SubElement(functions,"function")

name = ET.SubElement(function,"name")
name.text = "write_state"

currentState = ET.SubElement(function,"currentState")
currentState.text = "start"

nextState = ET.SubElement(function,"nextState")
nextState.text = "read_states"

outputs = ET.SubElement(function,"outputs")
output = ET.SubElement(outputs,"output")
messageName = ET.SubElement(output,"messageName")
messageName.text = "state"

function = ET.SubElement(functions,"function")

name = ET.SubElement(function,"name")
name.text = "read_states"

currentState = ET.SubElement(function,"currentState")
currentState.text = "read_states"

nextState = ET.SubElement(function,"nextState")
nextState.text = "forces"

inputs = ET.SubElement(function,"inputs")
input = ET.SubElement(inputs,"input")
messageName = ET.SubElement(input,"messageName")
messageName.text = "state"

function = ET.SubElement(functions,"function")

name = ET.SubElement(function,"name")
name.text = "forces"

currentState = ET.SubElement(function,"currentState")
currentState.text = "forces"

nextState = ET.SubElement(function,"nextState")
nextState.text = "end"

################## messages

messages = ET.SubElement(root,"messages")

################## messages -> message

message = ET.SubElement(messages,"message")

name = ET.SubElement(message,"name")
name.text = "state"

variables = ET.SubElement(message,"variables")

variable = ET.SubElement(variables,"variable")
type = ET.SubElement(variable,"type")
type.text = "int"
name = ET.SubElement(variable,"name")
name.text = "id"

variable = ET.SubElement(variables,"variable")
type = ET.SubElement(variable,"type")
type.text = "float"
name = ET.SubElement(variable,"name")
name.text = "coordx[" + str(ipt.nodes_tot) + "]"

variable = ET.SubElement(variables,"variable")
type = ET.SubElement(variable,"type")
type.text = "float"
name = ET.SubElement(variable,"name")
name.text = "coordy[" + str(ipt.nodes_tot) + "]"

##################

tree = ET.ElementTree(root)

tree.write("more_sporos.xml")
