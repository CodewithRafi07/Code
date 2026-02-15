import time
import sys
# Import the Text-to-Speech library (requires installation: pip install pyttsx3)
import pyttsx3 

def run_sorry_sequence():
    """
    Prints 'Sorry!' indefinitely with a 0.7 second delay between each print,
    displays a spinning animation, and *speaks* the word 'Sorry!' aloud.
    
    NOTE: For the audio to work, run this file locally after installing pyttsx3.
    """
    
    # 1. Initialize Text-to-Speech Engine
    tts_enabled = False
    try:
        # Initialize the engine to speak the text
        engine = pyttsx3.init()
        # Optional: set properties like speech rate or voice
        # engine.setProperty('rate', 180) # Adjust rate for a faster or slower voice
        tts_enabled = True
        print("TTS Engine initialized successfully. Prepare for spoken apologies!")
    except Exception:
        # This catches errors if pyttsx3 isn't installed or fails to connect to the system voice
        print("Warning: Could not initialize TTS engine (pyttsx3). Audio will be disabled. Only text will appear.")
        
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
            
            # 2. Speak the word 'Sorry!'
            if tts_enabled:
                engine.say("Sorry!")
                # runAndWait() blocks the script until the speech is complete
                # Note: This will slightly extend the loop duration beyond 0.7 seconds.
                engine.runAndWait() 
                
            # 3. Run the animation and delay for 0.7 seconds total
            for i in range(len(spinner_frames)):
                frame = spinner_frames[i]
                
                # Construct the animation message: [Frame] Delaying...
                animation_message = f"\r    [{frame}] Waiting for next 'Sorry!' (0.7s print delay)..."
                
                # Print the animation message without a newline (end="")
                print(animation_message, end="")
                
                # Force the output buffer to display immediately 
                sys.stdout.flush() 
                
                # Wait for the duration of one animation frame
                time.sleep(frame_delay)
            
            # After the full 0.7s animation loop, clear the animation line
            print("\r" + " " * 60, end="")
            sys.stdout.flush()

    except KeyboardInterrupt:
        # Handle the user pressing Ctrl+C gracefully
        if tts_enabled:
             engine.stop()
        print("\n--- Sequence interrupted by user. Goodbye! ---")

if __name__ == "__main__":
    run_sorry_sequence()