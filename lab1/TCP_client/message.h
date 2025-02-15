#include <iostream>
#include <string>
#include <variant>

class Message {
private:
    char type; 
    int i;    
    float f; 
    char c; 
public:

    void setInt(int value);
    void setFloat(float value);
    void setChar(char value);


    void print();
};


