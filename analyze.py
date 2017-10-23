import matplotlib.pyplot as plt
import numpy as np 

noInter = []
scatter = []
breakup = []

def load(project,filename):
    '''Strip unnecessary data from the output and return an array of data'''
    data = []
    file = open('../' +project +'-build/' +filename)
    file = file.read().splitlines()
    for line in file:
        if "Out: " in line:
            line = line.split("Out: ")
            line = line[1].split(',')
            for i in range(len(line)):
                line[i] = line[i].strip()
                line[i] = float(line[i])
            data.append(line)
    return data

def discrim(data):
    for i in range(0,len(data)):
        if data[i][4] == 0 and data[i][0] <= 14.03:
            noInter.append(data[i])
        if data[i][4] == 1 and data[i][0] <= 14.03:
            scatter.append(data[i])
        if data[i][4] == 2 and data[i][0] <= 14.03:
            breakup.append(data[i])

def hist(array, name, sub_number):
    energy = []
    for i in range(0,len(array)):
        if array[i][0]!=0:
            energy.append(array[i][0])
    #plt.subplot(1,2,sub_number)
    plt.title(name)
    plt.ylabel("Neutrons")
    plt.xlabel("Energy")    
    plt.hist(energy,bins=100,zorder=1)

def load_ENDF():
    x = []
    y = []
    endf = np.loadtxt("../Output/dn2n_endfb7_1.3_spec.txt")
    for i in range(0,len(endf)):
        x.append(endf[i][0])
        y.append(endf[i][1]*0.0015)
    plt.scatter(x,y,zorder=2)

discrim(load('d_breakup','test.txt'))
hist(breakup,'Breakup',1)
plt.show()

 