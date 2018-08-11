from vpython import * # must import visual or vis first
# import graphing features
# f1 = gcurve(color=color.cyan) # a graphics curve
# for x in arange(0, 8.05, 0.7): # x goes from 0 to 8
a = 0
while(a <= 100):
#     f1.plot(pos=(x,5*cos(2*x)*exp(-0.2*x))) # plot
    x = float(input())
    points(pos=[vector(-x,0,0), vector(x,0,0)], radius=5,color=color.red)
    a = a + 1
