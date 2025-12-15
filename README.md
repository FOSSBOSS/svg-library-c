
For full details of the original project go to
http://www.codedrome.com/svg-library-in-c/

<pre>
This is my unholy fork of this project, I couldnt care less about standard practice, or coding conventions.

To compile:
$gcc -g -Wall -lm main.c -o main

added features: Regular polygons with sides > 3. 
                rotate shapes with animateRotate()

Future Features: polyline, gears, star polygons, diamonds. and other features of the svg standard. 

ok so once I realizaed the arg list was unmanageable, I decided to create a cannonical refference
for arguments. I still have to reform the functions to match, but Here is the 
Premise:

(cx, cy, rmax, rmin=0.0, theta=0.0, **opts)

where opts may include things like, color (stroke, fill), faces, and points.

</pre>
