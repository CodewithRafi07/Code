import turtle
import time

# --- Configuration ---
# Set the length of each side of the star
side_length = 200

# Set the angle to turn at each point of the star
# A regular pentagram (5-pointed star) requires an external angle of 144 degrees.
turn_angle = 144

# --- Setup the Drawing Environment ---
# Create the drawing screen
screen = turtle.Screen()
screen.setup(width=600, height=600)
screen.title("Python Turtle Star Drawing")
screen.bgcolor("white") # Set background color

# Create the Turtle object (the pen that draws)
pen = turtle.Turtle()
pen.speed(3) # Set the drawing speed (1=slowest, 10=fastest, 0=no delay)
pen.color("blue") # Set the color of the pen/line
pen.pensize(3) # Set the thickness of the line

# --- Drawing Logic ---
# A star has 5 sides. We use a loop to repeat the drawing action 5 times.
# Each time, the pen moves forward, then turns 144 degrees.

# Start filling the shape with color (optional, but makes it look nice)
pen.begin_fill()
pen.fillcolor("gold") # Set the fill color

for _ in range(5):
    # 1. Move forward by the defined length
    pen.forward(side_length)
    
    # 2. Turn right by the star angle (144 degrees)
    pen.right(turn_angle)

# Stop filling the shape
pen.end_fill()

# --- Cleanup and Completion Message ---
# Hide the turtle icon after drawing is complete
pen.hideturtle()

# Wait for a moment before closing the window (so you can see the result)
time.sleep(3)

# Close the turtle graphics window when clicked
# Note: For some environments, screen.mainloop() or screen.exitonclick() might be better.
# We'll use exitonclick() to ensure the window doesn't immediately close.
screen.exitonclick()

# If running this in a constrained environment where input is not allowed, 
# you might need to comment out the line above and just let the script finish.