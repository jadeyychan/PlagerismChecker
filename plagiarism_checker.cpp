/* 
Name: scan_files.cpp
Author: Jade Chan
Date: Nov 11, 2015

Summary: This program checks the similarity of two files based on a file of 
synonyms. 
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

typedef unordered_map<string, string > stringmap;

void Hash_Map (vector<string> wordLines, stringmap &syn);
vector<string> replace_syn(vector<string> file_words, int n_tuple, stringmap &syn);
bool exists (string word, stringmap &syn);
vector<string> parsing_tuples(vector<string> file_words, vector<string> file_tuples, int n_tuple);
int word_checker(vector<string> file1_words, vector<string> file2_words);

int main(int argc, char* argv[]) {

	stringmap * syn = new stringmap(); // Hashmap
	vector<string> file1_words; // Words in file1
	vector<string> file2_words; // Words in file2
	vector<string> file1_tuples; // Tuples of words in file1
	vector<string> file2_tuples; // Tuples of words in file1
	int percentage;
	int tuple = 3; // default tuple size = 3

	// Checking number of arguments
	if ((argc > 5) || (argc < 4)){
		cerr << "Wrong usage!" << endl;
		exit(1);
	} 

	// Reading in file of synonyms
	ifstream synfile(argv[1]);
	if (synfile.is_open()) {

		// getting each line
		string eachLine;
		while (getline(synfile, eachLine)) {
			vector<string> wordLines;

			// getting each word
			string eachWord;
			for (int i = 0; i < eachLine.length(); i++) {
				eachWord = "";

				// for each line, as long as it is not a blank space or endline,
				// add it to the string eachWord				
				while ((eachLine[i] != ' ') && (eachLine[i] != '\n') && (i < eachLine.length())) {
						eachWord = eachWord + eachLine[i];
						i = i + 1;
					}
				// Adding each word to the vector of each line
				wordLines.push_back(eachWord);
			}
			Hash_Map(wordLines, *syn);
		} 
	}
	synfile.close();

	//reading in file1 to be checked
	ifstream file1(argv[2]);
	if (file1.is_open()) {
		string curr_word;
		while (file1 >> curr_word) {
			// pushing each word to the vector file1_words
			file1_words.push_back(curr_word);
		}
	}
	file1.close();

	//reading in file2
	ifstream file2(argv[3]);
	if (file2.is_open()) {
		string curr_word;
		while (file2 >> curr_word) {
			// pushing each word to the vector file2_words
			file2_words.push_back(curr_word);
		}
	}
	file2.close();

	// Handling tuple input
	if (argv[4]) {
		tuple = stoi(argv[4]);
	}

	// Replacing each synonym with the first word in the line to equate them
	file1_words = replace_syn(file1_words, tuple, *syn);
	file2_words = replace_syn(file2_words, tuple, *syn);

	// Concatenating the words into 1 string, based on tuple size
	file1_tuples = parsing_tuples(file1_words, file1_tuples, tuple);
	file2_tuples = parsing_tuples(file2_words, file2_tuples, tuple);

	// Checking the similarity of the tuples
	percentage = word_checker(file1_tuples, file2_tuples);

	// Printing the percentage
	cout << percentage << "%" << endl;

	// Freeing memory used by Hashmap
	syn->clear();

	return 0;
}

/* 
PURPOSE:	Maps words in each line of synonyms file to the first word of each line.
			Ex. 'run sprint jog'
				Key : Value
				sprint : run
				jog: run
PARAMETERS:	This function takes in a vector of strings, and a hashmap.
RETURNS:	None, hashmap is passed by reference
*/
void Hash_Map (vector<string> wordLines, stringmap &syn) {

	string val = wordLines[0];

	for (int i = 1; i < wordLines.size(); i++) {
		string key = wordLines[i];
		pair <string, string> toInsert (key, val);
		syn.insert(toInsert);
	}
}

/* 
PURPOSE:	Equates the words in the synonyms file.
			Replaces the synonyms with the value, the first word in each line.
PARAMETERS:	Vector of strings - the words from file
			int, the number of words in each tuple
			stringmap, the hashmap of the synonyms
RETURNS:	Vector of strings - words from file with synonyms replaced
*/
vector<string> replace_syn(vector<string> file_words, int n_tuple, stringmap &syn) {
	for (int i = 0; i < file_words.size(); i++) {
		string curr_word = file_words[i];
		if (exists(curr_word, syn)) {
			// get the value of the key
			string new_word = syn[curr_word];

			// replace word with the key
			file_words[i] = new_word;		
		}
	}

	return file_words;
}

/* 
PURPOSE:	Checks if a word is hashed in the stringmap
PARAMETERS:	a string - the word to be checked
			stringmap of all the hashed words
RETURNS:	True if the word is hashed,
			False otherwise
*/
bool exists (string word, stringmap &syn) {
	for (int i = 0; i < syn.bucket_count(); i++) {
		for (auto elem = syn.begin(i); elem != syn.end(i); elem++) {
			string new_word = elem -> first;
			if (word == elem -> first) {
				return true;			
			}
		}
	}
	return false;
}

/* 
PURPOSE:	Concatenates multiple strings into 1, based on tuple size
PARAMETERS:	vector of strings, the words
			vector of strings, the tuples
			int, the number of words in each tuple
RETURNS:	vector of strings, the tupled words
*/
vector<string> parsing_tuples(vector<string> file_words, vector<string> file_tuples, int n_tuple) {

	for (int i = 0; i < file_words.size(); i++) {
		if(( i + n_tuple -1) < file_words.size()) { // ensure we are not running through the end of the array
			string tuple;
			for (int j = 0; j < n_tuple; j++) {
				tuple = tuple + " " + file_words[i+j];
			}
			file_tuples.push_back(tuple);
		}
	}
	return file_tuples;
}

/* 
PURPOSE:	Checks the similarity of the strings in each vector
PARAMETERS:	2 vectors of string
RETURNS:	int, the percentage of similarity
*/
int word_checker(vector<string> file1_tuples, vector<string> file2_tuples) {

	int count = 0;
	int percentage;
	int file1_length = file1_tuples.size();

	if (file1_length != file2_tuples.size()) {
		cout << "incorrect files!" << endl;
		exit(1);
	} else {
		for (int i=0; i < file1_length; i++) {
			string tuple_1 = file1_tuples[i];
			string tuple_2 = file2_tuples[i];

			if (tuple_1 == tuple_2) {
				count++;
			}
		}
	}
	percentage = (count * 100) / file1_length;
	return percentage;
}
