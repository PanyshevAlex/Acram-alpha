#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include "texcaller.h"
#include "parser.hpp"
#include <fstream>
/*
struct Expr {
    std::string node;
    std::vector<Expr> leaves;
    Expr(std::string node) : node(node) {};
    Expr(std::string node, Expr leaf) : node(node) {
        leaves.push_back(leaf);
    }
    Expr(std::string inode, Expr leaf1, Expr leaf2) {
        if (inode == "+") {
            if (leaf1.node == "0")
                node = leaf2.node;
            else if (leaf2.node == "0")
                node = leaf1.node;
            else {
                node = inode;
                leaves.push_back(leaf1);
                leaves.push_back(leaf2);
            }
        }
        else if (inode == "*") {
            if (leaf1.node == "0")
                node = "0";
            else if (leaf2.node == "0")
                node = "0";
            else if (leaf1.node == "1") {
                node = leaf2.node;
                if (leaf2.leaves.size() == 1) {
                    leaves.push_back(leaf2.leaves[0]);
                }
                if (leaf2.leaves.size() == 2) {
                    leaves.push_back(leaf2.leaves[0]);
                    leaves.push_back(leaf2.leaves[1]);
                }
            }
            else if (leaf2.node == "1") {
                node = leaf1.node;
                if (leaf1.leaves.size() == 1) {
                    leaves.push_back(leaf1.leaves[0]);
                }
                if (leaf1.leaves.size() == 2) {
                    leaves.push_back(leaf1.leaves[0]);
                    leaves.push_back(leaf1.leaves[1]);
                }
            }
            else {
                node = inode;
                leaves.push_back(leaf1);
                leaves.push_back(leaf2);
            }
        }
        else {
            node = inode;
            leaves.push_back(leaf1);
            leaves.push_back(leaf2);
        }
        
    }
};
// TODO: this parser allows expressions like (2+3)2
class Parser {
public:
    Parser(const char* input) : input(input) {};
    Expr parse();
    
private:
    const char* input;
    std::string parse_node();
    Expr parse_one_leaf();
    Expr parse_two_leaves(int priority);
};

std::string Parser::parse_node() {
    while (std::isspace(*input))
        input++;
    
    if (std::isdigit(*input)) {
        std::string digit;
        while (std::isdigit(*input) || *input == '.') {
            digit.push_back(*input);
            input++;
        }
        return digit;
    }
    
    static const std::string nodes[] = {"+", "-", "*", "/", "^", "sin", "cos", "(", ")", "x"};
    
    for (auto& i : nodes) {
        if (std::strncmp(input, i.c_str(), i.length()) == 0) {
            input += i.length();
            return i;
        }
    }
    
    return "";
}

Expr Parser::parse_one_leaf() {
    auto element = parse_node();
    if (element.empty())
        throw std::runtime_error("Unexpected value");
    if (std::isdigit(element[0]) || element == "x")
        return Expr(element);
    if (element == "(") {
        auto sub_Expr = parse();
        if (parse_node() != ")")
            throw std::runtime_error("Expected ')'");
        return sub_Expr;
    }
    
    return Expr(element, parse_one_leaf());
}

int get_priority(std::string node) {
    if (node == "+")
        return 1;
    if (node == "-")
        return 1;
    if (node == "*")
        return 2;
    if (node == "/")
        return 2;
    if (node == "^")
        return 3;
    if (node == "sin")
        return 3;
    if (node == "cos")
        return 3;
    return 0;
}

Expr Parser::parse_two_leaves(int priority) {
    Expr left_element = parse_one_leaf();
    
    while (true) {
        std::string op = parse_node();
        int op_priority = get_priority(op);
        
        if (op_priority <= priority) {
            input -= op.length();
            return left_element;
        }
        
        auto right_element = parse_two_leaves(op_priority);
        left_element = Expr(op, left_element, right_element);
    }
}


Expr Parser::parse() {
    return parse_two_leaves(0);
}
*/

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

int main() {
    //Expr test = Expr("*", Expr("2"), Expr("2"));
    Parser pars("(x*x)/x");
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

