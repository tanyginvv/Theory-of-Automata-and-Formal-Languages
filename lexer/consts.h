#pragma once
#include <iostream>
#include <set>
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <vector>
#include <cctype>
#include <deque>

std::map<std::string, std::string> keywords = {
    std::make_pair("int", "Type"),
    std::make_pair("using", "Keyword"),
    std::make_pair("namespace", "Keyword"),
    std::make_pair("std", "Keyword"),
    std::make_pair("void", "Type"),
    std::make_pair("string", "Type"),
    std::make_pair("for", "Keyword"),
    std::make_pair("while", "Keyword"),
    std::make_pair("false", "Keyword"),
    std::make_pair("true", "Keyword"),
    std::make_pair("vector", "Type"),
    std::make_pair("continue", "Keyword"),
    std::make_pair("break", "Keyword"),
    std::make_pair("return", "Keyword"),
    std::make_pair("if", "Keyword"),
    std::make_pair("else", "Keyword"),
    std::make_pair("char", "Type"),
    std::make_pair("bool", "Type"),
    std::make_pair("auto", "Type"),
    std::make_pair("const", "Keyword"),
    std::make_pair("cout", "Keyword"),
    std::make_pair("cin", "Keyword"),
    std::make_pair("struct", "Keyword"),
    std::make_pair("endl", "Keyword")
};

std::map<std::string, std::string> separators = {
    std::make_pair("(", "Separator"),
    std::make_pair(")", "Separator"),
    std::make_pair("[", "Separator"),
    std::make_pair("]", "Separator"),
    std::make_pair(";", "Separator"),
    std::make_pair(",", "Separator"),
    std::make_pair("^", "Arithmetic operator"),
    std::make_pair("/", "Arithmetic operator"),
    std::make_pair("*", "Arithmetic operator"),
    std::make_pair("+", "Arithmetic operator"),
    std::make_pair("-", "Arithmetic operator"),
    std::make_pair("=", "Assignment operator"),
    std::make_pair(">", "Logical operation"),
    std::make_pair("<", "Logical operation"),
    std::make_pair(":", "Separator"),
    std::make_pair("{", "Separator"),
    std::make_pair("}", "Separator")
};

std::map<std::string, std::string> separatorsDouble = {
    std::make_pair("<<", "Operator output"),
    std::make_pair(">>", "Operator input"),
    std::make_pair("==", "Logical operation"),
    std::make_pair("<=", "Logical operation"),
    std::make_pair(">=", "Logical operation"),
    std::make_pair("=<", "Logical operation"),
    std::make_pair("=>", "Logical operation"),
    std::make_pair("&&", "Logical operation"),
    std::make_pair("||", "Logical operation"),
    std::make_pair("!=", "Logical operation"),
};

std::string numbers = "1234567890.";

const std::string errorOutputStr = " --- Error --- ";