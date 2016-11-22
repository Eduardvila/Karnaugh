//
//  Base.cpp
//  Karnaugh
//
//  Created by Arne Philipeit on 24/10/2016.
//  Copyright © 2016 Arne Philipeit. All rights reserved.
//

#include "Base.h"

void replaceAll(string &s, const string &search, const string &replace) {
    for(size_t pos = 0; ; pos += replace.length()) {
        pos = s.find(search, pos);
        
        if (pos == string::npos) break;
        
        s.erase(pos, search.length());
        s.insert(pos, replace);
    }
}

int f(int n) {
    int f = 1;
    
    for (int i = 2; i <= n; i++)
        f *= i;
    
    return f;
}

int * groupsKOutOfN(int k, int n, int &nGroups) {
    nGroups = f(n) / (f(k) * f(n - k));
    
    if (nGroups < 1) {
        return NULL;
    }
    
    int * orders = new int[nGroups * k];
    
    //Set first group:
    for (int i = 0; i < k; i++) {
        orders[i] = i;
    }
    
    //Set all other groups as increments:
    for (int i = 1; i < nGroups; i++) {
        bool alreadyIncreased = false;
        
        for (int j = k - 1; j >= 0; j--) {
            int p = orders[(i - 1) * k + j];
            
            if (alreadyIncreased == true) {
                orders[i * k + j] = p;
                
            }else if (p < n - (k - j)) {
                p++;
                orders[i * k + j] = p;
                
                for (int l = j + 1; l < k; l++) {
                    p++;
                    orders[i * k + l] = p;
                }
                
                alreadyIncreased = true;
            }
        }
    }
    
    return orders;
}

char charForBValue(bvalue_t bvalue) {
    if (bvalue == 0) {
        return '0';
    }else if (bvalue == 1) {
        return '1';
    }else{
        return 'x';
    }
}

bvalue_t bvalueForChar(char c) {
    if (c == '0') {
        return 0;
    }else if (c == '1') {
        return 1;
    }else{
        return -1;
    }
}

int numberOfInputsInExpression(string expression) {
    int n = 0;
    
    for (int i = 0; i < expression.length(); i++) {
        char c = expression[i];
        
        if (c >= 97 && c < 117) {
            if (n < c - 96) {
                n = c - 96;
            }
        }
    }
    
    return n;
}

bvalue_t valueForExpression(string expression, int nInputs, bvalue_t inputValues[]) {
    replaceAll(expression, " ", "");
    
    if (expression.length() == 0) {
        return 0;
    }
    
    for (int i = 0; i < nInputs; i++) {
        replace(expression.begin(), expression.end(), (char)(97 + i), charForBValue(inputValues[i]));
    }
    
    while (true) {
        size_t oldLength = expression.length();
        
        replaceAll(expression, "0'", "1");
        replaceAll(expression, "1'", "0");
        replaceAll(expression, "x'", "x");
        
        if (oldLength != expression.length()) {
            continue;
        }
        
        replaceAll(expression, "(0)", "0");
        replaceAll(expression, "(1)", "1");
        replaceAll(expression, "(x)", "x");
        
        if (oldLength != expression.length()) {
            continue;
        }
        
        replaceAll(expression, "0.0", "0");
        replaceAll(expression, "0.1", "0");
        replaceAll(expression, "1.0", "0");
        replaceAll(expression, "1.1", "1");
        
        replaceAll(expression, "0.x", "0");
        replaceAll(expression, "x.0", "0");
        replaceAll(expression, "1.x", "x");
        replaceAll(expression, "x.1", "x");
        replaceAll(expression, "x.x", "x");
        
        if (oldLength != expression.length()) {
            continue;
        }
        
        replaceAll(expression, "0+0", "0");
        replaceAll(expression, "0+1", "1");
        replaceAll(expression, "1+0", "1");
        replaceAll(expression, "1+1", "1");
        
        replaceAll(expression, "0+x", "x");
        replaceAll(expression, "x+0", "x");
        replaceAll(expression, "1+x", "1");
        replaceAll(expression, "x+1", "1");
        replaceAll(expression, "x+x", "x");
        
        if (oldLength == expression.length()) {
            break;
        }
    }
    
    if (expression.length() > 1) {
        return -1;
    }
    
    return bvalueForChar(expression[0]);
}
