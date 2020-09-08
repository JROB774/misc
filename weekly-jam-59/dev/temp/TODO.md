TODO
===============================================================================
+ Make some custom bullets [ ]
+ Come up with some enemy variations [ ]
+ Stun animation [ ]
+ Defender Bullet [ ]
	- Angle correctly [X]
	- Slightly infront [ ]
	- Collision is sorta a pain, ask Josh for line. [ ]
	- Why does it drift away?
	- Change so only the tip is dangerous ;) [ ]
+ Defender flag such that weak and ranged enemies hide behind them [ ]
+ Enemey (Defender) when stuck in a corner jitter because of how movement rebound works. [ ]
+ Make it so non-players can't leave the viewport (or if they do they stay present in the other one.) [ ]
+ Change snotleks bullets to parabola [ ]
+ Snotleks seems to reset self.flags.state... Ctrl+h to self.STATE [ ]
+ Snoteleks bullets falls self destructs when it hits your y coordinate [-]
+ Snoteleks bullets are basing their entire trajectory on the halfline but that only works when player.x is about launchPos.x [ ] 

COMPLETE
===============================================================================
+ Heart Tile collision [X]
+ Deal dmg is delayed [X]
+ Make it so theres no movement rebound when stunned (else you can be pushed by enemies into walls.) [X]
+ Decide how enemy collisions work, i.e. stun time, delays. Probs best to add a dealDmg delay such that after being damaged you cant be damaged again until the timer goes down. [X]
+ Innerproduct for spike [X]
+ Camera Positioning relative to viewport (world) pos inorder to work out tile loading and off screen detection code. [X]
+ Spike Block redo [X]
+ Defender Jitter, low priority [X]
+ Power up system [X]
+ Flashing ItemSpawns [X]
+ Test other Itemspawns [X]
+ Snotleks bullet doesnt spawn [X]
+ Snotleks bullet follows you. Probs a pass by refernce issue [X]

DELEGATED/NOT REQUIRED
===============================================================================
+ Find render on player [-]