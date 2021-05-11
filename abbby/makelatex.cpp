//
//  makelatex.cpp
//  abbby
//
//  Created by Панышев Александр Сергеевич on 11.05.2021.
//  Copyright © 2021 Панышев Александр Сергеевич. All rights reserved.
//

#include "makelatex.hpp"

std::string tex_one_leaves(Expr expression) {
    if (expression.node == "sin" || expression.node == "cos") {
        // sin(f(x)) -> "\\sin{(f(x))}"
        return "\\" + expression.node + "{(" + make_latex_expr(expression.leaves[0]) + ")}";
    }
    if (expression.node == "+" || expression.node == "-") {
        // -f(x) -> -f(x) and (+f(x))' -> +f'(x)
        return expression.node + make_latex_expr(expression.leaves[0]);
    }
    return "";
}

std::string tex_two_leaves(Expr expression) {
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
    return "";
}

std::string make_latex_expr(Expr expression) {
    switch (expression.leaves.size()) {
        case 0: {
            return expression.node;
        }
        case 1: {
            return tex_one_leaves(expression);
        }
        case 2: {
            return tex_two_leaves(expression);
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
