//
//  FunctionNode.h
//  Karnaugh
//
//  Created by Arne Philipeit on 10/11/2016.
//  Copyright © 2016 Arne Philipeit. All rights reserved.
//

#pragma once
#include <cstring>

#include "Function.h"

class FunctionNode {
private:
    string name;
    Function *function;
    FunctionNode *next;
    
public:
    //Constructors:
    FunctionNode(string name, Function * function);
    
    //Destructor:
    ~FunctionNode();
    
    //Basic access methods:
    string getName();
    void setName(string name);
    
    Function * getFunction();
    void setFunction(Function * function);
    
    FunctionNode * getNext();
    void setNext(FunctionNode * next);
    
    //Help methods:
    static bool isValidName(string name);
    
};
