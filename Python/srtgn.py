import matplotlib.pyplot as plt
import matplotlib.patches as patches
from matplotlib.widgets import Button
import numpy as np

class SizeMeasurementTool:
    def __init__(self):
        self.fig, self.ax = plt.subplots(figsize=(12, 8))
        plt.subplots_adjust(bottom=0.15)
        
        # Create sample image/grid
        self.create_sample_image()
        
        # Measurement state
        self.measurements = []
        self.current_rect = None
        self.start_point = None
        self.reference_size = 100  # pixels per unit
        
        # Connect events
        self.cid_press = self.fig.canvas.mpl_connect('button_press_event', self.on_press)
        self.cid_release = self.fig.canvas.mpl_connect('button_release_event', self.on_release)
        self.cid_motion = self.fig.canvas.mpl_connect('motion_notify_event', self.on_motion)
        
        # Add buttons
        ax_clear = plt.axes([0.3, 0.02, 0.15, 0.05])
        ax_reference = plt.axes([0.5, 0.02, 0.15, 0.05])
        
        self.btn_clear = Button(ax_clear, 'Clear All')
        self.btn_clear.on_clicked(self.clear_measurements)
        
        self.btn_reference = Button(ax_reference, 'Set Reference')
        self.btn_reference.on_clicked(self.set_reference)
        
        # Instructions
        self.ax.set_title('Size Measurement Tool\nClick and drag to measure | Reference: 100px = 1 unit', 
                         fontsize=12, pad=20)
        
        plt.show()
    
    def create_sample_image(self):
        """Create a sample grid background"""
        x = np.linspace(0, 10, 500)
        y = np.linspace(0, 10, 500)
        X, Y = np.meshgrid(x, y)
        Z = np.sin(X) * np.cos(Y)
        
        self.ax.imshow(Z, extent=[0, 500, 0, 500], cmap='gray', alpha=0.3)
        self.ax.set_xlim(0, 500)
        self.ax.set_ylim(0, 500)
        self.ax.set_aspect('equal')
        self.ax.grid(True, alpha=0.3)
    
    def on_press(self, event):
        """Handle mouse press event"""
        if event.inaxes != self.ax:
            return
        self.start_point = (event.xdata, event.ydata)
    
    def on_motion(self, event):
        """Handle mouse motion event"""
        if self.start_point is None or event.inaxes != self.ax:
            return
        
        # Remove previous temporary rectangle
        if self.current_rect is not None:
            self.current_rect.remove()
        
        # Draw new temporary rectangle
        x0, y0 = self.start_point
        width = event.xdata - x0
        height = event.ydata - y0
        
        self.current_rect = patches.Rectangle(
            (x0, y0), width, height,
            linewidth=2, edgecolor='red', facecolor='yellow', alpha=0.3
        )
        self.ax.add_patch(self.current_rect)
        self.fig.canvas.draw_idle()
    
    def on_release(self, event):
        """Handle mouse release event"""
        if self.start_point is None or event.inaxes != self.ax:
            return
        
        x0, y0 = self.start_point
        x1, y1 = event.xdata, event.ydata
        
        # Calculate dimensions
        width_px = abs(x1 - x0)
        height_px = abs(y1 - y0)
        width_units = width_px / self.reference_size
        height_units = height_px / self.reference_size
        area_units = width_units * height_units
        
        # Draw final rectangle
        rect = patches.Rectangle(
            (min(x0, x1), min(y0, y1)), width_px, height_px,
            linewidth=2, edgecolor='blue', facecolor='cyan', alpha=0.2
        )
        self.ax.add_patch(rect)
        
        # Add measurement label
        center_x = (x0 + x1) / 2
        center_y = (y0 + y1) / 2
        label = f'W: {width_units:.2f}u\nH: {height_units:.2f}u\nA: {area_units:.2f}u²'
        text = self.ax.text(center_x, center_y, label,
                           ha='center', va='center',
                           bbox=dict(boxstyle='round', facecolor='white', alpha=0.8),
                           fontsize=9, fontweight='bold')
        
        # Store measurement
        self.measurements.append({
            'rect': rect,
            'text': text,
            'width': width_units,
            'height': height_units,
            'area': area_units
        })
        
        # Print to console
        print(f"\nMeasurement #{len(self.measurements)}:")
        print(f"  Width:  {width_units:.2f} units ({width_px:.1f} pixels)")
        print(f"  Height: {height_units:.2f} units ({height_px:.1f} pixels)")
        print(f"  Area:   {area_units:.2f} square units")
        
        # Reset state
        self.current_rect = None
        self.start_point = None
        self.fig.canvas.draw()
    
    def clear_measurements(self, event):
        """Clear all measurements"""
        for measurement in self.measurements:
            measurement['rect'].remove()
            measurement['text'].remove()
        self.measurements.clear()
        print("\nAll measurements cleared!")
        self.fig.canvas.draw()
    
    def set_reference(self, event):
        """Set reference size from last measurement"""
        if not self.measurements:
            print("\nNo measurements yet! Draw a rectangle first.")
            return
        
        last = self.measurements[-1]
        # Assume last measurement represents 1 unit
        width_px = last['width'] * self.reference_size
        self.reference_size = width_px / 1.0
        
        print(f"\nReference updated: {self.reference_size:.1f} pixels = 1 unit")
        self.ax.set_title(f'Size Measurement Tool\nClick and drag to measure | Reference: {self.reference_size:.1f}px = 1 unit',
                         fontsize=12, pad=20)
        self.fig.canvas.draw()

# Run the tool
if __name__ == "__main__":
    print("=== Size Measurement Tool ===")
    print("Instructions:")
    print("1. Click and drag to draw measurement rectangles")
    print("2. Release to finalize the measurement")
    print("3. Use 'Clear All' to remove measurements")
    print("4. Use 'Set Reference' to calibrate scale from last measurement")
    print("\nStarting tool...")
    
    tool = SizeMeasurementTool()