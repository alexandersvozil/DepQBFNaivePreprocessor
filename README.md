# DepQBFNaivePreprocessor
A (very!) naive implementation of a QBF preprocessor for depqbf.
Compilation instructions:

1. Clone the repository.
2. Download DepQBF from http://lonsing.github.io/depqbf/ (version 4.01)
3. Compile DepQBF with "make"
4. Rename the folder of DepQBF from "depqbf-version_x_X_X" to "depqbf" and put it in the folder of the cloned repository.
5. use "cmake ." and "make" to compile the project.
6. it is advised to run "python executetests.py" to run the tests.


Credits:
Florian Lonsing for his wonderful QBF-Solver DepQBF: http://lonsing.github.io/depqbf/ 
Enrico Giunchiglia, Paolo Marin and Massimo Narizzano: Preprocessing Techniques
for QBFs (http://ceur-ws.org/Vol-451/paper09giunchiglia.pdf) - on page 8 they desribe a very intuitive and effective way to remove subsumption in clauses.
