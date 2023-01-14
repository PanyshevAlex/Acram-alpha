//
//  mystring.hpp
//  Acram alpha
//
//  Created by Панышев Александр Сергеевич on 12.05.2021.
//  Copyright © 2021 Панышев Александр Сергеевич. All rights reserved.
//

#ifndef mystring_hpp
#define mystring_hpp
#pragma once
#include <iostream>
#include <ostream>
#include <string.h>
#include <utility>
#include <cassert>
#endif /* mystring_hpp */

class mystring {
private:
    char *data_;
    int length_;
    
    
public:
    mystring();
        mystring(const char* str);
        mystring(const std::string);
        mystring(const mystring& other);
        mystring(mystring&& other);
        ~mystring();

        int length() const;
        bool is_empty() const;
        char* c_str() const;
        void push_back(const char& new_elem);
        char pop_back();
        char& operator [] (int num) const;

        mystring& operator = (mystring&& str);
        mystring& operator = (const char* str);
        mystring& operator = (char*&& str);
        mystring& operator = (const std::string& str);
        mystring& operator = (const mystring& str);

        mystring operator + (const std::string& str) const &;
        mystring operator + (const std::string& str) &&;
        mystring operator + (const mystring& other) const &;
        mystring operator + (const mystring& other) &&;
        mystring operator + (const char* str) const &;
        mystring operator + (const char* str) &&;


        
        bool operator == (const std::string& str) const;
        bool operator == (const mystring& str) const;
        bool operator == (const char* str) const;


        friend std::ostream& operator << (std::ostream& out, const mystring& string1);
};
