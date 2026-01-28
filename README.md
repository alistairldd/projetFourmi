Ant project carried out as part of the course Modular Programming.  
We simulate colonies of ants fighting each other and observe the result by creating a GIF. 
To try it by yourself, you should do : 

make all && ./main && convert -delay 10 -loop 0 *.ppm -scale 500% animation.gif

If you don't have the library required to convert, try (on linux) :
sudo apt update && sudo apt install imagemagick
