#This is the project I used DMA. First I tried timer 6 because this one is the simplest one.
#Somehow It didn't work. I don't understand why. Then i tried timer1 and it worked. constantly It transfer 0xFFFF and 0x0000 to GPIO D output data register. everytime timer 1 overflows it does that.