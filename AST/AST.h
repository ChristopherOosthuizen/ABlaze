/* 
 * Created by Christopher Oosthuizen on 08/01/2021.
 * This files main purpose is to hold the structes defined
 * by context free grammer to create a abstract syntax tree
 *
 */

#ifndef ABLAZE_AST_H
#define ABLAZE_AST_H
#include "Token.h"
#include <string>
#include <vector>

class Expression{
public:
    virtual string name(){return "Expression";}
};

class Literal: public Expression{
    public:
        string name() override;
        Literal(Token* token);
        Token* m_token;
};

    
    
class BinOP: public Expression{
    public:
        string name() override;
        BinOP(Expression* left, Token* op, Expression* right);
        Token* m_op;
        Expression* m_left;
        Expression* m_right;

};

class Decleration: public Expression{

    public:
        string name() override;
        Decleration(Literal* type, Expression* name,Literal* op,Expression* value, bool initalize, bool m_isArray);
        Expression* m_name;
        Literal* m_op;
        Expression* m_value;
        Literal* m_type;
        bool m_initalize;
        bool m_isArray;
};

class Body: public Expression{
        public:
                string name() override;
                Body(Expression* control, vector<Expression*>* expressions);
                Expression* m_control;
                vector<Expression*>* m_lines;

};

class FunctionCall: public Expression{
        public:
                string name() override;
                FunctionCall(Literal* name,  vector<Expression*>* args);
                Literal* m_name;
                vector<Expression*>* m_args;
};

class Unary: public Expression{
        public:
                string name() override;
                Unary(Literal* op, Literal* iden, bool post);
                Literal* m_op;
                Literal* m_iden;
                bool m_post;
};

class IfStat: public Expression{
        public:
                string name() override;
                IfStat(Expression* control);
                Expression* m_control;
};

class WhileStat: public Expression{
        public:
                string name() override;
                WhileStat(Expression* control);
                Expression* m_control;
};

class ForStat: public Expression{
        public:
                string name() override;
                ForStat(Expression* initial, Expression* condition, Expression* repition);

                Expression* m_condition;
                Expression* m_repitition;
                Expression* m_initial;
};

class Function: public Expression{
        public:
                string name() override;
                Function(Literal* type, FunctionCall* call);
                Literal* m_type;
                FunctionCall* m_call;
};

class Return: public Expression{
        public:
                string name() override;
                Return(Expression* value);
                Expression* m_value;
};

class Import: public Expression{
        public:
                string name() override;
                Import(Expression* value);
                Expression* m_value;
};

class ArrayLiteral: public Expression{

        public:
                string name() override;
                ArrayLiteral(Literal* iden, Expression* value);
                Literal* m_iden;
                Expression* m_value;
};
#endif
