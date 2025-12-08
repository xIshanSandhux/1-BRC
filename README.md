# miniGrep

Building a mini grep to get used to C++ and trying to optimize it to be as fast and mem efficienct as possible

## Reading file
using fread for reading file which brought the file read down from around 100000 ns to around 44000 ns.

also could used fstream for reading the file to make the unoptimized version is bit more slower as it would add a bit more overhead, but using ifstream as I am only reading the file.

i can further optimize it more by using read() or mmap instead of fread, might do that later (prolly will just for fun).

however these change are only noticeable when playing with large files. with small files its more or less the same.


## search substring
for the unoptimized version using the .find() to find the word match in each line,

using a set to store the line match found in, and only checking for first occurrence of the word and not all because if the lines contains >1 words the one i am trying to, all of them will be printed in the same line. so yeah.....

Oh also using sets, because the lookup is O(1) where as vectors got lookup O(n) because thats an array and set is a dict with unique values. so yeah.....
