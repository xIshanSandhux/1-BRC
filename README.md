# miniGrep

Building a mini grep to get used to C++ and trying to optimize it to be as fast and mem efficienct as possible

## Reading file
using fread for reading file which brought the file read down from around 100000 ns to around 44000 ns.

also could used fstream for reading the file to make the unoptimized version is bit more slower as it would add a bit more overhead, but using ifstream as I am only reading the file.

i can further optimize it more by using read() instead of fread or mmap, might do that later.

however these change are only noticeable when playing with large files. with small files its more or less the same.


## search substring
for the unoptimized version using the .find() to find all the occurence of the word in the whole file.
