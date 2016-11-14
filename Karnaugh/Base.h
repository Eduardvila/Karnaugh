//
//  Base.h
//  Karnaugh
//
//  Created by Arne Philipeit on 24/10/2016.
//  Copyright © 2016 Arne Philipeit. All rights reserved.
//

#pragma once
#include <iostream>

using namespace std;

typedef char bvalue_t;

void replaceAll(string &s, const string &search, const string &replace);

int f(int n);
int * groupsKOutOfN(int k, int n, int &nGroups);

char charForBValue(bvalue_t bvalue);
bvalue_t bvalueForChar(char c);

int numberOfInputsInExpression(string expression);
bvalue_t valueForExpression(string expression, int nInputs, bvalue_t inputValues[]);
