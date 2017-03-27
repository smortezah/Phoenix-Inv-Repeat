## INSTALLATION
**Cmake** is required for the installation. Also, **git** is suggested for cloning the repository.
```bash
sudo apt-get install cmake git
git clone https://github.com/smortezah/Phoenix.git
cd Phoenix/
cmake .
make
```

## USAGE
```commandline
./phoenix [OPTION]... -m [MODEL(S)] -r [REFERENCE(S)] -t [TARGET(S)]
```
To see the options, type:
`./phoenix -h`

This will print the following:
```commandline
Synopsis:
    ./phoenix [OPTION]... -m [MODEL(S)] -r [REFERENCE(S)] -t [TARGET(S)]
    
         EXAMPLE: ./phoenix -n 4 -d -m 0,11,100:1,9,10 -r HS21,HSY -t PT19,PT21,PT22,PTY
         
Mandatory arguments:
    -m [<i_1>,<k_1>,<a_1>:<i_2>,<k_2>,<a_2>:...],   --model [<i_1>,<k_1>,<a_1>:...]
         context model(s) (at least one).
         <i>:  inverted repeat (0=don't use, 1=use).
         <k>:  context-order size,
         <a>:  1/alpha,
         EXAMPLE: -m 0,11,100:1,9,10

    -r [REFERENCE(S)],   --reference [REFERENCE(S)]
         reference file(s) address(es)
         EXAMPLE: -r ref1,ref2

    -t [TARGET(S)],   --target [TARGET(S)]
         target file(s) address(es)
         EXAMPLE: -t tar1,tar2,tar3

Options:
    -h,  --help
         usage guide

    -d,  --decompress
         enable decompression

    -n [NUMBER (integer)],  --n_threads [NUMBER (integer)]
         number of threads (default: 2).
         requires an integer number (n >= 1)

    -g [NUMBER (float)],  --gamma [NUMBER (float)]
         gamma (default: 0.95).
         requires a float number (0 <= g < 1)

    -V,  --version
         version number

    -v,  --verbose
         verbose mode (more information)
```

## CITATION
Please cite the following papers, if you use <i>Phoenix</i>:
* M. Hosseini, D. Pratas and A.J. Pinho, "On the role of inverted repeats in DNA sequence similarity," *11th International Conference on Practical Applications of Computational Biology & Bioinformatics (PACBB 2017)*, Springer International Publishing, 2017. (**ACCEPTED**)

## ISSUES
Please let me know if there is any issues, at [issues]
(https://github.com/smortezah/Phoenix/issues).

## LICENSE
GPL v3.

For more information, please visit
[http://www.gnu.org/licenses/gpl-3.0.html]
(http://www.gnu.org/licenses/gpl-3.0.html).
