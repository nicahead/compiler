#include<stdio.h>
#include <math.h>
#define PI 3.14159

extern int curIndex;
extern int varNum;
extern FILE *yyout;
extern VarIndex VarList[1000];
char res[100];
void yyerror(char*);

#define SIZE_OF_NODE ((char *)&p->content - (char *)p) //计算type所占字节数
//生成类型为常数的节点
Node *set_content(float value)
{
    Node *p;
    size_t sizeNode;
    sizeNode = SIZE_OF_NODE + sizeof(float);  //设定分配空间字节数
    if ((p = malloc(sizeNode)) == NULL)   
        yyerror("out of memory");  //内存不足
    p->type = TYPE_CONTENT;    //参数赋给节点
    p->content = value;
    return p;   //返回这个节点的地址
}

//生成类型为变量的节点,保存变量的索引值
Node *set_index(int value)
{
    Node *p;
    size_t sizeNode;
    sizeNode = SIZE_OF_NODE + sizeof(int);
    if ((p = malloc(sizeNode)) == NULL)
        yyerror("out of memory");
    p->type = TYPE_INDEX;
    p->index = value;
    return p;
}

//生成类型为操作符的节点
Node *opr(int name, int num, ...)
{
    va_list valist;   //指向参数的指针
    Node *p;
    size_t sizeNode;
    int i;
    sizeNode = SIZE_OF_NODE + sizeof(OpNode) + (num - 1) * sizeof(Node*);
    if ((p = malloc(sizeNode)) == NULL)
        yyerror("out of memory");
    p->type = TYPE_OP;
    p->op.name = name;
    p->op.num = num;
    va_start(valist, num);  //将其它的参数分离出来，加入节点的参数表
    for (i = 0; i < num; i++)
        p->op.node[i] = va_arg(valist, Node*);  //返回Node型的参数，赋给p->op.node[i]
    va_end(valist);    //结束可变参数的获取
    return p;
}

//释放节点所占内存空间
void freeNode(Node *p)
{
    int i;
    if (!p) return;
    if (p->type == TYPE_OP)
    {
        for (i = 0; i < p->op.num; i++)
            freeNode(p->op.node[i]);
    }
    free (p);
}
//在内存中添加变量
void addVar(char *name)
{
    int i;
    for(i=0;i<varNum;i++)
    {
		if(strcmp(VarList[i].name,name)==0) //变量名已存在，则无需新建
			break;
    }
	if(i>=varNum)
	{
		strcpy(VarList[i].name,name); //存储变量名
		VarList[i].val=0;
		varNum++;
		curIndex=i;
	}
	else
	{
		curIndex=i;   //更新当前的最后一个索引
	}
}
//对语法树的解释分析
float exeNode(Node *p)
{
    if (!p) return 0;
    switch(p->type)
    {
    case TYPE_CONTENT:
        return p->content;
    case TYPE_INDEX:
        return VarList[p->index].val;
    case TYPE_OP:
        switch(p->op.name)
        {
        case WHILE:
            while(exeNode(p->op.node[0]))
				exeNode(p->op.node[1]);
            return 0;
		case FOR:    
		    exeNode(p->op.node[0]);
            while(exeNode(p->op.node[1])) 
			{
                exeNode(p->op.node[3]);
                exeNode(p->op.node[2]);
            }
            return 0;
        case IF:
            if (exeNode(p->op.node[0]))
                exeNode(p->op.node[1]);
            else if (p->op.num>2)
                exeNode(p->op.node[2]);
            return 0;
			
        case DELAY:
            fprintf(yyout,"delay ");
            sprintf(res,"%g",exeNode(p->op.node[0]));
            fprintf(yyout,res);
            fprintf(yyout,";\n");
            return 0;
        case RESET:
            fprintf(yyout,"reset;\n");
            return 0;
		case GETWINDOWSIZE:
            fprintf(yyout,"get window size;\n");
            return 0;
        case SETWINDOWSIZE:
            fprintf(yyout,"set window size ");
            sprintf(res,"%g",exeNode(p->op.node[0]));
            fprintf(yyout,res);
            fprintf(yyout,";\n");
            return 0;
		case SETCOLOR:
            fprintf(yyout,"set color ");
            sprintf(res,"%g",exeNode(p->op.node[0]));
            fprintf(yyout,res);
			sprintf(res,",%g",exeNode(p->op.node[1]));
            fprintf(yyout,res);
			sprintf(res,",%g",exeNode(p->op.node[2]));
            fprintf(yyout,res);
            fprintf(yyout,";\n");
            return 0;
		case SETPOINTSIZE:
            fprintf(yyout,"set point size ");
            sprintf(res,"%g",exeNode(p->op.node[0]));
            fprintf(yyout,res);
            fprintf(yyout,";\n");
            return 0;
		case SETLINEWIDTH:
            fprintf(yyout,"set line width ");
            sprintf(res,"%g",exeNode(p->op.node[0]));
            fprintf(yyout,res);
            fprintf(yyout,";\n");
            return 0;
		case MOVETO:
            fprintf(yyout,"move to ");
            sprintf(res,"%g",exeNode(p->op.node[0]));
            fprintf(yyout,res);
			sprintf(res,",%g",exeNode(p->op.node[1]));
            fprintf(yyout,res);
            fprintf(yyout,";\n");
            return 0;
		case LINETO:
            fprintf(yyout,"line to ");
            sprintf(res,"%g",exeNode(p->op.node[0]));
            fprintf(yyout,res);
			sprintf(res,",%g",exeNode(p->op.node[1]));
            fprintf(yyout,res);
            fprintf(yyout,";\n");
            return 0;
		case POINT:
            fprintf(yyout,"point ");
            sprintf(res,"%g",exeNode(p->op.node[0]));
            fprintf(yyout,res);
			sprintf(res,",%g",exeNode(p->op.node[1]));
            fprintf(yyout,res);
            fprintf(yyout,";\n");
            return 0;
		case LINE:
            fprintf(yyout,"line ");
            sprintf(res,"%g",exeNode(p->op.node[0]));
            fprintf(yyout,res);
			sprintf(res,",%g",exeNode(p->op.node[1]));
            fprintf(yyout,res);
			sprintf(res,",%g",exeNode(p->op.node[2]));
            fprintf(yyout,res);
			sprintf(res,",%g",exeNode(p->op.node[3]));
            fprintf(yyout,res);
            fprintf(yyout,";\n");
            return 0;
		case ROTATE:
            fprintf(yyout,"rotate ");
            sprintf(res,"%g",exeNode(p->op.node[0]));
            fprintf(yyout,res);
            fprintf(yyout,";\n");
            return 0;
		case SCALE:
            fprintf(yyout,"scale ");
            sprintf(res,"%g",exeNode(p->op.node[0]));
            fprintf(yyout,res);
			sprintf(res,",%g",exeNode(p->op.node[1]));
            fprintf(yyout,res);
            fprintf(yyout,";\n");
            return 0;
		case TRANSLATE:
            fprintf(yyout,"translate ");
            sprintf(res,"%g",exeNode(p->op.node[0]));
            fprintf(yyout,res);
			sprintf(res,",%g",exeNode(p->op.node[1]));
            fprintf(yyout,res);
            fprintf(yyout,";\n");
            return 0;
        case ';':
            exeNode(p->op.node[0]);
            return exeNode(p->op.node[1]);
        case '=':
            return VarList[p->op.node[0]->index].val = exeNode(p->op.node[1]);
        case UMINUS:
            return (exeNode(p->op.node[0])) * -1;
        case '+':
            return exeNode(p->op.node[0]) + exeNode(p->op.node[1]);
        case '-':
            return exeNode(p->op.node[0]) - exeNode(p->op.node[1]);
        case '*':
            return exeNode(p->op.node[0]) * exeNode(p->op.node[1]);
        case '/':
            return exeNode(p->op.node[0]) / exeNode(p->op.node[1]);
        case '<':
            return exeNode(p->op.node[0]) < exeNode(p->op.node[1]);
        case '>':
            return exeNode(p->op.node[0]) > exeNode(p->op.node[1]);
		case SIN:
            return sin(exeNode(p->op.node[0])*PI/180.0);
		case COS:
            return cos(exeNode(p->op.node[0])*PI/180.0);
		case '%':
            return (int)exeNode(p->op.node[0]) % (int)exeNode(p->op.node[1]);
        case GE:
            return exeNode(p->op.node[0]) >= exeNode(p->op.node[1]);
        case LE:
            return exeNode(p->op.node[0]) <= exeNode(p->op.node[1]);
        case NE:
            return exeNode(p->op.node[0]) != exeNode(p->op.node[1]);
        case EQ:
            return exeNode(p->op.node[0]) == exeNode(p->op.node[1]);
        case AND:
            return exeNode(p->op.node[0]) && exeNode(p->op.node[1]);
        case OR:
            return exeNode(p->op.node[0]) || exeNode(p->op.node[1]);
        }
    }
    return 0;
}
