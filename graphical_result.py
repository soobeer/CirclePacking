import matplotlib.pyplot as plt
import matplotlib.patches as mpatches

filename = 'YOUR_PATH/circles.txt'

data = []
with open(filename, 'r') as coords_data:
    lines = coords_data.readlines()
    for line in lines:
        cline = line.strip('\n').split(' ')
        info = []
        for n in cline:
            info.append(float(n))
        data.append(info)

#print(data)

fig, ax = plt.subplots()
rectangle = mpatches.Rectangle((-data[0][0]/2, -data[0][1]/2), data[0][0], data[0][1], color = "lightgrey")
ax.add_patch(rectangle)

index = 1
while index < len(data):
    circle = mpatches.Circle((data[index][1], data[index][2]), data[index][0], ec = "blue", color = "cornflowerblue")
    ax.add_patch(circle)
    index += 1

plt.axis('equal')
plt.axis('off')
plt.grid()
plt.tight_layout()

plt.show()
