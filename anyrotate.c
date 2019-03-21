/*
 *
 * MIT License
 *
 * Copyright (c) 2017 SuperSodaSea
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include <stdio.h>
#include "math.h"

typedef struct
{
    float m11,m12,m13,m14,
    m21,m22,m23,m24,
    m31,m32,m33,m34,
    m41,m42,m43,m44;
    
}myStruct1;


typedef struct
{
    float m11,m12,m13,m14;
    
}myStruct2;


myStruct2 multipStruct(const myStruct1 * my1,const myStruct2 *my2)
{
    myStruct2 tmp;
    
    tmp.m11 = my2->m11*my1->m11 + my2->m12*my1->m21 + my2->m13*my1->m31 + my2->m14*my1->m41;
    
    tmp.m12 = my2->m11*my1->m12 + my2->m12*my1->m22 + my2->m13*my1->m32 + my2->m14*my1->m42;
    
    tmp.m13 = my2->m11*my1->m13 + my2->m12*my1->m23 + my2->m13*my1->m33 + my2->m14*my1->m43;
    
    tmp.m14 = my2->m11*my1->m14 + my2->m12*my1->m24 + my2->m13*my1->m34 + my2->m14*my1->m44;
    
    return tmp;
}

int myStruct2Length(const myStruct2 *my2)
{
    return (int)sqrtf(my2->m11*my2->m11 +my2->m12*my2->m12 +my2->m13*my2->m13);
}

void printfMyStruct1(const myStruct1 *my1)
{
    printf("矩阵如下:\n");
    printf("%f,%f,%f,%f",my1->m11,my1->m12,my1->m13,my1->m14);
    printf("%f,%f,%f,%f",my1->m21,my1->m22,my1->m23,my1->m24);
    printf("%f,%f,%f,%f",my1->m31,my1->m32,my1->m33,my1->m34);
    printf("%f,%f,%f,%f",my1->m41,my1->m42,my1->m43,my1->m44);
}

/*
 
 沿不在原点的直线旋转的矩阵运算
 loc: 原点位置
 n:旋转的轴向量
 angle:角度
 
 思路分三部：
 1。将loc移到原点
 2。在原点饶n旋转
 3。将原点移动回去loc
 
 参数要求：n为单位向量，angle为弧度
 
 */
myStruct1 calculMyStruct1WithVectorWithAngle(myStruct2 loc,myStruct2 n,float angle)
{
    //构造单位矩阵
    myStruct1 tmp = {1,0,0,0,    0,1,0,0,    0,0,1,0,  0,0,0,1};
    
    if(myStruct2Length(&n) != 1)
    {
        printf("请先将旋转轴向量单位化!");
        return tmp;
    }
    
    loc.m14 = 1;
    float sinu = sinf(angle);
    float cosu = cosf(angle);
    
    //计算过原点的饶任意轴旋转矩阵
    tmp.m11 = n.m11*n.m11*(1-cosu) + cosu;
    tmp.m12 = n.m11*n.m12*(1-cosu) + n.m13*sinu;
    tmp.m13 = n.m11*n.m13*(1-cosu) - n.m12*sinu;
    
    tmp.m21 = n.m11*n.m12*(1-cosu) - n.m13*sinu;
    tmp.m22 = n.m12*n.m12*(1-cosu) + cosu;
    tmp.m23 = n.m13*n.m12*(1-cosu) + n.m11*sinu;
    
    tmp.m31 = n.m11*n.m13*(1-cosu) + n.m12*sinu;
    tmp.m32 = n.m12*n.m13*(1-cosu) - n.m11*sinu;
    tmp.m33 = n.m13*n.m13*(1-cosu) + cosu;
    
    //构建－loc向量
    myStruct2 loc1 = {-loc.m11,-loc.m12,-loc.m13, 1 };
    
    //构建－loc＊R33
    myStruct2 my2= multipStruct(&tmp,&loc1);
    
    //构建－loc＊R33＋loc
    myStruct2 loc2 = {my2.m11+loc.m11 ,my2.m12+loc.m12 ,my2.m13+loc.m13 , 1};
    
    //构建平移矩阵
    tmp.m41 = loc2.m11;
    tmp.m42 = loc2.m12;
    tmp.m43 = loc2.m13;
    
    return tmp;
}
