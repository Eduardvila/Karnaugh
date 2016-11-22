//
//  Function.cpp
//  Karnaugh
//
//  Created by Arne Philipeit on 19/10/2016.
//  Copyright © 2016 Arne Philipeit. All rights reserved.
//

#include "Function.h"

#pragma mark Constructors
Function::Function(int n) {
    nInputs = n;
    nRows = pow(2, nInputs);
    
    results = new bvalue_t[nRows];
    
    for (int i = 0; i < nRows; i++) {
        results[i] = 0;
    }
}

Function::Function(string expression) {
    nInputs = numberOfInputsInExpression(expression);
    nRows = pow(2, nInputs);
    
    results = new bvalue_t[nRows];
    
    for (int i = 0; i < nRows; i++) {
        bvalue_t * inputValues = getInputValuesForRow(i);
        
        results[i] = valueForExpression(expression, nInputs, inputValues);
        
        delete[] inputValues;
    }
}

Function::Function(string expression, int n) {
    nInputs = n;
    nRows = pow(2, nInputs);
    
    results = new bvalue_t[nRows];
    
    for (int i = 0; i < nRows; i++) {
        bvalue_t * inputValues = getInputValuesForRow(i);
        
        results[i] = valueForExpression(expression, nInputs, inputValues);
        
        delete[] inputValues;
    }
}

#pragma mark -
#pragma mark Destructor
Function::~Function() {
    delete[] results;
}

#pragma mark -
#pragma mark Basic access methods
int Function::getNumberOfInputs() const {
    return nInputs;
}

int Function::getNumberOfRows() const {
    return nRows;
}

int Function::getRowForInputValues(bvalue_t * inputValues) const {
    int a = 0;
    
    for (int i = 0; i < nInputs; i++) {
        bvalue_t input = inputValues[i];
        
        if (input != 0 && input != 1) {
            return -1;
        }else if (input != 1) {
            continue;
        }
        
        a += pow(2, nInputs - 1 - i);
    }
    
    return a;
}

bvalue_t * Function::getInputValuesForRow(int rowIndex) const {
    bvalue_t * inputValues = new bvalue_t[nInputs];
    
    for (int i = 0; i < nInputs; i++) {
        int a = pow(2, nInputs - i - 1);
        
        inputValues[i] =(rowIndex / a) % 2;
    }
    
    return inputValues;
}

bvalue_t Function::getResultForRow(int rowIndex) const {
    if (rowIndex < 0 || rowIndex >= nRows) {
        return -1;
    }else{
        return results[rowIndex];
    }
}

bvalue_t Function::getResultForInputValues(bvalue_t * inputValues) const {
    int rowIndex = getRowForInputValues(inputValues);
    
    if (rowIndex == -1) {
        return -1;
    }else{
        return results[rowIndex];
    }
}

void Function::setResultForRow(int rowIndex, bvalue_t result) {
    if (rowIndex < 0 || rowIndex >= nRows) {
        return;
    }else if (result != 0 && result != 1 && result != -1) {
        return;
    }
    
    results[rowIndex] = result;
}

void Function::setResultForInputValues(bvalue_t * inputValues, bvalue_t result) {
    int rowIndex = getRowForInputValues(inputValues);
    
    if (rowIndex == -1) {
        return;
    }else if (result != 0 && result != 1 && result != -1) {
        return;
    }
    
    results[rowIndex] = result;
}


#pragma mark -
#pragma mark Prime Implicants
void Function::getPrimeImplicants(int &n, string * &primeImplicants) const {
    bool all0s = true;
    bool all1s = true;
    
    for (int i = 0; i < nRows; i++) {
        if (results[i] == 1) all0s = false;
        if (results[i] != 1) all1s = false;
    }
    
    if (all0s) {
        n = 0;
        
        return;
    }else if (all1s) {
        n = 1;
        
        primeImplicants = new string[1];
        primeImplicants[0] = "1";
        
        return;
    }
    
    n = 0;
    primeImplicants = new string[(int) pow(nRows, 2)];
    
    Function ** combinedPrimeImplicantsFunctions =  new Function * [nInputs - 1];
    
    for (int i = 0; i < nInputs; i++) {
        Function * combinedPrimeImplicantsFunction = new Function(nInputs);
        
        int nGroups;
        int *groups = groupsKOutOfN(i + 1, nInputs, nGroups);
        
        for (int j = 0; j < nGroups; j++) {
            for (int k = 0; k < pow(2, i + 1); k++) {
                string primeImplicantDescription = "";
                
                for (int l = 0; l < i + 1; l++) {
                    if (l > 0)
                        primeImplicantDescription += ".";
                    primeImplicantDescription += (char)(97 + groups[j * (i + 1) + l]);
                    
                    if ((k / (int)pow(2, l)) % 2 == 1) {
                        primeImplicantDescription += "'";
                    }
                }
                
                Function primeImplicantFunction(primeImplicantDescription, nInputs);
                bool addPrimeImplicantFunction = true;
                
                if ((primeImplicantFunction < *this) == false) {
                    addPrimeImplicantFunction = false;
                    
                    //The analysed prime implicant function is not a subset of this function.
                    
                }else{
                    for (int l = 0; l < i; l++) {
                        if (primeImplicantFunction < *combinedPrimeImplicantsFunctions[l]) {
                            addPrimeImplicantFunction = false;
                            break;
                            
                            //The analysed prime implicant is fully contained in a larger,
                            //previously found prime implicant.
                        }
                    }
                }
                
                if (addPrimeImplicantFunction == false) {
                    continue;
                }
                
                n++;
                primeImplicants[n - 1] = primeImplicantDescription;
                
                *combinedPrimeImplicantsFunction += primeImplicantFunction;
            }
        }
        
        delete[] groups;
        groups = NULL;
        
        combinedPrimeImplicantsFunctions[i] = combinedPrimeImplicantsFunction;
        
        if (*combinedPrimeImplicantsFunction == *this) {
            break;
            
            //The set of all found prime implicants fully describes this function.
            //There aren't any smaller prime implicants.
        }
    }
    
    delete[] combinedPrimeImplicantsFunctions;
}

void Function::getEssentialPrimeImplicants(int &n, string * &essentialPrimeImplicants) const {
    int m;
    string * primeImplicants;
    
    getPrimeImplicants(m, primeImplicants);
    
    //For all rows in the truth table:
    //If the result is 1:
    //  Find how many prime implicants cover the result
    //  If only one prime implicant covers it:
    //    Remember which one
    int * epiIndexesForRows = new int[nRows];
    
    for (int i = 0; i < nRows; i++) {
        epiIndexesForRows[i] = -2;
    }
    
    for (int i = 0; i < nRows; i++) {
        if (results[i] == 0) {
            epiIndexesForRows[i] = -1;
            continue;
        }
        
        for (int j = 0; j < m; j++) {
            Function f(primeImplicants[j], nInputs);
            
            if (f.getResultForRow(i) == 1) {
                if (epiIndexesForRows[i] == -2) {
                    epiIndexesForRows[i] = j;
                }else{
                    epiIndexesForRows[i] = -1;
                }
            }
        }
    }
    
    //Find which prime implicants are essential:
    bool * isEpi = new bool[m];
    
    for (int i = 0; i < m; i++) {
        isEpi[i] = false;
    }
    
    n = 0;
    
    for (int i = 0; i < nRows; i++) {
        int index = epiIndexesForRows[i];
        if (index >= 0) {
            isEpi[index] = true;
            n++;
        }
    }
    
    //Store all essential prime implicants in an array:
    essentialPrimeImplicants = new string[n];
    
    n = 0;
    for (int i = 0; i < m; i++) {
        if (isEpi[i]) {
            essentialPrimeImplicants[n] = primeImplicants[i];
            n++;
        }
    }
    
    delete[] primeImplicants;
    delete[] epiIndexesForRows;
    delete[] isEpi;
}

void Function::getNonessentialPrimeImplicants(int &n, string * &nonessentialPrimeImplicants) const {
    int m;
    string * primeImplicants;
    
    getPrimeImplicants(m, primeImplicants);
    
    //For all rows in the truth table:
    //If the result is 1:
    //  Find how many prime implicants cover the result
    //  If only one prime implicant covers it:
    //    Remember which one
    int * epiIndexesForRows = new int[nRows];
    
    for (int i = 0; i < nRows; i++) {
        epiIndexesForRows[i] = -2;
    }
    
    for (int i = 0; i < nRows; i++) {
        if (results[i] == 0) {
            epiIndexesForRows[i] = -1;
            continue;
        }
        
        for (int j = 0; j < m; j++) {
            Function f(primeImplicants[j], nInputs);
            
            if (f.getResultForRow(i) == 1) {
                if (epiIndexesForRows[i] == -2) {
                    epiIndexesForRows[i] = j;
                }else{
                    epiIndexesForRows[i] = -1;
                }
            }
        }
    }
    
    //Find which prime implicants are essential:
    bool * isEpi = new bool[m];
    
    for (int i = 0; i < m; i++) {
        isEpi[i] = false;
    }
    
    n = 0;
    
    for (int i = 0; i < nRows; i++) {
        int index = epiIndexesForRows[i];
        if (index >= 0) {
            isEpi[index] = true;
            n++;
        }
    }
    
    //Store all nonessential prime implicants in an array:
    nonessentialPrimeImplicants = new string[m - n];
    
    n = 0;
    for (int i = 0; i < m; i++) {
        if (isEpi[i] == false) {
            nonessentialPrimeImplicants[n] = primeImplicants[i];
            n++;
        }
    }
    
    delete[] primeImplicants;
    delete[] epiIndexesForRows;
    delete[] isEpi;
}

#pragma mark -
#pragma mark Expressions
string Function::getExpression() const {
    int n, m;
    string * essentialPrimeImplicants;
    string * nonessentialPrimeImplicants;
    
    getEssentialPrimeImplicants(n, essentialPrimeImplicants);
    getNonessentialPrimeImplicants(m, nonessentialPrimeImplicants);
    
    //Expression has to start with sum of essential prime implicants:
    string epiSum = "";
    
    for (int i = 0; i < n; i++) {
        if (i > 0) epiSum += " + ";
        epiSum += essentialPrimeImplicants[i];
    }
    
    Function epiFunction(epiSum, nInputs);
    
    if (epiSum == *this) {
        delete[] essentialPrimeImplicants;
        delete[] nonessentialPrimeImplicants;
        
        return epiSum;
    }
    
    //Add any necessary nonessential prime implicants:
    for (int i = 1; i < m; i++) {
        int nGroups;
        int *groups;
        
        groups = groupsKOutOfN(i, m, nGroups);
        
        for (int j = 0; j < nGroups; j++) {
            string piSum = epiSum;
            
            for (int k = 0; k < i; k++) {
                if (n > 0 || k > 0) piSum += " + ";
                piSum += nonessentialPrimeImplicants[groups[j * i + k]];
            }
            
            Function piFunction(piSum, nInputs);
            
            if (piSum == *this) {
                delete[] essentialPrimeImplicants;
                delete[] nonessentialPrimeImplicants;
                
                delete[] groups;
                
                return piSum;
            }
        }
        
        delete[] groups;
    }
    
    delete[] essentialPrimeImplicants;
    delete[] nonessentialPrimeImplicants;
    
    return "";
}

void Function::setResultsWithExpression(string expression) {
    if (expression == NULL ||
        expression.length() < 1) {
        return;
    }
    
    for (int i = 0; i < nRows; i++) {
        bvalue_t * inputValues = getInputValuesForRow(i);
        
        results[i] = valueForExpression(expression, nInputs, inputValues);
        
        delete[] inputValues;
    }
}


#pragma mark -
#pragma mark Sum Notation
string Function::getSumNotation() const {
    string sumNotaiton = "SUM[";
    
    for (int i = 0; i < nRows; i++) {
        if (results[i] == 0) {
            continue;
        }
        
        if (sumNotaiton.length() > 4) {
            sumNotaiton += ", ";
        }
        
        sumNotaiton += std::to_string(i);
    }
    
    sumNotaiton += "]";
    
    return sumNotaiton;
}

void Function::setResultsWithSumNotation(string sumNotation) {
    if (sumNotation == NULL) {
        return;
    }
    
    size_t l = sumNotation.length();
    
    //Check syntax:
    if (l < 5) {
        return;
    }else if (sumNotation[0] != 'S' ||
              sumNotation[1] != 'U' ||
              sumNotation[2] != 'M' ||
              sumNotation[3] != '[' ||
              sumNotation[l - 1] != ']') {
        return;
    }
    
    for (int i = 4; i < l - 1; i++) {
        char c = sumNotation[i];
        
        if (c == ' ' || c == ',') continue;
        if (c >= 48 && c <= 57) continue;
        
        return;
    }
    
    //Set all results to 0:
    for (int i = 0; i < nRows; i++) {
        results[i] = 0;
    }
    
    //Iterate through the list of coefficients:
    string term = "";
    
    for (int i = 4; i < l; i++) {
        if (i < l - 1) {
            if (' ' == sumNotation[i]) {
                continue;
            }else if (',' != sumNotation[i]) {
                term += sumNotation[i];
                
                continue;
            }
        }
        
        int n = 0;
        
        for (int j = 0; j < term.length(); j++) {
            char c = term[term.length() - 1 - j];
            
            n += (c - 48) * pow(10, j);
        }
        
        if (n < nRows) {
            results[n] = 1;
        }
        
        term = "";
    }
}

#pragma mark -
#pragma mark Truth Tables
void Function::printTruthTable() const {
    for (int i = 0; i < nInputs; i++) {
        cout << (char)(97 + i) << " ";
    }
    cout << "| X\n";
    
    for (int i = 0; i < nInputs * 2; i++) {
        cout << "-";
    }
    cout << "|--\n";
    
    for (int i = 0; i < nRows; i++) {
        bvalue_t * inputValues = getInputValuesForRow(i);
        
        for (int j = 0; j < nInputs; j++) {
            cout << charForBValue(inputValues[j]) << " ";
        }
        
        delete[] inputValues;
        
        cout << "| ";
        cout << charForBValue(results[i]);
        cout << "\n";
    }
    
    cout << endl;
}

void Function::readInTruthTable() {
    for (int i = 0; i < nInputs; i++) {
        cout << (char)(97 + i) << " ";
    }
    cout << "| X\n";
    
    for (int i = 0; i < nInputs * 2; i++) {
        cout << "-";
    }
    cout << "|--\n";
    
    for (int i = 0; i < nRows; i++) {
        bvalue_t * inputValues = getInputValuesForRow(i);
        
        for (int j = 0; j < nInputs; j++) {
            cout << charForBValue(inputValues[j]) << " ";
        }
        
        delete[] inputValues;
        
        char c;
        
        cout << "| ";
        cin >> c;
        
        results[i] = bvalueForChar(c);
    }
    
    cout << endl;
}

#pragma mark -
#pragma mark Karnaugh Maps
void Function::printKarnaughMap() const {
    if (nInputs > 4) {
        return;
    }
    
    if (nInputs == 1) {
        char a = charForBValue(results[0]);
        char b = charForBValue(results[1]);
        
        cout << " |-|\n";
        cout << " |" << a << "|\n";
        cout << "-|-|\n";
        cout << "a|" << b << "|\n";
        cout << " |-|\n";
        cout << endl;
        
    }else if (nInputs == 2) {
        char a = charForBValue(results[0]);
        char b = charForBValue(results[1]);
        char c = charForBValue(results[2]);
        char d = charForBValue(results[3]);
        
        cout << "   |b \n";
        cout << " |-|-|\n";
        cout << " |" << a << "|" << b << "|\n";
        cout << "-|-|-|\n";
        cout << "a|" << c << "|" << d << "|\n";
        cout << " |-|-|\n";
        cout << endl;
        
    }else if (nInputs == 3) {
        char m0 = charForBValue(results[0]);
        char m1 = charForBValue(results[1]);
        char m2 = charForBValue(results[2]);
        char m3 = charForBValue(results[3]);
        char m4 = charForBValue(results[4]);
        char m5 = charForBValue(results[5]);
        char m6 = charForBValue(results[6]);
        char m7 = charForBValue(results[7]);
        
        cout << "     |b   \n";
        cout << " |-|-|-|-|\n";
        cout << " |" << m0 << "|" << m1 << "|" << m3 << "|" << m2 << "|\n";
        cout << "-|-|-|-|-|\n";
        cout << "a|" << m4 << "|" << m5 << "|" << m7 << "|" << m6 << "|\n";
        cout << " |-|-|-|-|\n";
        cout << "   |  c|  \n";
        cout << endl;
        
    }else if (nInputs == 4) {
        char m0 = charForBValue(results[0]);
        char m1 = charForBValue(results[1]);
        char m2 = charForBValue(results[2]);
        char m3 = charForBValue(results[3]);
        char m4 = charForBValue(results[4]);
        char m5 = charForBValue(results[5]);
        char m6 = charForBValue(results[6]);
        char m7 = charForBValue(results[7]);
        char m8 = charForBValue(results[8]);
        char m9 = charForBValue(results[9]);
        char mA = charForBValue(results[10]);
        char mB = charForBValue(results[11]);
        char mC = charForBValue(results[12]);
        char mD = charForBValue(results[13]);
        char mE = charForBValue(results[14]);
        char mF = charForBValue(results[15]);
        
        cout << "     |c    \n";
        cout << " |-|-|-|-| \n";
        cout << " |" << m0 << "|" << m1 << "|" << m3 << "|" << m2 << "| \n";
        cout << " |-|-|-|-|-\n";
        cout << " |" << m4 << "|" << m5 << "|" << m7 << "|" << m6 << "|b\n";
        cout << "-|-|-|-|-| \n";
        cout << "a|" << mC << "|" << mD << "|" << mF << "|" << mE << "| \n";
        cout << " |-|-|-|-|-\n";
        cout << " |" << m8 << "|" << m9 << "|" << mB << "|" << mA << "| \n";
        cout << " |-|-|-|-| \n";
        cout << "   |  d|   \n";
        cout << endl;
    }
}

#pragma mark -
#pragma mark Operators
void Function::operator+=(const Function &f) {
    if (f.getNumberOfInputs() != nInputs) {
        return;
    }
    
    for (int i = 0; i < nRows; i++) {
        bvalue_t v1 = results[i];
        bvalue_t v2 = f.getResultForRow(i);
        
        if (v1 == -1) {
            if (v2 == 1) {
                results[i] = 1;
            }else{
                results[i] = -1;
            }
            
        }else if (v2 == -1) {
            if (v1 == 1) {
                results[i] = 1;
            }else{
                results[i] = -1;
            }
            
        }else{
            if (v1 == 1 || v2 == 1) {
                results[i] = 1;
            }else{
                results[i] = 0;
            }
        }
    }
}

void Function::operator*=(const Function &f) {
    if (f.getNumberOfInputs() != nInputs) {
        return;
    }
    
    for (int i = 0; i < nRows; i++) {
        bvalue_t v1 = results[i];
        bvalue_t v2 = f.getResultForRow(i);
        
        if (v1 == -1) {
            if (v2 == 0) {
                results[i] = 0;
            }else{
                results[i] = -1;
            }
            
        }else if (v2 == -1) {
            if (v1 == 0) {
                results[i] = 0;
            }else{
                results[i] = -1;
            }
            
        }else{
            if (v1 == 1 && v2 == 1) {
                results[i] = 1;
            }else{
                results[i] = 0;
            }
        }
    }
}

Function operator!(const Function &f) {
    int nInputs = f.getNumberOfInputs();
    int nRows = f.getNumberOfRows();
    
    Function f_inv(nInputs);
    
    for (int i = 0; i < nRows; i++) {
        bvalue_t v = f.getResultForRow(i);
        
        if (v == 0) {
            f_inv.setResultForRow(i, 1);
        }else if (v == 1) {
            f_inv.setResultForRow(i, 0);
        }else{
            f_inv.setResultForRow(i, -1);
        }
    }
    
    return f_inv;
}

Function operator+(const Function &f1, const Function &f2) {
    if (f1.getNumberOfInputs() != f2.getNumberOfInputs()) {
        return NULL;
    }
    
    int nInputs = f1.getNumberOfInputs();
    int nRows = f1.getNumberOfRows();
    
    Function f_or(nInputs);
    
    for (int i = 0; i < nRows; i++) {
        bvalue_t v1 = f1.getResultForRow(i);
        bvalue_t v2 = f2.getResultForRow(i);
        
        if (v1 == -1) {
            if (v2 == 1) {
                f_or.setResultForRow(i, 1);
            }else{
                f_or.setResultForRow(i, -1);
            }
            
        }else if (v2 == -1) {
            if (v1 == 1) {
                f_or.setResultForRow(i, 1);
            }else{
                f_or.setResultForRow(i, -1);
            }
            
        }else{
            if (v1 == 1 || v2 == 1) {
                f_or.setResultForRow(i, 1);
            }else{
                f_or.setResultForRow(i, 0);
            }
        }
    }
    
    return f_or;
}

Function operator*(const Function &f1, const Function &f2) {
    if (f1.getNumberOfInputs() != f2.getNumberOfInputs()) {
        return NULL;
    }
    
    int nInputs = f1.getNumberOfInputs();
    int nRows = f1.getNumberOfRows();
    
    Function f_and(nInputs);
    
    for (int i = 0; i < nRows; i++) {
        bvalue_t v1 = f1.getResultForRow(i);
        bvalue_t v2 = f2.getResultForRow(i);
        
        if (v1 == -1) {
            if (v2 == 0) {
                f_and.setResultForRow(i, 0);
            }else{
                f_and.setResultForRow(i, -1);
            }
            
        }else if (v2 == -1) {
            if (v1 == 0) {
                f_and.setResultForRow(i, 0);
            }else{
                f_and.setResultForRow(i, -1);
            }
            
        }else{
            if (v1 == 1 && v2 == 1) {
                f_and.setResultForRow(i, 1);
            }else{
                f_and.setResultForRow(i, 0);
            }
        }
    }
    
    return f_and;
}

bool operator<(const Function &f1, const Function &f2) {
    if (f1.getNumberOfInputs() != f2.getNumberOfInputs()) {
        return NULL;
    }
    
    int nRows = f1.getNumberOfRows();
    
    for (int i = 0; i < nRows; i++) {
        bvalue_t v1 = f1.getResultForRow(i);
        bvalue_t v2 = f2.getResultForRow(i);
        
        if (v1 != 1) {
            continue;
        }else if (v2 == 0) {
            return false;
        }
    }
    
    return true;
}

bool operator>(const Function &f1, const Function &f2) {
    return f2 < f1;
}

bool operator==(const Function &f1, const Function &f2) {
    if (f1.getNumberOfInputs() != f2.getNumberOfInputs()) {
        return NULL;
    }
    
    int nRows = f1.getNumberOfRows();
    
    for (int i = 0; i < nRows; i++) {
        bvalue_t v1 = f1.getResultForRow(i);
        bvalue_t v2 = f2.getResultForRow(i);
        
        if (v1 != v2) {
            return false;
        }
    }
    
    return true;
}

bool operator!=(const Function &f1, const Function &f2) {
    return !(f1 == f2);
}
