/*
 * Copyright (c) 2023 The Regents of Northwest Minzu University.
 * All rights reserved.
 * @Author: MRJianghu
 * @Email: jianghu2021@petalmail.com
 * @Date: 2023-06-05 20:28:17
 * @LastEditors: MRJianghu
 * @LastEditTime: 2023-06-05 20:29:43
 * @Description:
 *
 */
#ifndef _SEQUENCE_STACK_H
#define _SEQUENCE_STACK_H

#include <stdio.h>
#include <stdbool.h>
#define MAXSIZE 50
#define ELEMENT_TYPE char
typedef struct
{
    ELEMENT_TYPE data[MAXSIZE];
    ELEMENT_TYPE top;
} SequenctStack;

// 初始化
void InitStack(SequenctStack *stack);
// 判断栈空
bool IsEmpty(SequenctStack stack);
// 判断栈满
bool ISFull(SequenctStack stack);
// 进栈
bool Push(SequenctStack *stack, ELEMENT_TYPE value);
// 出栈
bool Pop(SequenctStack *stack, ELEMENT_TYPE *value);
// 读栈顶元素
bool GetTop(SequenctStack stack, ELEMENT_TYPE *value);

#endif