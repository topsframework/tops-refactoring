ToPS
======

[![Build Status](https://travis-ci.org/topsframework/tops-refactoring.svg)](https://travis-ci.org/topsframework/tops-refactoring)
[![Coverage Status](https://coveralls.io/repos/topsframework/tops-refactoring/badge.svg)](https://coveralls.io/r/topsframework/tops-refactoring)
[![Coverity Scan Build Status](https://scan.coverity.com/projects/4131/badge.svg)](https://scan.coverity.com/projects/4131)

ToPS is an objected-oriented framework implemented with C++ which 
facilitates the integration of probabilistic models for sequences 
over an user defined alphabet. ToPS contains the implementation of 
eight distinct models to analyze discrete sequences:

1. Independent and identically distributed model
2. Variable-Length Markov Chain (VLMC)
3. Inhomogeneous Markov Chain
4. Hidden Markov Model
5. Pair Hidden Markov Model
6. Profile Hidden Markov Model
7. Similarity Based Sequence Weighting
8. Generalized Hidden Markov Model (GHMM)

Users can implement models either by manual description of the 
probability values in a configuration file, or by using training 
algorithms provided by the system. ToPS framework also includes 
a set of programs that implement bayesian classifiers, sequence 
samplers, and sequence decoders. Finally, ToPS is an extensible and 
portable system that facilitates the implementation of other 
probabilistic models, and the development of new programs.

Documentation
=============

http://tops.sourceforge.net/tutorial.pdf
http://tops.sourceforge.net/api.html

Git Repository
==============

You can download the development version of ToPS by executing the 
command below:

```bash
git clone https://github.com/ayoshiaki/tops.git
```

Platforms
=========

ToPS was designed to run on Unix/Linux operating systems. 
Tested platforms include: MacOS X and Ubuntu Linux.

Software Requirement
====================

ToPS was written in C++. It was compiled using the g++ version 4.2.1 
and it requires:

- Boost C++ Libraries version 1.52
- Git

Installing ToPS
===============

1. Download ToPS from GitHub  

   ```bash
   git clone --recursive https://github.com/topsframework/tops-refactoring.git
   ```

   This will create a directory named tops

2. Go to the tops directory:

   ```bash
   cd tops-refactoring
   ```

3. Run make

   ```bash
   make
   ```

5. Run make install

   ```bash
   sudo make install
   ```

6. If you are using linux run ldconfig

   ```bash
   sudo ldconfig
   ```
