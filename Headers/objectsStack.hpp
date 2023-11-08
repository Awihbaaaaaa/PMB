#ifndef OBJECTSSTACK_HPP
#define OBJECTSSTACK_HPP
#include <iostream>

/*
The following class includes a definition of an object.
Each object includes coordinates on x,y axes, velocity value v, heading theta, an angular velocity w as well as a pointer to the next object.
*/
class objStateSpace{
    public:
        double x;
        double y;
        double v;
        double theta;
        double w;
        objStateSpace * nextObj;

    objStateSpace(const double* value) :  x(value[0]),
                                y(value[1]),
                                v(value[2]),
                                theta(value[3]),
                                w(value[4]),
                                nextObj(nullptr) {};
    objStateSpace() : x(0.0), y(0.0), v(0.0), theta(0.0), w(0.0), nextObj(nullptr) {};

    friend std::ostream& operator<<(std::ostream &os, const objStateSpace &obj);  
};

/* 
The following class is a stack for storing the objects. It includes implementations 
of function for pushing an element, pulling an element, checking if the stack is empty,
finding the stack size and displaying the stack content.
*/
class objectsStack{
    public:
        objStateSpace * head;
    
    objectsStack() : head(nullptr) {
        std::cout << "A stack has been created.\n";
    }

    ~objectsStack() {
        // Delete all objs in the linked list
        while (head) {
            objStateSpace* temp = head;
            head = head->nextObj;
            delete temp;
        }
    }

    void displayStack();
    void push(const double* val);
    objStateSpace pop();
    bool isEmpty();
    int nrOfObjects();
    void printDataPoint(double pnt);
};


#endif