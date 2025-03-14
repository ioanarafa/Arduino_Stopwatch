Main Functionalities:

Stopwatch:

    Measures the elapsed time starting from when the "Start" button is pressed.
    Allows pausing and resetting the stopwatch through dedicated buttons.
    Uses the millis() function to continuously update and display the current time on the LCD screen.
    
Timer:

    Allows the user to set a time interval (minutes and seconds) via buttons.
    When the set time expires, the device triggers an alarm using both a buzzer and an LED for audible and visual notifications.
    The logic is based on calculating the remaining time, and upon completion, the alarm() function is called.
    
User Interface:

    The I2C LCD 16x2 display shows the current time and the operating mode (stopwatch or timer) in real time.
    Buttons (Start, Stop, Reset, Set Minutes, Set Seconds, Mode) allow switching between the two modes and controlling the specific functionalities.
    
Hardware and Connections:

    Arduino Mega: The main microcontroller that runs the code.
    I2C LCD 16x2 Module: Displays the time and the current operating mode.
    Buttons:
    Start – connected to pin 22.
    Stop – connected to pin 23.
    Reset – connected to pin 25.
    Set Minutes – connected to pin 31.
    Set Seconds – connected to pin 33.
    Mode – connected to pin 27.
    Buzzer and LED: Used for the alarm when the timer expires (Buzzer on pin 35 and LED on pin 37).
    Utilizes pull-up resistors (by setting the pins as INPUT_PULLUP) to ensure stable button signal readings.
    
Software Implementation:

    Stopwatch: The time is continuously updated using millis(), and the buttons enable control for starting, pausing, and resetting the stopwatch.
    Timer: The user inputs values for minutes and seconds, and upon the completion of the set time interval, an alarm function is triggered that activates the buzzer and LED.
