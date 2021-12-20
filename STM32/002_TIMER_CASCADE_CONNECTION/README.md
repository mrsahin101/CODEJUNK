#This is example for how to connect timers cascade. I made timer1 as master then timer8 (which is timer1 TRGO is connected to via ITR0 to timer8 input. Check data sheet for specific MCU.)
timer1 start counting. Each time it overflows it triggers timer8. and we toggle Green LED each timer timer 8 generates interrupt.
Also we have to trigger timer to start counting. I did that using MCO1(MicroController Output). once it triggered it starts counting
