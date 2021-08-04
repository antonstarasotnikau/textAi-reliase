#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
/*#include <cgicc/CgiDefs.h>
#include <cgicc/Cgicc.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTMLClasses.h>*/
#include <fstream>

using namespace std;
//using namespace cgicc;

unsigned char toLowerCase(char c);
unsigned char toCyrillic(char s);
bool isSeparator(char c);
void getCharsFromString(char textArray[], string text);
int canonizeText(string source, string text[]);
double compaireText(string text1[], int sizeText1, string text2[], int sizeText2);
double antiPlagiarism(string source1, string source2);
void stringToCharsArray(char textArray[], string text);
void stringToCharsArray(char textArray[], string text);
const int N = 10240;
double match = 0.0;
string text1[N];
string text2[N];

int getInt(string name);//get length of text putted in the form on your site(this function we use only for example)
string getDB();//get origin text from db.txt (don't modify tis function)

int main()
{
	/*setlocale(LC_ALL, "Russian");
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
    cout << "</html>\n";*/
	string text1 = "Generally, plagiarism is not in itself a crime, but like counterfeiting fraud can be punished in a court[11][12] for prejudices caused by copyright infringement,[13][14] violation of moral rights,[15] or torts. In academia and industry, it is a serious ethical offense.[16][17] Plagiarism and copyright infringement overlap to a considerable extent, but they are not equivalent concepts,[18] and many types of plagiarism do not constitute copyright infringement, which is defined by copyright law and may be adjudicated by courts.";
	string fragment1 = "Generally, plagiarism is not in itself a crime, but like counterfeiting ";
cout << antiPlagiarism(text1, fragment1);
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
	match = compaireText(text1, sizeText1, text2, sizeText2);

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
				word[iw] = '\0'; //зак