#include "consts.h"

using namespace std;

bool MakeToken(string& word, int indexStr, size_t position, deque<string>& vectorOfStringAndChar, ofstream& outputFile)
{
    if (word == "")
    {
        return false;
    }

    for (auto& item : separatorsDouble)
    {
        size_t index = word.find(item.first);
        if (index != string::npos)
        {
            word.erase(index, item.first.size());
            string secondStr = word.substr(index, word.size() - index);
            word = word.substr(0, index);
            MakeToken(word, indexStr, position, vectorOfStringAndChar, outputFile);
            outputFile << item.first << " --- " << item.second << " --- line " << indexStr  << " --- position " << position + word.size() << "\n";
            MakeToken(secondStr, indexStr, position + index + word.size(), vectorOfStringAndChar, outputFile);
            return true;
        }
    }

    for (auto& item : separators)
    {
        size_t index = word.find(item.first);
        if (index != string::npos)
        {
            word.erase(index, item.first.size());
            string secondStr = word.substr(index, word.size() - index);
            word = word.substr(0, index);
            MakeToken(word, indexStr, position, vectorOfStringAndChar, outputFile);
            outputFile << item.first << " --- " << item.second << " --- line " << indexStr  << " --- position " << position + word.size() << "\n";
            MakeToken(secondStr, indexStr, position + index + word.size(), vectorOfStringAndChar, outputFile);
            return true;
        }
    }

    //numbers
    if (numbers.find(word[0]) != string::npos)
    {
        bool isPoint = false;
        bool isSecondPoint = false;
        for (auto i = 1; i < word.size(); i++)
        {
            if (isPoint && word[i] == '.')
            {
                isSecondPoint = true;
            }
            if (word[i] == '.')
            {
                isPoint = true;
            }
            if ((numbers.find(word[i]) == string::npos) || word[0] == '.'  || isSecondPoint)
            {
                outputFile << word << errorOutputStr << "line " << indexStr << " --- position " << position << "\n";
                return true;
            }
        }

        if (isPoint)
        {
            outputFile << word << " --- Float --- line " << indexStr << " --- position " << position << "\n";
        }
        else
        {
            outputFile << word << " --- Number --- line " << indexStr << " --- position " << position << "\n";
        }
        return true;
    }

    string output;
    if (keywords.find(word) != keywords.end())
    {
        output = keywords[word];
        outputFile << word << " --- " << output << " --- line " << indexStr << " --- position " << position << "\n";
        return true;
    }

    if (separators.find(word) != separators.end())
    {
        output = separators[word];
        outputFile << word << " --- " << output << " --- line " << indexStr << " --- position " << position << "\n";
        return true;
    }

    if (word == "\"" && !vectorOfStringAndChar.empty())
    {
        word = vectorOfStringAndChar[0];
        vectorOfStringAndChar.pop_front();
        size_t i = word.find("\"") + 1;
        if (i < word.size() && word.find("\"", i) != string::npos)
        {
            outputFile << word << " --- String --- line " << indexStr << " --- position " << position << "\n";
        }
        else
        {
            outputFile << word << errorOutputStr << indexStr << " --- position " << position << "\n";
        }
        return true;
    }

    if (word == "\'" && !vectorOfStringAndChar.empty())
    {
        word = vectorOfStringAndChar[0];
        vectorOfStringAndChar.pop_front();
        size_t i = word.find("\'") + 1;
        if (i < word.size() && word.find("\'", i) != string::npos && word.size() == 3)
        {
            outputFile << word << " --- Char --- line " << indexStr << " --- position " << position << "\n";
        }
        else
        {
            outputFile << word << errorOutputStr << indexStr << " --- position " << position << "\n";
        }
        return true;
    }

    if (isalpha(word[0]) || word[0] == '_') {
        for (auto i = 1; i < word.size(); i++) {
            if (!isalnum(word[i]) && word[i] != '_') {
                outputFile << word << errorOutputStr << indexStr << " --- position " << position << "\n";
                return true;
            }
        }

        if (word.size() > 20) {
            string internalName = "$" + to_string(indexStr) + "$";
            outputFile << word << " --- Identifier --- line " << indexStr << " --- position " << position << " (Replaced by " << internalName << ")\n";
        }
        else {
            outputFile << word << " --- Identifier --- line " << indexStr << " --- position " << position << "\n";
        }
        return true;
    }


    outputFile << word << errorOutputStr << indexStr << " --- position " << position << "\n";
}

void processStringAndChar(deque<string>& vectorOfStringAndChar, string& inputStr, size_t indexStart)
{
    if (inputStr.find("\"", indexStart) != string::npos)
    {
        string str;
        size_t start = inputStr.find("\"", indexStart);
        size_t end = inputStr.size() - 1;
        if (start + 1 < inputStr.size() && inputStr.find("\"", start + 1) != string::npos)
        {
            end = inputStr.find("\"", start + 1);
        }
        str = inputStr.substr(start, end - start + 1);
        vectorOfStringAndChar.push_back(str);
        inputStr.erase(start + 1, end - start);

        processStringAndChar(vectorOfStringAndChar, inputStr, start + 1);
    }
    if (inputStr.find("\'", indexStart) != string::npos)
    {
        string str;
        size_t start = inputStr.find("\'", indexStart);
        size_t end = inputStr.size() - 1;
        if (start + 1 < inputStr.size() and inputStr.find("\'", start + 1) != string::npos)
        {
            end = inputStr.find("\'", start + 1);
        }
        str = inputStr.substr(start, end - start + 1);
        vectorOfStringAndChar.push_back(str);
        inputStr.erase(start + 1, end - start);

        processStringAndChar(vectorOfStringAndChar, inputStr, start + 1);
    }
}

void ProcessStr(ifstream& inputFile, string& inputStr, int& indexStr, ofstream& outputFile)
{
    int commentsIndex = 0;
    if ((commentsIndex = inputStr.find("//", commentsIndex)) != string::npos)
    {
        string comment = inputStr.substr(commentsIndex, inputStr.size() - commentsIndex);
        inputStr = inputStr.substr(0, commentsIndex);
        outputFile << comment << " --- Comment --- line " << indexStr << " position "<< commentsIndex + 1 << "\n";
    }

    size_t startMultilineComment = inputStr.find("/*");
    size_t endMultilineComment = inputStr.find("*/");

    if (startMultilineComment != string::npos)
    {
        bool closed = false;

        size_t startPos = startMultilineComment + 1;  

        if (endMultilineComment == string::npos || endMultilineComment <= startMultilineComment)
        {
            string comment = inputStr.substr(startMultilineComment);
            inputStr.erase(startMultilineComment);
            outputFile << comment << " --- Comment --- line " << indexStr << " --- position " << startPos << "\n";
            while (getline(inputFile, inputStr))
            {
                indexStr++;

                endMultilineComment = inputStr.find("*/");
                if (endMultilineComment != string::npos)
                {
                    string comment = inputStr.substr(0, endMultilineComment + 2);
                    inputStr.erase(0, endMultilineComment + 2);
                    outputFile << comment << " --- Comment --- line " << indexStr << " --- position " << 1 << "\n"; 
                    closed = true;
                    break;
                }
            }

            if (!closed)
            {
                outputFile << "ERROR - Unclosed multiline comment " << "\n";
                return;
            }
        }
        else
        {
            string comment = inputStr.substr(startMultilineComment, endMultilineComment - startMultilineComment + 2);
            inputStr.erase(startMultilineComment, endMultilineComment - startMultilineComment + 2);
            outputFile << comment << " --- Comment --- line " << indexStr << " --- position " << startPos << "\n";
        }
    }
    
    size_t singleQuotePos = inputStr.find("'");
    if (singleQuotePos != string::npos && inputStr.find("'", singleQuotePos + 1) == string::npos)
    {
        outputFile << inputStr << " --- ERROR - Unclosed Single Quote --- line " << indexStr << " --- position " << singleQuotePos + 1 << "\n";
        inputStr.clear();
        return;
    }

    size_t doubleQuotePos = inputStr.find("\"");
    if (doubleQuotePos != string::npos && inputStr.find("\"", doubleQuotePos + 1) == string::npos)
    {
        outputFile << inputStr << " --- ERROR - Unclosed Double Quote --- line " << indexStr << " --- position " << doubleQuotePos + 1 << "\n";
        inputStr.clear();
        return;
    }

    deque<string> vectorOfStringAndChar;
    processStringAndChar(vectorOfStringAndChar, inputStr, 0);

    size_t position = 1;
    if (!inputStr.empty())
    {
        vector<string> words;
        string item;
        istringstream strStream(inputStr);
        while (getline(strStream, item, ' '))
        {
            MakeToken(item, indexStr, position, vectorOfStringAndChar, outputFile);
            position += item.size() + 1;
        }
    }
}

void Process(ifstream& input, ofstream& output) {
    string inputStr;
    int indexStr = 1;

    while (true) {
        getline(input, inputStr);
        if (inputStr.find("EOF") != string::npos) {
            output << "EOF --- End of File\n";
            break;
        }

        if (input.eof()) {
            output << "EOF --- End of File\n";
            break;
        }

        if (inputStr.empty()) {
            continue;
        }

        ProcessStr(input, inputStr, indexStr, output);
        indexStr++;
    }
}
int main()
{
    ifstream inputFile("input.txt");
    ofstream outputFile("output.txt");
    Process(inputFile, outputFile);
}