# Spring103Final
Final ENGR103H project: handheld CPX game

#  Tower Stacker
The Tower Stacker is a game where you want to replicate the pattern by "stacking" LED blocks to score points! A pattern will flash on screen
and you will be able to press the left and right buttons to stack left and right LEDs to recreate the pattern. If you lose the screen will
flash red, and if you win the screen will light up and you'll score a point. To continue playing rounds after you lose or win, flick the switch
and you can continue playing. Beware! As you continue winning the pattern will flash for less and less time making the game harder. Try to get a
high score and beat everyone!

Rules:
- Press a button to start and allow the pattern to flash
- Try to replicate the pattern by pressing right and left buttons
- If you win a point will be added and the pattern will flash for less amount of time next round
- If you lose your score will be reset
- Flick the switch to reset and play another round
- Press a button again to start the round

Inputs:
- Switch
    - This resets the round and lets the user play again
- Left Button
    - This is used to start the round and select left LEDs
- Right Button
    - This is used to start the round and select right LEDs

Outputs:
- Serial Monitor
    - This includes instructions, score, and "you win" + "you lose" text
- LED Pattern Flash
    - The pattern that flashes that the user is meant to replicate
- LED Yellow Preview
    - Two LEDs that light up during user input state that show which two LEDs the user can pick from
- LED White User Placement
    - LED that lights up white after user picks which one they want to light up
- LED Red Flash during loss
    - LEDs light up red when the user loses
- LED Sparkle during win
    - LED sparkle pattern that plays during win
- Speaker Beep during pattern flash
    - When the pattern flashes the speaker beeps for a period of time
- Speaker Dings during win
    - When the user wins there is a playful beep pattern that plays

