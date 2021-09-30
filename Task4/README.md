##  Matrix multiplication

Common options for siglethread and multithread programs 

`-s` sets the size of arrays, takes 3 numbers cause we don't need number of rows in second array

Use: `-s <number of rows array 1> <number of colums array 1> <number of colums array 2>`

`-r` sets random seed for generatin values, need this to create the same matrices in two programs

Use: `-r <random seed>`

How to run:

#### 1. In `SingleThread` directory 

```
./build.sh
./run.sh -s 10 10 10 -r 42
```

#### 2. In `MultiThread` directory 

2.1. With number of threads from env

```
./build.sh
./run.sh -s 10 10 10 -r 42
```
Option
`-t` sets the size of threads
Use: `-s <number of threads>`

2.2 With typing number of threads

```
./build.sh
./run.sh -t 5 -s 10 10 10 -r 42
```

#### 3. In `Task4` directory 
`diff multi.txt single.txt` what should have empty output that means multithreading program works correctly

## Notes
### 1. 
You can run program without -s of -r flag, it would use default settings.
### 2.
Please choose the same random seed in singlethread and multithread programs, otherwise generated matrices will be different
