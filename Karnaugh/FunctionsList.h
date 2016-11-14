//
//  FunctionsList.h
//  Karnaugh
//
//  Created by Arne Philipeit on 10/11/2016.
//  Copyright © 2016 Arne Philipeit. All rights reserved.
//

#pragma once
#include <cstring>

#include "Function.h"
#include "FunctionNode.h"

class FunctionsList {
private:
    FunctionNode * head;
    
public:
    //Constructors:
    FunctionsList();
    
    //Destructor:
    ~FunctionsList();
    
    //Function Management:
    int getNumberOfFunctions();
    
    bool hasFunctionWithName(string name);
    
    int getIndexOfFunctionWithName(string name);
    string getNameOfFunctionAtIndex(int index);
    
    Function * getFunctionWithName(string name);
    Function * getFunctionAtIndex(int index);
    
    void setNameOfFunctionWithName(string oldName, string newName);
    void setNameOfFunctionAtIndex(int index, string newName);
    
    void setFunctionWithName(string name, Function * function);
    void setFunctionAtIndex(int index, Function * function);
    
    void addFunction(string name, Function * function);
    void insertFunctionBeforeFunctionWithName(string existingFunctionName, string newName, Function * newFunction);
    void insertFunctionBeforeFunctionAtIndex(int index, string newName, Function * newFunction);
    void deleteFunctionWithName(string name);
    void deleteFunctionAtIndex(int index);
    
};
