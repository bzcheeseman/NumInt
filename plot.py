import matplotlib.pyplot as plt
import numpy as np
import sys


def main(path):

    x0 = np.genfromtxt(path + "r0.tsv")
    x1 = np.genfromtxt(path + "r1.tsv")
    y0 = np.genfromtxt(path + "i0.tsv")
    y1 = np.genfromtxt(path + "i1.tsv")

    plt.figure()
    plt.title("Complex Plane")
    plt.plot(x0, y0, label = "Cavity 1 (Mode starts here)")
    plt.plot(x1, y1, label = "Cavity 2")
    plt.legend()
    plt.show()

    plt.figure()
    plt.title("Squared Amplitudes")
    plt.plot(xrange(len(x0)), np.sqrt(x0**2 + y0**2), label = "Cavity 1")
    plt.plot(xrange(len(x1)), np.sqrt(x1**2 + y1**2), label = "Cavity 2")
    plt.legend()
    plt.show()


if __name__ == "__main__":
    main(sys.argv[1])