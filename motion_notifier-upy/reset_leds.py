from machine import Pin

ledG = Pin("GP0", Pin.OUT)
ledY = Pin("GP1", Pin.OUT)
ledR = Pin("GP2", Pin.OUT)

for led in [ledG, ledY, ledR]:
    led.off()