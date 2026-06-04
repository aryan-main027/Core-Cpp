#include<iostream>

#define MAX(a,b) ((a) > (b) ? (a) : (b))

void dumy(int x = 10 , int y = 10){
    #define X 10
}
int main(){

    // std::cout<<"Hello World"<<std::endl;

    // int a = 2;
    // int b = 3;
    // std::cout<<MAX(a,b)<<std::endl;
    // std::cin.get();

    dumy(10,1);
    // std::cout<<dumy()<<std::endl;
    std::cout<<X<<std::endl;
    std::cin.get();
}

/*
    define x 10:

    after x whatever i write preprocessor will replace x with that content whixh is after that 
    now this is where c++ scope rule and error occurred 

    if X 10;

    and we are doing cout << X << endl;
    then preproccessor will replace X with 10; Completely and 

*/