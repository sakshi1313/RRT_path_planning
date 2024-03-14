import matplotlib.pyplot as plt
import matplotlib.animation as animation

points = [
(0, 0),
(9, 4),
(12, 13),
(16, 22),
(22, 16),
(30, 10),
(39, 13),
(45, 5),
(54, 9),
(58, 25)
]

obstacle_points = [
(51, 24),
(33, 49),
(41, 60),
(87, 93)
]

fig, ax = plt.subplots()
ax.set_xlim(0, 100)  
ax.set_ylim(0, 100)  

def update(frame):
    if frame < len(points) - 1:  
        ax.plot([points[frame][0], points[frame+1][0]], [points[frame][1], points[frame+1][1]], color='b', linestyle='-')
        ax.scatter(points[frame][0], points[frame][1], color='g')  
        ax.scatter(points[len(points)-1][0] - 1, points[len(points)-1][1], color='y');
    elif frame < len(points) + len(obstacle_points):  
        obstacle = obstacle_points[frame - len(points)]
        ax.scatter(obstacle[0], obstacle[1], color='r')  
    else:  
        ani.event_source.stop()


ani = animation.FuncAnimation(fig, update, frames=len(points) + len(obstacle_points), interval=600)


plt.xlabel('X')
plt.ylabel('Y')
plt.title('Path Animation from Start to Goal with Obstacles')
plt.show()
