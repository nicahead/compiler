%{
#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>  //用到可变参数
#include<string.h>
#include<stdio.h>
#include "node.h"
#include "util.c"

VarIndex VarList[1000];  //存储的变量数组
void yyerror(char*);
int yylex(void);
extern int curIndex;
extern int varNum;
%}

//对lex返回的值自动进行类型转换
%token <iValue> NUMBER
%token <sIndex> VARIABLE

%token _BEGIN END
%token WHILE FOR 
%token GETWINDOWSIZE SETWINDOWSIZE SETCOLOR SETPOINTSIZE SETLINEWIDTH
%token MOVETO LINETO POINT LINE ROTATE SCALE TRANSLATE
%token RESET DELAY 
%nonassoc IF  //%nonassoc的含义是没有结合性。它一般与%prec结合使用表示该操作有同样的优先级
%nonassoc ELSE
%left OR AND
%left '^'
%left '&'
%left EQ NE
%right SIN COS
%left GE LE '>' '<'
%left '+' '-'
%left '*' '/' '%'
%nonassoc UMINUS
%type <nPtr> stmt expr stmt_list MOVE  //将<nPtr>与Union的指针类型绑定，表示后面的返回值是<ptr>类型

//扩充YYSTYPE的内容
%union
{
  float iValue;  //值
  int sIndex;    //数组索引
  Node *nPtr;    //节点地址
};


%%
program : _BEGIN function END 			     { exit(0); }
        ;
		
function : function stmt 		     { exeNode($2); freeNode($2); }
         | /*Empty*/
	 ;
		  
stmt : ';'       			     {$$= opr(';', 2, NULL, NULL); } 
     | expr';'  			     {$$= $1; }
     | VARIABLE'='expr';'		     {$$= opr('=', 2, set_index($1), $3); }
     | WHILE'('expr')'stmt 		     {$$= opr(WHILE, 2, $3, $5); } 
     | FOR '(' expr ';' expr ';' expr ')' stmt     {$$ = opr(FOR, 4, $3, $5, $7, $9);} 	 
     | IF'('expr')'stmt %prec IF 	     {$$= opr(IF, 2, $3, $5); }
     | IF'('expr')'stmt ELSE stmt %prec ELSE {$$= opr(IF, 3, $3, $5, $7); }
     | MOVE				     {$$= $1;}
     | '{'stmt_list'}'                       {$$= $2; }
     ;
	 
stmt_list : stmt_list stmt 	             {$$= opr(';', 2, $1, $2); }
          | stmt 			     {$$= $1; }
          ;	 
	 
MOVE : DELAY'('expr')'';' {$$=opr(DELAY,1,$3);}
     | RESET';' {$$=opr(RESET,0);}
	 | GETWINDOWSIZE';' {$$=opr(GETWINDOWSIZE,0);}
	 | SETWINDOWSIZE'('expr')'';' {$$=opr(SETWINDOWSIZE,1,$3);}
	 | SETCOLOR'('expr','expr','expr')'';' {$$=opr(SETCOLOR,3,$3,$5,$7);} 
	 | SETPOINTSIZE'('expr')'';' {$$=opr(SETPOINTSIZE,1,$3);}
	 | SETLINEWIDTH'('expr')'';' {$$=opr(SETLINEWIDTH,1,$3);}
	 | MOVETO'('expr','expr')'';' {$$=opr(MOVETO,2,$3,$5);}
	 | LINETO'('expr','expr')'';' {$$=opr(LINETO,2,$3,$5);}
	 | POINT'('expr','expr')'';' {$$=opr(POINT,2,$3,$5);}
	 | LINE'('expr','expr','expr','expr')'';' {$$=opr(LINE,4,$3,$5,$7,$9);}
	 | ROTATE'('expr')'';' {$$=opr(ROTATE,1,$3);}
	 | SCALE'('expr','expr')'';' {$$=opr(SCALE,2,$3,$5);}
	 | TRANSLATE'('expr','expr')'';' {$$=opr(TRANSLATE,2,$3,$5);}
     ;
	 
expr : NUMBER 		    		     {$$= set_content($1); } 
     | VARIABLE		     		     {$$= set_index($1); }
     | '-'expr %prec UMINUS  		     {$$= opr(UMINUS, 1, $2); }  
     | expr'+'expr 	   		     {$$= opr('+', 2, $1, $3); }
     | expr'-'expr	  		     {$$= opr('-', 2, $1, $3); } 
     | expr'*'expr	 		     {$$= opr('*', 2, $1, $3); }
     | expr'/'expr 		             {$$= opr('/', 2, $1, $3); }
     | expr'<'expr		             {$$= opr('<', 2, $1, $3); }
     | expr'>'expr	 	             {$$= opr('>', 2, $1, $3); } 
     | SIN '(' expr ')'	 	             {$$= opr(SIN, 1, $3); }  
     | COS '(' expr ')'	 	             {$$= opr(COS, 1, $3); }  	 
	 | expr'%'expr			     {$$= opr('%', 2, $1, $3); }
     | expr GE expr 	 		     {$$= opr(GE, 2, $1, $3); }
     | expr LE expr	 		     {$$= opr(LE, 2, $1, $3); }
     | expr NE expr 	 		     {$$= opr(NE, 2, $1, $3); }
     | expr EQ expr 	 		     {$$= opr(EQ, 2, $1, $3); } 
     | expr AND expr 	  		     {$$= opr(AND, 2, $1, $3); }
     | expr OR expr 	  		     {$$= opr(OR, 2, $1, $3); }
     | '('expr')' 	 		     {$$= $2; }
     ; 
	 
%%

void yyerror(char *s)
{
    printf("%s\n", s);
}
#include "lex.yy.c"
int main(void)
{
    yyin=fopen("source_code.txt","r");
    yyout=fopen("paint.txt","w");
    yyparse();
    return 0;
}
//http://blog.csdn.net/liwei_cmg/article/category/207528