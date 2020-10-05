# Andrew Braun
# With portions from B. Bird
EXTRA_CXXFLAGS=
EXTRA_CFLAGS=
CXXFLAGS=-O3 -Wall -std=c++17 $(EXTRA_CXXFLAGS)
CFLAGS=-O3 -Wall -std=c11 $(EXTRA_CFLAGS)

all: compressor decompressor

clean:
	rm -f compressor decompressor *.o
