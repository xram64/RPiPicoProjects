from machine import Pin, Timer

ledG = Pin("GP0", Pin.OUT)
ledY = Pin("GP1", Pin.OUT)
ledR = Pin("GP2", Pin.OUT)

for led in [ledG, ledY, ledR]:
        led.off()

timer1 = Timer()
# timer2 = Timer()
# timer3 = Timer()

def tick(timer):
    global ledG, ledY, ledR
    
    for led in [ledG, ledY, ledR]:
        led.toggle()
    
timer1.init(freq=3, mode=Timer.PERIODIC, callback=tick)
# timer2.init(freq=6, mode=Timer.PERIODIC, callback=tick)
# timer3.init(freq=9, mode=Timer.PERIODIC, callback=tick)