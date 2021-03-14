#include "gtest/gtest.h"
#include "AST.h"
#include "ASTGen.h"
#include "ErrorThrower.h"
#include "Lexer.h"
#include "ByteGen.h"


TEST(ByteCode,pluss){
        Lexer lexer("int main(){i++;}");
        ASTGen gen(lexer.readAllTokens());
        Body* body = gen.generateAST();
        ByteGen byt(body);
        vector<string>* strs = byt.generateByteCode();
        ASSERT_EQ(strs->size(),11);
        ASSERT_EQ(strs->at(0),"call"); 
        ASSERT_EQ(strs->at(1),"main"); 
        ASSERT_EQ(strs->at(2),"main:"); 
        ASSERT_EQ(strs->at(3),"load"); 
        ASSERT_EQ(strs->at(4),"i"); 
        ASSERT_EQ(strs->at(5),"push"); 
        ASSERT_EQ(strs->at(6),"1"); 
        ASSERT_EQ(strs->at(7),"add"); 
        ASSERT_EQ(strs->at(8),"asi"); 
        ASSERT_EQ(strs->at(9),"i"); 
        ASSERT_EQ(strs->at(10),"halt"); 
}


TEST(ByteCode,returns){
        Lexer lexer("int one(){return 1;} int main(){one();}");
        ASTGen gen(lexer.readAllTokens());
        Body* body = gen.generateAST();
        ByteGen byt(body);
        vector<string>* strs = byt.generateByteCode();
        ASSERT_EQ(strs->size(),11);
        ASSERT_EQ(strs->at(0),"call"); 
        ASSERT_EQ(strs->at(1),"main"); 
        ASSERT_EQ(strs->at(2),"one:"); 
        ASSERT_EQ(strs->at(3),"push"); 
        ASSERT_EQ(strs->at(4),"1"); 
        ASSERT_EQ(strs->at(5),"return"); 
        ASSERT_EQ(strs->at(6),"return"); 
        ASSERT_EQ(strs->at(7),"main:"); 
        ASSERT_EQ(strs->at(8),"call"); 
        ASSERT_EQ(strs->at(9),"one"); 
        ASSERT_EQ(strs->at(10),"halt"); 
}


TEST(ByteCode,equations){
        Lexer lexer("int main(){int i=12+8*2;}");
        ASTGen gen(lexer.readAllTokens());
        Body* body = gen.generateAST();
        ByteGen byt(body);
        vector<string>* strs = byt.generateByteCode();
        ASSERT_EQ(strs->size(),14);
        ASSERT_EQ(strs->at(0),"call"); 
        ASSERT_EQ(strs->at(1),"main"); 
        ASSERT_EQ(strs->at(2),"main:"); 
        ASSERT_EQ(strs->at(3),"push"); 
        ASSERT_EQ(strs->at(4),"12"); 
        ASSERT_EQ(strs->at(5),"push"); 
        ASSERT_EQ(strs->at(6),"8"); 
        ASSERT_EQ(strs->at(7),"push"); 
        ASSERT_EQ(strs->at(8),"2"); 
        ASSERT_EQ(strs->at(9),"times");
        ASSERT_EQ(strs->at(10),"add"); 
        ASSERT_EQ(strs->at(11),"store"); 
        ASSERT_EQ(strs->at(12),"i"); 
        ASSERT_EQ(strs->at(13),"halt"); 

}


TEST(ByteCode,equationsAdvanced){
        Lexer lexer("int main(){int i=12*3+6*8+8*3;}");
        ASTGen gen(lexer.readAllTokens());
        Body* body = gen.generateAST();
        ByteGen byt(body);
        vector<string>* strs = byt.generateByteCode();
        ASSERT_EQ(strs->size(),23);
        ASSERT_EQ(strs->at(0),"call"); 
        ASSERT_EQ(strs->at(1),"main"); 
        ASSERT_EQ(strs->at(2),"main:"); 
        ASSERT_EQ(strs->at(3),"push"); 
        ASSERT_EQ(strs->at(4),"12"); 
        ASSERT_EQ(strs->at(5),"push"); 
        ASSERT_EQ(strs->at(6),"3"); 
        ASSERT_EQ(strs->at(7),"times"); 
        ASSERT_EQ(strs->at(8),"push"); 
        ASSERT_EQ(strs->at(9),"6"); 
        ASSERT_EQ(strs->at(10),"push"); 
        ASSERT_EQ(strs->at(11),"8"); 
        ASSERT_EQ(strs->at(12),"times"); 
        ASSERT_EQ(strs->at(13),"add"); 
        ASSERT_EQ(strs->at(14),"push"); 
        ASSERT_EQ(strs->at(15),"8"); 
        ASSERT_EQ(strs->at(16),"push"); 
        ASSERT_EQ(strs->at(17),"3"); 
        ASSERT_EQ(strs->at(18),"times"); 
        ASSERT_EQ(strs->at(19),"add"); 
        ASSERT_EQ(strs->at(20),"store"); 
        ASSERT_EQ(strs->at(21),"i"); 
        ASSERT_EQ(strs->at(22),"halt"); 
        
}


TEST(ByteCode,print){
        Lexer lexer("int main(){print(6*2*3);}");
        ASTGen gen(lexer.readAllTokens());
        Body* body = gen.generateAST();
        ByteGen byt(body);
        vector<string>* strs = byt.generateByteCode();
        ASSERT_EQ(strs->size(),13);
        ASSERT_EQ(strs->at(0),"call"); 
        ASSERT_EQ(strs->at(1),"main"); 
        ASSERT_EQ(strs->at(2),"main:"); 
        ASSERT_EQ(strs->at(3),"push"); 
        ASSERT_EQ(strs->at(4),"6"); 
        ASSERT_EQ(strs->at(5),"push"); 
        ASSERT_EQ(strs->at(6),"2"); 
        ASSERT_EQ(strs->at(7),"times"); 
        ASSERT_EQ(strs->at(8),"push"); 
        ASSERT_EQ(strs->at(9),"3"); 
        ASSERT_EQ(strs->at(10),"times"); 
        ASSERT_EQ(strs->at(11),"print"); 
        ASSERT_EQ(strs->at(12),"halt"); 

}

TEST(ByteCode,equals){
        Lexer lexer("int main(){if(1 ==1){print(\"yes\");}}");
        ASTGen gen(lexer.readAllTokens());
        Body* body = gen.generateAST();
        ByteGen byt(body);
        vector<string>* strs = byt.generateByteCode();
}



TEST(ByteCode,ifs){
        Lexer lexer("int main(){int i=12; int o =13; int right = i < o; if( right ){ print( i ); } else { print(o); }}");
        vector<Token*> tokens = lexer.readAllTokens();
        ASTGen gen(tokens);
        Body* body = gen.generateAST();
        ByteGen byt(body);
        vector<string>* strs = byt.generateByteCode();
        ASSERT_EQ(strs->size(),35);
        ASSERT_EQ(strs->at(0),"call"); 
        ASSERT_EQ(strs->at(1),"main"); 
        ASSERT_EQ(strs->at(2),"main:"); 
        ASSERT_EQ(strs->at(3),"push"); 
        ASSERT_EQ(strs->at(4),"12"); 
        ASSERT_EQ(strs->at(5),"store"); 
        ASSERT_EQ(strs->at(6),"i"); 
        ASSERT_EQ(strs->at(7),"push"); 
        ASSERT_EQ(strs->at(8),"13"); 
        ASSERT_EQ(strs->at(9),"store"); 
        ASSERT_EQ(strs->at(10),"o"); 
        ASSERT_EQ(strs->at(11),"load"); 
        ASSERT_EQ(strs->at(12),"i"); 
        ASSERT_EQ(strs->at(13),"load"); 
        ASSERT_EQ(strs->at(14),"o"); 
        ASSERT_EQ(strs->at(15),"islt"); 
        ASSERT_EQ(strs->at(16),"store"); 
        ASSERT_EQ(strs->at(17),"right"); 
        ASSERT_EQ(strs->at(18),"startlocal");
        ASSERT_EQ(strs->at(19),"load"); 
        ASSERT_EQ(strs->at(20),"right"); 
        ASSERT_EQ(strs->at(21),"jif"); 
        ASSERT_EQ(strs->at(22),"startif18"); 
        ASSERT_EQ(strs->at(23),"load"); 
        ASSERT_EQ(strs->at(24),"o"); 
        ASSERT_EQ(strs->at(25),"print"); 
        ASSERT_EQ(strs->at(26),"jmp"); 
        ASSERT_EQ(strs->at(27),"endif18"); 
        ASSERT_EQ(strs->at(28),"startif18:"); 
        ASSERT_EQ(strs->at(29),"load"); 
        ASSERT_EQ(strs->at(30),"i"); 
        ASSERT_EQ(strs->at(31),"print"); 
        ASSERT_EQ(strs->at(32),"endif18:"); 
        ASSERT_EQ(strs->at(33),"poplocal");
        ASSERT_EQ(strs->at(34),"halt"); 
}

TEST(ByteCode,whiles){
        Lexer lexer("int main(){int i=0; while(i<12){i =i+1; print(i);} }");
        vector<Token*> tokens = lexer.readAllTokens();
        ASTGen gen(tokens);
        Body* body = gen.generateAST();
        ByteGen byt(body);
        vector<string>* strs = byt.generateByteCode();
        ASSERT_EQ(strs->size(),34);
        ASSERT_EQ(strs->at(0),"call"); 
        ASSERT_EQ(strs->at(1),"main"); 
        ASSERT_EQ(strs->at(2),"main:"); 
        ASSERT_EQ(strs->at(3),"push"); 
        ASSERT_EQ(strs->at(4),"0"); 
        ASSERT_EQ(strs->at(5),"store"); 
        ASSERT_EQ(strs->at(6),"i"); 
        ASSERT_EQ(strs->at(7),"startWhile7:"); 
        ASSERT_EQ(strs->at(8),"startlocal"); 
        ASSERT_EQ(strs->at(9),"load"); 
        ASSERT_EQ(strs->at(10),"i"); 
        ASSERT_EQ(strs->at(11),"push"); 
        ASSERT_EQ(strs->at(12),"12"); 
        ASSERT_EQ(strs->at(13),"islt"); 
        ASSERT_EQ(strs->at(14),"not"); 
        ASSERT_EQ(strs->at(15),"startlocal"); 
        ASSERT_EQ(strs->at(16),"jif"); 
        ASSERT_EQ(strs->at(17),"endWhile7"); 
        ASSERT_EQ(strs->at(18),"load"); 
        ASSERT_EQ(strs->at(19),"i"); 
        ASSERT_EQ(strs->at(20),"push"); 
        ASSERT_EQ(strs->at(21),"1"); 
        ASSERT_EQ(strs->at(22),"add"); 
        ASSERT_EQ(strs->at(23),"asi"); 
        ASSERT_EQ(strs->at(24),"i"); 
        ASSERT_EQ(strs->at(25),"load"); 
        ASSERT_EQ(strs->at(26),"i"); 
        ASSERT_EQ(strs->at(27),"print"); 
        ASSERT_EQ(strs->at(28),"poplocal"); 
        ASSERT_EQ(strs->at(29),"jmp"); 
        ASSERT_EQ(strs->at(30),"startWhile7"); 
        ASSERT_EQ(strs->at(31),"endWhile7:"); 
        ASSERT_EQ(strs->at(32),"poplocal"); 
        ASSERT_EQ(strs->at(33),"halt"); 
}


TEST(ByteCode,fors){
        Lexer lexer("int main(){for(int i=0; i<12; i= i+1){print(i);} }");
        vector<Token*> tokens = lexer.readAllTokens();
        ASTGen gen(tokens);
        Body* body = gen.generateAST();
        ByteGen byt(body);
        vector<string>* strs = byt.generateByteCode();
        ASSERT_EQ(strs->size(),34);
        ASSERT_EQ(strs->at(0),"call"); 
        ASSERT_EQ(strs->at(1),"main"); 
        ASSERT_EQ(strs->at(2),"main:"); 
        ASSERT_EQ(strs->at(3),"startlocal"); 
        ASSERT_EQ(strs->at(4),"push"); 
        ASSERT_EQ(strs->at(5),"0"); 
        ASSERT_EQ(strs->at(6),"store"); 
        ASSERT_EQ(strs->at(7),"i"); 
        ASSERT_EQ(strs->at(8),"startFor3:"); 
        ASSERT_EQ(strs->at(9),"load"); 
        ASSERT_EQ(strs->at(10),"i"); 
        ASSERT_EQ(strs->at(11),"push"); 
        ASSERT_EQ(strs->at(12),"12"); 
        ASSERT_EQ(strs->at(13),"islt"); 
        ASSERT_EQ(strs->at(14),"not"); 
        ASSERT_EQ(strs->at(15),"startlocal"); 
        ASSERT_EQ(strs->at(16),"jif"); 
        ASSERT_EQ(strs->at(17),"endFor3"); 
        ASSERT_EQ(strs->at(18),"load"); 
        ASSERT_EQ(strs->at(19),"i"); 
        ASSERT_EQ(strs->at(20),"print"); 
        ASSERT_EQ(strs->at(21),"load"); 
        ASSERT_EQ(strs->at(22),"i"); 
        ASSERT_EQ(strs->at(23),"push"); 
        ASSERT_EQ(strs->at(24),"1"); 
        ASSERT_EQ(strs->at(25),"add"); 
        ASSERT_EQ(strs->at(26),"asi"); 
        ASSERT_EQ(strs->at(27),"i"); 
        ASSERT_EQ(strs->at(28),"poplocal"); 
        ASSERT_EQ(strs->at(29),"jmp"); 
        ASSERT_EQ(strs->at(30),"startFor3"); 
        ASSERT_EQ(strs->at(31),"endFor3:"); 
        ASSERT_EQ(strs->at(32),"poplocal"); 
        ASSERT_EQ(strs->at(33),"halt"); 
}

TEST(ByteCode,functions){
        Lexer lexer("void  runner(int i){print(i);} int main(){ int o = 12; runner(o);}");
        vector<Token*> tokens = lexer.readAllTokens();
        ASTGen gen(tokens);
        Body* body = gen.generateAST();
        ByteGen byt(body);
        vector<string>* strs = byt.generateByteCode();
        ASSERT_EQ(strs->size(),19);
        ASSERT_EQ(strs->at(0),"call"); 
        ASSERT_EQ(strs->at(1),"main"); 
        ASSERT_EQ(strs->at(2),"runner:"); 
        ASSERT_EQ(strs->at(3),"store"); 
        ASSERT_EQ(strs->at(4),"i"); 
        ASSERT_EQ(strs->at(5),"load"); 
        ASSERT_EQ(strs->at(6),"i"); 
        ASSERT_EQ(strs->at(7),"print"); 
        ASSERT_EQ(strs->at(8),"return"); 
        ASSERT_EQ(strs->at(9),"main:"); 
        ASSERT_EQ(strs->at(10),"push"); 
        ASSERT_EQ(strs->at(11),"12"); 
        ASSERT_EQ(strs->at(12),"store"); 
        ASSERT_EQ(strs->at(13),"o"); 
        ASSERT_EQ(strs->at(14),"load"); 
        ASSERT_EQ(strs->at(15),"o"); 
        ASSERT_EQ(strs->at(16),"call"); 
        ASSERT_EQ(strs->at(17),"runner"); 
        ASSERT_EQ(strs->at(18),"halt"); 
}

TEST(ByteCode,arrays){
        Lexer lexer("int main(){int: i = int[]; append(i,12); i[0]; delete(i,0);}");
        vector<Token*> tokens = lexer.readAllTokens();
        ASTGen gen(tokens);
        Body* body = gen.generateAST();
        ByteGen byt(body);
        vector<string>* strs = byt.generateByteCode();
        ASSERT_EQ(strs->size(),23);
        int i=0;
        ASSERT_EQ(strs->at(i++),"call"); 
        ASSERT_EQ(strs->at(i++),"main"); 
        ASSERT_EQ(strs->at(i++),"main:"); 
        ASSERT_EQ(strs->at(i++),"new");
        ASSERT_EQ(strs->at(i++),"list");
        ASSERT_EQ(strs->at(i++),"store");
        ASSERT_EQ(strs->at(i++),"i");
        ASSERT_EQ(strs->at(i++),"load");
        ASSERT_EQ(strs->at(i++),"i");
        ASSERT_EQ(strs->at(i++),"push");
        ASSERT_EQ(strs->at(i++),"12");
        ASSERT_EQ(strs->at(i++),"append");
        ASSERT_EQ(strs->at(i++),"load");
        ASSERT_EQ(strs->at(i++),"i");
        ASSERT_EQ(strs->at(i++),"push");
        ASSERT_EQ(strs->at(i++),"0");
        ASSERT_EQ(strs->at(i++),"at");
        ASSERT_EQ(strs->at(i++),"load");
        ASSERT_EQ(strs->at(i++),"i");
        ASSERT_EQ(strs->at(i++),"push");
        ASSERT_EQ(strs->at(i++),"0");
        ASSERT_EQ(strs->at(i++),"delete");
}

TEST(ByteCode,arrayss){
        Lexer lexer("int main(){ int: array = int[]; append(array,12);append(array,13)append(array,19);for(int i=0; i<3;i++){ print(array[i]);} } ");
        vector<Token*> tokens = lexer.readAllTokens();
        ASTGen gen(tokens);
        Body* body = gen.generateAST();
        ByteGen byt(body);
        vector<string>* strs = byt.generateByteCode();
}

TEST(ByteCode,casts){
        Lexer lexer("(int)12.0;");
        vector<Token*> tokens = lexer.readAllTokens();
        ASTGen gen(tokens);
        Body* body = gen.generateAST();
        ByteGen byt(body);
        vector<string>* strs = byt.generateByteCode();
        ASSERT_EQ(strs->size(),6);
        ASSERT_EQ(strs->at(0),"call"); 
        ASSERT_EQ(strs->at(1),"main"); 
        ASSERT_EQ(strs->at(2),"push"); 
        ASSERT_EQ(strs->at(3),"12.0"); 
        ASSERT_EQ(strs->at(4),"cast"); 
        ASSERT_EQ(strs->at(5),"int"); 
 
}

TEST(ByteCode,min){
        Lexer lexer("var min(var a, var b){ if(a>b) return a; return b;}");
        vector<Token*> tokens = lexer.readAllTokens();
        ASTGen gen(tokens);
        Body* body = gen.generateAST();
        ByteGen byt(body);
        vector<string>* strs = byt.generateByteCode();
        ASSERT_EQ(strs->size(),27);
        int i =0;
        ASSERT_EQ(strs->at(i++),"call"); 
        ASSERT_EQ(strs->at(i++),"main"); 
        ASSERT_EQ(strs->at(i++),"min:"); 
        ASSERT_EQ(strs->at(i++),"store"); 
        ASSERT_EQ(strs->at(i++),"b"); 
        ASSERT_EQ(strs->at(i++),"store"); 
        ASSERT_EQ(strs->at(i++),"a"); 
        i+= 16;
        ASSERT_EQ(strs->at(i++),"load"); 
        ASSERT_EQ(strs->at(i++),"b"); 
        ASSERT_EQ(strs->at(i++),"return"); 
 
}

TEST(ByteCode,extends){
        Lexer lexer("struct Pos{int x; int y;} struct Place extends Pos{string name;}");
        vector<Token*> tokens = lexer.readAllTokens();
        ASTGen gen(tokens);
        Body* body = gen.generateAST();
        ByteGen byt(body);
        vector<string>* strs = byt.generateByteCode();
        int i =0;
        ASSERT_EQ(strs->at(i++),"startlocal"); 
        ASSERT_EQ(strs->at(i++),"store"); 
        ASSERT_EQ(strs->at(i++),"x"); 
        ASSERT_EQ(strs->at(i++),"store"); 
        ASSERT_EQ(strs->at(i++),"y"); 
        ASSERT_EQ(strs->at(i++),"structdec"); 
        ASSERT_EQ(strs->at(i++),"Pos"); 
        ASSERT_EQ(strs->at(i++),"startlocal"); 

        ASSERT_EQ(strs->at(i++),"store"); 
        ASSERT_EQ(strs->at(i++),"name"); 
        ASSERT_EQ(strs->at(i++),"structdecEx"); 
        ASSERT_EQ(strs->at(i++),"Place"); 
        ASSERT_EQ(strs->at(i++),"Pos"); 
 
}




TEST(ByteCode,structs){
        Lexer lexer("struct Pos{int x; int y;} int main(){Pos position = new Pos; position.x =12; position.y = 14; print(position.x+position.y); }");
        vector<Token*> tokens = lexer.readAllTokens();
        ASTGen gen(tokens);
        Body* body = gen.generateAST();
        ASSERT_EQ(body->m_lines->size() , 2);
        ASSERT_EQ(body->m_lines->at(0)->name() , "Body");
        ASSERT_EQ(body->m_lines->at(1)->name() , "Body");
        ByteGen byt(body);
        vector<string>* strs = byt.generateByteCode();
        //ASSERT_EQ(strs->size(),33);
        int i =0;
        ASSERT_EQ(strs->at(i++),"startlocal"); 
        ASSERT_EQ(strs->at(i++),"store"); 
        ASSERT_EQ(strs->at(i++),"x"); 
        ASSERT_EQ(strs->at(i++),"store"); 
        ASSERT_EQ(strs->at(i++),"y"); 
        ASSERT_EQ(strs->at(i++),"structdec"); 
        ASSERT_EQ(strs->at(i++),"Pos"); 
        ASSERT_EQ(strs->at(i++),"call"); 
        ASSERT_EQ(strs->at(i++),"main");
        ASSERT_EQ(strs->at(i++),"main:");
        ASSERT_EQ(strs->at(i++),"new");
        ASSERT_EQ(strs->at(i++),"Pos");
        ASSERT_EQ(strs->at(i++),"store");
        ASSERT_EQ(strs->at(i++),"position");
        ASSERT_EQ(strs->at(i++),"push");
        ASSERT_EQ(strs->at(i++),"12");
        ASSERT_EQ(strs->at(i++),"load");
        ASSERT_EQ(strs->at(i++),"position");
        ASSERT_EQ(strs->at(i++),"set");
        ASSERT_EQ(strs->at(i++),"x");
        ASSERT_EQ(strs->at(i++),"push");
        ASSERT_EQ(strs->at(i++),"14");
        ASSERT_EQ(strs->at(i++),"load");
        ASSERT_EQ(strs->at(i++),"position");
        ASSERT_EQ(strs->at(i++),"set");
        ASSERT_EQ(strs->at(i++),"y");
        ASSERT_EQ(strs->at(i++),"load");
        ASSERT_EQ(strs->at(i++),"position");
        ASSERT_EQ(strs->at(i++),"select");
        ASSERT_EQ(strs->at(i++),"x");
        ASSERT_EQ(strs->at(i++),"load");
        ASSERT_EQ(strs->at(i++),"position");
        ASSERT_EQ(strs->at(i++),"select");
        ASSERT_EQ(strs->at(i++),"y");
        ASSERT_EQ(strs->at(i++),"add");
        ASSERT_EQ(strs->at(i++),"print");
        ASSERT_EQ(strs->at(i++),"halt");

}


TEST(ByteCode,classes){
        Lexer lexer("struct Pos{int x; int y; void Pos(int x, int y){this.x = x; this.y = y;} void xup(){x++;} } int main(){Pos position = new Pos(12,14);  position.xup();  }");
        vector<Token*> tokens = lexer.readAllTokens();
        ASTGen gen(tokens);
        Body* body = gen.generateAST();
        ASSERT_EQ(body->m_lines->size() , 2);
        ASSERT_EQ(body->m_lines->at(0)->name() , "Body");
        ASSERT_EQ(body->m_lines->at(1)->name() , "Body");
        ByteGen byt(body);
        vector<string>* strs = byt.generateByteCode();
        //ASSERT_EQ(strs->size(),33);
        int i =0;
        ASSERT_EQ(strs->at(i++),"startlocal"); 
        ASSERT_EQ(strs->at(i++),"store"); 
        ASSERT_EQ(strs->at(i++),"x"); 
        ASSERT_EQ(strs->at(i++),"store"); 
        ASSERT_EQ(strs->at(i++),"y"); 
        ASSERT_EQ(strs->at(i++),"functionPush"); 
        ASSERT_EQ(strs->at(i++),"Pos"); 
        ASSERT_EQ(strs->at(i++),"functionPush"); 
        ASSERT_EQ(strs->at(i++),"xup"); 
        ASSERT_EQ(strs->at(i++),"structdec"); 
        ASSERT_EQ(strs->at(i++),"Pos"); 
        ASSERT_EQ(strs->at(i++),"call"); 
        ASSERT_EQ(strs->at(i++),"main");

        ASSERT_EQ(strs->at(i++),"Pos:");
        ASSERT_EQ(strs->at(i++),"loadclass");
        ASSERT_EQ(strs->at(i++),"store");
        ASSERT_EQ(strs->at(i++),"this");
        ASSERT_EQ(strs->at(i++),"store");
        ASSERT_EQ(strs->at(i++),"y");
        ASSERT_EQ(strs->at(i++),"store");
        ASSERT_EQ(strs->at(i++),"x");
        ASSERT_EQ(strs->at(i++),"load");
        ASSERT_EQ(strs->at(i++),"x");
        ASSERT_EQ(strs->at(i++),"load");
        ASSERT_EQ(strs->at(i++),"this");
        ASSERT_EQ(strs->at(i++),"set");
        ASSERT_EQ(strs->at(i++),"x");
        ASSERT_EQ(strs->at(i++),"load");
        ASSERT_EQ(strs->at(i++),"y");
        ASSERT_EQ(strs->at(i++),"load");
        ASSERT_EQ(strs->at(i++),"this");
        ASSERT_EQ(strs->at(i++),"set");
        ASSERT_EQ(strs->at(i++),"y");
        ASSERT_EQ(strs->at(i++),"load");
        ASSERT_EQ(strs->at(i++),"this");
        ASSERT_EQ(strs->at(i++),"return");

        ASSERT_EQ(strs->at(i++),"Pos.xup:");
        ASSERT_EQ(strs->at(i++),"loadclass");
        ASSERT_EQ(strs->at(i++),"store");
        ASSERT_EQ(strs->at(i++),"this");
        ASSERT_EQ(strs->at(i++),"load");
        ASSERT_EQ(strs->at(i++),"x");
        ASSERT_EQ(strs->at(i++),"push");
        ASSERT_EQ(strs->at(i++),"1");
        ASSERT_EQ(strs->at(i++),"add");
        ASSERT_EQ(strs->at(i++),"asi");
        ASSERT_EQ(strs->at(i++),"x");
        ASSERT_EQ(strs->at(i++),"return");


        ASSERT_EQ(strs->at(i++),"main:");
        ASSERT_EQ(strs->at(i++),"push");
        ASSERT_EQ(strs->at(i++),"12");
        ASSERT_EQ(strs->at(i++),"push");
        ASSERT_EQ(strs->at(i++),"14");
        ASSERT_EQ(strs->at(i++),"new");
        ASSERT_EQ(strs->at(i++),"Pos");
        ASSERT_EQ(strs->at(i++),"store");
        ASSERT_EQ(strs->at(i++),"position");
        ASSERT_EQ(strs->at(i++),"load");
        ASSERT_EQ(strs->at(i++),"position");
        ASSERT_EQ(strs->at(i++),"classcall");
        ASSERT_EQ(strs->at(i++),"xup");


}