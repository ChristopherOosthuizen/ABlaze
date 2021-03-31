
#include "gtest/gtest.h"
#include "Vm.h"
#include "ByteLexer.h"

TEST(VM,PLUS){
        ByteLexer lexer("push 120 push 6 add halt");
        vector<ByteToken*> tokens = lexer.readAllTokens();

       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(vm.m_stack[vm.m_stack.size()-1].m_val.m_int,126);

}

TEST(VM,strings){
        ByteLexer lexer("push \"hello\" push \" there\" add halt");
        vector<ByteToken*> tokens = lexer.readAllTokens();

       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(vm.m_stack[vm.m_stack.size()-1].m_type,ByteType::STRING);

       ASSERT_EQ(vm.m_stack[vm.m_stack.size()-1].m_val.m_string,"hello there");

}


TEST(VM,Double){
        ByteLexer lexer("push 120.4 push 6.5 add halt");
        vector<ByteToken*> tokens = lexer.readAllTokens();

       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(vm.m_stack[vm.m_stack.size()-1].m_type,ByteType::DOUBLE);

       ASSERT_EQ(vm.m_stack[vm.m_stack.size()-1].m_val.m_double,126.9);

}


TEST(VM,isle){
        ByteLexer lexer("push 12 push 14 isle halt");
        vector<ByteToken*> tokens = lexer.readAllTokens();

       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(vm.m_stack[vm.m_stack.size()-1].m_val.m_int ,1);

}

TEST(VM,isge){
        ByteLexer lexer("push 12 push 14 isge halt");
        vector<ByteToken*> tokens = lexer.readAllTokens();

       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(vm.m_stack[vm.m_stack.size()-1].m_val.m_int ,0);

}

TEST(VM,isgt){
        ByteLexer lexer("push 12 push 14 isgt halt");
        vector<ByteToken*> tokens = lexer.readAllTokens();

       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(vm.m_stack[vm.m_stack.size()-1].m_val.m_int ,0);

}



TEST(VM,nots){
        ByteLexer lexer("push 12 push 14 isgt not halt");
        vector<ByteToken*> tokens = lexer.readAllTokens();

       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(vm.m_stack[vm.m_stack.size()-1].m_val.m_int ,1);

}



TEST(VM,islt){
        ByteLexer lexer("push 12 push 14 islt halt");
        vector<ByteToken*> tokens = lexer.readAllTokens();

       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(vm.m_stack[vm.m_stack.size()-1].m_val.m_int ,1);

}



TEST(VM,MINUS){
         ByteLexer lexer("push 120 push 6 minus halt");
        vector<ByteToken*> tokens = lexer.readAllTokens();
       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(vm.m_stack[vm.m_stack.size()-1].m_val.m_int ,114);

}

TEST(VM,TIMES){
        ByteLexer lexer("push 120 push 6 times halt");
        vector<ByteToken*> tokens = lexer.readAllTokens();
       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(vm.m_stack[vm.m_stack.size()-1].m_val.m_int ,720);

}

TEST(VM,DIVIDE){
         ByteLexer lexer("push 120 push 6 divide halt");
        vector<ByteToken*> tokens = lexer.readAllTokens();
       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(vm.m_stack[vm.m_stack.size()-1].m_val.m_int ,20);

}


TEST(VM,LOADSTORE){
        ByteLexer lexer("push 12 store i load i push 6 add store i load i halt");
        vector<ByteToken*> tokens = lexer.readAllTokens();
       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(tokens.size(),14);
       ASSERT_EQ(vm.m_stack.size(),1);
       ASSERT_EQ(vm.m_stack[vm.m_stack.size()-1].m_val.m_int ,18);

}


TEST(VM,IF){
        ByteLexer lexer("push 12 store i push 13 store o load i load o isgt jif 19 load o jmp 21 load i halt");
        vector<ByteToken*> tokens = lexer.readAllTokens();
       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(vm.m_stack.size(),1);
       ASSERT_EQ(vm.m_stack[vm.m_stack.size()-1].m_val.m_int ,13);

}

TEST(VM,CALL){
        ByteLexer lexer("push 25 push 99 call 7 halt store i store o load i load o isgt jif 21 load o return load i return");
          vector<ByteToken*> tokens = lexer.readAllTokens();
       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(tokens.size(),24);
       ASSERT_EQ(vm.m_stack.size(),1);
       ASSERT_EQ(vm.m_stack[vm.m_stack.size()-1].m_val.m_int ,99);
}

TEST(VM,labels){
        ByteLexer lexer("push 12 store i push 13 store o load i load o isgt jif if load o jmp end if: load i end: halt");
          vector<ByteToken*> tokens = lexer.readAllTokens();
       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(tokens.size(),24);
       ASSERT_EQ(vm.m_stack.size(),1);
       ASSERT_EQ(vm.m_stack[vm.m_stack.size()-1].m_val.m_int ,13);
}

TEST(VM,len){
        ByteLexer lexer("push \"Hello\" len halt");
          vector<ByteToken*> tokens = lexer.readAllTokens();
       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(vm.m_stack.size(),1);
       ASSERT_EQ(vm.m_stack[vm.m_stack.size()-1].m_val.m_int ,5);
}


TEST(VM,locals){
        ByteLexer lexer("call  main  main:  push  12  store  i  startlocal  push  1  jif  startif7  jmp  endif7  startif7:  push  14  store  i  load  i  print  endif7:  poplocal  startlocal  push  1  jif  startif24  jmp  endif24  startif24:  push  15  store  i  load  i  print  endif24:  poplocal  load  i  print  halt  ");
          vector<ByteToken*> tokens = lexer.readAllTokens();
       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(vm.m_stack.size(),0);
}

TEST(VM,forLoop){
        ByteLexer lexer("call main main: startlocal push 0 store i startFor3: load i push 20 islt not startlocal jif endFor3 load i print load i push 1 add asi i poplocal jmp startFor3 endFor3: poplocal halt");
          vector<ByteToken*> tokens = lexer.readAllTokens();
       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(vm.m_stack.size(),0);

}


TEST(VM,recursion){
        ByteLexer lexer("call main right: store i startlocal load i push 2 equal_equal jif startif5 jmp endif5 startif5: push 12 return endif5: poplocal push 4 push 2 call right add return return main: push 3 call right print halt ");
          vector<ByteToken*> tokens = lexer.readAllTokens();
       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(vm.m_stack.size(),0);

}

TEST(VM,array){
       ByteLexer lexer("new list store i load i push 0 append load i push 3 append load i push 0 delete load i push 0 at halt");
       vector<ByteToken*> tokens = lexer.readAllTokens();
       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(vm.m_stack.size(),1);
       ASSERT_EQ(vm.m_stack[vm.m_stack.size()-1].m_val.m_int ,3);

}


TEST(VM,structs){
       ByteLexer lexer("startlocal store y store x structdec pos new pos store position push 13 load position set x load position select x halt");
       vector<ByteToken*> tokens = lexer.readAllTokens();
       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(vm.m_stack.size(),1);

}

TEST(Vm,constructor){
       ByteLexer lexer("startlocal functionPush pos store y store x structdec pos call main pos.pos: loadclass store this push 14 asi y load this return main: new pos classcall pos store position push 13 load position set x load position select x load position select y halt");
       vector<ByteToken*> tokens = lexer.readAllTokens();
       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(vm.m_stack.size(),2);
       ASSERT_EQ(vm.m_stack[1].m_val.m_int,14);

}

TEST(Vm,classcall){
       ByteLexer lexer("startlocal functionPush pos functionPush xup store y store x structdec pos call main pos.xup: loadclass store this load y push 1 add asi y return pos.pos: loadclass store this push 14 asi y load this return main: new pos classcall pos store position push 13 load position set x load position select x load position classcall xup load position select y halt");
       vector<ByteToken*> tokens = lexer.readAllTokens();
       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(vm.m_stack.size(),2);
       ASSERT_EQ(vm.m_stack[1].m_val.m_int,15);

}

TEST(Vm,extendsFunctions){
       ByteLexer lexer("functionPush price functionPush pris structdec Pos functionPush price functionPush pric structdecEx Loc Pos call main Pos.price: store this push 13 return Pos.pris: store this push 15 return Loc.price: store this push 19 return Loc.pric: store this push 20 return main: new Loc store l load l classcall price load l classcall pric load l classcall pris halt");
       vector<ByteToken*> tokens = lexer.readAllTokens();
       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(vm.m_stack.size(),3);
       ASSERT_EQ(vm.m_stack[0].m_val.m_int,19);
       ASSERT_EQ(vm.m_stack[1].m_val.m_int,20);
       ASSERT_EQ(vm.m_stack[2].m_val.m_int,15);

}

TEST(Vm,staticFuncs){
       ByteLexer lexer("startlocal functionPush right0 functionPush left0 structdec Pos call main0 Pos.right0: pop push 12 return Pos.left0: pop push 14 return main0: new Pos store pos load pos classcall right0 load Pos classcall left0 halt");
       vector<ByteToken*> tokens = lexer.readAllTokens();
       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(vm.m_stack.size(),2);
       ASSERT_EQ(vm.m_stack[0].m_val.m_int,12);
       ASSERT_EQ(vm.m_stack[1].m_val.m_int,14);


}

TEST(VM,classwihoutSTate){
       ByteLexer lexer("startlocal functionPush make0 structdec Pos call main Pos.make0: store this call much0 return much0: store this push 15 return main: new Pos store pos load pos classcall make0 halt");
       vector<ByteToken*> tokens = lexer.readAllTokens();
       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(vm.m_stack.size(),1);
       ASSERT_EQ(vm.m_stack[0].m_type,ByteType::INT);
       ASSERT_EQ(vm.m_stack[0].m_val.m_int,15);
}


TEST(VM,classcallStates){
       ByteLexer lexer("startlocal functionPush make0 functionPush much0 structdec Pos call main Pos.make0: store this call much0 return Pos.much0: store this push 15 return main: new Pos store pos load pos classcall make0 halt");
       vector<ByteToken*> tokens = lexer.readAllTokens();
       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(vm.m_stack.size(),1);
       ASSERT_EQ(vm.m_stack[0].m_type,ByteType::INT);
       ASSERT_EQ(vm.m_stack[0].m_val.m_int,15);
}

TEST(VM,casts){
       ByteLexer lexer("startlocal push 12 cast double push 14  cast string push \"15\" cast int halt");
       vector<ByteToken*> tokens = lexer.readAllTokens();
       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(vm.m_stack.size(),3);
       ASSERT_EQ(vm.m_stack[0].m_type,ByteType::DOUBLE);
       ASSERT_EQ(vm.m_stack[1].m_type,ByteType::STRING);
       ASSERT_EQ(vm.m_stack[2].m_type ,ByteType::INT);
       ASSERT_EQ(vm.m_stack[2].m_val.m_int ,15);
}

TEST(VM,extens){
       ByteLexer lexer("startlocal push 12 store x push 14 store y structdec Pos  startlocal push \"Hello\" store name structdecEx Location Pos startlocal push \"none\" store type structdecEx Place Location new Location store loc new Place store pl load pl select x load pl select name load pl select type load loc select y halt");
       vector<ByteToken*> tokens = lexer.readAllTokens();
       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(vm.m_stack.size(),4);
       ASSERT_EQ(vm.m_stack[0].m_val.m_int,12);
       ASSERT_EQ(vm.m_stack[1].m_val.m_string,"Hello");
       ASSERT_EQ(vm.m_stack[2].m_val.m_string,"none");
       ASSERT_EQ(vm.m_stack[3].m_val.m_int ,14);
}

TEST(VM,dup){
       ByteLexer lexer("push 12 dup 4 halt");
       vector<ByteToken*> tokens = lexer.readAllTokens();
       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(vm.m_stack.size(),4);
       ASSERT_EQ(vm.m_stack[0].m_val.m_int,12);
       ASSERT_EQ(vm.m_stack[1].m_val.m_int,12 );
       ASSERT_EQ(vm.m_stack[2].m_val.m_int,12);
       ASSERT_EQ(vm.m_stack[3].m_val.m_int ,12);
}






TEST(VM,maps){
      ByteLexer lexer("startlocal new map store lis push 15 push \"hello\" load lis set   push 19 push \"ok\" load lis set load lis classcall keys0 load lis classcall vals0 load lis push \"ok\"  at halt");
       vector<ByteToken*> tokens = lexer.readAllTokens();
       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(vm.m_stack.size(),3);
       ASSERT_EQ(vm.m_stack[0].m_type,ByteType::OBJ);
       ASSERT_EQ(vm.m_stack[1].m_type,ByteType::OBJ);
       ASSERT_EQ(vm.m_stack[2].m_type,ByteType::INT);
}



