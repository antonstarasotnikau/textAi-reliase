#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <cgicc/CgiDefs.h>
#include <cgicc/Cgicc.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTMLClasses.h>
#include <fstream>

using namespace std;
using namespace cgicc;

const int  N = 10240;
const int N_COUNT_WORD = 3;

double antiPlagiarism(string text, string fragment);
char charInLowerCasee(char symbol);
void arrayCharToLowerCase(char array[N]);
int getLenghtWord(char word[N], char text[N], int startSearch);
bool compareTwoArrayChar(char firstWord[N], char secondWord[N]);
void deleteDuplicate(char text[N], int lenghtCurrentWord, int lenghtNextWord);
bool isSeparator(char c);
void findAndDeleteDuplicate(char text[N]);
void findAndDeleteDoubleSeporator(char text[N]);
int getLenghtNCountWord(char word[N], char text[N], int startSearch, int nCountWord);
char changeLatineInKirilSymbol(char symbol);
bool isLatineSymbol( char symbol);
void convertStringToArrayChar(char array[N], string string);
void changeLatineLettersInWord(char word[]);
bool hasLatineAndKirilLetters(char word[]);
void replaceAllLatineLettersInRussianWord(char text[N]);


int getInt(string name);//get length of text putted in the form on your site(this function we use only for example)
string getDB();//get origin text from db.txt (don't modify tis function)

int main()
{
	string string1 = "";
	setlocale(LC_ALL, "Russian");
    Cgicc form;
    string name;

    cout << "Content-type:text/html\r\n\r\n";
    cout << "<html>\n";
    cout << "<head>\n";
    cout << "<title>Ggi Server</title>\n";
    cout << "</head>\n";
    cout << "<body>\n";
    cout << "<p>";
    
	
    name = form("name");
    if (!name.empty()) {
    	cout << antiPlagiarism(getDB(), name) << "\n";
    } else {
    	cout << "Text is not provided!\n";
    }	
    cout << "</p>\n";
    cout << "</body>\n";
    cout << "</html>\n";
	
    return 0;				
}

string getDB(){
	std::ifstream in("db.txt");

    string dbText = "", s1;
    while (getline(in, s1))
    	dbText += s1;
  
    in.close();
    
    return dbText;
}
double antiPlagiarism(string text, string fragment) {
        char textChar[N];
        char fragmentChar[N];

        convertStringToArrayChar(textChar, text);
        convertStringToArrayChar(fragmentChar, fragment);

        arrayCharToLowerCase(textChar);
        arrayCharToLowerCase(fragmentChar);

        //findAndDeleteDuplicate(textChar);
        //findAndDeleteDuplicate(fragmentChar);

        double countPartFragmentFromText = 0;
        double countPartFragmentCompare = 0;
        char nWordInText[N];
        char nWordInFragment[N] = {""};
        int j = 0;
        int jFrag = 0;
        while (fragment[j] != '\0') {
                //for(int k = 0; k <3; k++) {
                        nWordInFragment[0] = '\0';
                        jFrag = getLenghtNCountWord(nWordInFragment, fragmentChar, j, N_COUNT_WORD);

                        int i = 0;
                        int iFrag = 0;
                        while (fragmentChar[j] != '\0') {
                                nWordInText[0] = '\0';
                                iFrag = getLenghtNCountWord(nWordInText, textChar, i, N_COUNT_WORD);
                                if (compareTwoArrayChar(nWordInText, nWordInFragment)) {
                                        countPartFragmentCompare++;
                                }
                                countPartFragmentFromText++;

                                //cout << i << " " << iFrag << " " << nWordInText << " " << j << " " << jFrag << " " << nWordInFragment << " " << countPartFragmentCompare << endl;
                                i += getLenghtNCountWord(nWordInText, textChar, i, 1);
                                if(iFrag == 0) {
                                        break;
                                }
                        }

                        j += getLenghtNCountWord(nWordInText, fragmentChar, j, 1);
                        if(jFrag == 0) {
                                cout << "sdas";
                                break;
                        }
                }

        return (100 - (countPartFragmentCompare / countPartFragmentFromText) * 100);
}

void convertStringToArrayChar(char array[N], string string) {
        unsigned int i = 0;
        for (i = 0; i < string.length(); i++) {
                array[i] = string[i];
        }
        array[i] = '\0';
}

int getLenghtNCountWord(char word[N], char text[N], int startSearch, int nCountWord) {
        int iw = 0;
        int count = 0;
        int i = 0;
        for (i = startSearch; text[i] !='\0'; i++)
                if (!isSeparator(text[i])) {
                        word[iw] = text[i];
                        iw++;
                        if (isSeparator(text[i + 1]) || text[i + 1] == '\0') {
                                count++;
                                if (count == nCountWord) {
                                        word[iw] = '\0';
                                        return iw;
                                }
                        }
                }
                else {
                        word[iw] = text[i];
                        iw++;
                }
        /*if (text[i] == '\0')
                return -1;*/

        return 0;
}

void findAndDeleteDoubleSeporator(char text[N]) {
        int i = 0;
        while (text[i] != '\0') {
                if (isSeparator(text[i]) && isSeparator(text[i + 1])) {
                        deleteDuplicate(text, i, 1);
                }
                else {
                        i++;
                }
        }
}

void findAndDeleteDuplicate(char text[N]) {
        char currentWord[N];
        char nextWord[N];
        int i = 0;
        while (text[i] != '\0') {
                if (isSeparator(text[i])) {
                        i++;
                }
                else {
                        int lenghtCurrentWord = getLenghtWord(currentWord, text, i);
                        int lenghtNextWord = getLenghtWord(nextWord, text, i + lenghtCurrentWord);
                        if (lenghtCurrentWord == lenghtNextWord && compareTwoArrayChar(currentWord, nextWord)) {
                                deleteDuplicate(text, i, lenghtNextWord);
                        }
                        else {
                                i = i + lenghtCurrentWord;
                        }
                }
        }
}

void deleteDuplicate(char text[N], int lenghtCurrentWord, int lenghtNextWord) {
        int i = 0;
        for (i = lenghtCurrentWord; text[i] !='\0'; i++)
                text[i] = text[i + lenghtNextWord];
        text[i] = '\0';
}

int getLenghtWord(char word[N], char text[N], int startSearch) {
        int iw = 0;
        for (int i = startSearch; text[i] !='\0'; i++)
                if (!isSeparator(text[i])) {
                        word[iw] = text[i];
                        iw++;
                        if (isSeparator(text[i + 1]) || text[i + 1] == '\0') {
                                word[iw] = '\0';
                                return iw;
                        }
                }

        return 0;
}

char charInLowerCasee(char symbol) {
        if ((symbol >= 65 && symbol <= 90) || (symbol >= 128 && symbol <= 143))
                symbol += 32;
        if ((symbol >= 144 && symbol <= 159))
                symbol += 80;
        return symbol;
}

void arrayCharToLowerCase(char array[N]) {
        for (int i = 0; array[i] != '\0'; i++)
                array[i] = charInLowerCasee(array[i]);
}

bool compareTwoArrayChar(char firstWord[N], char secondWord[N]) {
        int i = 0;
        int j = 0;
        while (firstWord[i] != '\0' || secondWord[j] != '\0' ) {
                if (isSeparator(firstWord[i])) {
                        i++;
                        continue;
                }
                if (isSeparator(secondWord[j])) {
                        j++;
                        continue;
                }
                if (firstWord[i] != secondWord[j]) {
                        return false;
                }
                i++;
                j++;
        }
        if (firstWord[i] == '\0' && secondWord[j] == '\0')
                return true;

        return false;
}

bool isSeparator(char c) {
    char separators[] = {" ,.!?:;"};
    for (int  i = 0; separators[i] !='\0'; i++)
        if (separators[i] == c)
            return true;
    return false;
}

bool isLatineSymbol( char symbol) {
	if ((symbol >= 65 && symbol <= 90) || (symbol >= 97 && symbol <= 122))
		return true;
	return false;
}

void replaceAllLatineLettersInRussianWord(char text[N]) {
        int i = 0;
        while (text[i] != '\0') {
                if (isLatineSymbol(text[i]))
                        changeLatineInKirilSymbol(text[i]);
                i++;
        }
}

bool hasLatineAndKirilLetters(char word[]) {
        int countLatineLetters = 0;
        int countKirilLetters = 0;
        int i = 0;
        for (i = 0; word[i] != '\0'; i++) {
                if (isLatineSymbol(word[i]))
                        countLatineLetters++;
                else {
                        countKirilLetters++;
                }
        }
        if ((countKirilLetters != 0 && countLatineLetters != 0))
                return false;
        else
                return true;
}

void changeLatineLettersInWord(char word[]) {
        int i = 0;
        for (i = 0; word[i] != '\0'; i++) {
                if (isLatineSymbol(word[i]))
                        word[i] = changeLatineInKirilSymbol(word[i]);
        }
}

char changeLatineInKirilSymbol(char symbol) {
	switch (symbol) {
		case 69://E
			symbol = 133;
			break;
		case 84://T
			symbol = 146;
			break;
		case 79://O
			symbol = 142;
			break;
		case 80://P
			symbol = 144;
			break;
		case 88://X
			symbol = 149;
			break;
		case 65://A
			symbol = 128;
			break;
		case 72://H
			symbol = 141;
			break;
		case 75://K
			symbol = 138;
			break;
		case 67://C
			symbol = 145;
			break;
		case 66://B
			symbol = 130;
			break;
		case 77://M
			symbol = 140;
			break;
		case 101://e
			symbol = 165;
			break;
		case 111://o
			symbol = 174;
			break;
		case 112://p
			symbol = 224;
			break;
		case 120://x
			symbol = 229;
			break;
		case 121://y
			symbol = 227;
			break;
		case 97://a
			symbol = 160;
			break;
		case 99://c
			symbol = 225;
			break;
		default:
			break;
	}

	return symbol;
}
