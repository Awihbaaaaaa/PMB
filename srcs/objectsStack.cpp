#include <iostream>
#include "../Headers/objectsStack.hpp"
#include <iomanip>

void objectsStack::printDataPoint(double pnt)
{
    std::cout << std::left << std::setw(0) << pnt << "    |    ";
}

void objectsStack::displayStack() {
    objStateSpace* tempObj = head;
    while (tempObj) {
        printDataPoint(tempObj -> x);
        printDataPoint(tempObj -> y);
        printDataPoint(tempObj -> v);
        printDataPoint(tempObj -> theta);
        printDataPoint(tempObj -> w);
        tempObj = tempObj->nextObj;
        if (tempObj)
            std::cout << std::endl;
    }
    std::cout << "\n";
}

void objectsStack::push(const double* val) {
    objStateSpace* newObj = new objStateSpace(val);
    if (!newObj) {
        std::cout << "Stack overflow.\n";
    } else {
        newObj->nextObj = head;
        head = newObj;
    }
}

objStateSpace objectsStack::pop() {
    if (head == nullptr) {
        std::cout << "Stack is empty.\n";
        return objStateSpace(nullptr); // Return a default value for T
    }
    objStateSpace* tempObj = head;
    head = head->nextObj;
    objStateSpace popedObj;
    popedObj.x = tempObj -> x,
    popedObj.y = tempObj -> y,
    popedObj.v = tempObj -> v,
    popedObj.theta = tempObj -> theta,
    popedObj.w = tempObj -> w;

    delete tempObj;
    return popedObj;
}

bool objectsStack::isEmpty() {
    if(head != nullptr)
        return false;
    return true;
}

int objectsStack::nrOfObjects(){
    int nrOfObjects = 0;
    
    objStateSpace* tempObj = head;

    while(tempObj){
        nrOfObjects ++;
        tempObj = tempObj -> nextObj;
    }

    return nrOfObjects;
}

