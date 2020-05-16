// Huffman.cpp: 定义应用程序的入口点。
//

#ifndef BITOPR
#define BITOPR
#define GETBYTEBIT(byte, offset) (byte & 0x80 >> offset) /* 从左向右：0-7 */
#define GETBYTEBITREV(byte, offset) (byte & (size_t)0x01 << offset)
#define MODBYTEBIT(byte, value, offset) value ? (byte | 0x80 >> offset) : (byte & ~(0x80 >> offset))
#define MODBYTEBITREV(byte, value, offset) value ? (byte | 0x01 << offset) : (byte & ~(0x01 << offset))
#endif // !BITOPR
#include "Huffman.h"

#define SYMNUMBER 0x101
using namespace std;

struct {
	size_t hex;/* 编码值 */
	size_t bitsLen;/* 编码位长(异常：(bitsLen) > sizeof(size_t)) */
}encode_table[SYMNUMBER];
typedef struct _huffman_node {
	unsigned char sym;
	_huffman_node* pLnode;
	_huffman_node* pRnode;
	_huffman_node() :sym(0), pLnode(nullptr), pRnode(nullptr) {}
	_huffman_node(unsigned char csym) :sym(csym), pLnode(nullptr), pRnode(nullptr) {}
	_huffman_node(_huffman_node* pL, _huffman_node* pR)
	{
		sym = 0;
		pLnode = pL;
		pRnode = pR;
	}
}huffman_node;
typedef struct _sym_list_node {
	size_t frequency;
	huffman_node* pHuffNode;
	_sym_list_node* pNext;
	_sym_list_node(size_t freq, huffman_node* pNewNode, _sym_list_node* next = nullptr) :frequency(freq), pHuffNode(pNewNode), pNext(next) {}
}sym_list_node;

/* 字符统计，遍历第一次 */
void calcSymFreq(unsigned const char *src, size_t len, size_t* sym_freq)
{
	unsigned const char* end = src + len;
	for (; src < end;)
	{
		sym_freq[*src++]++;
	}
}
/* 根据字符频率创建有序链表 */
sym_list_node* createList(size_t* sym_freq, const size_t len = SYMNUMBER)
{
	sym_list_node* pListRoot = new sym_list_node(sym_freq[0], new huffman_node(0), nullptr);
	sym_list_node* pTmp = pListRoot;
	for (size_t i = 1; i < len; ++i, pTmp = pListRoot)
	{
		for (; nullptr != pTmp->pNext && sym_freq[i] >= pTmp->pNext->frequency; pTmp = pTmp->pNext);
		pTmp->pNext = new sym_list_node(sym_freq[i], new huffman_node(i), pTmp->pNext);
	}
	return pListRoot;
}
/* 创建huffmanTree，返回根结点。删除list */
huffman_node* buildHuffmanTree(sym_list_node* pListRoot)
{
	sym_list_node sym_list_node_t = sym_list_node(0, nullptr, pListRoot);
	sym_list_node* pTmp, * pNewNode;
	sym_list_node* pLnode, * pRnode;
	for (pTmp = &sym_list_node_t, pLnode = pTmp->pNext, pRnode = pLnode->pNext;
		pRnode != nullptr;
		pTmp = &sym_list_node_t, pLnode = pTmp->pNext, pRnode = pLnode->pNext)
	{
		pTmp->pNext = pRnode->pNext;
		pNewNode = new sym_list_node(pLnode->frequency + pRnode->frequency, new huffman_node(pLnode->pHuffNode, pRnode->pHuffNode));
		for (; nullptr != pTmp->pNext && pNewNode->frequency >= pTmp->pNext->frequency; pTmp = pTmp->pNext);
		pNewNode->pNext = pTmp->pNext;
		pTmp->pNext = pNewNode;
		delete pLnode;
		delete pRnode;
	}
  huffman_node* huffmanTree = sym_list_node_t.pNext->pHuffNode;
  delete pLnode;
  return huffmanTree;
}
/* 创建符号对照表 */
void createSymTable(huffman_node* pNode, size_t hex, size_t bitsLen)
{
	if (nullptr == pNode->pLnode || nullptr == pNode->pRnode)
	{
		encode_table[pNode->sym].bitsLen = bitsLen;
		encode_table[pNode->sym].hex = hex;
		return;
	}
	bitsLen++;
	createSymTable(pNode->pLnode, hex << 1 & 0xFE, bitsLen);
	createSymTable(pNode->pRnode, hex << 1 | 0x01, bitsLen);
}

//字符串编码
void encodeHuffman(unsigned char* src, size_t len, unsigned char* dst, rsize_t dstLen)
{
	unsigned char* srcEnd = src + len;
	unsigned char* dstEnd = dst + dstLen;
	size_t idx = 0; /* 记录当前字节待处理的位置 */
	for (; src < srcEnd; ++src)
	{
		size_t hex = encode_table[*src].hex;
		size_t bitsLen = encode_table[*src].bitsLen;
		for (size_t offset = 0; offset < bitsLen && dst < dstEnd;)
		{
			*dst = MODBYTEBIT(*dst, (bool)GETBYTEBITREV(hex, bitsLen - ++offset), idx);
			if (++idx == 8)
			{
				++dst;
				idx = 0;
			}
		}
	}
	/* 剩余位置EOF */
	for (; idx < 8 && dst < dstEnd; ++idx)
	{
		*dst = MODBYTEBIT(*dst, (bool)GETBYTEBITREV(encode_table[0x100].hex, encode_table[0x100].bitsLen), idx);
	}
}
//字符串解码
void decodeHuffman(huffman_node* pHuffmanTree, unsigned char* src, size_t len, std::string& strOut)
{
	huffman_node* pHuffmanNode = pHuffmanTree;
	unsigned char* end = src + len;
	char ch = *src;
	size_t idx = 0; /* 记录当前字节待处理的位置 */
	for (; src < end;)
	{
		pHuffmanNode = GETBYTEBIT(ch, idx) ? pHuffmanNode->pRnode : pHuffmanNode->pLnode;
		if (pHuffmanNode->pLnode == nullptr || pHuffmanNode->pRnode == nullptr)
		{
			strOut += pHuffmanNode->sym;
			pHuffmanNode = pHuffmanTree;
		}
		if (++idx == 8)
		{
			ch = *++src;
			idx = 0;
		}
	}
}

int main()
{
	size_t sym_freq[0x101] = { 0 }; /* 保存字符统计值 */
	calcSymFreq((unsigned char*)"Hello 这是中文啊哈哈哈", 23, sym_freq);
	sym_list_node* list = createList(sym_freq);
	huffman_node* pNode = buildHuffmanTree(list);
	createSymTable(pNode, 0, 0);

	unsigned char dst[100] = { 0 };
	encodeHuffman((unsigned char*)"Hello 这是中文啊哈哈哈", 23, dst, 100);
	std::string out;
	decodeHuffman(pNode, dst, 10, out);
	cout << "Hello CMake." << out.c_str() << endl;
	return 0;
}
