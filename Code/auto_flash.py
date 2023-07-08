import wmi
import subprocess
import win32api
import re
import time

UF2_SOURCE = "\"build\\WordclockX.uf2\""

c = wmi.WMI()
watcher = c.watch_for(raw_wql = "SELECT * FROM __InstanceCreationEvent WITHIN 2 WHERE TargetInstance ISA \'Win32_USBHub\'")

while 1:
    # Get connected drives
    old_drives = set(win32api.GetLogicalDriveStrings().split('\000')[:-1])

    # Wait for new insertion event
    #print("Listening for new devices")
    usb = watcher()

    # Get new device parameters
    name = usb.Caption
    device_id = usb.DeviceID
    res = re.findall(r"VID_([a-fA-F0-9]+)&PID_([a-fA-F0-9]+)", device_id)
    vid = res[0][0]
    pid = res[0][1]

    # Debug: report findings
    #print("Detected new USB device: VID: %s, PID: %s, Caption: %s" % (vid, pid, name))

    # Check for correct device type
    if vid != "2E8A" or pid != "0003" or name != "USB-Massenspeichergerät":
        #print("Debug: Not the device we are looking for, abort mission!")
        continue

    # Get new drive path
    new_drive = set(win32api.GetLogicalDriveStrings().split('\000')[:-1]) - old_drives

    # Sanity check
    if len(new_drive) != 1:
        #print("Debug: Not exactly one new drive detected, abort mission!")
        continue
    
    print("Debug: Attempting to flash UF2 to drive %s" % list(new_drive)[0])

    # Copy UF2 to device
    subprocess.run("copy %s %s" % (UF2_SOURCE, list(new_drive)[0]), shell = True)

    # Wait for UF2 device to disconnect
    time.sleep(3)
