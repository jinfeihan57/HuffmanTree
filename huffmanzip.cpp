#include <stdio.h>

struct HuffmanNode {
    int freqCount;
    unsigned char symbol;
    HuffmanNode *left_node;
    HuffmanNode *right_node;
};
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <cstring>
#define SYMBOLES  256
using namespace std;
struct Element
{
    int parent;//双亲
    int lchild;//左孩子
    int rchild;//右孩子
    int weight;//权值
};
/*@description 找到现有树中最小权重的节点的下标
 *@a  存储Huffman树的数组
 *@n  树中各个symbol的个数
 *@s1 最小两个节点在Huffman数组的下标
 *@s2 同上
 *@return 无
 */
void Select(Element a[],int n,int &s1,int &s2)  // 传入引用
{
    for(int i=0;i<n;i++)
    {
        if(a[i].parent==-1)
        {
            s1=i;
            break;
        }
    }
    for(int i=0;i<n;i++)
    {
        if(a[i].parent==-1&&a[s1].weight>a[i].weight)
            s1=i;
    }
    for(int j=0;j<n;j++)
    {
        if(a[j].parent==-1&&j!=s1)
        {
            s2=j;
            break;
        }
    }
    for(int j=0;j<n;j++)
    {
        if(a[j].parent==-1&&a[s2].weight>a[j].weight&&j!=s1)
            s2=j;
    }
}
/*@description 建立Huffman树
 *@ht 存储Huffman树的数组
 *@w  树中各个symbol的个数
 *@n  symbol种类的个数
 *@return 无
 */
void HuffTree(Element ht[], int w[], int n)
{
    for(int i=0;i<2*n-1;i++)//初始化，所有结点均设为相同值
    {
        ht[i].parent=-1;
        ht[i].lchild=-1;
        ht[i].rchild=-1;
        ht[i].weight=0;
    }
    for(int i=0;i<n;i++)//构造huffman树的所有叶子节点
    {
        ht[i].weight=w[i];
    }
    for(int k=n;k<2*n-1;k++)//n-1次合并
    {
        int i1,i2;
        Select(ht,k,i1,i2);//查找权值最小的两个根结点，下标为i1,i2
        ht[i1].parent=k;//将i1和i2合并，且i1和i2的双亲是k
        ht[i2].parent=k;
        ht[k].weight=ht[i1].weight+ht[i2].weight;
        ht[k].rchild=ht[i1].weight >= ht[i2].weight ? i1 : i2;
        ht[k].lchild=ht[i1].weight < ht[i2].weight ? i1 : i2;
    }
}
//输出哈夫曼树
void Print(Element ht[],int n)
{
    cout<<"index weight parent lchild rchild"<<endl;
    cout<<left;//左对齐输出
    for(int i=0;i<n;i++)
    {
        cout<<setw(5)<<i<<" ";
        cout<<setw(6)<<ht[i].weight<<" ";
        cout<<setw(6)<<ht[i].parent<<" ";
        cout<<setw(6)<<ht[i].lchild<<" ";
        cout<<setw(6)<<ht[i].rchild<<endl;
    }
}

/*@description 将文件内容加载至内存中
 *@path 输入文件路径 
 *@ptr  读取文件内存指针的指针 需要外部free
 *@return 返回文件的大小
 */
size_t read_file(const char *path, unsigned char **ptr) {    
    FILE *f = fopen(path, "rb");
    if (!f) {
        fprintf(stderr, "failed to open file %s\n", path);
        exit(1);
    }
    fseek(f, 0L, SEEK_END); // 将文件指针指向文末
    size_t size = ftell(f); // 获取文件大小
    rewind(f);              // 将文件指针恢复到开始位置

    *ptr = (unsigned char *)malloc(size);
    if (!ptr) {
        fprintf(stderr, "failed to allocate memory to hold %s\n", path);
        exit(1);
    }

    size_t pos = 0;
    while (!feof(f)) {
        size_t read = fread(&(*ptr)[pos], 1, size, f);  // 将文件全部读入内存*ptr中
        if (ferror(f)) {
            fprintf(stderr, "error while reading file %s\n", path);
            exit(1);
        }
        pos += read;
    }
    fclose(f);
    return pos;
}

/*@description 统计指定内存中各个符号出现的次数  unsigned char 共有256中符号可能
 *@ptr 指定内存的地址 
 *@size  指定内存的大小
 *@return 返回统计后的记录结果的内存指针，需要外部free
 */
int *FreqCount(unsigned char *ptr, size_t size){
    int *freqCountBuf = (int *)malloc(SYMBOLES * sizeof(int));
    if(!freqCountBuf){
        std::cout << "malloc ERROR!" << std::endl;
        return NULL;
    }
    memset(freqCountBuf, 0, SYMBOLES * sizeof(int));
    for(int i = 0; i < size; i++){
        freqCountBuf[ptr[i]]++; 
    }
    // for(int i = 0; i < 256; i++){
    //     std::cout << i << "    " << freqCountBuf[i] << std::endl;; 
    // }
    return freqCountBuf;
}

int main(int argc, char *argv[])
{
    unsigned char *ptr = NULL;
    size_t fileSize = read_file(argv[1], &ptr);
    int *freqCountBuf = FreqCount(ptr, fileSize);
    int x[]={5,29,7,8,14,23,3,11};//权值的集合
    // Element *hufftree=new Element[8*2-1];
    // HuffTree(hufftree,x,8);
    // Print(hufftree,8*2);
    Element *hufftree=new Element[SYMBOLES*2-1];//动态创建数组
    HuffTree(hufftree,freqCountBuf,SYMBOLES);
    Print(hufftree,SYMBOLES*2-1);

    //使用过的内存需要释放
    free(ptr);
    free(freqCountBuf);
    return 0;

}