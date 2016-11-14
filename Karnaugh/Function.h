//
//  Function.h
//  Karnaugh
//
//  Created by Arne Philipeit on 19/10/2016.
//  Copyright © 2016 Arne Philipeit. All rights reserved.
//

#pragma once
#include <math.h>
#include <cstring>
#include <iostream>

#include "Base.h"

using namespace std;

class Function{
private:
    int nInputs;
    int nRows;
    bvalue_t *results;
    
public:
    //Constructors:
    Function(int nInputs);
    Function(string expression);
    Function(string expression, int nInputs);
    
    //Destructor:
    ~Function();
    
    //Basic access methods:
    int getNumberOfInputs() const;
    int getNumberOfRows() const;
    
    int getRowForInputValues(bvalue_t * inputValues) const;
    bvalue_t * getInputValuesForRow(int rowIndex) const;
    
    bvalue_t getResultForRow(int rowIndex) const;
    bvalue_t getResultForInputValues(bvalue_t * inputValues) const;
    
    void setResultForRow(int rowIndex, bvalue_t result);
    void setResultForInputValues(bvalue_t * inputValues, bvalue_t result);
    
    //Prime Implicants:
    void getPrimeImplicants(int &n, string * &primeImplicants) const;
    void getEssentialPrimeImplicants(int &n, string * &essentialPrimeImplicants) const;
    void getNonessentialPrimeImplicants(int & n, string * &nonessentialPrimeImplicnats) const;
    
    //Expressions:
    string getExpression() const;
    void setResultsWithExpression(string expresion);
    
    //Sum Notation:
    string getSumNotation() const;
    void setResultsWithSumNotation(string sumNotation);
    
    //Truth Tables:
    void printTruthTable() const;
    void readInTruthTable();
    
    //Karnaugh Maps:
    void printKarnaughMap() const;
    
    //Operators:
    void operator+=(const Function &f); //Or  Combination
    void operator*=(const Function &f); //And Combination
    
};

//Operators:
Function operator!(const Function &f); //Negation

Function operator+(const Function &f1, const Function &f2); //Or  Combination
Function operator*(const Function &f1, const Function &f2); //And Combination

bool operator<(const Function &f1, const Function &f2); //f1 subset of f2 (Minterms)
bool operator>(const Function &f1, const Function &f2); //f2 subset of f1 (Minterms)

bool operator==(const Function &f1, const Function &f2); //f1 equals       f2
bool operator!=(const Function &f1, const Function &f2); //f1 differs from f2
