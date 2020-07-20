# blinks-fireworks
#### Blinks radial animation, an attempt!

**My first Blinks app!** I tried to reproduce the fireworks that Jon Bobrow tweeted in the 4th of july ([here](https://twitter.com/Move38inc/status/1279143312566476801?s=20)), hopefully you find it useful. Some notes:
 - This was developed on VSCode (thread to setup [here](https://forum.move38.com/t/getting-out-of-arduino/502/21?u=finiteattractor)).
 - Review the config files and adjust based on your environment.
 - This is my first attempt at Blinks, and last time I used C++ was ~15 yrs ago! So take it with a piece of salt and let me know if there is a better way of doing things.
 - I was trying to create sort of a framework for easier future expriments...not sure if I succeeded though. Files under fx folder.
 - StateMachine based, with support for delayed state transitions allowing you to control speed of animation.
 - There is a nice debug utility there (fx/dbg.h) - designed to work with [Termethos](https://www.termethos.com).
