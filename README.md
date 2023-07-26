# rotmg-autonexus
This is a "helper" 😉 for us noobs who cant make it past a week without laying ourselves down in a tomb. 

## Description
This clientless tool (for MacOS) is really simple in its nature. 

**display_checker.cpp** - is to get the coordinates of the region of interest (where you want to track the color)

**hack.cpp** - this is the tool, i know the name needs to be changed

***

An "image tracker" tracks the color of a specific region (specifically right next to the "HP" in the HP bar"), when 
- a certain hue of red is present (rgb(>=162, <= 75, <=62)) or
  - <img width="493" alt="Screenshot 2023-07-18 at 19 10 44" src="https://github.com/aimanalrafi/rotmg-autonexus/assets/80934568/16cf8022-06ab-4988-899d-47d78f82d05b">
 
- a gray color (meaning HP has fallen significantly low and has stayed long enough for the gray area to appear)
  - <img width="494" alt="Screenshot 2023-07-18 at 19 09 10" src="https://github.com/aimanalrafi/rotmg-autonexus/assets/80934568/6042b6c7-ed4b-424b-bc37-bcb173a01be1">

it triggers an action which mimics the press of the key 'R' from the user (if you do not know what R does, then you probably don't need this tool ;))

## Issues
Tracking the color of a specific region will not guarantee the prevention of going 6 feet under because
1. The yellow inverted triangle (which signifies hp is decreasing right next to "HP" which may (though not thoroughly tested) affect the results
2. High damage dealing shots may just shotgun you to hell without the chance to nexus
3. Pretty sure more will come once i start using this more frequently

## Future improvements
1. Track integer value of HP instead of color. Trigger nexus as soon as a procentual threshold (of Max HP) has been reached
2. Predictive Auto-Nexus: Based on the projectiles shot at you and the probability of you getting hit by it, the tool makes a prediction on when to Nexus. Of course, this requires training which means maybe a small test case would have to be built, on which we can train the model on ,so that we don't have to sacrifice our characters. There is already a WebApp i believe for this, the one to train dodging O3 bullets. We can utilize this.

## Suggestions
And if you have any more suggestions, feel free to let me know! Imma check how I can make this open source (if it already isn't)
