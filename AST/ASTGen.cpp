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

//The inital function to kick of the generation that returns the program as a ast
Body* ASTGen::generateAST(){
        vector<Expression*>* exprs = new vector<Expression*>();
        while(m_pos < m_tokens.size() && peek()!= nullptr && !peek()->m_symbol.empty()){
                exprs->push_back(lineStat());
        }
        return new Body(nullptr, exprs);
}
//return weather the next is a token type
bool ASTGen::equals(TokenType type){
        return peek()->m_type == type;
}

//return weather the next token equals the type and if so delete next
bool ASTGen::eat(TokenType type){
        if(equals(type)){
                delete next();
                return 1;
        }
        return 0;
}

//return the next token without incremanting pos
Token* ASTGen::peek(){
        Token* token = next();
        m_pos--;
        return token;
}

//return the next token as a literal
Literal* ASTGen::nextLit(){
        return new Literal(next());
}

//return next token and increase pos
Token* ASTGen::next(){
        if(m_pos >= m_tokens.size()){
                return nullptr;
        }
        return m_tokens[m_pos++];
}


//checks to see if something m_pos+i is a type
bool ASTGen::equalsForward(int i,TokenType type){
        return m_tokens.size() >m_pos+i && m_tokens[m_pos+i]->m_type == type;
}

//eat the next token and if its not right throw error
void ASTGen::consume( TokenType type,const string& message ){
        if(!eat(type))
                ErrorThrower::unNamedError(message,peek()->m_line);
}

//convert a body token into a body inclusing constrol ststement
Expression* ASTGen::body(){
       TokenType type = peek()->m_type;
       Literal* lit =nextLit(); 
       Expression* control= nullptr; 
       if(type == TokenType::ELSE){
               control = new ElseStat();
       }else if(type == TokenType::IF){
               control = new IfStat(expression());
       }else if(type == TokenType::FOR){
               consume(TokenType::OPEN_PARENTHESE,"missing parenthese for for");
               Expression* inital = lineStat();
               Expression* controlStat = expression();
               consume(TokenType::SEMI_COLON,"Missing SemiColon for for");
               Expression* rep = expression();
               consume(TokenType::CLOSE_PARENTHESE,"Missing close parenthese");
               control = new ForStat(inital,controlStat,rep);
       }else if(type == TokenType::WHILE){
               control = new WhileStat(expression()); 
       }else if(type == TokenType::STRUCT){
               Literal* name = nextLit();
               bool extends= false;
               Literal* extender = nullptr; 
               if(eat(TokenType::EXTENDS)){
                       extends = true;
                       extender =nextLit();
               }
               control = new Struct(name,extends,extender);
       }else if(type == TokenType::CLASS){
               control = new Class(nextLit());
       }else if(isIden(lit->m_token)){
                control = new Function(lit,(FunctionCall*)functionCall());
       }
       return new Body(control,lines());
}

//return lines for a function such as {} or single line
vector<Expression*>* ASTGen::lines(){
        vector<Expression*>* exprs = new vector<Expression*>();
        if(eat(TokenType::OPEN_BRACE)){
                while(!equals(TokenType::CLOSE_BRACE)){
                       exprs->push_back(lineStat());
                }
                consume(TokenType::CLOSE_BRACE,"UNclosed body ");
        }else{
                exprs->push_back(lineStat());
        }
        return exprs;
}

//return the order of operations for a given token
int ASTGen::order(Token* token){
        if(token == NULL)
                return -1;
        switch(token->m_type){
               case TokenType::XOR:
                case TokenType::AND_AND:
                case TokenType::OR_OR: return 5;
                case TokenType::LESS_EQUAL:
                case TokenType::MORE_EQUAL:
                case TokenType::GREATER:
                case TokenType::LESS:
                case TokenType::EQUAL_EQUAL:
                case TokenType::NOT_EQUAL:return 4;
                case TokenType::PLUS:
                case TokenType::MINUS:return 3;
                case TokenType::TIMES: 
                case TokenType::REMAND:
                case TokenType::DIVIDE:return 2;
                case TokenType::TIMES_TIMES: 
                case TokenType::DIVIDE_DIVIDE:return 1;
                case TokenType::MINUS_MINUS:
                case TokenType::NOT:
                case TokenType::PLUS_PLUS:return 0;
        }
        return -1;
}

//return weather the token is a type iden
bool ASTGen::isIden(Token* token){
        switch(token->m_type){
                case TokenType::VAR:
                case TokenType::IDEN_DOUBLE:
                case TokenType::IDEN_CHAR:
                case TokenType::IDEN_STRING:
                case TokenType::VOID:
                case TokenType::IDEN_INT:
                case TokenType::IDEN_BOOL:return 1;
        }
        return 0;
}

//return weather the next is a contorl type
bool ASTGen::isBod(){
        switch(peek()->m_type){
                case TokenType::FOR:
                case TokenType::IF:
                case TokenType::WHILE:
                case TokenType::ELSE:
                case TokenType::CLASS:
                case TokenType::STRUCT:return 1;
        }
        return 0;

}

//return the next ststment while 
Expression* ASTGen::lineExpr(){
        Expression* expr = nullptr;
        bool isDec =isIden(peek()) || (equalsForward(1,TokenType::COLON) || (equals(TokenType::IDEN)&& equalsForward(1,TokenType::IDEN))); 
        if( isBod() ||(isDec && equalsForward(2,TokenType::OPEN_PARENTHESE))){
                return body();
        }else if(isDec){
               expr = decleration(); 
        }else 
                 expr = expression();
        return expr;
 
}

//return the next while assuming the next is a semicolon
Expression* ASTGen::lineStat(){
        Expression* expr = lineExpr();
        if(expr->name() !="Body")
                consume(TokenType::SEMI_COLON, "Missing Semi Colon");
        return expr;
}

//return weather the next is a equals token
bool ASTGen::isEquals(){
        switch(peek()->m_type){
                case TokenType::EQUAL:
                case TokenType::TIMES_EQUAL:
                case TokenType::DIVIDE_EQUAL:
                case TokenType::PLUS_EQUAL:
                case TokenType::MINUS_EQUAL: return 1;
        }
        return 0;
}

//return the next general ststement
Expression* ASTGen::expression(){

        Expression* expression = binaryOperation(6); 
        if(isEquals()){
                expression = assignment(expression);
        }else if(equals(TokenType::PLUS_PLUS) || equals(TokenType::MINUS_MINUS)){
                expression = new Unary(nextLit(), expression,true);
        }

        return expression; 
}

//Convert decleration tokens into decleration
Expression* ASTGen::decleration(){
       Literal* type = new Literal(next());
       bool isArr = eat(TokenType::COLON); 
       if(!equals(TokenType::IDEN))
                ErrorThrower::illgalIdentifier(peek()->m_line,peek()->m_symbol);
       Expression* name = literal();
       Expression* value = nullptr;
       if(eat(TokenType::EQUAL)){
                value = expression();
       }
       return new Decleration(type,name,nullptr,value,true,isArr);
}

//return a asseminet expression
Expression* ASTGen::assignment(Expression* name){
       Literal* op =nextLit(); 
       Expression* value = expression();
       return new Decleration(nullptr,name,op,value,false,false);
}

//rescursavly go down to keep pemdas in a binary operation
Expression* ASTGen::binaryOperation(int precidence){
        
        if(precidence <= 0){
                return unary();
        } 
        Expression* expr = binaryOperation(precidence-1);

        while(order(peek()) == precidence){
                Token* op= next();
                Expression* right = binaryOperation(precidence-1);
                expr = new BinOP(expr,op,right);
        }
        return expr;
}

Expression* ASTGen::unary(){

        if(order(peek()) == 0 || equals(TokenType::MINUS)){
                Token* op = next();
                Expression* right = unary();               
                return new Unary(new Literal(op), right,0);
        }
        Expression* lit = literal();
        if(eat(TokenType::DOT)){
                Expression* sub= unary(); 
                return new Dot(lit,sub);
        }
        return lit;
}

Expression* ASTGen::parans(){
        Expression* expr = expression();
        consume(TokenType::CLOSE_PARENTHESE,"Unclosed ");
        return expr; 

}

Expression* ASTGen::functionCall(){
        Literal* lit = nextLit();
        consume(TokenType::OPEN_PARENTHESE,"Exprected open parenthese");
        vector<Expression*>* args = new vector<Expression*>();
        while(!equals(TokenType::CLOSE_PARENTHESE)){
                args->push_back(lineExpr());
                eat(TokenType::COMMA);
        }
        consume(TokenType::CLOSE_PARENTHESE,"Error: Unclosed function call on line:");
        return new FunctionCall(lit,args);
}

//return weather a token is a builtin function
bool ASTGen::isFunc(){
        switch(peek()->m_type){
                case TokenType::IMPORT:
                case TokenType::RETURN:
                case TokenType::NEW:
                case TokenType::LEN:
                case TokenType::PRINTLN: 
                case TokenType::PRINT: return 1;
        }
        return 0;
}

Expression* ASTGen::literal(){
        if(eat(TokenType::OPEN_PARENTHESE)){
                Expression* expr = parans();
                if( expr->name() == "Literal" && isIden(((Literal*)expr)->m_token) ){
                       expr = new Cast((Literal*)expr,unary()); 

                }
                return expr;
        }
        if(equals(TokenType::IDEN) &&equalsForward(1,TokenType::OPEN_PARENTHESE)){
                return functionCall();
        }else if(isFunc()){
                return builtIn();
        }
        
        Literal* literal = nextLit();
        
        if (eat(TokenType::OPEN_BRACKET)) {
                Expression* value = nullptr;
                if(!equals(TokenType::CLOSE_BRACKET)){
                        value = expression();
                }
                consume(TokenType::CLOSE_BRACKET," Unclosed array ");
                return new ArrayLiteral(literal,value);
        }
        return literal; 
}

Expression* ASTGen::builtIn(){
        Literal* lit = nextLit();
        if(equals(TokenType::SEMI_COLON)){
                return new BuiltIn(lit,nullptr);
        }
        return new BuiltIn(lit,expression());
}
