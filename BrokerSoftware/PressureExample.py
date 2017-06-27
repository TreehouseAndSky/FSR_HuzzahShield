import json
from pprint import pprint

with open('data.json') as data_file:    
    data = json.load(data_file)   

pressure = data["pressure"]

pprint(data)
pprint(pressure)
pprint(pressure[0])
pprint(pressure[0].keys())
pprint(pressure[0]['column'])
pprint(pressure[0]['column'][0])

print('\n')

with open('data2.json') as data_file2:    
    data2 = json.load(data_file2) 

#matrix[0] = data2["1"];
#matrix[1] = data2["2"];
#matrix[2] = data2["3"];
#matrix[3] = data2["4"];

pprint(data2)
pprint(data2["row0"])
pprint(data2["row0"][0])

var0x0 = data2["row0"][0]
pprint(var0x0)

row0 = data["row0"]
row1 = data["row1"]
row2 = data["row2"]
row3 = data["row3"]

matrix[0] = row0
matrix[1] = row1
matrix[2] = row2
matrix[3] = row3

