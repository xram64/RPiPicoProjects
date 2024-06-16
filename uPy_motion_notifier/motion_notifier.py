# Motion Notifier
# xram | v0.2 (6/12/24)

# Notes
#  - The ADC pins on the Pico W have 12-bit resolution (0-4095), mapped to a 16-bit int (0-65535).

import json
import network
import requests
from rp2 import country as rp2_country
from time import sleep, sleep_ms, sleep_us, ticks_us, ticks_diff
from machine import Pin, ADC
import debug

# Constants
DEBUG = True  # Global debug flag
MAX_WAIT_TIME: int = 30  # Max wait time for WiFi connection, in seconds

# [DEBUG] Initialize debug LEDs 
leds = debug.LEDs(enabled=DEBUG)

# Read secrets
with open('secrets.json') as f:
    SECRETS = json.load(f)

# Set WLAN region
rp2_country('US')


# OpenPIR sensor
sensor_PIR_OUT = Pin("GP22", Pin.IN)    # Pin 29 - `OUT` Digital PIR output
sensor_PIR_A   = ADC(26)                # Pin 31 (ADC0) - `A` Raw analog PIR output

# Hall-effect sensor
sensor_hall_OUT = Pin("GP15", Pin.IN)   # Pin 20


def connect_to_wifi(wlan: network.WLAN) -> None|tuple[str,str,str,str]:
    global MAX_WAIT_TIME
    global SECRETS
    global leds
    
    # Connect using WiFi login
    wlan.connect(SECRETS['WIFI']['SSID'], SECRETS['WIFI']['PSK'])
    
    leds.set_blink_timer(leds.ledY, 6)  # [DEBUG]
    
    # Wait for connect or fail
    print('Waiting for connection...')
    for _ in range(MAX_WAIT_TIME):
        status_check = wlan.status()
        if status_check < 0 or status_check >= 3:  # Error if < 0, Connected if = 3
            break
        sleep(1)
    
    leds.kill_timer(leds.ledY)  # [DEBUG]
    
    # Handle connection errors
    if wlan.status() != 3:  # Not connected
        leds.on(leds.ledR)  # [DEBUG]
        print(f'Connection failed with status {wlan.status()}.')
        return
    else:
        leds.on(leds.ledG)  # [DEBUG]
        print('Connected!')
        status = wlan.ifconfig()
        return status


def send_gotify_notification():
    # Build notification request data
    gotify_token = SECRETS['GOTIFY']['TOKEN']
    gotify_data = {'title': 'Test', 'message': 'Test message...', 'priority': '10'}
    gotify_data_urlencoded = f'title={gotify_data["title"]}&priority={gotify_data["priority"]}&message={gotify_data["message"]}'
    
    # Send notification
    response = requests.post(
        f'http://192.168.1.81:8070/message?token={gotify_token}',
        headers={'content-type': 'application/x-www-form-urlencoded'},
        data=gotify_data_urlencoded
    )
    print(response.content)
    response.close()


def _poll_sensor_PIR_A() -> int:
    return sensor_PIR_A.read_u16()
def _poll_sensor_PIR_OUT() -> bool:
    return sensor_PIR_OUT.value() == 1
def _poll_sensor_hall_OUT() -> bool:
    return sensor_hall_OUT.value() == 1

def motion_is_detected() -> bool:
    # When the PIR sensor reads logic 1 ('high'), motion is detected,
    #  so we can pass through the boolean sensor value.
    return _poll_sensor_PIR_OUT()

def door_is_closed() -> bool:
    # When the hall-effect sensor reads logic 0 ('low'), the door is closed (sensor is near magnet),
    #  so we pass the inverse of the boolean sensor value.
    return not _poll_sensor_hall_OUT()


if __name__ == '__main__':
    # Create a WLAN in STA mode and put interface up
    wlan = network.WLAN(network.STA_IF)
    wlan.active(True)
    
    # Attempt initial WiFi connection
    connected = False
    while not connected:
        result = connect_to_wifi(wlan)
        
        if result:
            # Connection successful
            print(f'IP: {result[0]} | Subnet mask: {result[1]} | Gateway: {result[2]} | DNS: {result[3]}')
            break
        else:
            # Connection failed
            sleep(15)  # Wait and retry
    
    # Maintain WiFi connection while polling sensors and waiting for a notification trigger
    poll_rate = 250  # sensor poll rate in ms
    while True:
        trigger_notification = False
        
        # When DEBUG is enabled, use LEDs to show when each sensor is triggered
        # (The debug LED timers should match the sensor poll rate)
        if DEBUG:
            if door_is_closed(): leds.set_oneshot_timer(leds.ledY, poll_rate)
            if motion_is_detected(): leds.set_oneshot_timer(leds.ledR, poll_rate)
        
        # If door is closed (hall-effect sensor) and motion is detected (PIR sensor), set the notification trigger
        if door_is_closed() and motion_is_detected():
            trigger_notification = True
        
        # Try sending notification if trigger is active
        try:
            if trigger_notification:
                trigger_notification = False
                send_gotify_notification()
        except:
            print('WiFi connection dropped. Attempting to reconnect...')
            
            status_check = wlan.status()
            if status_check < 0 or status_check >= 3:  # Error if < 0, Connected if = 3
                wlan.disconnect()
                wlan.connect(SECRETS['WIFI']['SSID'], SECRETS['WIFI']['PSK'])
                if wlan.status() == 3:
                    print('WiFi connection re-established!')
                else:
                    pass
    
        # Sleep between sensor checks
        sleep_ms(poll_rate)
    