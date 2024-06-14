# Debug Functions for Motion Notifier
# xram | 6/13/24

from time import sleep_ms
from machine import Pin, ADC, Timer


# Convert a 16-bit (0-65535) value into a position on a line to render a plot
#  of the analog signal, where `+t` points downwards along the console.
def _render_analog_output(reading:int, buckets:int = 32, digital_pin:Pin|None = None):
    DIV = 65536/buckets  # Calculate the divisor needed to make the desired number of buckets
    MARKER = '*'
    
    # Mark current position of signal output onto render line
    render_line = ' '*buckets
    position = int(reading / DIV)
    render_line = render_line[:position] + MARKER + render_line[position+1:]
    
    # Add borders to the render line
    render_line = '||' + render_line + '||'
    
    # Add the digital value returned by the sensor to the end of the line
    if digital_pin:
        render_line = render_line + '      ' + ('    ' if digital_pin.value() == 0 else 'MMMM')
    
    # Print the render line
    print(render_line)


# Print output graph of analog sensor
def print_analog_sensor_output(sensor_pin:ADC, sample_rate_ms:int = 20, buckets:int = 128, digital_pin:Pin|None = None) -> None:
    while True:
        reading = sensor_pin.read_u16()
        _render_analog_output(reading, buckets, digital_pin)
        sleep_ms(sample_rate_ms)


class LEDs():
    def __init__(self,
        led_green_pin:str|int   = "GP0",  # Pin 1
        led_yellow_pin:str|int  = "GP1",  # Pin 2
        led_red_pin:str|int     = "GP2",  # Pin 4
        enabled:bool = True  # Toggle LED debug functionality globally
    ):
        self.enabled = enabled
        
        self.ledG = Pin(led_green_pin, Pin.OUT)
        self.ledY = Pin(led_yellow_pin, Pin.OUT)
        self.ledR = Pin(led_red_pin, Pin.OUT)

        self.ledG.off()
        self.ledY.off()
        self.ledR.off()
        
        self.all: list[Pin] = [self.ledG, self.ledY, self.ledR]
        self.timers: dict[Pin, None|Timer] = {key: None for key in self.all}
    
    # Switch all LEDs off
    def all_off(self):
        if not self.enabled: return  # Skip if debug is disabled
        for led in self.all: led.off()
    
    # Switch all LEDs on
    def all_on(self):
        if not self.enabled: return  # Skip if debug is disabled
        for led in self.all: led.on()
    
    # Switch off an LED or all LEDs in a list
    def off(self, led:Pin|list[Pin]):
        if not self.enabled: return  # Skip if debug is disabled
        if type(led) == list:
            for x in led: x.off()
        else:
            led.off() # type: ignore
    
    # Switch on an LED or all LEDs in a list
    def on(self, led:Pin|list[Pin]):
        if not self.enabled: return  # Skip if debug is disabled
        if type(led) == list:
            for x in led: x.on()
        else:
            led.on() # type: ignore

    # Set a blink timer for an LED
    def set_blink_timer(self, led:Pin, freq_hz:int = 5):
        if not self.enabled: return  # Skip if debug is disabled
        
        # Clear the timer on this pin if any already exists
        if self.timers[led]:
            self.timers[led].deinit()  # type: ignore
        
        led_timer = Timer()
        led_timer.init(mode=Timer.PERIODIC, freq=freq_hz, callback=lambda tm: led.toggle())
        
        self.timers[led] = led_timer
    
    # Set a one-shot timer for an LED
    def set_oneshot_timer(self, led:Pin, duration_ms:int = 1000):
        if not self.enabled: return  # Skip if debug is disabled
        
        def _timer(tm):
            led.off()
            # self.kill_timer(led)
        
        # Clear the timer on this pin if any already exists
        if self.timers[led]:
            self.timers[led].deinit()  # type: ignore
        
        # Switch LED on to start, then set one-shot timer to switch LED off after a delay
        led.on()
        led_timer = Timer()
        led_timer.init(mode=Timer.ONE_SHOT, period=duration_ms, callback=_timer)
        
        self.timers[led] = None
    
    # Kill an existing LED timer
    def kill_timer(self, led:Pin):
        if not self.enabled: return  # Skip if debug is disabled
        
        if self.timers[led]:
            self.timers[led].deinit()  # type: ignore
            led.off()
        else:
            print('Error: No timer currently exists on this pin.')
    
    # Cycle all LEDs
    def run_test_cycle(self):
        if not self.enabled: return  # Skip if debug is disabled
        
        self.all_off()
        
        for t in range(30):
            if (t%3 != 2): self.ledG.on()
            else: self.ledG.off()

            if (t%3 != 0): self.ledY.on()
            else: self.ledY.off()

            if (t%3 != 1): self.ledR.on()
            else: self.ledR.off()

            sleep_ms(250)
        
        self.all_off()


# Run debug routine: Cycle LEDs
if __name__ == '__main__':
    # leds = LEDs()
    # leds.run_test_cycle()
    
    
    print_analog_sensor_output(ADC(26), digital_pin=Pin("GP22"))
