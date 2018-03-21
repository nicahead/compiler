//定义树结点的权举类型
typedef enum { TYPE_CONTENT, TYPE_INDEX, TYPE_OP } NodeEnum;

//操作符
typedef struct
{
    int name;  //操作符名称
    int num;   //操作元个数
    struct NodeTag * node[1];  //操作元地址 可扩展
} OpNode;

//节点
typedef struct NodeTag
{
    NodeEnum type;  //树结点类型
    union
    {
        float content;  //内容
        int index;      //索引
        OpNode op;      //操作符类型
    };
} Node;

//变量
typedef struct VarIndex
{
	char name[100];    //变量名
    float val;         //变量值
}VarIndex; 
