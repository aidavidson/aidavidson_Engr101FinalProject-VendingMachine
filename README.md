# aidavidson_Engr101FinalProject-VendingMachine
this project includes Arduino code where I took input from various components to influence the output on the liquid crystal display. the following input components include: thermistor, 4x4 matrix, joystick, pushbuttons and PIR sensor. The outputting devices include the LCD Screen and the DC Motor.

## aspects

Thermistor --> Fan

the thermistor takes the temperature and in my code I determine if the temperature is hot enough to turn on the fan for the user.

PIR Sensor --> LCD Monitor

starting off in my code the LCD is receiving no power and only when the PIR Sensor determines there is motion by the vending machine
does the user interface activate.

4x4 matrix / Joystick / pushbuttons

these components allow the user to make decisions based on what they see in the vending machine so the user scrolls down with the joystick to
see the different items in there and presses in to swap from drinks to snacks and vice versa. After they use the matrix to input which Item they
want to buy and the pushbuttons confirm if the user is continuing to the final payment screen or back to the screen to choose more items

