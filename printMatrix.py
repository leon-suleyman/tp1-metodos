import matplotlib 
import matplotlib.pyplot as plt 


m = 10
n = 20
matrixA = []
xPoints = []
yPoints = []
for y in range(0,(n-1)):
    for x in range(0,m):
        row = []
        for k in range(0,n-1):
            for j in range(0,(m)):
                if k == y :
                    if j-1==x:
                        row.append(1)
                        xPoints.append(k*(n-1) + j)
                        yPoints.append(y*(n+1) + x)
                    if j==x:
                        row.append(2)
                        xPoints.append(k*(n-1) + j)
                        yPoints.append(y*(n+1) + x)
                    elif j+1==x:
                        row.append(3)
                        xPoints.append(k*(n-1) + j)
                        yPoints.append(y*(n+1) + x)
                elif j==x :
                    if k-1==y or (y == 0 and k==n-1):
                        row.append(4)
                        xPoints.append(k*(n-1) + j)
                        yPoints.append(y*(n+1) + x)
                    elif k+1==y or (y == n-1 and k == 0):
                        row.append(4)
                        xPoints.append(k*(n-1) + j)
                        yPoints.append(y*(n+1) + x)
                else:
                    row.append(0)
        matrixA.append(row)

plt.plot(xPoints, yPoints,'o')
plt.show()
