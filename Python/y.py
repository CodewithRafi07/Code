import matplotlib.pyplot as plt
import numpy as np

# 1. Define the Vectors
# u = [a, b, c]
u_coords = np.array([3, 4, 1])
# v = [a', b', c']
v_coords = np.array([1, 2, 4])

# 2. Calculate the Resultant Vector
w_coords = u_coords + v_coords  # u + v = [4, 6, 5]

# --- Plotting Setup ---
fig = plt.figure(figsize=(10, 8))
# Create the 3D axes
ax = fig.add_subplot(111, projection='3d')
origin = [0, 0, 0] # Start point for vectors u and u+v

# --- 3. Plotting the Vectors (Solid Arrows) ---

# Plot Vector u (blue) from the origin
ax.quiver(*origin, *u_coords, color='blue', arrow_length_ratio=0.1, label='u', linewidth=2)
# Label the endpoint of u
ax.text(u_coords[0] + 0.1, u_coords[1] + 0.1, u_coords[2], f'({u_coords[0]}, {u_coords[1]}, {u_coords[2]})', color='black')

# Plot Resultant Vector u + v (red) from the origin
ax.quiver(*origin, *w_coords, color='red', arrow_length_ratio=0.08, label='u + v', linewidth=3)
# Label the endpoint of u + v
ax.text(w_coords[0] + 0.1, w_coords[1] + 0.1, w_coords[2], f'({w_coords[0]}, {w_coords[1]}, {w_coords[2]})', color='red')

# --- 4. Plotting the Parallelogram (Dashed Lines) ---

# Plot Vector v (grey dashed) from the origin, representing the (a', b', c') point
ax.quiver(*origin, *v_coords, color='grey', linestyle='--', arrow_length_ratio=0.1, label='v', linewidth=1)
# Label the endpoint of v
ax.text(v_coords[0] + 0.1, v_coords[1] + 0.1, v_coords[2], f'({v_coords[0]}, {v_coords[1]}, {v_coords[2]})', color='black')

# Plot Vector v (black dashed) starting from the HEAD of u (to complete the addition)
ax.quiver(*u_coords, *v_coords, color='black', linestyle='--', arrow_length_ratio=0.1, linewidth=1)

# Plot Vector u (black dashed) starting from the HEAD of v (to complete the parallelogram)
ax.quiver(*v_coords, *u_coords, color='black', linestyle='--', arrow_length_ratio=0.1, linewidth=1)

# --- 5. Formatting and Display ---

# Dynamically set axis limits
max_val = max(np.max(w_coords) + 1, 5)
ax.set_xlim([0, max_val])
ax.set_ylim([0, max_val])
ax.set_zlim([0, max_val])

# Set axis labels (matching the image)
ax.set_xlabel('x')
ax.set_ylabel('y')
ax.set_zlabel('z')

# Set the view angle for better visualization
ax.view_init(elev=20, azim=45)

plt.title('3D Vector Addition (u + v)')
plt.legend()
plt.show()