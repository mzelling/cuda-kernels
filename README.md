### cuda-kernels

To run any of the *.cpp files in this repo, make sure you have nvcc (NVIDIA C compiler) installed.

Then run 
nvcc -x cu <cpp-file> -o <executable-file>. 

For example, to compile the vector addition example, run 
nvcc -x cu vec_add.cpp -o vec_add.

Then you can launch the program by running ./vec_add on the command line.
(If the program is not executable, make sure to run "sudo chmod +x vec_add" to give execution permission.)





