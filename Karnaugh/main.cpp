//
//  main.cpp
//  Karnaugh
//
//  Created by Arne Philipeit on 19/10/2016.
//  Copyright © 2016 Arne Philipeit. All rights reserved.
//

#include <iostream>
#include "FunctionsList.h"
#include "FunctionNode.h"
#include "Function.h"

using namespace std;

FunctionsList list;

void executeCommand(string line);

void readList();
void writeList();

int main() {
    string line;
    
    while (true) {
        getline(cin, line);
        
        if (line == "") {
            continue;
        }else{
            executeCommand(line);
        }
    }
    
    return 0;
}

void executeCommand(string line) {
    if (line.find("exit") == 0) {
        exit(0);
    }else if (line.find("define ") == 0) {
        size_t e = line.find(" ", 7);
        
        if (e == string::npos) {
            cout << "Invalid define command.\n";
            
            return;
        }
        
        string newFunctionName = line.substr(7, e - 7);
        Function * function = NULL;
        
        if (FunctionNode::isValidName(newFunctionName) == false) {
            cout << "Invalid function name.\n";
            
            return;
        }
        
        int nInputs = -1;
        if (line.find("with ", e + 1)  == e + 1 &&
            line.find(" inputs", e + 7) >= e + 7) {
            string s = line.substr(e + 6, line.find(" inputs", e + 7)  - e - 6);
            
            for (int i = 0; i < s.length(); i++) {
                if (!isdigit(s[i])) {
                    cout << "Invalid define command.\n";
                    
                    return;
                }
            }
            
            nInputs = stoi(s);
            
            if (nInputs < 1 ||
                nInputs > 26) {
                return;
            }
            
            e = line.find(" inputs", e + 7) + 7;
        }else{
            cout << "Invalid define command. Number of inputs not specified.\n";
            
            return;
        }
        
        function = new Function(nInputs);
        
        if (line.find("with expression") == e + 1) {
            string expression;
            
            cout << "Expression: ";
            getline(cin, expression);
            cout << endl;
            
            function->setResultsWithExpression(expression);
            
        }else if (line.find("with truth table") == e + 1) {
            function->readInTruthTable();
            
        }else if (line.find("with sum notation") == e + 1) {
            string sumNotation;
            
            cout << "Sum notation: ";
            getline(cin, sumNotation);
            cout << endl;
            
            function->setResultsWithSumNotation(sumNotation);
        }
        
        if (list.hasFunctionWithName(newFunctionName)) {
            list.setFunctionWithName(newFunctionName, function);
        }else{
            list.addFunction(newFunctionName, function);
        }
        
    }else if (line.find("print prime implicants for ") == 0) {
        string functionName = line.substr(27);
        Function * function = list.getFunctionWithName(functionName);
        
        if (function == NULL) {
            return;
        }
        
        int n;
        string * primeImplicants;
        
        function->getPrimeImplicants(n, primeImplicants);
        
        for (int i = 0; i < n; i++) {
            cout << primeImplicants[i] << endl;
        }
        cout << endl;
        
    }else if (line.find("print essential prime implicants for ") == 0) {
        string functionName = line.substr(37);
        Function * function = list.getFunctionWithName(functionName);
        
        if (function == NULL) {
            return;
        }
        
        int n;
        string * essentialPrimeImplicants;
        
        function->getEssentialPrimeImplicants(n, essentialPrimeImplicants);
        
        for (int i = 0; i < n; i++) {
            cout << essentialPrimeImplicants[i] << endl;
        }
        cout << endl;
        
    }else if (line.find("print nonessential prime implicants for ") == 0) {
        string functionName = line.substr(40);
        Function * function = list.getFunctionWithName(functionName);
        
        if (function == NULL) {
            return;
        }
        
        int n;
        string * nonessentialPrimeImplicants;
        
        function->getNonessentialPrimeImplicants(n, nonessentialPrimeImplicants);
        
        for (int i = 0; i < n; i++) {
            cout << nonessentialPrimeImplicants[i] << endl;
        }
        cout << endl;
        
    }else if (line.find("print sum of products for ") == 0) {
        string functionName = line.substr(26);
        Function * function = list.getFunctionWithName(functionName);
        
        if (function == NULL) {
            return;
        }
        
        cout << function->getExpression() << endl << endl;
        
    }else if (line.find("print sum notation for ") == 0) {
        string functionName = line.substr(23);
        Function * function = list.getFunctionWithName(functionName);
        
        if (function == NULL) {
            return;
        }
        
        cout << function->getSumNotation() << endl << endl;
        
    }else if (line.find("print truth table for ") == 0) {
        string functionName = line.substr(22);
        Function * function = list.getFunctionWithName(functionName);
        
        if (function == NULL) {
            return;
        }
        
        function->printTruthTable();
        
    }else if (line.find("print Karnaugh map for ") == 0) {
        string functionName = line.substr(23);
        Function * function = list.getFunctionWithName(functionName);
        
        if (function == NULL) {
            return;
        }
        
        function->printKarnaughMap();
        
    }else{
        cout << "The command \"";
        cout << line;
        cout << "\" could not be executed.\n";
    }
}

void readList() {
    FILE * listFile;
    char * line = NULL;
    size_t length = 0;
    ssize_t read;
    
    listFile = fopen("~/karnaugh.txt", "r");
    
    if (listFile == NULL) {
        return;
    }
    
    while ((read = getline(&line, &length, listFile)) != -1) {
        int n = -1;
        
        for (int i = 0; i < length; i++) {
            if (line[i] == ' ') {
                n = i;
                break;
            }
        }
        
        if (n == -1) continue;
        
        string * l = new string(line);
        
        string name = l->substr(0, n);
        string expression = l->substr(n + 1, length - n - 1);
        
        delete l;
        
        if (FunctionNode::isValidName(name) == false) {
            continue;
        }
        
        Function * function = new Function(expression);
        
        if (list.hasFunctionWithName(name)) {
            list.setFunctionWithName(name, function);
        }else{
            list.addFunction(name, function);
        }
    }
    
    if (line != NULL) {
        free(line);
    }
    
    fclose(listFile);
}

void writeList() {
    FILE * listFile;
    
    listFile = fopen("~/karnaugh.txt", "w");
    
    if (listFile == NULL) {
        return;
    }
    
    for (int i = 0; i < list.getNumberOfFunctions(); i++) {
        string name = list.getNameOfFunctionAtIndex(i);
        Function * function = list.getFunctionAtIndex(i);
        
        fprintf(listFile, "%s %s\n", name.c_str(), function->getExpression().c_str());
    }
    
    fclose(listFile);
}
