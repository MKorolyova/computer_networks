#include "message.h"
#include <cstring> 

    void Message::setInt(int data){
        this->type = 0; 
        this->i = data; 
    }

    void Message::setFloat(float data) {
        this->type = 1; 
        this->f = data; 
    }

    void Message::setChar( char data){
        this->type = 2; 
        this->c = data; 
    }


    void Message::print() {
        switch (this->type) {
            case 0:
                std::cout << "Integer: " << i << std::endl;
                break;
            case 1:
                std::cout << "Float: " << f << std::endl;
                break;
            case 2:
                std::cout << "Char: " << c << std::endl;
                break;
            default:
                break;
        }
    }

