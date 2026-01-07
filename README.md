# 1-BRC (1 Billion Row Challenge)
I came across this challenge when I was trying to come up with a project so that I can learn C++.

Initially I started of with building a mini version of grep and was trying to optimize it to be as fast as possible.

When I came across the 1-BRC challenge I pivoted to it because the challenge sound fun and interest and aligned with my goals.

## Naive Approach
I just used the C++ provided:
    - ifstream: for file handline
    - getline: to read each line
    - stof: string to float converter
    - map: for my sorted db
    - string: to get the temp and station

I created a struct which would have the min, max, sum and count. In the struct I created 3 functions which would just initialize the station data, update the data and give the mean.

I stored the station data with in the sorted map in the format <Station, tempStats>.

I opened and read the file using ifstream (read only mode of fstream) and then get each line using the getline() inbuild function and then parse the station and temp at each line (converting from string to float using stof) and then store them in a tempr var.

After that I checked if the station existed in the map or not if it did then I would update the vals otherwise I would initialize it.

At the end I would just print the stats from the map.

## Optimized Version
Since the time I started optimizations, perf (profiler) became my best friend. I would not optimize anything without consulting perf.

### Optimization 1: memap
The first bottleneck was file reading. After reading how ifstream works and the abstraction layers, I realized that I dont need any of it. So I decided not to use them and go straight to the source. 

However instead of read() I decided to go with mmap as I found out that its better for larger files.

Along with that I changed from getline to memchr.

### Optimization 2: string to number conversion
The second bottleneck was the conversion. Converting from string to float was extremely slow and was occupying 4 bytes.

I started looking into the c++ inbuilt function from_chars and I did implement it initally. But then later I realized that I know that the temperature is going to be between [-99.9,99.9].

So I decided to build my own custom temperature parser and decided to convert them into int 16 which is 2 bytes and then later at the end I would convert them to float before printing the result as that is cheaper that converting to float and storing it in the db.

### Optimization 3: Custom DB
My initial optimazation was using unordered map and reserving 10000 bucket element spaces. However even after that it was not as fast as I expected it to be because unordered maps are node based which are not very cache friendly.

One of the rules was not to use 3rd party libraries, therefore I decided to build one from scratch. 

I went with open addressing and linear probing approach because it was cache friendly. 

## Currently working on
Currently working on the chunking of the file for multithreading.

