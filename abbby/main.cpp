#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include "texcaller.h"
#include "parser.hpp"
#include "makelatex.hpp"
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
                    int a = std::stoi(expression.leaves[1].node);
                    return Expr("*", Expr("*", expression.leaves[1], Expr("^", expression.leaves[0], std::to_string(a - 1))), diff(expression.leaves[0]));
                }
            }
            
        }
            
        
    }
    return expression;
}

//
/*
std::string make_latex_expr(Expr expression) {
    switch (expression.leaves.size()) {
        case 0: {
            return expression.node;
        }
        case 1: {
            if (expression.node == "sin" || expression.node == "cos") {
                // sin(f(x)) -> "\\sin{(f(x))}"
                return "\\" + expression.node + "{(" + make_latex_expr(expression.leaves[0]) + ")}";
            }
            if (expression.node == "+" || expression.node == "-") {
                // -f(x) -> -f(x) and (+f(x))' -> +f'(x)
                return expression.node + make_latex_expr(expression.leaves[0]);
            }
        }
        case 2: {
            if (expression.node == "+" || expression.node == "-") {
                return make_latex_expr(expression.leaves[0]) + expression.node + make_latex_expr(expression.leaves[1]);
            }
            if (expression.node == "*") {
                std::string mul_return = "";
                if (expression.leaves[0].leaves.size() == 2 || expression.leaves[0].node == "-")
                    mul_return += "(" + make_latex_expr(expression.leaves[0]) + ")";
                else
                    mul_return += make_latex_expr(expression.leaves[0]);
                mul_return += "\\cdot ";
                if (expression.leaves[1].leaves.size() == 2 || expression.leaves[1].node == "-")
                    mul_return += "(" + make_latex_expr(expression.leaves[1]) + ")";
                else
                    mul_return += make_latex_expr(expression.leaves[1]);
                return mul_return;
            }
            if (expression.node == "/") {
                return "\\frac{" + make_latex_expr(expression.leaves[0]) + "}{" + make_latex_expr(expression.leaves[1]) + "} ";
            }
            if (expression.node == "^") {
                std::string left = make_latex_expr(expression.leaves[0]);
                std::string right = make_latex_expr(expression.leaves[1]);
                if (expression.leaves[0].leaves.size() == 0)
                    return left + "^{" + right + "}";
                else
                    return "(" + left + ")" + "^{" + right + "}";
            }
        }
    }
    return "";
}

std::string make_latex(Expr expr, Expr diffexpr) {
    std::string expression =
        "\\documentclass[a4paper,12pt]{article} "
        "\\usepackage[T2A]{fontenc}"
        "\\usepackage[utf8]{inputenc}"
        "\\usepackage[english,russian]{babel}"
        "\\usepackage{amsmath,amsfonts,amssymb,amsthm,mathtools}"
        "\\begin{document}"
        "$" + make_latex_expr(expr) + "\\rightarrow $"
        "$" + make_latex_expr(diffexpr) + "$"
        "\\end{document}";
    return expression;
}
*/
int main() {
    //Expr test = Expr("*", Expr("2"), Expr("2"));
    std::string input;
    std::cin >> input;
    Parser pars(input.c_str());
    Expr myExpr = pars.parse();
    Expr d = diff(myExpr);
    
   
    
    try {
        std::string pdf;
        std::string info;
        
        texcaller::convert(pdf, info, make_latex(myExpr, d), "LaTeX", "PDF", 5);

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

