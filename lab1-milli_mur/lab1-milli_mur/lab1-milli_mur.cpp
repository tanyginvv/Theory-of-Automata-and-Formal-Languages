#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <set>
#include <algorithm> 

const int MEALY_2_MOORE = 1;
const int MOORE_2_MEALY = 2;
const std::string EMPTY_ELEMENT = "  ";


void ParseLineMoore(std::vector<std::vector<std::string>>& table, std::string line)
{
	size_t lineLength = line.size();
	size_t i = 0;
	std::vector<std::string> parsedLine;

	while (true)
	{
		if (line[i] == ' ')
		{
			if (i == 0)
			{
				parsedLine.emplace_back(EMPTY_ELEMENT);
			}
			i++;
			continue;
		}
		std::string el;
		while (line[i] != ' ' && i < lineLength)
		{
			el += line[i];
			i++;
		}
		parsedLine.emplace_back(el);
		if (i == lineLength)
		{
			break;
		}
	}
	table.emplace_back(parsedLine);
}

void ParseLineMealy(std::vector<std::vector<std::string>>& table, std::string line)
{
	size_t lineLength = line.size();
	size_t i = 0;
	std::vector<std::string> parsedLine;

	while (true)
	{
		std::string el;
		while (line[i] != ' ' && i < lineLength)
		{
			el += line[i];
			i++;
		}
		i++;
		parsedLine.emplace_back(el);
		if (i >= lineLength)
		{
			break;
		}
	}
	table.emplace_back(parsedLine);
}


void TransformMealyToMoore(std::ifstream& inputFile, std::vector<std::vector<std::string>>& tableIn, std::vector<std::vector<std::string>>& tableOut)
{
	std::string line;
	int lineCount = 1;
	int xCount = 0;
	while (getline(inputFile, line))
	{
		ParseLineMealy(tableIn, line);
		xCount++;
	}
	std::vector<std::string> conditionsWithSignals;
	for (size_t i = 0; i < tableIn.size(); i++)
	{
		for (size_t j = 0; j < tableIn[i].size(); j++)
		{
			conditionsWithSignals.push_back(tableIn[i][j]);
		}
	}

	std::set<std::string> s(conditionsWithSignals.begin(), conditionsWithSignals.end());
	conditionsWithSignals.assign(s.begin(), s.end());
	std::sort(conditionsWithSignals.begin(), conditionsWithSignals.end());

	tableOut.resize(xCount + 1);
	for (size_t i = 0; i < tableOut.size(); i++)
	{
		tableOut[i].resize(conditionsWithSignals.size());
	}
	std::vector < std::string> conditons, signals, startConditions;
	int elCount = 0;
	for (auto el : conditionsWithSignals)
	{
		std::string condition, signal, startCondition;
		size_t i = 0;
		size_t elLength = el.size();
		while (true)
		{
			while (el[i] != '/')
			{
				startCondition += el[i];
				i++;
			}
			condition = 'q' + std::to_string(elCount);
			elCount++;
			while ((i < elLength) && (el[i] != ' '))
			{
				i++;
				signal += el[i];
			}
			break;
		}
		conditons.push_back(condition);
		signals.push_back(signal);
		startConditions.push_back(startCondition);
	}
	for (size_t i = 0; i < signals.size(); i++)
	{
		tableOut[0][i] = signals[i];
	}
	std::map<std::string, std::string> matches;
	std::set<std::string>::iterator it = s.begin();
	for (size_t i = 0; i < conditionsWithSignals.size(); i++)
	{
		matches[*it] = 'q' + std::to_string(i);
		it++;
	}
	for (size_t i = 1; i < tableOut.size(); i++) {
		for (size_t j = 0; j < tableOut[i].size(); j++) {
			for (size_t m = 0; m < tableIn.size(); m++) {
				int indexInTable;
				indexInTable = std::stoi(startConditions[j].substr(1));
				std::string a;
				a = tableIn[m][indexInTable];
				for (auto item : matches) {
					if (a == item.first) {
						tableOut[m + 1][j] = item.second;
					}
				}
			}
		}
	}
}

void TransofrmMooreToMealy(std::ifstream& inputFile, std::vector<std::vector<std::string>>& tableIn, std::vector<std::vector<std::string>>& tableOut)
{
	std::string line;
	while (getline(inputFile, line))
	{
		ParseLineMoore(tableIn, line);
	}

	std::map<std::string, std::string> conditionToOutputSignal;
	for (size_t i = 0; i < tableIn[0].size(); i++)
	{
		std::string state;
		state += 's'+ std::to_string(i);
		conditionToOutputSignal.insert(std::pair<std::string,std::string>(state, tableIn[0][i]));
	}
	tableOut.resize(tableIn.size());
	for (size_t i = 0; i < tableOut.size(); i++)
	{
		tableOut[i].resize(conditionToOutputSignal.size());
	}
	for (size_t j = 1; j < tableOut.size(); j++) {
		for (size_t k = 0; k < tableOut[j].size(); k++){
			for (auto item : conditionToOutputSignal) {
				if ( tableIn[j][k] == item.first) {
					tableOut[j - 1][k] = tableIn[j][k] + "/" + item.second;
				}
			}
		}
	}
	
}



int main()
{
	std::ifstream inputFile("inputMoore2.txt");
	std::ofstream outputFile("output.txt");
	std::vector<std::vector<std::string>> tableIn, tableOut;

	std::string firstLine;
	std::getline(inputFile, firstLine);
	std::istringstream convert(firstLine);
	int columns, rows, typeAuto;

	convert >> columns >> rows >> typeAuto;

	if (typeAuto == MEALY_2_MOORE)
	{
		TransformMealyToMoore(inputFile, tableIn, tableOut);
		std::ofstream dotFileIn("mealyIn.dot");

		dotFileIn << "digraph G {\n";

		for (size_t i = 0; i < tableIn.size(); i++) {
			std::string fromNode = tableIn[i][0];

			for (size_t j = 0; j < tableIn[i].size(); j++) {
				std::string toNode = tableIn[i][j];
				std::string delimiter = "/";
				std::string state = tableIn[0][0].substr(0, 1) + std::to_string(j);
				int pos = toNode.find(delimiter);
				std::string pointer = toNode.substr(pos + 1);
				std::string end = toNode.substr(0, pos);


				dotFileIn << "\t" << state << " -> " << end << " [label=\"" << "x" + std::to_string(i + 1) + "/" + pointer << "\"];\n";
			}
		}

		dotFileIn << "}";

		dotFileIn.close();

		std::string command1 = "dot -Tpng mealyIn.dot -o mealyIn.png";
		system(command1.c_str());
		
		std::ofstream dotFileOut("mealyOut.dot");

		dotFileOut << "digraph G {\n";

		for (size_t i = 1; i < tableOut.size(); i++) {
			std::string fromNode = "x"+ std::to_string(i);

			for (size_t j = 0; j < tableOut[i].size(); j++) {
				std::stringstream ss(tableOut[i][j]);
				std::string toNode = "q" + std::to_string(j);
				dotFileOut << "\t" << toNode << " -> " << tableOut[i][j] << " [label=\"" << fromNode << "\"];\n";
			}
		}

		dotFileOut << "}";

		dotFileOut.close();

		std::string command2 = "dot -Tpng mealyOut.dot -o mealyOut.png";
		system(command2.c_str());
	}
	else if (typeAuto == MOORE_2_MEALY)
	{
		TransofrmMooreToMealy(inputFile, tableIn, tableOut);
		std::ofstream dotFileIn("mooreIn.dot");

		dotFileIn << "digraph G {\n";

		for (size_t i = 1; i < tableIn.size(); i++) {
			std::string fromNode = "x" + std::to_string(i);

			for (size_t j = 0; j < tableIn[i].size(); j++) {
				std::stringstream ss(tableIn[i][j]);
				std::string toNode = tableIn[1][1].substr(0,1) + std::to_string(j);
				dotFileIn << "\t" << toNode << " -> " << tableIn[i][j] << " [label=\"" << fromNode << "\"];\n";
			}
		}

		dotFileIn << "}";

		dotFileIn.close();

		std::string command1 = "dot -Tpng mooreIn.dot -o mooreIn.png";
		system(command1.c_str());

		std::ofstream dotFileOut("mooreOut.dot");

		dotFileOut << "digraph G {\n";

		for (size_t i = 0; i < tableOut.size()- 1; i++) {
			std::string fromNode = tableOut[i][0];

			for (size_t j = 0; j < tableOut[i].size(); j++) {
				std::string toNode = tableOut[i][j];
				std::string delimiter = "/";
				std::string state = tableOut[0][0].substr(0,1) + std::to_string(j);
				int pos = toNode.find(delimiter);
				std::string pointer = toNode.substr(pos + 1);
				std::string end = toNode.substr(0, pos);


				dotFileOut << "\t" << state << " -> " << end << " [label=\"" << "x"+std::to_string(i+1) + "/" + pointer << "\"];\n";
			}
		}

		dotFileOut << "}";

		dotFileOut.close();

		std::string command2 = "dot -Tpng mooreOut.dot -o mooreOut.png";
		system(command2.c_str());
	}
	else {
		std::cerr << "ERROR COMMAND" << std::endl;
	}
	
	for (size_t i = 0; i < tableOut.size(); i++)
	{
		for (size_t j = 0; j < tableOut[i].size(); j++)
		{
			outputFile << tableOut[i][j] << ' ';
		}
		outputFile << '\n';
	}

}