import numpy
from numpy import mat, zeros
from math import pi as PI
from math import cos, sin, fabs, sqrt

# Inner Orientation Element(IOE)
x0 = 0
y0 = 0
f = 153.24

# number of point pairs
num_point = 4
# Outer Orientation Element(OOE)
Xs0 = 38000e+3
Ys0 = 27000e+3
Zs0 = 50000 * f
phi0 = 0
omega0 = 0
kappa0 = 0

# define the Rotation matrix
class R(object):
    def __init__(self, phi, omega, kappa):
        a1 = cos(phi) * cos(kappa) - sin(phi) * sin(omega) * sin(kappa)
        a2 = -cos(phi) * sin(kappa) - sin(phi) * sin(omega) * cos(kappa)
        a3 = -sin(phi) * cos(omega)
        b1 = cos(omega) * sin(kappa)
        b2 = cos(omega) * cos(kappa)
        b3 = -sin(omega)
        c1 = sin(phi) * cos(kappa) + cos(phi) * sin(omega) * sin(kappa)
        c2 = -sin(phi) * sin(kappa) + cos(phi) * sin(omega) * cos(kappa)
        c3 = cos(phi) * cos(omega)
        self.R = mat([[a1, a2, a3],
                     [b1, b2, b3],
                     [c1, c2, c3]])

# define the optimization model
class model(object):
    def __init__(self):
        # init the OOE
        self.Xs = Xs0
        self.Ys = Ys0
        self.Zs = Zs0
        self.phi = phi0
        self.omega = omega0
        self.kappa = kappa0

        # init the equation parameters
        self.x = mat(zeros((6, 1)))
        self.A = mat(zeros((2 * num_point, 6)))
        self.l = mat(zeros((2 * num_point, 1)))

        # init the Rotation matrix
        self.R = R(phi0, omega0, kappa0).R

    def train(self, coordinate):
        # coordinate: the list of given coordinate pair

        # update the parameters matrix A
        for i in range(self.A.shape[0]):
            for j in range(self.A.shape[1]):
                if (i + 1) % 2 == 1:
                    self.A[i, j] = self.get_A(10 + j + 1, coordinate[int(i/2)])
                else:
                    self.A[i, j] = self.get_A(20 + j + 1, coordinate[int((i - 1)/2)])
        # update the parameters matrix l
        for i in range(self.l.shape[0]):
            if (i + 1) % 2 == 1:
                self.l[i] = self.get_l(11, coordinate[int(i/2)])
            else:
                self.l[i] = self.get_l(21, coordinate[int((i - 1)/2)])

        # calculate the ΔXs and so on
        self.x = (self.A.T * self.A).I * (self.A.T * self.l)

        # update the value
        self.Xs = float(self.Xs + self.x[0])
        self.Ys = float(self.Ys + self.x[1])
        self.Zs = float(self.Zs + self.x[2])
        self.phi = float(self.phi + self.x[3])
        self.omega = float(self.omega + self.x[4])
        self.kappa = float(self.kappa + self.x[5])
        self.R = R(self.phi, self.omega, self.kappa).R

    def get_A(self, num, pair_coordinate):

        [Xbar, Ybar, Zbar] = self.R.T * mat([[pair_coordinate['ground_coordinate'][0] - self.Xs],
                                             [pair_coordinate['ground_coordinate'][1] - self.Ys],
                                             [pair_coordinate['ground_coordinate'][2] - self.Zs]])
        tem_x = pair_coordinate['image_coordinate'][0] - x0
        tem_y = pair_coordinate['image_coordinate'][1] - y0
        # tem_x = float(- f * Xbar / Zbar)
        # tem_y = float(- f * Ybar / Zbar)

        if num == 11:
            return (self.R[0, 0] * f + self.R[0, 2] * tem_x) / Zbar
        elif num == 12:
            return (self.R[1, 0] * f + self.R[1, 2] * tem_x) / Zbar
        elif num == 13:
            return (self.R[2, 0] * f + self.R[2, 2] * tem_x) / Zbar
        elif num == 14:
            return tem_y * sin(self.omega) - ((tem_x/f) * (tem_x * cos(self.kappa) + tem_y * sin(self.kappa)) + f * cos(self.kappa)) * cos(self.omega)
        elif num == 15:
            return -f * sin(self.kappa) - (tem_x/f) * (tem_x * sin(self.kappa) + tem_y * cos(self.kappa))
        elif num == 16:
            return tem_y

        elif num == 21:
            return (self.R[0, 1] * f + self.R[0, 2] * tem_y) / Zbar
        elif num == 22:
            return (self.R[1, 1] * f + self.R[1, 2] * tem_y) / Zbar
        elif num == 23:
            return (self.R[2, 1] * f + self.R[2, 2] * tem_y) / Zbar
        elif num == 24:
            return - tem_x * sin(self.omega) - ((tem_y/f) * (tem_x * cos(self.kappa) + tem_y * sin(self.kappa)) - f * sin(self.kappa)) * cos(self.omega)
        elif num == 25:
            return -f * cos(self.kappa) - (tem_y/f) * (tem_x * sin(self.kappa) + tem_y * cos(self.kappa))
        elif num == 26:
            return - tem_x

        else:
            print("A index error")

    def get_l(self, num, pair_coordinate):
        [Xbar, Ybar, Zbar] = self.R.T * mat([[pair_coordinate['ground_coordinate'][0] - self.Xs],
                                             [pair_coordinate['ground_coordinate'][1] - self.Ys],
                                             [pair_coordinate['ground_coordinate'][2] - self.Zs]])
        if num == 11:
            return pair_coordinate['image_coordinate'][0] - (x0 - f * Xbar / Zbar)
        if num == 21:
            return pair_coordinate['image_coordinate'][1] - (y0 - f * Ybar / Zbar)

if __name__ == "__main__":
    # instantiate a model
    model = model()

    # input the coordinate
    coordinate = [{'image_coordinate': [-86.15, -68.99],
                   'ground_coordinate':[36589.41e+3, 25273.32e+3, 2195.17e+3]},
                  {'image_coordinate': [-53.40, 82.21],
                   'ground_coordinate': [37631.08e+3, 31324.51e+3, 728.69e+3]},
                  {'image_coordinate': [-14.78, -76.63],
                   'ground_coordinate': [39100.97e+3, 24934.98e+3, 2386.50e+3]},
                  {'image_coordinate': [10.46, 64.43],
                   'ground_coordinate': [40426.54e+3, 30319.81e+3, 757.31e+3]}
                  ]

    #gain the pre OOE
    pre_XS = model.Xs
    pre_Ys = model.Ys
    pre_Zs = model.Zs
    pre_phi = model.phi
    pre_omega = model.omega
    pre_kappa = model.kappa

    # train
    for i in range(10000):
        model.train(coordinate)

        if fabs(pre_XS - model.Xs) < 0.001 and fabs(pre_Ys - model.Ys) < 0.001 and fabs(pre_Zs - model.Zs) < 0.001 and fabs(pre_phi - model.phi) < 1e-6 and fabs(pre_omega - model.omega) < 1e-6 and fabs(pre_kappa - model.kappa) < 1e-6:
            print("The conditions have been met.")
            break
        else:
            # update the pre OOE
            pre_XS = model.Xs
            pre_Ys = model.Ys
            pre_Zs = model.Zs

    print('Xs: {}, \nYs: {}, \nZs: {}, \nphi: {}, \nomega: {}, \nkappa: {}'.format(model.Xs/1000, model.Ys/1000, model.Zs/1000, model.phi, model.omega, model.kappa))
    R = R(model.phi, model.omega, model.kappa).R
    print("R: ",R)

    # calculate the mess
    V = model.A * model.x - model.l
    sigma = sqrt((V.T * V)/(2 * num_point - 6))
    print("sigma: ", sigma)
    Qxx = (model.A.T * model.A).I
    mess = sigma * numpy.sqrt(Qxx)
    print("mess: ", mess[0], "\n", mess[1], "\n", mess[2], "\n", mess[3], "\n", mess[4], "\n", mess[5])
