# Motion Notifier
# xram | v0.1 (6/8/24)

import os
import requests
import network
import time

## WLAN ##
wlan = network.WLAN(network.STA_IF)  # Create a WLAN in STA mode
wlan.active(True)  # Put WLAN interface up
wlan.connect('')

