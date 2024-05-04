#include <iostream>
#include <windows.h>
#include <string>
#include <fstream>
#include <sstream>
#pragma comment(lib, "urlmon.lib")
using namespace std;

string formatDate(const string& date);
void getRate(string filename, string date, string valCode);
string readFileData(const string& filename);
void parseShowRate(const string& fileContent, const string& date, const string& valCode);


string formatDate(const string& date) {
    return date.substr(6, 2) + "-" + date.substr(4, 2) + "-" + date.substr(0, 4);
}

void getRate(string filename, string date, string valCode) {
    string srcURL = "https://bank.gov.ua/NBUStatService/v1/statdirectory/exchange?valcode=" + valCode + "&date=" + date + "&json";

    if (S_OK == URLDownloadToFileA(NULL, srcURL.c_str(), filename.c_str(), 0, NULL)) {
        cout << "\n\nSaved to file " << filename << endl << endl;
    }
    else {
        cout << "Error!\n]n"; 
    }
}
string readFileData(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening file." << endl;
        return 0;
    }
    string content;
    string line;
    while (getline(file, line)) {
        content += line + '\n';
    }
    file.close();
    return content;
}

void parseShowRate(const string& fileContent, const string& date, const string& valCode) {
    string rateStr = "\"rate\":";
    size_t pos = fileContent.find(rateStr);
    if (pos != -1) {
        pos += rateStr.length();
        size_t endPos = fileContent.find(',', pos);
        if (endPos != -1) {
            string rate = fileContent.substr(pos, endPos - pos);
            cout << "Date: " << formatDate(date) << ", Currency: " << valCode << ", 1 " << valCode << " = " << rate << " Hrvynia." << endl;
        }
        else {
            cout << "Parsing error." << endl;
        }
    }
    else {
        cout << "Could not find the exchange rate in the file." << endl;
    }
}

int main() {
    string filename, date, valCode;
    cout << "Input date (yyyymmdd): ";
    cin >> date;

    cout << "Input currency name: ";
    cin >> valCode;

    cout << "Enter a filename to save: ";
    cin >> filename;

    getRate(filename, date, valCode);
    string fileContent = readFileData(filename);
    parseShowRate(fileContent, date, valCode);
}