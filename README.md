# SENG 480B Assignment 3

Written by Andrew Braun (V00851919).

This is a compressor and decompressor for Path 1 (process-oriented) of Assignment 3.

## Compressed file format

The format of a compressed file consists of a series of blocks. All the blocks are
outputted at once using a basic Arithmetic Coding scheme. Each block starts with the
compressed data, which is then followed by these (in order):
1. Index of the original row in the Burrows-Wheeler transform (32-bit unsigned integer).
2. CRC number for the uncompressed data in the block (32-bit unsigned integer).
3. An End-of-Block symbol (257).

The file ends with a special End-of-File symbol. Note that this symbol is different from
the End-of-Block symbol.

## Compressor

The compressor (uvzz_compress.cpp) reads an input file from standard input. If it reads
20000 bytes of uncompressed data, it creates a block from it and compresses it. It then
continues to compress the input stream until all the data is in compressed blocks. It
then outputs those blocks to standard output with Arithmetic Coding.

A block of data, representing up to 20000 bytes of uncompressed data, has the following
actions performed on it by the compressor:
1. Transformed via the Burrows-Wheeler transform (BWT.hpp). Rather than creating the
full matrix of shifted rows, the compressor instead creates the suffix array for the
given block and derives the result from it. Note that to use the suffix array, the
compressor appends a new symbol with the value 0 to the end of the uncompressed block.
All the other values in the block are incremented by 1 (and are later decremented by the
decompressor). This special symbol allows for the suffix array to be created in a way
that easily gives the BWT result.
2. Transformed via Move-to-Front encoding (move_to_front.hpp). The stack consists of 257
entries, as the BWT symbol is included in the Move-to-Front transform. This operation
results in more runs of zeroes in the result.
3. Compressed using RLE. Because the Move-to-Front transformation results in more runs
of zeroes, only runs of zeroes have their run lengths outputted. The run length is
outputted immediately after a zero is outputted. Run lengths have a fixed
length of 8 bits. A fixed length was chosen over a variable length to allow for the
value to be used in Arithmetic Coding easily.

## Decompressor

The decompressor (uvzz_decompress.cpp) reads a compressed input file from standard
input. Whenever the decompressor reads an End-of-Block symbol, it creates a block.

For each block, the decompressor first undoes the RLE and Move-to-Front transformations.
Then, it processes each block using inverse BWT. The inverse BWT algorithm uses fast
inversion, in which it creates a mapping of pointers from the first column to the last
column. It uses this mapping to derive the original row. This process uses less memory
and is faster than recreating the entire matrix.

## References

The suffix array algorithm and deriving the BWT result from the suffix array are from
C. Kingsford at the Carnegie Mellon University.
www.cs.cmu.edu/~ckingsf/bioinfo-lectures/bwt.pdf

The CRC.h file for calculating the CRC comes from D. Bahr.

Many of the other algorithms used, including the input_stream.hpp and output_stream.hpp
files as well as the Arithmetic Coding implementations come from B. Bird at the
University of Victoria.

