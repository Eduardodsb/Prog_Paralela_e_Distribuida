# Programação Paralela e Distribuida

Trabalhos realizados na disciplina de Programação Paralela e Distribuida em 2019.2. Tal disciplina tem como foco a aprendizagem de MPI, OpenMP e OpenACC e a mesma foi lecionada pelo professor Gabriel Pereira.


## MPI - Requisítos necessários:

### - Ambiente linux:
* Bibliotecas do MPI

#### comando para instalar o OpenMPI:<br>
```
$ sudo apt-get install openmpi-bin
$ sudo apt-get install openmpi-doc
$ sudo apt-get install libopenmpi-dev
```
#### comando para compilar e executar:<br>
```
$ mpicc -o CodName CodName.c 
$ mpirun -np 4 ./CodName
```
obs: 4 representa o número de threads

### - Ambiente Windows:
* Bibliotecas do MPI
* Visual Studio

#### Link para baixar e instalar o MPI:<br>
https://www.microsoft.com/en-us/download/details.aspx?id=57467

É necessário incluir a biblioteca no Visual Studio (https://blogs.technet.microsoft.com/windowshpc/2015/02/02/how-to-compile-and-run-a-simple-ms-mpi-program/)


## OpenMP - Requisítos necessários

#### comando para compilar e executar:<br>
```
$ gcc -o CodName CodName.c -fopenmp 
$ pgcc -mp -Minfo=all -o cods cods.c 
$ ./CodName
```

## OpenACC - Requisítos necessários

#### comando para compilar e executar:<br>
```
$ gcc -o CodName CodName.c -fopenacc
$ pgcc -acc -ta=nvidia -Minfo=all -o cods cods.c
$ ./CodName
```
