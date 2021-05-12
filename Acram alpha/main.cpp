//
//  main.cpp
//  Acram aplha
//
//  Created by Панышев Александр Сергеевич on 11.05.2021.
//  Copyright © 2021 Панышев Александр Сергеевич. All rights reserved.
//

#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include "texcaller.h"
#include "parser.hpp"
#include "makelatex.hpp"
#include "mystring.hpp"
#include <fstream>

Expr diff(Expr expression) {
    switch (expression.leaves.size()) {
        case 0: {
            if (std::isdigit(expression.node[0]))
                // number' -> 0
                return Expr("0");
            if (expression.node == "x")
                // x' -> 1
                return Expr("1");
        }
        case 1: {
            if (expression.node == "sin") {
                // sin'(f(x)) -> cos(f(x)) * f'(x)
                return Expr("*", Expr("cos", expression.leaves[0]), diff(expression.leaves[0]));
            }
            if (expression.node == "cos") {
                // cos'(f(x)) -> -sin(f(x)) * f'(x)
                return Expr("*", Expr("-", Expr("sin", expression.leaves[0])), diff(expression.leaves[0]));
            }
            if (expression.node == "+" || expression.node == "-") {
                // (-f(x))' -> -f'(x) and (+f(x))' -> +f'(x)
                return Expr(expression.node, diff(expression.leaves[0]));
            }
        }
        case 2: {
            if (expression.node == "+" || expression.node == "-") {
                // (f(x) + g(x))' -> f'(x) + g'(x) and same with '-'
                return Expr(expression.node, diff(expression.leaves[0]), diff(expression.leaves[1]));
            }
            if (expression.node == "*") {
                return Expr("+", Expr("*", diff(expression.leaves[0]), expression.leaves[1]), Expr("*", expression.leaves[0], diff(expression.leaves[1])));
            }
            if (expression.node == "/") {
                // (f(x) / g(x))' -> (f'*g - f*g') / (g^2)
                return Expr("/", Expr("-", Expr("*", diff(expression.leaves[0]), expression.leaves[1]), Expr("*", expression.leaves[0], diff(expression.leaves[1]))), Expr("^", expression.leaves[1], Expr("2")));
            }
            if (expression.node == "^") {
                if (std::isdigit(expression.leaves[1].node[0])) {
                    int a = std::stoi(expression.leaves[1].node.c_str());
                    return Expr("*", Expr("*", expression.leaves[1], Expr("^", expression.leaves[0], mystring(std::to_string(a - 1)))), diff(expression.leaves[0]));
                }
            }
            
        }
    }
    return expression;
}


int main() {
    

    std::string input;
    std::cin >> input;
    mystring myinput = mystring(input);
    Parser pars(myinput.c_str());
    Expr myExpr = pars.parse();
    Expr d = diff(myExpr);
    
    
    try {
        std::string pdf;
        std::string info;
        mystring difftex = make_latex(myExpr, d);
        texcaller::convert(pdf, info, difftex.c_str(), "LaTeX", "PDF", 5);

        std::ofstream fout("output.pdf", std::ios::out | std::ios::trunc);
        if (!fout.is_open()) {
            std::cout << "not open" << std::endl;
            return 1;
        }
        fout << pdf;
        fout.close();
        
    } catch (std::domain_error &e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
     
    return 0;
 
}

