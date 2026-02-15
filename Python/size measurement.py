import turtle
import time
import math

class ProfessionalAnatomyIllustration:
    def __init__(self):
        # Setup screen with larger size for detail
        self.screen = turtle.Screen()
        self.screen.setup(width=1200, height=1000)
        self.screen.bgcolor("#f5f5f0")
        self.screen.title("Professional Male Anatomy Medical Illustration")
        
        # Main drawing turtle
        self.pen = turtle.Turtle()
        self.pen.speed(0)
        self.pen.pensize(2)
        self.pen.hideturtle()
        
        # Shading turtle
        self.shader = turtle.Turtle()
        self.shader.speed(0)
        self.shader.hideturtle()
        
        # Label turtle
        self.label_pen = turtle.Turtle()
        self.label_pen.hideturtle()
        self.label_pen.penup()
        self.label_pen.speed(0)
        
        # Add professional title
        self.add_professional_header()
        
    def add_professional_header(self):
        """Add medical illustration header"""
        # Title box
        self.label_pen.goto(-500, 450)
        self.label_pen.pendown()
        self.label_pen.color("#2c3e50")
        self.label_pen.pensize(3)
        for _ in range(2):
            self.label_pen.forward(1000)
            self.label_pen.right(90)
            self.label_pen.forward(80)
            self.label_pen.right(90)
        self.label_pen.penup()
        
        self.label_pen.goto(0, 420)
        self.label_pen.color("#2c3e50")
        self.label_pen.write("MALE REPRODUCTIVE ANATOMY", 
                            align="center", font=("Arial", 20, "bold"))
        self.label_pen.goto(0, 390)
        self.label_pen.write("Professional Medical Illustration - Cross-Sectional View", 
                            align="center", font=("Arial", 12, "italic"))
        self.label_pen.goto(0, 370)
        self.label_pen.color("#e74c3c")
        self.label_pen.write("FOR MEDICAL EDUCATION & CLINICAL REFERENCE ONLY", 
                            align="center", font=("Arial", 10, "bold"))
    
    def draw_with_gradient(self, x, y, width, height, color1, color2, direction="vertical"):
        """Draw rectangle with gradient effect"""
        steps = 50
        self.shader.penup()
        
        for i in range(steps):
            ratio = i / steps
            if direction == "vertical":
                self.shader.goto(x, y - (height * ratio))
                line_width = width
                line_height = height / steps
            else:
                self.shader.goto(x + (width * ratio), y)
                line_width = width / steps
                line_height = height
            
            # Interpolate colors
            r1, g1, b1 = self.hex_to_rgb(color1)
            r2, g2, b2 = self.hex_to_rgb(color2)
            r = int(r1 + (r2 - r1) * ratio)
            g = int(g1 + (g2 - g1) * ratio)
            b = int(b1 + (b2 - b1) * ratio)
            
            color = f'#{r:02x}{g:02x}{b:02x}'
            self.shader.color(color)
            self.shader.pendown()
            self.shader.setheading(90 if direction == "vertical" else 0)
            self.shader.pensize(width / steps if direction == "vertical" else height)
            self.shader.forward(line_height if direction == "vertical" else line_width)
            self.shader.penup()
    
    def hex_to_rgb(self, hex_color):
        """Convert hex color to RGB tuple"""
        hex_color = hex_color.lstrip('#')
        return tuple(int(hex_color[i:i+2], 16) for i in (0, 2, 4))
    
    def draw_cross_hatching(self, x, y, width, height, angle, spacing=5):
        """Add cross-hatching for texture"""
        self.shader.penup()
        self.shader.goto(x, y)
        self.shader.pensize(0.5)
        self.shader.color("#8b7355")
        
        lines = int(width / spacing)
        for i in range(lines):
            self.shader.penup()
            self.shader.goto(x + i * spacing, y)
            self.shader.setheading(angle)
            self.shader.pendown()
            self.shader.forward(height)
    
    def draw_detailed_glans(self):
        """Draw highly detailed glans with corona"""
        print("Drawing: Glans penis (Head) with detailed corona...")
        
        # Main glans body
        self.pen.penup()
        self.pen.goto(0, -150)
        self.pen.pendown()
        self.pen.color("#d2691e")
        self.pen.begin_fill()
        self.pen.setheading(180)
        self.pen.circle(70, 180)
        
        # Add subtle curve to sides
        for angle in range(0, 180, 5):
            rad = math.radians(angle)
            x = -70 * math.cos(rad)
            y = -150 + 70 * math.sin(rad) * 0.8
            self.pen.goto(x, y)
        
        self.pen.goto(0, -150)
        self.pen.end_fill()
        
        # Corona ridge (prominent)
        self.pen.penup()
        self.pen.goto(-75, -145)
        self.pen.pendown()
        self.pen.color("#a0522d")
        self.pen.pensize(4)
        self.pen.setheading(0)
        
        # Draw wavy corona
        for i in range(36):
            angle = i * 10
            if i % 2 == 0:
                self.pen.circle(75, 10)
            else:
                self.pen.circle(73, 10)
        
        self.pen.pensize(2)
        
        # Add surface texture
        for i in range(8):
            angle = i * 45
            self.shader.penup()
            x = 30 * math.cos(math.radians(angle))
            y = -150 + 30 * math.sin(math.radians(angle)) * 0.5
            self.shader.goto(x, y)
            self.shader.dot(3, "#a0522d")
        
        # Urethral meatus (opening)
        self.pen.penup()
        self.pen.goto(0, -215)
        self.pen.pendown()
        self.pen.color("#ff69b4")
        self.pen.begin_fill()
        self.pen.setheading(0)
        for _ in range(2):
            self.pen.circle(8, 90)
            self.pen.circle(3, 90)
        self.pen.end_fill()
        
        # Frenulum
        self.pen.penup()
        self.pen.goto(0, -145)
        self.pen.pendown()
        self.pen.color("#e9967a")
        self.pen.pensize(3)
        self.pen.setheading(270)
        self.pen.forward(50)
        
        # Label
        self.add_detailed_label(120, -180, "Glans Penis", 
                               "Highly sensitive erectile tissue\nContains numerous nerve endings", 
                               (70, -180))
        self.add_detailed_label(90, -210, "Urethral Meatus", 
                               "External opening of urethra", 
                               (8, -215))
        time.sleep(0.5)
    
    def draw_detailed_shaft(self):
        """Draw shaft with cross-sectional internal anatomy"""
        print("Drawing: Penile shaft with internal structures...")
        
        # Outer skin layer
        self.pen.penup()
        self.pen.goto(-50, 100)
        self.pen.pendown()
        self.pen.color("#deb887")
        self.pen.begin_fill()
        self.pen.setheading(270)
        self.pen.forward(240)
        self.pen.goto(50, -140)
        self.pen.setheading(90)
        self.pen.forward(240)
        self.pen.goto(-50, 100)
        self.pen.end_fill()
        
        # Add skin texture
        self.draw_cross_hatching(-50, 100, 100, -240, 135, spacing=8)
        self.draw_cross_hatching(-50, 100, 100, -240, 45, spacing=8)
        
        # Corpus cavernosum (two chambers - left)
        self.pen.penup()
        self.pen.goto(-40, 80)
        self.pen.pendown()
        self.pen.color("#ff6b6b")
        self.pen.begin_fill()
        for _ in range(2):
            self.pen.forward(15)
            self.pen.right(90)
            self.pen.forward(200)
            self.pen.right(90)
        self.pen.end_fill()
        
        # Corpus cavernosum (right)
        self.pen.penup()
        self.pen.goto(25, 80)
        self.pen.pendown()
        self.pen.begin_fill()
        for _ in range(2):
            self.pen.forward(15)
            self.pen.right(90)
            self.pen.forward(200)
            self.pen.right(90)
        self.pen.end_fill()
        
        # Draw trabecular structure (spongy tissue)
        for i in range(20):
            y_pos = 80 - (i * 10)
            # Left chamber
            self.shader.penup()
            self.shader.goto(-38, y_pos)
            self.shader.pendown()
            self.shader.color("#cc5555")
            self.shader.pensize(1)
            self.shader.setheading(0)
            self.shader.forward(13)
            
            # Right chamber
            self.shader.penup()
            self.shader.goto(27, y_pos)
            self.shader.pendown()
            self.shader.forward(13)
        
        # Corpus spongiosum (surrounds urethra)
        self.pen.penup()
        self.pen.goto(-8, 80)
        self.pen.pendown()
        self.pen.color("#ffb6c1")
        self.pen.begin_fill()
        self.pen.setheading(270)
        self.pen.forward(200)
        self.pen.setheading(0)
        self.pen.forward(16)
        self.pen.setheading(90)
        self.pen.forward(200)
        self.pen.goto(-8, 80)
        self.pen.end_fill()
        
        # Urethra (central canal)
        self.pen.penup()
        self.pen.goto(-2, 80)
        self.pen.pendown()
        self.pen.color("#ffe4e1")
        self.pen.begin_fill()
        self.pen.setheading(270)
        self.pen.forward(200)
        self.pen.setheading(0)
        self.pen.forward(4)
        self.pen.setheading(90)
        self.pen.forward(200)
        self.pen.goto(-2, 80)
        self.pen.end_fill()
        
        # Tunica albuginea (fibrous covering)
        self.pen.penup()
        self.pen.goto(-42, 82)
        self.pen.pendown()
        self.pen.color("#f0e68c")
        self.pen.pensize(2)
        self.pen.setheading(270)
        self.pen.forward(205)
        
        self.pen.penup()
        self.pen.goto(42, 82)
        self.pen.pendown()
        self.pen.setheading(270)
        self.pen.forward(205)
        
        self.pen.pensize(2)
        
        # Add detailed labels
        self.add_detailed_label(-120, 30, "Corpus Cavernosum", 
                               "Erectile tissue chambers\nFill with blood during erection", 
                               (-32, 30))
        self.add_detailed_label(-120, -30, "Corpus Spongiosum", 
                               "Surrounds urethra\nMaintains urethral patency", 
                               (0, -30))
        self.add_detailed_label(100, 0, "Urethra", 
                               "Conveys urine and semen", 
                               (2, 0))
        time.sleep(0.5)
    
    def draw_detailed_scrotum_and_testes(self):
        """Draw detailed scrotum with internal testicular anatomy"""
        print("Drawing: Scrotum and testicular anatomy...")
        
        # Scrotal sac - left side
        self.pen.penup()
        self.pen.goto(-80, 100)
        self.pen.pendown()
        self.pen.color("#d2b48c")
        self.pen.begin_fill()
        self.pen.setheading(270)
        self.pen.forward(120)
        self.pen.circle(40, 180)
        self.pen.forward(120)
        self.pen.end_fill()
        
        # Right side
        self.pen.penup()
        self.pen.goto(80, 100)
        self.pen.pendown()
        self.pen.begin_fill()
        self.pen.setheading(270)
        self.pen.forward(120)
        self.pen.circle(-40, 180)
        self.pen.forward(120)
        self.pen.end_fill()
        
        # Median raphe (scrotal seam) with detail
        self.pen.penup()
        self.pen.goto(0, 100)
        self.pen.pendown()
        self.pen.color("#a0826d")
        self.pen.pensize(3)
        self.pen.setheading(270)
        for i in range(25):
            self.pen.forward(8)
            if i % 2 == 0:
                self.pen.left(2)
            else:
                self.pen.right(2)
        
        # Add scrotal rugae (wrinkles)
        for offset in [-20, 20]:
            for i in range(5):
                self.shader.penup()
                self.shader.goto(offset, 80 - i * 30)
                self.shader.pendown()
                self.shader.color("#c19a6b")
                self.shader.pensize(1)
                self.shader.setheading(270)
                for j in range(5):
                    self.shader.circle(offset * 0.3, 30)
                    self.shader.circle(-offset * 0.3, 30)
        
        # Left testis (detailed)
        self.pen.penup()
        self.pen.goto(-40, 150)
        self.pen.pendown()
        self.pen.color("#f5deb3")
        self.pen.begin_fill()
        
        # Oval shape for testis
        for i in range(36):
            angle = i * 10
            rad = math.radians(angle)
            x = -40 + 35 * math.cos(rad)
            y = 150 + 50 * math.sin(rad)
            self.pen.goto(x, y)
        self.pen.end_fill()
        
        # Testicular lobules (internal structure)
        for i in range(6):
            angle = i * 60
            rad = math.radians(angle)
            x_start = -40 + 10 * math.cos(rad)
            y_start = 150 + 15 * math.sin(rad)
            x_end = -40 + 30 * math.cos(rad)
            y_end = 150 + 43 * math.sin(rad)
            
            self.shader.penup()
            self.shader.goto(x_start, y_start)
            self.shader.pendown()
            self.shader.color("#daa520")
            self.shader.pensize(1.5)
            self.shader.goto(x_end, y_end)
        
        # Right testis
        self.pen.penup()
        self.pen.goto(40, 150)
        self.pen.pendown()
        self.pen.color("#f5deb3")
        self.pen.begin_fill()
        for i in range(36):
            angle = i * 10
            rad = math.radians(angle)
            x = 40 + 35 * math.cos(rad)
            y = 150 + 50 * math.sin(rad)
            self.pen.goto(x, y)
        self.pen.end_fill()
        
        # Right testicular lobules
        for i in range(6):
            angle = i * 60
            rad = math.radians(angle)
            x_start = 40 + 10 * math.cos(rad)
            y_start = 150 + 15 * math.sin(rad)
            x_end = 40 + 30 * math.cos(rad)
            y_end = 150 + 43 * math.sin(rad)
            
            self.shader.penup()
            self.shader.goto(x_start, y_start)
            self.shader.pendown()
            self.shader.color("#daa520")
            self.shader.pensize(1.5)
            self.shader.goto(x_end, y_end)
        
        # Epididymis (left)
        self.pen.penup()
        self.pen.goto(-75, 190)
        self.pen.pendown()
        self.pen.color("#ff8c00")
        self.pen.pensize(4)
        
        # Head of epididymis
        self.pen.circle(8)
        
        # Body (coiled structure)
        self.pen.penup()
        self.pen.goto(-75, 180)
        self.pen.pendown()
        self.pen.pensize(3)
        for i in range(8):
            self.pen.setheading(270)
            self.pen.circle(5, 180)
            self.pen.circle(-5, 180)
        
        # Epididymis (right)
        self.pen.penup()
        self.pen.goto(75, 190)
        self.pen.pendown()
        self.pen.pensize(4)
        self.pen.circle(8)
        
        self.pen.penup()
        self.pen.goto(75, 180)
        self.pen.pendown()
        self.pen.pensize(3)
        for i in range(8):
            self.pen.setheading(270)
            self.pen.circle(5, 180)
            self.pen.circle(-5, 180)
        
        # Vas deferens
        self.pen.penup()
        self.pen.goto(-75, 80)
        self.pen.pendown()
        self.pen.color("#8b4513")
        self.pen.pensize(3)
        self.pen.setheading(90)
        self.pen.forward(20)
        self.pen.circle(20, 90)
        self.pen.forward(30)
        
        self.pen.penup()
        self.pen.goto(75, 80)
        self.pen.pendown()
        self.pen.setheading(90)
        self.pen.forward(20)
        self.pen.circle(-20, 90)
        self.pen.forward(30)
        
        self.pen.pensize(2)
        
        # Labels
        self.add_detailed_label(-200, 150, "Testis", 
                               "Produces spermatozoa\nSecretes testosterone", 
                               (-75, 150))
        self.add_detailed_label(-200, 100, "Epididymis", 
                               "Stores and matures sperm\n20-day maturation period", 
                               (-85, 120))
        self.add_detailed_label(180, 150, "Testis", 
                               "Contains seminiferous tubules", 
                               (75, 150))
        self.add_detailed_label(150, 50, "Vas Deferens", 
                               "Transports sperm to urethra", 
                               (85, 80))
        time.sleep(0.5)
    
    def draw_blood_vessels(self):
        """Draw detailed vasculature"""
        print("Drawing: Vascular supply (arteries and veins)...")
        
        # Dorsal artery
        self.shader.penup()
        self.shader.goto(0, 100)
        self.shader.pendown()
        self.shader.color("#dc143c")
        self.shader.pensize(2)
        self.shader.setheading(270)
        
        for i in range(30):
            self.shader.forward(7)
            if i % 3 == 0:
                self.shader.left(2)
            else:
                self.shader.right(1)
        
        # Dorsal vein
        self.shader.penup()
        self.shader.goto(-5, 100)
        self.shader.pendown()
        self.shader.color("#4169e1")
        self.shader.pensize(2.5)
        self.shader.setheading(270)
        
        for i in range(30):
            self.shader.forward(7)
            if i % 3 == 0:
                self.shader.right(2)
            else:
                self.shader.left(1)
        
        # Testicular artery (left)
        self.shader.penup()
        self.shader.goto(-40, 200)
        self.shader.pendown()
        self.shader.color("#ff6347")
        self.shader.pensize(1.5)
        
        for i in range(10):
            self.shader.setheading(270)
            self.shader.forward(5)
            self.shader.circle(3, 90)
        
        # Testicular artery (right)
        self.shader.penup()
        self.shader.goto(40, 200)
        self.shader.pendown()
        
        for i in range(10):
            self.shader.setheading(270)
            self.shader.forward(5)
            self.shader.circle(-3, 90)
        
        # Small capillaries
        for x_offset in [-30, -10, 10, 30]:
            for i in range(5):
                self.shader.penup()
                self.shader.goto(x_offset, 80 - i * 40)
                self.shader.pendown()
                self.shader.color("#ff7f7f")
                self.shader.pensize(0.5)
                self.shader.setheading(45 if x_offset < 0 else 135)
                self.shader.forward(10)
        
        # Label
        self.add_detailed_label(100, 50, "Dorsal Vessels", 
                               "Artery (red) & Vein (blue)\nProvide blood supply", 
                               (2, 50))
        time.sleep(0.3)
    
    def draw_nerves(self):
        """Draw nerve supply"""
        print("Drawing: Nerve supply (dorsal nerve)...")
        
        # Dorsal nerve
        self.shader.penup()
        self.shader.goto(10, 100)
        self.shader.pendown()
        self.shader.color("#ffd700")
        self.shader.pensize(1.5)
        self.shader.setheading(270)
        
        for i in range(30):
            self.shader.forward(7)
            # Small branches
            if i % 5 == 0:
                current_pos = self.shader.position()
                current_heading = self.shader.heading()
                
                self.shader.setheading(current_heading + 45)
                self.shader.forward(8)
                
                self.shader.penup()
                self.shader.goto(current_pos)
                self.shader.pendown()
                self.shader.setheading(current_heading)
        
        # Pudendal nerve branches to scrotum
        for x_offset in [-50, 50]:
            self.shader.penup()
            self.shader.goto(x_offset, 120)
            self.shader.pendown()
            self.shader.setheading(270)
            for i in range(6):
                self.shader.forward(10)
                if i % 2 == 0:
                    # Branch
                    pos = self.shader.position()
                    head = self.shader.heading()
                    self.shader.setheading(head + (30 if x_offset < 0 else -30))
                    self.shader.forward(5)
                    self.shader.penup()
                    self.shader.goto(pos)
                    self.shader.pendown()
                    self.shader.setheading(head)
        
        # Label
        self.add_detailed_label(100, 80, "Dorsal Nerve", 
                               "Provides sensation\nBranches throughout", 
                               (12, 80))
        time.sleep(0.3)
    
    def add_detailed_label(self, x, y, title, description, arrow_to):
        """Add professional medical label with arrow"""
        # Create label box
        self.label_pen.penup()
        self.label_pen.goto(x, y)
        
        # Draw arrow
        self.label_pen.pendown()
        self.label_pen.color("#2c3e50")
        self.label_pen.pensize(1.5)
        self.label_pen.goto(arrow_to[0], arrow_to[1])
        
        # Arrow head
        angle_to_target = self.label_pen.towards(arrow_to[0], arrow_to[1])
        self.label_pen.setheading(angle_to_target)
        self.label_pen.stamp()
        
        # Label text background
        self.label_pen.penup()
        self.label_pen.goto(x, y)
        self.label_pen.pendown()
        self.label_pen.color("#ecf0f1")
        self.label_pen.begin_fill()
        for _ in range(2):
            self.label_pen.forward(120)
            self.label_pen.right(90)
            self.label_pen.forward(40)
            self.label_pen.right(90)
        self.label_pen.end_fill()
        
        # Border
        self.label_pen.penup()
        self.label_pen.goto(x, y)
        self.label_pen.pendown()
        self.label_pen.color("#34495e")
        self.label_pen.pensize(2)
        for _ in range(2):
            self.label_pen.forward(120)
            self.label_pen.right(90)
            self.label_pen.forward(40)
            self.label_pen.right(90)
        
        # Text
        self.label_pen.penup()
        self.label_pen.goto(x + 60, y - 12)
        self.label_pen.color("#2c3e50")
        self.label_pen.write(title, align="center", font=("Arial", 9, "bold"))
        
        self.label_pen.goto(x + 60, y - 30)
        self.label_pen.color("#7f8c8d")
        self.label_pen.write(description, align="center", font=("Arial", 7, "normal"))
        
        self.label_pen.pensize(2)
    
    def add_anatomical_legend(self):
        """Add detailed anatomical legend"""
        print("Adding: Anatomical legend and annotations...")
        
        # Legend box
        legend_x = -550
        legend_y = 300
        
        self.label_pen.penup()
        self.label_pen.goto(legend_x, legend_y)
        self.label_pen.pendown()
        self.label_pen.color("#2c3e50")
        self.label_pen.pensize(3)
        self.label_pen.begin_fill()
        self.label_pen.fillcolor("#ecf0f1")
        for _ in range(2):
            self.label_pen.forward(200)
            self.label_pen.right(90)
            self.label_pen.forward(550)
            self.label_pen.right(90)
        self.label_pen.end_fill()
        
        # Legend title
        self.label_pen.penup()
        self.label_pen.goto(legend_x + 100, legend_y - 25)
        self.label_pen.color("#2c3e50")
        self.label_pen.write("ANATOMICAL STRUCTURES", align="center", 
                            font=("Arial", 11, "bold"))
        
        # Legend items
        structures = [
            ("External Anatomy:", ""),
            ("• Glans Penis", "Sensitive erectile tissue"),
            ("• Penile Shaft", "Main body with erectile chambers"),
            ("• Scrotum", "External sac containing testes"),
            ("", ""),
            ("Internal Structures:", ""),
            ("• Corpus Cavernosum", "Paired erectile chambers"),
            ("• Corpus Spongiosum", "Surrounds urethra"),
            ("• Urethra", "Urinary and seminal passage"),
            ("• Tunica Albuginea", "Fibrous tissue layer"),
            ("", ""),
            ("Testicular Anatomy:", ""),
            ("• Testes", "Produce sperm & testosterone"),
            ("• Epididymis", "Sperm storage & maturation"),
            ("• Vas Deferens", "Sperm transport duct"),
            ("• Seminiferous Tubules", "Sperm production site"),
            ("", ""),
            ("Vascular & Neural:", ""),
            ("• Dorsal Artery/Vein", "Blood supply"),
            ("• Dorsal Nerve", "Sensory innervation"),
            ("• Pudendal Nerve", "Motor & sensory"),
        ]
        
        y_offset = 50
        for structure, description in structures:
            self.label_pen.goto(legend_x + 10, legend_y - y_offset)
            if structure.endswith(":"):
                self.label_pen.color("#e74c3c")
                self.label_pen.write(structure, font=("Arial", 9, "bold"))
            elif structure:
                self.label_pen.color("#2c3e50")
                self.label_pen.write(structure, font=("Arial", 8, "normal"))
                if description:
                    self.label_pen.goto(legend_x + 150, legend_y - y_offset)
                    self.label_pen.color("#7f8c8d")
                    self.label_pen.write(description, font=("Arial", 7, "italic"))
            y_offset += 20
        
        time.sleep(0.5)
    
    def add_clinical_notes(self):
        """Add clinical reference information"""
        print("Adding: Clinical reference notes...")
        
        notes_x = 320
        notes_y = 300
        
        # Notes box
        self.label_pen.penup()
        self.label_pen.goto(notes_x, notes_y)
        self.label_pen.pendown()
        self.label_pen.color("#27ae60")
        self.label_pen.pensize(3)
        self.label_pen.begin_fill()
        self.label_pen.fillcolor("#e8f8f5")
        for _ in range(2):
            self.label_pen.forward(230)
            self.label_pen.right(90)
            self.label_pen.forward(400)
            self.label_pen.right(90)
        self.label_pen.end_fill()
        
        # Title
        self.label_pen.penup()
        self.label_pen.goto(notes_x + 115, notes_y - 25)
        self.label_pen.color("#27ae60")
        self.label_pen.write("CLINICAL NOTES", align="center", 
                            font=("Arial", 11, "bold"))
        
        # Clinical information
        clinical_info = [
            ("Dimensions (Adult):", ""),
            ("Flaccid: 7-10 cm", ""),
            ("Erect: 12-16 cm", ""),
            ("Diameter: 3-4 cm", ""),
            ("", ""),
            ("Physiological Functions:", ""),
            ("• Urination", ""),
            ("• Reproduction", ""),
            ("• Sexual function", ""),
            ("", ""),
            ("Erectile Mechanism:", ""),
            ("1. Sexual stimulation", ""),
            ("2. Nerve signals trigger", ""),
            ("   NO release", ""),
            ("3. Blood fills corpora", ""),
            ("4. Tunica albuginea", ""),
            ("   traps blood", ""),
            ("5. Rigidity achieved", ""),
            ("", ""),
            ("Blood Supply:", ""),
            ("• Internal pudendal a.", ""),
            ("• Deep & dorsal arteries", ""),
            ("• Venous drainage via", ""),
            ("  dorsal vein system", ""),
        ]
        
        y_off = 50
        for line, _ in clinical_info:
            self.label_pen.goto(notes_x + 15, notes_y - y_off)
            if line.endswith(":"):
                self.label_pen.color("#27ae60")
                self.label_pen.write(line, font=("Arial", 9, "bold"))
            elif line:
                self.label_pen.color("#2c3e50")
                self.label_pen.write(line, font=("Arial", 8, "normal"))
            y_off += 18
        
        time.sleep(0.5)
    
    def add_cross_section_indicator(self):
        """Add indicator showing this is a cross-sectional view"""
        print("Adding: Cross-section indicators...")
        
        # Section line
        self.shader.penup()
        self.shader.goto(-60, 330)
        self.shader.pendown()
        self.shader.color("#e74c3c")
        self.shader.pensize(3)
        self.shader.goto(60, 330)
        
        # Section markers
        for x in [-60, 60]:
            self.shader.penup()
            self.shader.goto(x, 335)
            self.shader.pendown()
            self.shader.setheading(270)
            self.shader.forward(10)
            self.shader.setheading(90)
            self.shader.penup()
            self.shader.goto(x, 330)
            self.shader.pendown()
            self.shader.forward(10)
        
        # Label
        self.label_pen.goto(0, 340)
        self.label_pen.color("#e74c3c")
        self.label_pen.write("⟷ SAGITTAL SECTION ⟷", align="center", 
                            font=("Arial", 9, "bold"))
        
        time.sleep(0.3)
    
    def add_scale_bar(self):
        """Add anatomical scale reference"""
        print("Adding: Scale bar...")
        
        scale_x = -200
        scale_y = -440
        
        # Scale bar
        self.shader.penup()
        self.shader.goto(scale_x, scale_y)
        self.shader.pendown()
        self.shader.color("#34495e")
        self.shader.pensize(3)
        self.shader.setheading(0)
        self.shader.forward(100)
        
        # End marks
        for x_off in [0, 100]:
            self.shader.penup()
            self.shader.goto(scale_x + x_off, scale_y - 5)
            self.shader.pendown()
            self.shader.setheading(90)
            self.shader.forward(10)
        
        # Label
        self.label_pen.goto(scale_x + 50, scale_y - 25)
        self.label_pen.color("#34495e")
        self.label_pen.write("5 cm", align="center", font=("Arial", 9, "bold"))
        
        time.sleep(0.2)
    
    def add_anatomical_orientation(self):
        """Add orientation markers (Superior/Inferior, Anterior/Posterior)"""
        print("Adding: Anatomical orientation markers...")
        
        # Superior arrow
        self.shader.penup()
        self.shader.goto(-500, -200)
        self.shader.pendown()
        self.shader.color("#3498db")
        self.shader.pensize(3)
        self.shader.setheading(90)
        self.shader.forward(80)
        
        # Arrow head
        self.shader.setheading(150)
        self.shader.forward(15)
        self.shader.penup()
        self.shader.goto(-500, -120)
        self.shader.pendown()
        self.shader.setheading(30)
        self.shader.forward(15)
        
        self.label_pen.goto(-500, -100)
        self.label_pen.color("#3498db")
        self.label_pen.write("SUPERIOR", align="center", font=("Arial", 10, "bold"))
        
        # Inferior arrow
        self.shader.penup()
        self.shader.goto(-500, -300)
        self.shader.pendown()
        self.shader.setheading(270)
        self.shader.forward(80)
        
        self.shader.setheading(240)
        self.shader.forward(15)
        self.shader.penup()
        self.shader.goto(-500, -380)
        self.shader.pendown()
        self.shader.setheading(300)
        self.shader.forward(15)
        
        self.label_pen.goto(-500, -410)
        self.label_pen.color("#3498db")
        self.label_pen.write("INFERIOR", align="center", font=("Arial", 10, "bold"))
        
        # Anterior/Posterior
        self.shader.penup()
        self.shader.goto(-200, -350)
        self.shader.pendown()
        self.shader.color("#9b59b6")
        self.shader.pensize(3)
        self.shader.setheading(0)
        self.shader.forward(60)
        
        self.shader.setheading(30)
        self.shader.forward(12)
        self.shader.penup()
        self.shader.goto(-140, -350)
        self.shader.pendown()
        self.shader.setheading(330)
        self.shader.forward(12)
        
        self.label_pen.goto(-110, -360)
        self.label_pen.color("#9b59b6")
        self.label_pen.write("ANTERIOR", align="center", font=("Arial", 9, "bold"))
        
        self.label_pen.goto(-230, -360)
        self.label_pen.write("POSTERIOR", align="center", font=("Arial", 9, "bold"))
        
        time.sleep(0.3)
    
    def add_reference_citation(self):
        """Add medical reference information"""
        print("Adding: Medical reference citation...")
        
        self.label_pen.goto(0, -460)
        self.label_pen.color("#7f8c8d")
        self.label_pen.write("Based on: Gray's Anatomy, Netter's Atlas, Clinical Anatomy References", 
                            align="center", font=("Arial", 8, "italic"))
        
        self.label_pen.goto(0, -475)
        self.label_pen.write("© Medical Education Illustration - Not for Clinical Diagnosis", 
                            align="center", font=("Arial", 8, "italic"))
        
        time.sleep(0.2)
    
    def draw_complete_illustration(self):
        """Execute complete professional medical illustration"""
        print("\n" + "="*60)
        print("PROFESSIONAL MEDICAL ANATOMY ILLUSTRATION")
        print("Creating detailed anatomical drawing...")
        print("="*60 + "\n")
        
        # Main anatomical structures
        self.draw_detailed_scrotum_and_testes()
        self.draw_detailed_shaft()
        self.draw_detailed_glans()
        
        # Additional details
        self.draw_blood_vessels()
        self.draw_nerves()
        
        # Annotations and references
        self.add_cross_section_indicator()
        self.add_anatomical_legend()
        self.add_clinical_notes()
        self.add_scale_bar()
        self.add_anatomical_orientation()
        self.add_reference_citation()
        
        # Final message
        print("\n" + "="*60)
        print("ILLUSTRATION COMPLETE")
        print("="*60)
        print("\nThis is a professional medical illustration showing:")
        print("  ✓ External anatomy with detailed surface features")
        print("  ✓ Internal cross-sectional anatomy")
        print("  ✓ Vascular supply (arteries and veins)")
        print("  ✓ Neural innervation")
        print("  ✓ Testicular internal structure")
        print("  ✓ Erectile tissue chambers")
        print("  ✓ Complete anatomical labeling")
        print("  ✓ Clinical reference information")
        print("  ✓ Anatomical orientation markers")
        print("  ✓ Scale reference")
        print("\nFor medical education and clinical reference only.")
        print("\nClick window to close.")
        
        self.screen.exitonclick()

# Execute professional medical illustration
if __name__ == "__main__":
    print("\n" + "="*60)
    print("INITIALIZING PROFESSIONAL MEDICAL ILLUSTRATION SYSTEM")
    print("="*60)
    print("\nPurpose: Educational medical anatomy illustration")
    print("Standard: Professional medical illustration quality")
    print("Reference: Clinical anatomy textbooks")
    print("\nStarting drawing process...\n")
    
    illustration = ProfessionalAnatomyIllustration()
    illustration.draw_complete_illustration()