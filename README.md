# SeamCarving

**A C++ implementation of greyscale seam carving with .pgm files**

The code can be built using `make clean && make`.

One executable will be created, `seam`.

To execute the program, simply run the following arugments:
```bash
./seam [filename] [VERTICAL_SEAMS] [HORIZONTAL_SEAMS]```

Examples:

```bash $ ./seam twoBalls.pgm 100 0
```
```bash

$ ./seam test.pgm 50 60
```
