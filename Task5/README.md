##  Count series task

In both folders common options

`-t` sets the number of threads to run

Use: `-t <number of threads>`

`-n` sets the number of iterations in series

Use: `-n <random seed>`

How to run:

1. With number of threads from env

```
./build.sh
./run.sh -n 100
```

2. With typing number of threads

```
./build.sh
./run.sh -t 5 -n 100
```
## Notes
### 1. 
In `Exponent` folder useless to choose number of iterations more than 20
### 2.
In `ArcSin` folder choose number of iterations up to 150, more will cause `nan`
