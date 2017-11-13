import matplotlib.pyplot as plt
import numpy as np

noInter = []
scatter = []
breakup = []

def order(projectname):
    output = open(projectname +"/" +projectname +".txt")
    ordered_output = open(projectname +"/ordered_" +projectname +".txt","w")
    output = output.read().splitlines()
    for line in output:
        if "Out: " in line:
            line = line.split('Out: ')
            ordered_output.write(line[1]+"\n")

def load(projectname):
    data = np.loadtxt(projectname +"/ordered_" +projectname +".txt",delimiter=',')
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

def load_cross_sec(filename,x_scale,y_scale):
    x = []
    y = []
    endf = np.loadtxt(filename,dtype=float)
    for i in range(0,len(endf)):
        x.append(endf[i][0]*x_scale)
        y.append(endf[i][1]*y_scale)
    plt.scatter(x,y,zorder=2)
    

PROJECT_NAME = input("Project: ")
order(PROJECT_NAME)
discrim(load(PROJECT_NAME))
#hist(noInter, "noInter", PROJECT_NAME)
#hist(scatter, "scatter", 1)
hist(breakup, "breakup", 2)
load_cross_sec('dn2n_endfb7_1.3_spec.txt',1,0.0015)
load_cross_sec('(n,2n)_ENDF',0.0000001,1300)
plt.show()
#plt.savefig(PROJECT_NAME +"/" +PROJECT_NAME)
