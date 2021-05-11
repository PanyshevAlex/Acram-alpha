//
//  makelatex.hpp
//  abbby
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
#endif /* makelatex_hpp */

std::string make_latex(Expr expr, Expr diffexpr);
std::string make_latex_expr(Expr expression);
std::string tex_one_leaves(Expr expression);
std::string tex_two_leaves(Expr expression);
