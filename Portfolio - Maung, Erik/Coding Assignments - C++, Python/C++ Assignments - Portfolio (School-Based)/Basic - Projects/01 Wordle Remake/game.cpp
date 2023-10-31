#include "utilities.h"
#include <iostream>
#include <cstring>
#include <cctype>
using namespace std;

const int MAXWORDS = 9000;
const char WORDFILENAME[] = "C:/VS/P5Files/words.txt";

int playOneRound(const char words[][MAXWORDLEN+1], int nWords, int wordnum);
int countGold(char probeWord[], char hiddenWord[]);
int countSilver(char probeWord[], char hiddenWord[]);

int main()
{
	char wordList[MAXWORDS][MAXWORDLEN + 1];			// initialize the word list
	int n = getWords(wordList, MAXWORDS, WORDFILENAME);
	if (n < 1) { // if there is not at least one word in the file
		cout << "No words were loaded, so I can't play the game." << endl; // return -1: not positive roundNum input
		return -1;
	}
	int roundNum = 0; // initialize round count variables

	int triesInRound = 0;
	int totalTries = 0;

	double average = 0;  // initialize round statistics variables
	int minTries = 0;
	int maxTries = 0;

	cout << "How many rounds do you want to play? ";
	cin >> roundNum;
	cin.ignore(10000, '\n');
	if (roundNum <= 0) {
		cout << "The number of rounds must be positive." << endl; // return -1: not positive roundNum input
		return -1;
	}

	cout.setf(ios::fixed);      // round doubles to 2 digits, show decimals even with just zeroes
	cout.setf(ios::showpoint);
	cout.precision(2);

	for (int i = 1; i <= roundNum; i++) {
		int wordnum = randInt(0, n - 1); // choose random word from list
		int wordLength = 0;

		for (int i = 0; wordList[wordnum][i] != '\0'; i++) { // run loop upto and including hitting zero byte
			wordLength++;						// increment wordLength to match
		}

		cout << endl << "Round " << i << endl; // each round has preceding newline
		cout << "The hidden word is " << wordLength << " letters long." << endl;
		triesInRound = playOneRound(wordList, n, wordnum); // count the tries (playOneRound returns this #)
		if (triesInRound == 1) {
			cout << "You got it in " << triesInRound << " try." << endl; // output # of tries (==1)
		}
		else {
			cout << "You got it in " << triesInRound << " tries." << endl; // output # of tries (>1)
		}

		totalTries += triesInRound;			   // cumulative counter of tries, all rounds
		if (i == 1) {						   // if this is first round, initialize min and max to tries #
			minTries = triesInRound;
			maxTries = triesInRound;
		}

		if (triesInRound > maxTries)		  // assign it to maxTries if greater
			maxTries = triesInRound;

		if (triesInRound < minTries)          // assign it to minTries if less
			minTries = triesInRound;

		average = static_cast<double>(totalTries) / i;
		cout << "Average: " << average << ", minimum: " << minTries << ", maximum: " << maxTries << endl;
	}

	return 0;
}

int playOneRound(const char words[][MAXWORDLEN+1], int nWords, int wordnum){
	char hiddenWord[7] = ""; // set as empty string so that it's known at compile time

	for (int i = 0; words[wordnum][i] != '\0'; i++) { // run loop upto and including hitting zero byte
		hiddenWord[i] = words[wordnum][i];	// assign word to hiddenWord, character by character
		hiddenWord[i + 1] = '\0';			// shift the zero byte incrementally
	}
	
	int numTries = 0; // use as counter for number of guess attempts

	for (;;) {

		char probeWord[102] = ""; // initialize to empty string so that it's known at compile time
								  // assume input will never be over 101 characters (at 102 for unnecessary sanity check)

		cout << "Probe word: ";
		cin.getline(probeWord, 102);

		bool isValidProbeWord = true; // assume is valid until we determine 

		if (strlen(probeWord) < MINWORDLEN || strlen(probeWord) > MAXWORDLEN) { // if not 4-6 characters
			isValidProbeWord = false; // set as INVALID
		}
		for (int i = 0; i < strlen(probeWord); i++) {
			if (!islower(probeWord[i])) { // check every character in the word in the probeWord array
				isValidProbeWord = false; // set as INVALID
				break;
			}
		} // note: if the entry is invalid, everything below gets skipped, including the numTries counter

		bool isUnknownWord = true; // assume the word is unknown until we find it in the wordList

		for (int i = 0; i < nWords ; i++) {
			if (strcmp(probeWord, words[i]) == 0) { // cycles through every word
				isUnknownWord = false; // change to Known Word if we find one match
			}
		}

		if (!isValidProbeWord) // if it is NOT valid:
			cout << "Your probe word must be a word of 4 to 6 lower case letters." << endl;
		else if (isUnknownWord) {
			cout << "I don't know that word." << endl;

		}
		else {
			numTries++;
			if (strcmp(probeWord, hiddenWord) != 0) {
				cout << "Golds: " << countGold(probeWord, hiddenWord) << ", Silvers: " << countSilver(probeWord, hiddenWord) << endl;
			}
		}

		bool isMatch = 1; // assume we found the word until proven otherwise

		if (strlen(probeWord) != strlen(hiddenWord)) { // first check if the lengths don't match
			isMatch = 0; // different lengths = automatically not match
		}

		for (int i = 0; i < strlen(hiddenWord) && i < strlen(probeWord); i++) { // make sure we only check character positions that both words contain
											   // (if they have different lengths, it's fine because we accounted for that in lines 133-135)
			if (probeWord[i] != hiddenWord[i]) // if we come across any mismatched character
				isMatch = 0;
		}

		if (isMatch) { // check if it's a perfect match FIRST (count gold and silver later if it isn't)
			break;
		}
	}



	return numTries;
}

int countGold(char probeWord[], char hiddenWord[]) {
	int gold = 0;
	int minLength = strlen(probeWord);  //  find which word is shorter (so we dont go out of bounds while comparing characters)
	if (strlen(probeWord) > strlen(hiddenWord)) {
		minLength = strlen(hiddenWord);
	}

	for (int i = 0; i < minLength; i++) {
		if (probeWord[i] == hiddenWord[i]) // if they match, it's a gold letter
			gold++;
	}

	return gold;
}

int countSilver(char probeWord[], char hiddenWord[]) {
	int silver = 0;
	char unusedHidden[7] = "111111"; // suppose '1' means an unused character, '0' means a used character
	char unusedProbe[7] = "111111";
	// this'll prevent double-counting silvers

	for (int i = 0; i < strlen(hiddenWord); i++) { // loop through hidden word
		for (int j = 0; j < strlen(probeWord); j++) { // loop through probe
			if ( (i < strlen(probeWord) && probeWord[i] == hiddenWord[i]) || (j < strlen(hiddenWord) && probeWord[j] == hiddenWord[j]) ) {
				continue; // skip this iteration, it's already a gold match
			} // check if there's a gold match, but don't go out of bounds

			if (i != j && unusedHidden[i] == '1' && unusedProbe[j] == '1') // if the characters are in different positions
				// AND if the hiddenWord i-th character is unused, then check:
				if (probeWord[j] == hiddenWord[i]) {
					silver++;				  // counts as silver
					unusedHidden[i] = '0';
					unusedProbe[j] = '0'; // set both letters as USED - don't double count!
				}
		}
	}

	return silver;
}