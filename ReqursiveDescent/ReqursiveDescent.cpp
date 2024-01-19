#include <iostream>
#include <cctype>

void error(const std::string& message) {
    std::cerr << "Error: " << message << std::endl;
    exit(1);
}

char getNextToken() {
    return std::cin.peek();
}

void match(char expectedToken) {
    if (getNextToken() == expectedToken) {
        std::cin.get();
    }
    else {
        error("Unexpected token");
    }
}

void E();
void X();
void Y();
void T();
void F();



void E() {
    T();
    X();
}

void X() {
    if (getNextToken() == '+') {
        match('+');
        T();
        X();
    }
}

void T() {
    F();
    Y();
}

void Y() {
    if (getNextToken() == '*') {
        match('*');
        F();
        Y();
    }
}

void F() {
    char nextToken = getNextToken();
    if (nextToken == '(') {
        match('(');
        E();
        match(')');
    }
    else if (nextToken == '-') {
        match('-');
        F();
    }
    else if (nextToken == 'a' || nextToken == 'b' || nextToken == '3' || nextToken == '5') {
        std::cin.get();
    }
    else {
        error("Unexpected token in F");
    }
}

int main() {
    //-a*(-a+-b+-5)+b*(-a+3*-b)
    std::cout << "Enter an expression: ";
    E();
    if (getNextToken() == '\n') {
        std::cout << "Input accepted." << std::endl;
    }
    else {
        error("Unexpected token after expression");
    }

    return 0;
}