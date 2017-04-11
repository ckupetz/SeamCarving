# SeamCarving

_A C++ implementation of greyscale seam carving with .pgm files_


### Building Code
The code can be built using `make clean && make`.

One executable will be created, `seam`.

To execute the program, simply run the following arugments:
```bash
./seam [filename] [VERTICAL_SEAMS] [HORIZONTAL_SEAMS]
```
### Example Commands:

```bash 
$ ./seam twoBalls.pgm 100 0
```
```bash

$ ./seam test.pgm 50 60
```
### Example Output
###### _0_ vertical seams, _100_ horizontal seams <br>
![](https://s23.postimg.org/4kla6onbv/joconde.png)
![](https://s23.postimg.org/b9rtmp8nv/joconde_processed.png)<br>
###### _50_ vertical seams, _0_ horizontal seams <br>
![](https://s23.postimg.org/4kla6onbv/joconde.png)
![](https://s23.postimg.org/uqcj983rv/joconde_processed_1.png)