//
//  parser.hpp
//  Acram alpha
//
//  Created by Панышев Александр Сергеевич on 11.05.2021.
//  Copyright © 2021 Панышев Александр Сергеевич. All rights reserved.
//

#ifndef parser_hpp
#define parser_hpp
#pragma once
#include <stdio.h>
#include <string>
#include <vector>
#include <cctype>
#include "mystring.hpp"
#endif /* parser_hpp */

struct Expr {
    mystring node;
    std::vector<Expr> leaves;
    Expr(mystring node);
    Expr(mystring node, Expr leaf);
    Expr(mystring inode, Expr leaf1, Expr leaf2);
};

class Parser {
public:
    Parser(const char* input);
    Expr parse();
    
private:
    const char* input;
    mystring parse_node();
    Expr parse_one_leaf();
    int get_priority(mystring node);
    Expr parse_two_leaves(int priority);
};
