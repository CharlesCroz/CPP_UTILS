//
// Created by charles on 09/10/22.
//

#include "Matrix.hpp"
#include <iostream>

void display(const Matrix<int> &mat){
    for(auto &a: mat){
        std::cout << a << "\n";
    }
}

int main(){
    Matrix<int> m(4, 3);
    for(auto a = m.begin() ; a != m.end() ; a++){
        *a = rand() % 10;
    }
    display(m);
    return 0;
}