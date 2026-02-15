import time
import sys

def run_sorry_sequence():
    """
    Prints 'Sorry!' indefinitely with a 0.7 second delay between each print,
    and displays a spinning animation during the waiting period.
    """
    
    # Define the animation frames (a classic terminal spinner)
    spinner_frames = ['|', '/', '-', '\\']
    
    # Define the total delay requested by the user
    total_delay = 0.7
    
    # Determine the delay per animation frame (total delay divided by number of frames)
    frame_delay = total_delay / len(spinner_frames)

    print("--- Starting the Infinite Sorry Sequence (Press Ctrl+C to stop) ---")

    try:
        while True:
            # 1. Print the core message 'Sorry!' on a new line
            print("Sorry!")

            # 2. Run the animation and delay for 0.7 seconds total
            for i in range(len(spinner_frames)):
                frame = spinner_frames[i]
                
                # Construct the animation message: [Frame] Delaying...
                # The '\r' (carriage return) moves the cursor to the start of the line, 
                # allowing us to overwrite the previous frame.
                animation_message = f"\r    [{frame}] Waiting for next 'Sorry!' (0.7s delay)..."
                
                # Print the animation message without a newline (end="")
                print(animation_message, end="")
                
                # Force the output buffer to display immediately (important for the \r trick)
                sys.stdout.flush() 
                
                # Wait for the duration of one animation frame
                time.sleep(frame_delay)
            
            # After the full 0.7s animation loop, clear the animation line
            # by printing spaces and resetting the cursor with '\r'.
            print("\r" + " " * 60, end="")
            sys.stdout.flush()

    except KeyboardInterrupt:
        # Handle the user pressing Ctrl+C gracefully
        print("\n--- Sequence interrupted by user. Goodbye! ---")

if __name__ == "__main__":
    run_sorry_sequence()