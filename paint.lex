%{
#include <stdlib.h>
void yyerror(char*);
int curIndex=0;
int varNum=0;
%}
/* 使用代变量表示任意字符 */
any .
%%
(\/\/{any}*[\n]) {;}
[0-9]+|[0-9]*\.[0-9]+ {sscanf(yytext,"%f",&yylval.iValue);return NUMBER;}
[\(\)\<\>=\+\-\*\/;\{\}\.,%] {return *yytext;}
(>=) {return GE;}
(<=) {return LE;}
(==) {return EQ;}
(!=) {return NE;}
(&&) {return AND;}
(\|\|) {return OR;}
(sin) {return SIN;}
(cos) {return COS;}
(begin) {return _BEGIN;}
(end) {return END;}
(while) {return WHILE;}
(for) {return FOR; }
(if) {return IF;}
(else) {return ELSE;}
(delay) {return DELAY;}
(reset) {return RESET;}
(getwindowsize) {return GETWINDOWSIZE;}
(setwindowsize) {return SETWINDOWSIZE;}
(setcolor) {return SETCOLOR;}
(setpointsize) {return SETPOINTSIZE;}
(setlinewidth) {return SETLINEWIDTH;}
(moveto) {return MOVETO;}
(lineto) {return LINETO;}
(point) {return POINT;}
(line) {return LINE;}
(rotate) {return ROTATE;}
(scale) {return SCALE;}
(translate) {return TRANSLATE;}
[a-zA-Z][a-zA-Z0-9]* {addVar(yytext);yylval.sIndex=curIndex;return VARIABLE;}
[\t\n' '] {;}
. {printf("undeclared symbol:[%s]\n",yytext);}
%%
int yywrap(void)
{
    return 1;
}

