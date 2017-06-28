import json
import numpy
from pprint import pprint

#with open('data.json') as data_file:    
#    data = json.load(data_file)   
    
#/*pressure = data["pressure"]
#
#pprint(data)
#pprint(pressure)
#pprint(pressure[0])
#pprint(pressure[0].keys())
#pprint(pressure[0]['column'])
#pprint(pressure[0]['column'][0])

#print('\n')

with open('data2.json') as data_file2:    
    data2 = json.load(data_file2) 


#pprint(data2) #print dict
#pprint(data2["row0"]) #print row
#pprint(data2["row0"][0]) #print element

pmatrix = numpy.zeros(shape=(4,4))

row0 = data2["row0"]
row1 = data2["row1"]
row2 = data2["row2"]
row3 = data2["row3"]

pmatrix[0] = row0
pmatrix[1] = row1
pmatrix[2] = row2
pmatrix[3] = row3

pprint(pmatrix) 
pprint("Pressure Matrix[4][2] = ")
pprint(pmatrix[3][2])


