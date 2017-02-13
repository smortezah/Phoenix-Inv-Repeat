<h2>INSTALLATION</h2>
Cmake is required for the installation. Also, git is suggested for cloning the repository.
<h3>Linux</h3>
<pre>
sudo apt-get install cmake git
git clone https://github.com/smortezah/Phoenix.git
cmake src
make
</pre>

<h2>EXECUTION</h2>
<pre>
./phoenix [OPTION]... -m [MODEL(s)] -t [TARGET] -r [REFERENCE]
</pre>
To see the options, type:
<pre>
./phoenix -h
</pre>
This will print the following:
<pre>
Synopsis:
    ./phoenix [OPTION]... -m [MODEL(s)] -t [TARGET] -r [REFERENCE]

Mandatory arguments:
    -t [TARGET],  --target [TARGET]
         target file address

    -r [REFERENCE],  --reference [REFERENCE]
         reference file address

Options:
    -h,  --help
         usage guide

    -V,  --version
         version number

    -v,  --verbose
         verbose mode (more information)

    -m [&lt;rt&gt;,&lt;s&gt;,&lt;a&gt;,&lt;i&gt;:...],  --model [&lt;rt&gt;,&lt;s&gt;,&lt;a&gt;,&lt;i&gt;:...]
         context model(s). Seperate different models with :
         &lt;rt&gt;: build a model based on reference (r) or target (t),
         &lt;s&gt;:  context-order size,
         &lt;a&gt;:  1/alpha,
         &lt;i&gt;:  inverted repeat (0=don't use, 1=use).
         EXAMPLE 1: -m r,4,1000,1
         EXAMPLE 2: -m t,18,1,0:r,13,100,0

    -n [NUMBER],  --number [NUMBER]
         number of something.
         requires an integer number (NUMBER)

    -d [NUMBER],  --fnumber [NUMBER]
         number of something.
         requires a float number (NUMBER)
</pre>

<h2>ISSUES</h2>
Please let me know if there is any issues, at <a href="https://github.com/smortezah/Phoenix/issues">issues</a>.

<h2>LICENSE</h2>
GPL v3.

For more information:
http://www.gnu.org/licenses/gpl-3.0.html
