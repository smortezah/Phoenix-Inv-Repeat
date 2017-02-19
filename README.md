<h2>INSTALLATION</h2>
Cmake is required for the installation. Also, git is suggested for cloning the repository.
<pre>
sudo apt-get install cmake git
git clone https://github.com/smortezah/Phoenix.git
cd Phoenix/
cmake .
make
</pre>

<h2>USAGE</h2>
<pre>
./phoenix [OPTION]... -m [MODEL] -r [REFERENCE(S)] -t [TARGET(S)]
</pre>
To see the options, type:

<code>
./phoenix -h
</code>

This will print the following:
<pre>
Synopsis:
    ./phoenix [OPTION]... -m [MODEL] -r [REFERENCE(S)] -t [TARGET(S)]
         EXAMPLE: ./phoenix -n 6 -m 0,11,100 -r HS21 -t PT21,PT22

Mandatory arguments:
    -m [&lt;i&gt;,&lt;k&gt;,&lt;a&gt;],   --model [&lt;i&gt;,&lt;k&gt;,&lt;a&gt;]
         context model(s).
         &lt;i&gt;:  inverted repeat (0=don't use, 1=use).
         &lt;k&gt;:  context-order size,
         &lt;a&gt;:  1/alpha,
         EXAMPLE: -m 9,100,1

    -r [REFERENCE(S)],   --reference [REFERENCE(S)]
         reference file(s) address(es)
         EXAMPLE: -r ref1,ref2

    -t [TARGET(S)],   --target [TARGET(S)]
         target file(s) address(es)
         EXAMPLE: -t tar1,tar2,tar3

Options:
    -h,  --help
         usage guide

    -V,  --version
         version number

    -v,  --verbose
         verbose mode (more information)

    -n [NUMBER],  --n_threads [NUMBER]
         number of threads.
         requires an integer number (n >= 1)
</pre>

<h2>CITATION</h2>
Please cite the following papers, if you use <i>Phoenix</i>:

<h2>ISSUES</h2>
Please let me know if there is any issues, at <a href="https://github.com/smortezah/Phoenix/issues">issues</a>.

<h2>LICENSE</h2>
GPL v3.

For more information:
http://www.gnu.org/licenses/gpl-3.0.html
