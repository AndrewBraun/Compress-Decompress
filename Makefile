# Andrew Braun
# V00851919
# With portions from B. Bird
EXTRA_CXXFLAGS=
EXTRA_CFLAGS=
CXXFLAGS=-O3 -Wall -std=c++17 $(EXTRA_CXXFLAGS)
CFLAGS=-O3 -Wall -std=c11 $(EXTRA_CFLAGS)

all: uvzz_compress uvzz_decompress

clean:
	rm -f uvzz_compress uvzz_decompress *.o
