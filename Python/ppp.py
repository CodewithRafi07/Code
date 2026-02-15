import time
import sys
import pyttsx3 

def type_text(text, delay=0.04):
    """Prints text character-by-character for a vintage 'typing' effect."""
    for char in text:
        sys.stdout.write(char)
        sys.stdout.flush()
        time.sleep(delay)
    sys.stdout.write('\n')

def run_proposal():
    """
    Runs the proposal script, including animation, TTS, and the main question.
    
    REMINDER: Customize the [NAME] placeholders below!
    """
    
    # --- CUSTOMIZATION SECTION ---
    # !! IMPORTANT: Update these placeholders !!
    YOUR_NAME = "Your Name"         
    GF_NAME = "Her Name" 
    
    proposal_text_start = (
        f"A message from {YOUR_NAME} to my dearest {GF_NAME}...\n"
        "--------------------------------------------------\n"
    )

    proposal_speech_text = (
        f"My love, {GF_NAME}. You are my best friend, my greatest adventure, and my true north. "
        "Every moment we share is a gift, and I can't imagine a future without your smile. "
        "This isn't a coding error or a test script. It's my heart, running on pure, simple truth. "
        "There is only one variable I need to define for life: our happiness. "
    )
    
    final_question = "So, my love, will you marry me? (Type YES or NO, then press Enter) "
    # -----------------------------
    
    # 1. Initialize Text-to-Speech Engine
    tts_enabled = False
    try:
        # Initialize the engine
        engine = pyttsx3.init()
        # Set a slightly slower, more deliberate pace for the proposal
        engine.setProperty('rate', 160) 
        tts_enabled = True
        # Announce the TTS status to the console, but not to the "girlfriend"
        print("[System: Audio initialized. Voice will read the proposal.]")
    except Exception:
        print("[System: Warning: Audio (pyttsx3) disabled. The proposal will appear as text only.]")
    
    # Start the display
    print("\n" * 5) # Print some lines for dramatic centering
    type_text(proposal_text_start, 0.01) # Fast print for the header
    time.sleep(1)

    # 2. Type out the heartfelt message
    type_text(proposal_speech_text, 0.04)
    time.sleep(2)
    
    # 3. Deliver the spoken and typed final question
    type_text("\n(The voice will now ask the final question...)\n", 0.03)
    time.sleep(1)

    if tts_enabled:
        # Speak the proposal and the final question
        engine.say(proposal_speech_text)
        engine.say("Will you marry me?")
        engine.runAndWait()
        
    # Ask for the input
    answer = input(final_question).strip().upper()
    
    print("-" * 50)
    
    # 4. Respond to the answer
    if answer == "YES":
        type_text(f"YES!!! {GF_NAME}, I love you! This is the start of forever!\n")
        # Simple Heart Animation
        for i in range(1, 5):
            print(" " * (5 - i) + "💖" * (2 * i - 1) + " " * (5 - i))
            time.sleep(0.3)
        print("\nPress Ctrl+C to close this beautiful terminal.")
    elif answer == "NO":
        type_text("Oh... Well, let me recompile my life choices and try again later. I still love you! (Just kidding... mostly.)\n")
    else:
        type_text("That input was invalid, but I'll take the ambiguous answer as a 'Maybe' and keep trying!")

    if tts_enabled:
        # Ensure engine is stopped after final messages
        engine.stop()

if __name__ == "__main__":
    try:
        run_proposal()
    except KeyboardInterrupt:
        sys.exit()