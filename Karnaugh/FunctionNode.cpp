//
//  FunctionNode.cpp
//  Karnaugh
//
//  Created by Arne Philipeit on 10/11/2016.
//  Copyright © 2016 Arne Philipeit. All rights reserved.
//

#include "FunctionNode.h"

#pragma mark Constructors
FunctionNode::FunctionNode(string n, Function * f) {
    name = n;
    function = f;
    next = NULL;
}

#pragma mark -
#pragma mark Destructor
FunctionNode::~FunctionNode() {
    delete function;
}

#pragma mark -
#pragma mark Basic access methods
string FunctionNode::getName() {
    return name;
}

void FunctionNode::setName(string n) {
    if (isValidName(n) == false) {
        return;
    }
    
    name = n;
}

Function * FunctionNode::getFunction() {
    return function;
}

void FunctionNode::setFunction(Function * f) {
    if (f == NULL) {
        return;
    }
    
    function = f;
}

FunctionNode * FunctionNode::getNext() {
    return next;
}

void FunctionNode::setNext(FunctionNode * n) {
    next = n;
}

#pragma mark -
#pragma mark Help methods
bool FunctionNode::isValidName(string name) {
    if (name == NULL ||
        name.length() < 1) {
        return false;
    }
    
    for (int i = 0; i < name.length(); i++) {
        if ((name[i] >= 65 && name[i] <= 91) ||
            (name[i] >= 97 && name[i] <= 123)) {
            continue;
        }else{
            return false;
        }
    }
    
    return true;
}
