#include "sequence_stack.h"

//初始化
void InitStack(SequenctStack* stack)
{
    stack->top = -1;
}
//判断栈空
bool IsEmpty(SequenctStack stack)
{
    if(stack.top == -1)
        return true;
    return false;
}
//判断栈满
bool ISFull(SequenctStack stack)
{
    if(stack.top == MAXSIZE - 1)
        return true;
    return false;
}
//进栈
bool Push(SequenctStack* stack, ELEMENT_TYPE value)
{
    if(stack->top == MAXSIZE - 1)
        return false;
    stack->data[++stack->top] = value;
    return true;
}
//出栈
bool Pop(SequenctStack* stack, ELEMENT_TYPE* value)
{
    if(IsEmpty(*stack))
        return false;
    *value = stack->data[stack->top--];
    return true;
}
//读栈顶元素
bool GetTop(SequenctStack stack, ELEMENT_TYPE* value)
{
    if(IsEmpty(stack)) {
        return false;
    }
    *value = stack.data[stack.top]; 
    return true;
}