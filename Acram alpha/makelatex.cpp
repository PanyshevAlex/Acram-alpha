//
//  makelatex.cpp
//  Acram aplha
//
//  Created by Панышев Александр Сергеевич on 11.05.2021.
//  Copyright © 2021 Панышев Александр Сергеевич. All rights reserved.
//

#include "makelatex.hpp"

bool is_expression(Expr expr) {
    if (expr.leaves.size() == 2 || (expr.leaves.size() == 1 && (expr.node == "+" || expr.node == "-")))
        return true;
    return false;
}

mystring tex_one_leaves(Expr expression) {
    if (expression.node == "sin" || expression.node == "cos" || expression.node == "exp" || expression.node == "sqrt" || expression.node == "ln" || expression.node == "tan" || expression.node == "cot" || expression.node == "arcsin" || expression.node == "arccos" || expression.node == "arctan" || expression.node == "arcctg") {
        // sin(f(x)) -> "\\sin{(f(x))}"
        return mystring("\\") + expression.node + "{(" + make_latex_expr(expression.leaves[0]) + ")}";
    }
    if (expression.node == "+" || expression.node == "-") {
        // -f(x) -> -f(x) and (+f(x))' -> +f'(x)
        return expression.node + make_latex_expr(expression.leaves[0]);
    }
    return mystring("");
}

mystring tex_two_leaves(Expr expression) {
    if (expression.node == "+") {
        return make_latex_expr(expression.leaves[0]) + expression.node + make_latex_expr(expression.leaves[1]);
    }
    if (expression.node == "-") {
        if (is_expression(expression.leaves[1]))
            return make_latex_expr(expression.leaves[0]) + expression.node + "("+ make_latex_expr(expression.leaves[1]) + ")";
        return make_latex_expr(expression.leaves[0]) + expression.node + make_latex_expr(expression.leaves[1]);
    }
    if (expression.node == "*") {
        mystring mul_return = "";
        if ((expression.leaves[0].leaves.size() == 2) || expression.leaves[0].node == "-")
            mul_return = mul_return + "(" + make_latex_expr(expression.leaves[0]) + ")";
        else
            mul_return = mul_return + make_latex_expr(expression.leaves[0]);
        mul_return = mul_return + "\\cdot ";
        if (expression.leaves[1].leaves.size() == 2 || expression.leaves[1].node == "-")
            mul_return = mul_return + "(" + make_latex_expr(expression.leaves[1]) + ")";
        else
            mul_return = mul_return + make_latex_expr(expression.leaves[1]);
        return mul_return;
    }
    if (expression.node == "/") {
        return mystring("\\frac{") + make_latex_expr(expression.leaves[0]) + "}{" + make_latex_expr(expression.leaves[1]) + "} ";
    }
    if (expression.node == "^") {
        mystring left = make_latex_expr(expression.leaves[0]);
        mystring right = make_latex_expr(expression.leaves[1]);
        if (expression.leaves[0].leaves.size() == 0)
            return left + "^{" + right + "}";
        else
            return mystring("(") + left + ")" + "^{" + right + "}";
    }
    return mystring("");
}

mystring make_latex_expr(Expr expression) {
    switch (expression.leaves.size()) {
        case 0: {
            if (expression.node == "pi")
                return mystring("\\") + expression.node;
            return expression.node;
        }
        case 1: {
            return tex_one_leaves(expression);
        }
        case 2: {
            return tex_two_leaves(expression);
        }
    }
    return mystring("");
}

mystring make_latex(Expr expr, Expr diffexpr) {
    mystring expression =
        mystring("\\documentclass[a4paper,12pt]{article} "
        "\\usepackage[T2A]{fontenc}"
        "\\usepackage[utf8]{inputenc}"
        "\\usepackage[english,russian]{babel}"
        "\\usepackage{amsmath,amsfonts,amssymb,amsthm,mathtools}"
        "\\begin{document}"
        "$") + make_latex_expr(expr) + "\\rightarrow $"
        "$" + make_latex_expr(diffexpr) + "$"
        "\\end{document}";
    return expression;
}
