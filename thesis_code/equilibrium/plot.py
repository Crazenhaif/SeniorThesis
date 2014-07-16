import numpy
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D



def main():
    fig = plt.figure()
    ax = fig.add_subplot(111,projection='3d')
    data = numpy.genfromtxt('pos.1e12.0.2722.txt')
    x = data[:,0]
    y = data[:,1]
    z = data[:,2]
    ax.scatter(x,y,z)
    plt.show()
    
main()
