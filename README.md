<h2>INSTALLATION</h2>
**Cmake** is required for the installation. Also, **git** is suggested for cloning the repository.
<pre>
sudo apt-get install cmake git
git clone https://github.com/smortezah/Phoenix.git
cd Phoenix/
cmake .
make
</pre>

<h2>USAGE</h2>
<pre>
./phoenix [OPTION]... -m [MODEL(S)] -r [REFERENCE(S)] -t [TARGET(S)]
</pre>
To see the options, type:

<code>
./phoenix -h
</code>

This will print the following:
<pre>
Synopsis:
    ./phoenix [OPTION]... -m [MODEL(S)] -r [REFERENCE(S)] -t [TARGET(S)]
    
         EXAMPLE: ./phoenix -n 4 -d -m 0,11,100:1,9,10 -r HS21,HSY -t PT19,PT21,PT22,PTY

Mandatory arguments:
    -m [&lt;i_1&gt;,&lt;k_1&gt;,&lt;a_1&gt;:&lt;i_2&gt;,&lt;k_2&gt;,&lt;a_2&gt;:...],   --model [&lt;i_1&gt;,&lt;k_1&gt;,&lt;a_1&gt;:...]
         context model(s).
         &lt;i&gt;:  inverted repeat (0=don't use, 1=use).
         &lt;k&gt;:  context-order size,
         &lt;a&gt;:  1/alpha,
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
         decompression

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

</pre>

<h2>CITATION</h2>
Please cite the following papers, if you use <i>Phoenix</i>:

<h2>ISSUES</h2>
Please let me know if there is any issues, at <a href="https://github.com/smortezah/Phoenix/issues">issues</a>.

<h2>LICENSE</h2>
GPL v3.

For more information:
http://www.gnu.org/licenses/gpl-3.0.html
