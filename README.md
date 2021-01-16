# NumberSlumber
Want to calculate the binary code of 1578 or some other much larger number ?

Well now your dreams can come true with NumberSlumber!

Just compile the short program herein.

Step 1: Set-up a sensible environment to compile the code.
Since this is free software we use GNU/Linux (GUIX)
```
guix environment --container --ad-hoc gcc-toolchain
```
Afterwards we can compile the program :
```
gcc src/main.c -o nums
```
And now we can crunch those numbers!

```
./nums 1578
```
or why not
```
./nums --inputbase 0123 3301120302
```
Just remember that I haven't implemented a check for sensible input yet!

Heads up: The input and output bases are order dependent! (Thats kind off the whole thing :^D )

Todo :
	1 ) input checking
        2 ) turn into a proper ANSI C project library

Pssst... you are not restricted to numbers...
