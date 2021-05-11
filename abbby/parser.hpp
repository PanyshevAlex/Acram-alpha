//
//  parser.hpp
//  abbby
//
//  Created by Панышев Александр Сергеевич on 11.05.2021.
//  Copyright © 2021 Панышев Александр Сергеевич. All rights reserved.
//

#ifndef parser_hpp
#define parser_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <cctype>
#endif /* parser_hpp */


struct Expr {
    std::string node;
    std::vector<Expr> leaves;
    Expr(std::string node);
    Expr(std::string node, Expr leaf);
    Expr(std::string inode, Expr leaf1, Expr leaf2);
};

class Parser {
public:
    Parser(const char* input);
    Expr parse();
    
private:
    const char* input;
    std::string parse_node();
    Expr parse_one_leaf();
    int get_priority(std::string node);
    Expr parse_two_leaves(int priority);
};

