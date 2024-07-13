# Entrypoint `main.py` for Motion Notifier
# xram | 7/6/24

# μPython will run a script named `main.py` automatically on boot.
# This script will be used as an entrypoint for the main program in the `motion_notifier.py` script.

import motion_notifier
motion_notifier.run()