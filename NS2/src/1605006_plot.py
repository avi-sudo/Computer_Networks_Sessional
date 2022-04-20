import matplotlib.pyplot as plt 

# line 1 points 
x1 = [10,20,30,40,50] 
y1 = [0.79,0.811,0.94,0.96,0.97] 
labels=[10,20,30,40,50]

# naming the x axis 
plt.xlabel('Number of Flows') 
# naming the y axis 
plt.ylabel('Packet Drop Ratio') 
plt.xticks(x1, labels, rotation ='horizontal')
# giving a title to my graph 
plt.title('Packet Drop Ratio vs Number of Flows') 
plt.grid(True)
#plt.rc('grid', linestyle="-", color='black')
plt.plot(x1,y1)
plt.scatter(x1,y1)

# function to show the plot 
plt.show() 

