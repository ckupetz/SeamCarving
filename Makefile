COMPILER=g++ -std=c++11
EXECUTABLES=seam

all:
	make $(EXECUTABLES)

seam: main.cpp
	$(COMPILER) -o $@ $^

clean:
	rm -f $(EXECUTABLES)
