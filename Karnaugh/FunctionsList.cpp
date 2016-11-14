//
//  FunctionsList.cpp
//  Karnaugh
//
//  Created by Arne Philipeit on 10/11/2016.
//  Copyright © 2016 Arne Philipeit. All rights reserved.
//

#include "FunctionsList.h"

#pragma mark Constructors
FunctionsList::FunctionsList() {
    head = NULL;
}

#pragma mark -
#pragma mark Destructor
FunctionsList::~FunctionsList() {
    FunctionNode * c = head;
    
    head = NULL;
    
    while (c != NULL) {
        FunctionNode * t = c;
        
        c = c->getNext();
        
        delete t;
    }
}

#pragma mark-
#pragma mark Function management
int FunctionsList::getNumberOfFunctions() {
    int n = 0;
    
    FunctionNode * c = head;
    
    while (c != NULL) {
        n++;
        
        c = c->getNext();
    }
    
    return n;
}

bool FunctionsList::hasFunctionWithName(string name) {
    return (getIndexOfFunctionWithName(name) != -1);
}

int FunctionsList::getIndexOfFunctionWithName(string name) {
    int index = 0;
    
    FunctionNode * c = head;
    
    while (c != NULL) {
        if (name == c->getName()) {
            return index;
            
        }else{
            index++;
            
            c = c->getNext();
        }
    }
    
    return -1;
}

string FunctionsList::getNameOfFunctionAtIndex(int index) {
    if (index < 0) {
        return NULL;
    }
    
    int i = 0;
    
    FunctionNode * c = head;
    
    while (c != NULL) {
        if (index == i) {
            return c->getName();
            
        }else{
            i++;
            
            c = c->getNext();
        }
    }
    
    return NULL;
}

Function * FunctionsList::getFunctionWithName(string name) {
    FunctionNode * c = head;
    
    while (c != NULL) {
        if (name == c->getName()) {
            return c->getFunction();
            
        }else{
            c = c->getNext();
        }
    }
    
    return NULL;
}

Function * FunctionsList::getFunctionAtIndex(int index) {
    if (index < 0) {
        return NULL;
    }
    
    int i = 0;
    
    FunctionNode * c = head;
    
    while (c != NULL) {
        if (index == i) {
            return c->getFunction();
            
        }else{
            i++;
            
            c = c->getNext();
        }
    }
    
    return NULL;
}

void FunctionsList::setNameOfFunctionWithName(string oldName, string newName) {
    if (FunctionNode::isValidName(newName) == false ||
        hasFunctionWithName(newName)) {
        return;
    }
    
    FunctionNode * c = head;
    
    while (c != NULL) {
        if (oldName == c->getName()) {
            c->setName(newName);
            
            return;
            
        }else{
            c = c->getNext();
        }
    }
}

void FunctionsList::setNameOfFunctionAtIndex(int index, string newName) {
    if (index < 0 ||
        FunctionNode::isValidName(newName) == false ||
        hasFunctionWithName(newName)) {
        return;
    }
    
    int i = 0;
    
    FunctionNode * c = head;
    
    while (c != NULL) {
        if (index == i) {
            c->setName(newName);
            
            return;
            
        }else{
            i++;
            
            c = c->getNext();
        }
    }
}

void FunctionsList::setFunctionWithName(string name, Function * function) {
    if (function == NULL) {
        return;
    }
    
    FunctionNode * c = head;
    
    while (c != NULL) {
        if (name == c->getName()) {
            c->setFunction(function);
            
            return;
            
        }else{
            c = c->getNext();
        }
    }
}

void FunctionsList::setFunctionAtIndex(int index, Function * function) {
    if (index < 0 ||
        function == NULL) {
        return;
    }
    
    int i = 0;
    
    FunctionNode * c = head;
    
    while (c != NULL) {
        if (index == i) {
            c->setFunction(function);
            
            return;
            
        }else{
            i++;
            
            c = c->getNext();
        }
    }
}

void FunctionsList::addFunction(string name, Function * function) {
    if (FunctionNode::isValidName(name) == false ||
        hasFunctionWithName(name)||
        function == NULL) {
        return;
    }
    
    FunctionNode * newNode = new FunctionNode(name, function);
    
    if (head == NULL) {
        head = newNode;
        
    }else{
        FunctionNode * c = head;
        
        while (c->getNext() != NULL) {
            c = c->getNext();
        }
        
        c->setNext(newNode);
    }
}

void FunctionsList::insertFunctionBeforeFunctionWithName(string n, string newName, Function * newFunction) {
    if (FunctionNode::isValidName(newName) == false ||
        hasFunctionWithName(newName)||
        newFunction == NULL) {
        return;
    }
    
    FunctionNode * newNode = new FunctionNode(newName, newFunction);
    
    FunctionNode *p = NULL; //Previous
    FunctionNode *c = head; //Current
    
    while (c != NULL) {
        if (n == c->getName()) {
            if (p == NULL) {
                head = newNode;
            }else{
                p->setNext(newNode);
            }
            
            newNode->setNext(c);
            
            return;
            
        }else{
            p = c;
            c = c->getNext();
        }
    }
    
    delete newNode;
}

void FunctionsList::insertFunctionBeforeFunctionAtIndex(int index, string newName, Function * newFunction) {
    if (index < 0 ||
        FunctionNode::isValidName(newName) == false ||
        hasFunctionWithName(newName)||
        newFunction == NULL) {
        return;
    }
    
    FunctionNode * newNode = new FunctionNode(newName, newFunction);
    
    int i = 0;
    
    FunctionNode *p = NULL; //Previous
    FunctionNode *c = head; //Current
    
    while (c != NULL) {
        if (index == i) {
            if (p == NULL) {
                head = newNode;
            }else{
                p->setNext(newNode);
            }
            
            newNode->setNext(c);
            
            return;
            
        }else{
            i++;
            
            p = c;
            c = c->getNext();
        }
    }
    
    delete newNode;
}

void FunctionsList::deleteFunctionWithName(string name) {
    FunctionNode *p = NULL; //Previous
    FunctionNode *c = head; //Current
    
    while (c != NULL) {
        if (name == c->getName()) {
            if (p == NULL) {
                head = c->getNext();
            }else{
                p->setNext(c->getNext());
            }
            
            delete c;
            
            return;
            
        }else{
            p = c;
            c = c->getNext();
        }
    }
}

void FunctionsList::deleteFunctionAtIndex(int index) {
    if (index < 0) {
        return;
    }
    
    int i = 0;
    
    FunctionNode *p = NULL; //Previous
    FunctionNode *c = head; //Current
    
    while (c != NULL) {
        if (index == i) {
            if (p == NULL) {
                head = c->getNext();
            }else{
                p->setNext(c->getNext());
            }
            
            delete c;
            
            return;
            
        }else{
            i++;
            
            p = c;
            c = c->getNext();
        }
    }
}
