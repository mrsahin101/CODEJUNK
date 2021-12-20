#This is example for using input capture. 
I configure the timer1 to toggle GPIO PORT D via Direct Memory Access(DMA)
Then Timer5 measuring the frequency with its own internal counter.
Also you can use external frequency source. 
I used timer5 because it is a 32-bit resoulution timer. which allows use to measure wider frequency range. because with 16-bit timers you have to choose which range u want to measure. Because their ARR register could overflow couple times till you calculate between 2 rising edge.