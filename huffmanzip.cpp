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
    for(int i=0;i<n;i++)//构造huffman树的所有叶子节点  叶子结点均在前256 即 0-255
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
    for(int i=0; i < n; i++)
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
    for(int i
    ; i < size; i++){
        freqCountBuf[ptr[i]]++; 
    }
    // for(int i = 0; i < 256; i++){
    //     std::cout << i << "    " << freqCountBuf[i] << std::endl;; 
    // }
    return freqCountBuf;
}

/*@description 在Huffman树中找到对应symbol的节点路径将节点以数组形式输出
 *@symbol 指定的symbol 
 *@ht  Huffman树
 *@return 返回存贮 symbol节点路径的数组指针，需要外部delete
 */
int *FindSymbolList(unsigned char symbol, Element ht[]){
    int *symbolList = new int[SYMBOLES];        // 为记录symbol的路径分配空间 256中symbol最大深度不会超过256
    for(int i = 0; i < SYMBOLES; i++){   // 将内存初始化为不可能的 -1 
        symbolList[i] = -1;
    } 
    int tmp = symbol;
    for(int i = 0 ; ht[tmp].parent != -1; i++){
        symbolList[i] = tmp;
        tmp =  ht[tmp].parent;
    }
    // for(int j = 0; symbolList[j] != -1; j++){
    //     std::cout << symbolList[j] << "   ";
    // }
    // std::cout << endl;
    return symbolList;
}

/*@description 根据symbolList和ht 输出symbol的Huffman编码
 *@symbolList 记录symbol在ht中的节点路径的数组
 *@ht  Huffman树
 *@fd  输出的文件描述符
 *@return 返回统计后的记录结果的内存指针，需要外部delete
 */
unsigned char *OutBinaryData(int *symbolList, Element ht[], FILE *fd){
    int i = 0;
    for(i = 0; symbolList[i] != -1; i++){       // 找到最后一个节点
        if(i > 255){
            std::cout << "Shoudn't GO This Way! Something definitely wrong!" << std::endl;
            break;
        }
    }
    static int byteSize = 0;
    static unsigned char outByte = 0;
    int treeNodeIndex = 510;
    while(true){
        if(ht[treeNodeIndex].lchild == symbolList[i-1]){
            std::cout << "0" ;
            byteSize++;
        }
        if(ht[treeNodeIndex].rchild == symbolList[i-1]){
            std::cout << "1  " ;
            outByte = outByte | (0x01 << byteSize);
            byteSize++;
        }
        if(byteSize == 8){
            printf("0x%x  ",outByte);
            fwrite(&outByte, 1, 1, fd);
            byteSize = 0;
            outByte = 0;
        }
        if((ht[symbolList[i-1]].lchild == -1) && (ht[symbolList[i-1]].rchild == -1)) break;
        treeNodeIndex = symbolList[i-1];
        i--;
    }
    std::cout << std::endl;
    return NULL;
}

/*@description 将ptr指向的内容根据ht Huffman树进行编码输出到outFileName文件中
 *@ptr 待压缩文件的内存空间
 *@size  待压缩的文件名
 *@outFileName  输出的文件名
 *@ht  Huffman树
 *@return 返回统计后的记录结果的内存指针，需要外部delete
 */
int HuffmanWrite2File(unsigned char *ptr, int size, char *outFileName, Element ht[]){
    FILE *fd = fopen(outFileName, "wb");
    if (!fd) {
        fprintf(stderr, "failed to open file %s\n", outFileName);
        exit(1);
    }
    for(int i = 0; i < size; i++){
        int *symbolList = FindSymbolList(ptr[i], ht);
        OutBinaryData(symbolList, ht, fd);
        delete [] symbolList;
    }
    fclose(fd);
}

int main(int argc, char *argv[])
{
    unsigned char *ptr = NULL;
    size_t fileSize = read_file(argv[1], &ptr);
    int *freqCountBuf = FreqCount(ptr, fileSize);
   
    Element *hufftree=new Element[SYMBOLES*2-1];//动态创建数组
    HuffTree(hufftree,freqCountBuf,SYMBOLES);
    //Print(hufftree,SYMBOLES*2-1);
    //std::cout << "*************************************" << std::endl;

    // int *symbolList = FindSymbolList('a', hufftree);
    // OutBinaryData(symbolList, hufftree, NULL);

    HuffmanWrite2File(ptr, fileSize, argv[2], hufftree);
    //使用过的内存需要释放
    // delete [] symbolList;
    delete [] hufftree;
    free(ptr);
    free(freqCountBuf);
    return 0;

}