##  Quick sort task

Options

`-r` mode of the program
Use:

if without it or `-r RANDOM` sets the randdom seed from time

if `-r <number>` random seed will be equal to number, need to reproduce the array

if `-r ORDERED` will create an ordered array of set length

if `-r LINE` will expect array from command line after running program

if `-r FILE` will expect array from `input.txt` file in the same folder

`-t` sets the number of threads to run

Use: `-t <number of threads>`

`-l` Sets the len of array to generate in RANDOM, ORDERED, number mode

In LINE and FILE mode it's a number of elements that will be read to sort

If it won't be set, it will be set by 50

Use `-l <length of array>`

How to run:

1. With number of threads from env

```
./build.sh
./run.sh -l 100 -r <mode_name>
```

2. With typing number of threads

```
./build.sh
./run.sh -l 100 -r <mode_name> -t 4
```
