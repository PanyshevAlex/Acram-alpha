//
//  parser.cpp
//  Acram aplha
//
//  Created by Панышев Александр Сергеевич on 11.05.2021.
//  Copyright © 2021 Панышев Александр Сергеевич. All rights reserved.
//

#include "parser.hpp"

Expr::Expr() : node("\0") {};

Expr::Expr(mystring node) : node(node) {};

Expr::Expr(mystring node, Expr leaf) : node(node) {
    leaves.push_back(leaf);
}

Expr::Expr(mystring inode, Expr leaf1, Expr leaf2) {
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

Parser::Parser(const char* input) : input(input) {};

mystring Parser::parse_node() {
    while (std::isspace(*input))
        input++;
    
    if (std::isdigit(*input)) {
        mystring digit;
        while (std::isdigit(*input) || *input == '.') {
            digit.push_back(*input);
            input++;
        }
        return digit;
    }
    
    static const mystring nodes[] = {"+", "-", "*", "/", "^", "sin", "cos", "(", ")", "x", "exp", "e", "pi", "sqrt", "ln", "tan", "cot", "arcsin", "arccos", "arctan", "arcctg"};
    
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
    if (element.is_empty())
        throw std::runtime_error("Unexpected value");
    if (std::isdigit(element[0]) || element == "x" || element == "e" || element == "pi")
        return Expr(element);
    if (element == "(") {
        auto sub_Expr = parse();
        if (!(parse_node() == ")"))
            throw std::runtime_error("Expected ')'");
        return sub_Expr;
    }
    
    return Expr(element, parse_one_leaf());
}

int Parser::get_priority(mystring node) {
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
    if (node == "exp")
        return 3;
    if (node == "sqrt")
        return 3;
    if (node == "ln")
        return 3;
    if (node == "tan")
        return 3;
    if (node == "cot")
        return 3;
    if (node == "arcsin")
        return 3;
    if (node == "arccos")
        return 3;
    if (node == "arctan")
        return 3;
    if (node == "arcctg")
        return 3;
    return 0;
}

Expr Parser::parse_two_leaves(int priority) {
    Expr left_element = parse_one_leaf();
    
    while (true) {
        mystring op = parse_node();
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
