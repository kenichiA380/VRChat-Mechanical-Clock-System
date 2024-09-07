# VRChat Mechanical Clock System
 An OSC client for sending time information to VRChat written in C
 
## Supports sending these data as floats over OSC:
### dates
- Year 0 ~ 100 corresponds to 0 ~ 1.0
- Month 1 ~ 12 corresponds to 0 ~ 1.0
- Day 1 ~ 31 corresponds to 0 ~ 0.93
- Day of week 1 ~ 7 corresponds to 0 ~ 0.7
- Moon phase one full circle corresponds to 0 ~ 1.0
 ### Times 
 (these are compatible with https://github.com/Yui0471/VRChat_OSC_Clock_System)
- Hour 0 ~ 12 corresponds to 0 - 0.6
- Minute 0 ~ 60 corresponds to 0 ~ 0.6
- Seconds 0 ~ 60 corresponds to 0 ~ 0.6

-   Note : unlike the VRChat OSC Clock System, time hands will move continuously like real mechanical watches.
   
### additional functions
- GMT (24 Hour hands) 0 ~ 24 corresponds to 0 ~ 0.6
- Milliseconds 0 ~ 1000 corresponds to 0 ~ 1.0
- Changeable refresh rate 1~ 16Hz : change your refresh rate (balance wheel beat rate) to simulate low beat or high beat clocks.
- Changeable parameter names
- Changeable destination IP address
- Changeable destination Port number

A demo implemetation for VRChat will be provided in Unity asset.
You will need to use Modular Avatar plugin to import it into your avatar.
