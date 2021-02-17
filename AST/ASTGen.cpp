// Created by Christopher Oosthuizen on 01/11/2021
// The AST generator is meant to return 
// A based off a list of tokens constructed by a lexer
#include "ASTGen.h"
#include "AST.h"
#include "ErrorThrower.h"


ASTGen::ASTGen(vector<Token*> tokens){
    m_tokens = tokens;
    m_pos = 0;
}

// return a AST based on the tokens given in
// the construction
Body* ASTGen::generateAST(){
        vector<Expression*>* lines = new vector<Expression*>;
        while(peek() != NULL&& peek()->m_type != TokenType::END){
                lines->push_back(expression(new Literal(next())));
                delete next();
        }
        return new Body(NULL,lines);
}



// check the token that is dist
// number ahead of the current token
Token* ASTGen::peek(int dist){
        if(m_pos+dist < m_tokens.size()){
                return m_tokens.at(m_pos+dist);
        }
        return NULL;
}

Token* ASTGen::peek(){
        return peek(0);
}


// return true if the type of token
// equals type 
bool ASTGen::equals(Token* token, TokenType type){
    if(token == NULL)
        return false; 
    return token->m_type == type;
}


// return true if 
// the token type is a 
// operators
bool ASTGen::isOP(Token* token){
        if(token ==nullptr)
                return false;
        switch(token->m_type){
                case TokenType::TIMES_TIMES:
                case TokenType::DIVIDE_DIVIDE:
                case TokenType::EQUAL_EQUAL:
                case TokenType::NOT_EQUAL:
                case TokenType::OR_OR:
                case TokenType::AND_AND:
                case TokenType::REMAND:
                case TokenType::DIVIDE:
                case TokenType::TIMES:
                case TokenType::GREATER:
                case TokenType::LESS:
                case TokenType::LESS_EQUAL:
                case TokenType::MORE_EQUAL:
                case TokenType::MINUS:
                case TokenType::PLUS:return true;

        }
        return false;
}

//returns weather a token is
// */ != and so on
bool ASTGen::isMulti(Token* token){
        if(token == nullptr)
                return false;
        switch(token->m_type){
                case TokenType::TIMES:
                case TokenType::REMAND:
                case TokenType::DIVIDE:
                case TokenType::TIMES_TIMES:
                case TokenType::DIVIDE_DIVIDE: 
                case TokenType::GREATER:
                case TokenType::LESS:
                case TokenType::LESS_EQUAL:
                case TokenType::MORE_EQUAL:
                 case TokenType::EQUAL_EQUAL:
                case TokenType::NOT_EQUAL: return true;
        }
        return false;
}


//retrun if the token is a equal
bool ASTGen::isEquals(Token* token){
        if(token == nullptr)
                return false;
        switch(token->m_type){
                case TokenType::PLUS_EQUAL:
                case TokenType::DIVIDE_EQUAL:
                case TokenType::TIMES_EQUAL:
                case TokenType::MINUS_EQUAL:
                case TokenType::MOD_EQUAL:
               case TokenType::EQUAL: return true;
        }
        return false;        
}

//return true if the token is a assignment closer
bool ASTGen::isCloser(Token* token){
        if(token == nullptr)
                return false;
        switch(token->m_type){
                case TokenType::COMMA:
                case TokenType::SEMI_COLON:
                case TokenType::CLOSE_BRACKET:
               case TokenType::CLOSE_PARENTHESE: return true;
        }
        return false;        
}

// return next token and if out of bounds return 
// null and increase m_pos
Token* ASTGen::next(){
   if(m_pos < m_tokens.size()){
        return m_tokens.at(m_pos++); 
    } 
    return NULL;
 
}


// Decide how a AST 
// should be constructed
Expression* ASTGen::expression(Expression* expr){
        if(isCloser(peek()))
                return expr;
        if(peek() !=nullptr){
                if(isOP(peek())) 
                        return binaryOperation(expr);
        }
        if(expr->name() == "ArrayLiteral"){
                if(isEquals(peek())){
                        return new Decleration(NULL,expr,new Literal(next()),expression(new Literal(next())),false,false);
                }
        }
        if(expr->name() == "BinOP"){
                if(!isOP(peek()))
                        return expr;
                return binaryOperation(expr);
        }
	if(isEquals(peek()))
		return decleration((Literal*)expr,false);
     
        switch(((Literal*)expr)->m_token->m_type){
                case TokenType::PRINT:
                case TokenType::PRINTLN:
                case TokenType::READFILE:
                case TokenType::WRITEFILE:
                case TokenType::INPUT: return functionCall((Literal*)expr);
                case TokenType::IMPORT: delete expr;
                                       return new Import(expression(new Literal(next()))); 
                case TokenType::RETURN:  delete expr;
                                        return new Return(expression(new Literal(next())));
                case TokenType::VOID: return body((Literal*)expr);
                case TokenType::IDEN_INT:
                case TokenType::IDEN_BOOL:
                case TokenType::IDEN_STRING:
                case TokenType::IDEN_DOUBLE:
                case TokenType::VAR: 
                        if(equals(peek(),TokenType::OPEN_BRACKET))
                                return expression(arrayLiteral((Literal*)expr));


                        if(equals(peek(1),TokenType::OPEN_PARENTHESE))
                                return body((Literal*)expr);

                        return decleration((Literal*)expr, true);
                case TokenType::OPEN_PARENTHESE: return expression(new Literal(next()));
                case TokenType::IDEN:
                                        if(equals(peek(),TokenType::PLUS_PLUS) || equals(peek(),TokenType::MINUS_MINUS))
                                                return expression(new Unary(new Literal(next()), (Literal*)expr,true)); 
                                      else if(equals(peek(),TokenType::OPEN_PARENTHESE))
                                              return functionCall((Literal*)expr);
                                       else if(equals(peek(),TokenType::OPEN_BRACKET))
                                                return expression(arrayLiteral((Literal*)expr));
                case TokenType::STRING:
                case TokenType::DOUBLE:
                case TokenType::BOOL:
                case TokenType::INT: return expr; 
                case TokenType::FOR:
                case TokenType::WHILE:
                case TokenType::ELSE:
                case TokenType::IF: return body((Literal*)expr);
                case TokenType::NOT: 
                case TokenType::PLUS_PLUS:
                case TokenType::MINUS_MINUS: return expression(new Unary((Literal*)expr,new Literal(next()),false));
            }
        return NULL; 
}


// Construct a binOP based on specifically equations
// such as +-,*
Expression* ASTGen::binaryOperation(Expression* left){

       Token* op = next();
        Literal* right = new Literal(next());

       if(equals(peek(), TokenType::SEMI_COLON)||equals(peek(), TokenType::OPEN_PARENTHESE))
                       return expression(new BinOP(left,op,expression(right)));


       if(equals(peek(),TokenType::CLOSE_PARENTHESE)){
                        delete next();
                       return expression(new BinOP(left,op,right)); 

        }
 
        else if(isMulti(op)){
               return expression(new BinOP(left,op,right)); 



        }

        else{
               if(!isMulti(peek()) )
                       return expression(new BinOP(left,op,right)); 

                return new BinOP(left,op,expression(right));
        }
}

// Construct dec constructs declarations such as
// var name = 12
Decleration* ASTGen::decleration(Literal* type, bool initalize){
    Literal* name ;
    bool isArray = false;
    if(equals(peek(),TokenType::COLON)){
        isArray = true;
        delete next();
    }
    if(!initalize)
            name= type;
    else{
        name=new Literal(next());
    }
    if(isCloser(peek())){
        return new Decleration(type, name,NULL, NULL,initalize,false);
    }

        Literal* op = new Literal(next());
    return new Decleration(type, name, op, expression(new Literal(next())),initalize,isArray);
}


// construct a fucntion call 
// object based on parmaters
FunctionCall* ASTGen::functionCall(Literal* name){
        delete next();
        vector<Expression*>* args = new vector<Expression*>;

        if(!equals(peek(),TokenType::CLOSE_PARENTHESE)){
                bool isComma = false;
               do{
                        args->push_back(expression(new Literal(next())));          
                        isComma = equals(peek(),TokenType::COMMA);
                        delete next();
               }while(isComma);
        }else{
                delete next();
        }
        return new FunctionCall(name,args);
}

//consturct a body based on
//paramters such as if while for and def
Body* ASTGen::body(Literal* type){
        Expression* bod; 
        Expression* initial =expression(new Literal(next())); 
        delete next();
        switch(type->m_token->m_type){
                case TokenType::VOID:
                case TokenType::IDEN_INT:
                case TokenType::IDEN_BOOL:
                case TokenType::IDEN_STRING:
                case TokenType::IDEN_DOUBLE:
 
                case TokenType::VAR: 
                        bod = new Function(type,(FunctionCall*)initial); break;
                case TokenType::IF: bod =new IfStat(initial); break;
                case TokenType::WHILE: bod = new WhileStat(initial); break;
                case TokenType::ELSE: bod = NULL; break;
                case TokenType::FOR: Expression* condition = expression(new Literal(next()));
                                     delete next();
                                     Expression* repitition= expression(new Literal(next()));
                                     delete next();
                                     bod = new ForStat(initial,condition,repitition);break;


        }
        if(equals(peek(),TokenType::OPEN_BRACE))
                delete next();
        vector<Expression*>* lines = new vector<Expression*>();
        while(!equals(peek(),TokenType::CLOSE_BRACE)){
               lines->push_back(expression(new Literal(next())));
               if(equals(peek(),TokenType::SEMI_COLON)){
                        delete next();
               }else{
                       ErrorThrower::missingSemiColon(peek()->m_line+1);
               }
        }
       return new Body(bod,lines); 

}

ArrayLiteral* ASTGen::arrayLiteral(Literal* name){
        delete next();
        Expression* value = expression(new Literal(next()));
        delete next();
        return new ArrayLiteral(name,value);
}
