//
//  makelatex.hpp
//  Acram aplha
//
//  Created by Панышев Александр Сергеевич on 11.05.2021.
//  Copyright © 2021 Панышев Александр Сергеевич. All rights reserved.
//

#ifndef makelatex_hpp
#define makelatex_hpp
#pragma once
#include <stdio.h>
#include <string>
#include "parser.hpp"
#include "mystring.hpp"
#endif /* makelatex_hpp */

bool is_expression(Expr expr);
mystring make_latex(Expr expr, Expr diffexpr);
mystring make_latex_expr(Expr expression);
mystring tex_one_leaves(Expr expression);
mystring tex_two_leaves(Expr expression);
