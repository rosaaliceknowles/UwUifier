#include <iostream>
#include <string>
#include <fstream>
#include <fcntl.h>
#include <io.h>
#include <regex>
#include <cstdlib>
#include <time.h>
#include <cstdio>
#include <locale>
#include <codecvt>
#include <vector>

using namespace std;


const wstring EMOTICONS[] = {L"OwO", L"UwU", L"QwQ", L">~<", L"xD", L"^w^", L":3", L":3c", L"(◕‿◕✿)", L"X3", L"x3", L"=3", L">:3", L";3", L"O_o", 
L"(๑ˇεˇ๑)", L"ʕ •ᴥ•ʔ", L"XD", L"(⁄ ⁄•⁄ω⁄•⁄ ⁄)", L"^ω^", L"^_^", L">^_^<", L"(＾ｕ＾)", L"（ ﾟ Дﾟ)", L"(`･ω･´)", L"(´･ω･`)", L"（・Ａ・）", L"(´；ω；`)", L"(＃ﾟДﾟ)"};
int emoticon_len = 0;
const string FNAME = "input.txt";
const string ONAME = "output.txt";
const string TEMP = "temp.txt";
const std::locale utf16_locale = std::locale(std::locale(), new std::codecvt_utf8_utf16<wchar_t>()); // i dont fucking know what this is but i saw it on stack overflow and it seemed to fix the issues i had
const vector<wchar_t> OMITCHARS = {L'0', L'1', L'2', L'3', L'4', L'5', L'6', L'7', L'8', L'9', L'@', L'\n'};


wstring textswap(wstring og, wstring key, wstring repl){
    return regex_replace(og, wregex(key), repl);
}

wstring get_emoticon(){
    return EMOTICONS[(rand() % emoticon_len)];
}


// function used in boolean expressions to omit certain wide characters from being used 
bool omit(vector<wchar_t> j, wchar_t i){
    for (wchar_t c : j){
        if (c == i){
            return false;
        }
    }
    return true;
}

wstring uwuify_chars(wstring txt){
    wstring output = txt;
    transform(output.begin(), output.end(), output.begin(), ::tolower); // makes text lowercase
    wstring a = L"";

    output = textswap(output, L"brain ", L"wittwe peabwain ");
    output = textswap(output, L"r", L"w");
    output = textswap(output, L"l", L"w");
    output = textswap(output, L"th", L"f");
    output = textswap(output, L"ph", L"f");
    output = textswap(output, L"augh", L"aw");
    output = textswap(output, L"au", L"aw");
    output = textswap(output, L"hey ", L"hewwo ");
    output = textswap(output, L"hey, ", L"hewwo, ");
    output = textswap(output, L"baby", L"babey");
    


    return output;
}

wstring uwuify_emoticons(wstring txt){
    wstring output = L"";
    for (int i = 0; i < txt.length() - 1; ++i){
        if (txt[i] == '.' && (txt[i + 1] == ' ' || txt[i + 1] == '\n')){
            output += L". " + get_emoticon();
        } else if (txt[i] == '!' && (txt[i + 1] == ' ' || txt[i + 1] == '\n')){
            output += L"! " + get_emoticon();
        } else if (txt[i] == '?' && (txt[i + 1] == ' ' || txt[i + 1] == '\n')){
            output += L"? " + get_emoticon();
        } else if (i == txt.length() - 2){
            output += txt[i];
            output += txt[i + 1];
            output += L' ';
            output += get_emoticon();
        } else {
            output += txt[i];
        }
    }


    return output;
}

wstring uwuify_stutter(wstring txt){
    wstring output = L"";
    wchar_t pchar = L'j'; // stores the previous character 
    for (wchar_t c : txt){
        if (pchar == L' ' && (rand() % 101 >= 80) && omit(OMITCHARS, c)){
            output += c;
            output += L"-";
            output += c;
            output += L"-";
        } else {
            output += c;
        }
        pchar = c;
    }

    return output;
}

wstring uwuify_tilde(wstring txt){
    wstring output = L"";

    for (int i = 0; i < txt.length() - 1; ++i){
        if (txt[i] == L'.' && txt[i + 1] == L' ' && txt[i - 1] != L'.' && rand() % 11 > 8){
            for (int j = 0; j < rand() % 4; ++j){
                output += L'~';
            }
        } else {
            output += txt[i];
        }

    }

    return output;
}


int main(void){ 
    srand(time(0)); // create a seed for the random values generated in this program 
    for (wstring s : EMOTICONS){
        emoticon_len++;
    }
    _setmode(_fileno(stdout), _O_U16TEXT); // lets the terminal print utf-16 characters 

    wfstream fin;
    fin.imbue(utf16_locale);
    fin.open(FNAME,ios::in);

    wstring inputText = L"";
    wstring temp;
    while (getline(fin,temp)){
        inputText += temp + L"\n";
    }

    fin.close();

    wcout << inputText << L"\n" << endl;

    /* Prints this:
     |
     |
     |
     V

    */
    wcout << " |\n |\n |\n V\n" << endl;

    inputText = uwuify_chars(inputText);
    inputText = uwuify_stutter(inputText);
    inputText = uwuify_tilde(inputText);
    inputText = uwuify_emoticons(inputText);


    wcout << inputText << L"\n" << endl;

    // create the text output file if it doesn't already exist 
    fin.open("output.txt",ios::out);
    fin.close();

    // create the temporary file and insert the uwuified text into it 
    wofstream wfin ("temp.txt");
    wfin.imbue(utf16_locale);
    wfin << inputText << endl;
    wfin.close();


    // delete the old output file and replace it with the new file 
    if (remove("output.txt") != 0){
        cerr << "ERROR 1" << endl;
    }
    if (rename("temp.txt","output.txt") != 0){
        cerr << "ERROR 2" << endl;
    }

    wcout << L"TEXT UWUIFIED: CHECK OUTPUT.TXT\n" << endl;

    
    system("pause");
    return 0;
}