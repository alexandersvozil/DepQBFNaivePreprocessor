# DepQBFNaivePreprocessor
A naive implementation of a QBF preprocessor for depqbf.
Compilation instructions:

1. Clone the repository.
2. Download DepQBF from http://lonsing.github.io/depqbf/ (version 4.01)
3. Compile DepQBF with "make"
4. Rename the folder of DepQBF from "depqbf-version_x_X_X" to "depqbf" and put it in the folder of the cloned repository.
5. use "cmake ." and "make" to compile the project.
6. it is advised to run "python executetests.py" to run the tests.


Credits:

1. Florian Lonsing for his wonderful QBF-Solver DepQBF: http://lonsing.github.io/depqbf/
2. Enrico Giunchiglia, Paolo Marin and Massimo Narizzano: Preprocessing Techniques
for QBFs (http://ceur-ws.org/Vol-451/paper09giunchiglia.pdf) - on page 8 they describe a very intuitive and effective way to remove subsumption in clauses. 

IMPORTANT NOTE: 
I used gcc 4.9.2 to compile the code. Version 4.8.4 of gcc seems to have problems to produce the correct program resulting in several segmentation faults when using the program with Long Distance Resolution on the benchmarks (argument 'l')


