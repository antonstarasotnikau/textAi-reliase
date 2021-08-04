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

unsigned char toLowerCase(char c);
unsigned char toCyrillic(char s);
bool isSeparator(char c);
void getCharsFromString(char textArray[], string text);
int canonizeText(string source, string text[]);
double compaireText(string text1[], int sizeText1, string text2[], int sizeText2);
double antiPlagiarism(string source1, string source2);
void stringToCharsArray(char textArray[], string text);
const int N = 102400;
string text1[N];
string text2[N];

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

    cout << st1 << endl;
    cout << st2;
    	cout << antiPlagiarism(st1, st2) << endl;
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
/* -------------------------------------------------------------------
/* Author : antonstarasotnikau, OlgaSkalaban
/* GitHub : https://github.com/antonstarasotnikau/textAi-reliase.git
/* Parametres: source1 - text etalon;
/* source2 - inputed fragment for checking.
/* Description: method is canonize both input string and find percent
/* for unique
/* v2.0.0
/* -------------------------------------------------------------------- */
double antiPlagiarism(string source1, string source2) {

	//Canonize both text
	int sizeText1 = canonizeText(source1, text1);
	int sizeText2 = canonizeText(source2, text2);
	//Find percent unique
	double match = compaireText(text1, sizeText1, text2, sizeText2);

	return match;
}

void stringToCharsArray(char textArray[], string text)
{
	int i = 0;
	for (i = 0; i < text.length(); i++)
		textArray[i] = text[i];
	textArray[i] = '\0';
}

double compaireText(string text1[], int sizeText1, string text2[], int sizeText2) {
	int temp_same = 0;
	int same = 0;
	int lenShingles = 3;
	double match = 0;
	string shingle[lenShingles];

	int countShingles1 = sizeText1-lenShingles+1;
	int countShingles2 = sizeText2-lenShingles+1;

	for (int i = 0; i < countShingles1; i++) {
		for (int j = 0; j < lenShingles; j++) {
			shingle[j] = text1[i+j];
			if (shingle[j] == text2[i+j])
				temp_same++;
		}
		if (temp_same == lenShingles)
			same++;
			temp_same = 0;
	}
	/*
	cout << endl << "Size text1 " << sizeText1 << " words" << endl;
	cout << endl << "Size text2 " << sizeText2 << " words" << endl;
	cout << endl << "Count of shingles TEXT 1 = " << countShingles1 << endl;
	cout << endl << "Count of shingles TEXT 2 = " << countShingles2 << endl;
	cout << endl << "Sames = " << same << endl;
	*/
	if (same == 0)
		match = 0.0;
	else
		match = 100 - (100 * same * 2 / double(countShingles1 + countShingles2));

	return match;
}

int canonizeText(string source, string text[]) {
	char word[N];
	char textArray[N];
	int iw = 0;
	int iText = 0;

	getCharsFromString(textArray, source);

	for (int i = 0; textArray[i] != '\0'; i++) {
		if (!isSeparator(textArray[i])) {
			textArray[i] = toCyrillic(textArray[i]);
			word[iw] = toLowerCase(textArray[i]);
			iw++;
			if (isSeparator(textArray[i+1]) || textArray[i+1] == '\0') {
				word[iw] = '\0'; //çàêðûâàåì ñëîâî
//				cout << word << endl;
				text[iText] = word;
				iText++;
				iw = 0;
			}
		}
	}
	return iText;
}

void getCharsFromString(char textArray[], string text) {
	int i = 0;
	for (i = 0; text[i] != '\0'; i++)
		textArray[i] = text[i];
	textArray[i] = '\0';
}

unsigned char toLowerCase(char symb) {
	unsigned char c = symb;
	if ((c >= 128) && (c <= 143))
		return c + 32;
	else if ((c >= 144) && (c <= 159))
		return c + 80;
	else
		return c;
}

bool isSeparator(char c) {
	char separator[] = {" , . ! : ; ? / () - ' ' "};
	for (int i = 0; separator[i] != '\0'; i++) {
		if (c == separator[i])
			return true;
	}
	return false;
}

unsigned char toCyrillic(char symb) {
	unsigned char s = symb;
	switch(s) {
		case 65:
			s = 128;
			break;
		case 97:
			s = 160;
			break;
		case 66:
			s = 130;
			break;
		case 67:
			s = 145;
			break;
		case 99:
			s = 225;
			break;
		case 69:
			s = 133;
			break;
		case 101:
			s = 165;
			break;
		case 72:
			s = 141;
			break;
		case 75:
			s = 138;
			break;
		case 77:
			s = 140;
			break;
		case 79:
			s = 142;
			break;
		case 111:
			s = 174;
			break;
		case 80:
			s = 144;
			break;
		case 112:
			s = 224;
			break;
		case 84:
			s = 146;
			break;
		case 88:
			s = 149;
			break;
		case 120:
			s = 229;
			break;
		case 121:
			s = 227;
			break;
	}
	return s;
}
