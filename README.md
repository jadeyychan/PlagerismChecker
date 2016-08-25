# Plagiarism Checker
Author: Jade Chan, Date: Nov 11, 2015

### SUMMARY
This program checks the similarity of two files, accounting for synomyms.

---

### COMPILE & RUN
	g++ plagiarism_checker.cpp -o plagiarism_checker
	./plagiarism_checker synonyms.txt file1.txt file2.txt

---

### SOLUTION
This program first reads through the synonyms file, and hashes then accordingly. 
It takes the words in each line, and hashes them as the key, with the 
value being the first word in that line.

	For example, the line "run sprint jog" is hashed as
		   key : value
		----------------
		sprint : run
		   jog : run

Then, it equates the synonyms, writing them to be the same word.
The words in each input file are then read through, and checked if they are
one of the hashed words. If they are — they are replaced by the first word in the line of synonyms.

	For example, the line 
		"go for a jog" 

	is replaced with 
		"go for a run"

The words are then concatenated into 1 string according to the tuples count.

The two files are then checked against each other, and the percentage is
generated. 

---

### DATA STRUCTURES USED

1. Hashmap:
This was used to store each synonym and the key. This data structure was chosen
as it provides constant time for the lookup for a word. 

2. Vectors:
This was used to store all the words in a file. This data structure was chosen 
as the words in a file only need to be traversed linearly. Access to the middle
of the vector was not necessary.

---

### ALGORITHMIC COMPLEXITY & RUNTIME
This program currently runs on n^2 time, as there are 2 doubly-nested for-loops in
the exists, and parsing_tuples function. 


