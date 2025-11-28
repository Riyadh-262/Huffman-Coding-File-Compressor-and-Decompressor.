#include<iostream>
#include<fstream>
#include<cstring>

using namespace std;

struct Node
{
    char ch;
    int freq;
    Node* left;
    Node* right;
};

struct Heap
{
    Node* data[256];
    int size;
};

void initHeap(Heap &h);
void insertHeap(Heap &h,Node* n);
Node* extractMin(Heap &h);
Node* buildHuffmanTree(int freq[256]);
void printCodes(Node* root,char code[256],int top,char codes[256][256]);
void compressFile(const char* inputFile,const char* outputFile);
void decompressFile(const char* inputFile,const char* outputFile);

void initHeap(Heap &h)
{
    h.size = 0;
}

void insertHeap(Heap &h,Node* n)
{
    int i=h.size;
    h.data[i]=n;
    h.size++;

    while(i!=0&&h.data[i]->freq<h.data[(i-1)/2]->freq)
    {
        Node* temp=h.data[i];
        h.data[i]=h.data[(i-1)/2];
        h.data[(i-1)/2]=temp;
        i=(i-1)/2;
    }
}
Node* extractMin(Heap &h)
{

    Node* temp=h.data[0];
    h.data[0]=h.data[h.size-1];
    h.size--;
    int i=0;

    while(true)
    {
        int smallest=i;
        int l=2*i+1;
        int r=2*i+2;
        if(l<h.size&&h.data[l]->freq<h.data[smallest]->freq) smallest=l;
        if(r<h.size&&h.data[r]->freq<h.data[smallest]->freq) smallest=r;
        if(smallest!=i)
        {
            Node* t=h.data[i];
            h.data[i]=h.data[smallest];
            h.data[smallest]=t;
            i=smallest;
        }
        else
        {
            break;
        }
    }
    return temp;
}

Node* buildHuffmanTree(int freq[256])
{
    Heap h;
    initHeap(h);

    for(int i=0; i<256; i++)
    {
        if(freq[i]>0)
        {
            Node* n=new Node;
            n->ch=(char)i;
            n->freq=freq[i];
            n->left=n->right=NULL;
            insertHeap(h,n);
        }
    }
    while(h.size>1)
    {
        Node* left=extractMin(h);
        Node* right=extractMin(h);
        Node* parent=new Node;
        parent->ch='\0';
        parent->freq=left->freq+right->freq;
        parent->left=left;
        parent->right=right;
        insertHeap(h,parent);
    }
    return extractMin(h);
}

void printCodes(Node* root,char code[256],int top,char codes[256][256])
{
    if(root->left)
    {
        code[top]='0';
        printCodes(root->left,code,top+1,codes);
    }
    if(root->right)
    {
        code[top]='1';
        printCodes(root->right,code,top+1,codes);
    }
    if(!root->left&&!root->right)
    {
        code[top]='\0';
        strcpy(codes[(unsigned char)root->ch],code);
    }
}

void compressFile(const char* inputFile,const char* outputFile)
{
    int freq[256]= {0};
    ifstream fin(inputFile,ios::binary);
    if(!fin)
    {
        cout<<"Cannot open input file!\n";
        return;
    }
    char c;
    while(fin.get(c))freq[(unsigned char)c]++;
    fin.clear();
    fin.seekg(0);

    Node* root=buildHuffmanTree(freq);
    char codes[256][256];
    char arr[256];
    for(int i=0; i<256; i++)
    {
        codes[i][0] = '\0';
    }
    printCodes(root,arr,0,codes);

    ofstream fout(outputFile,ios::binary);
    for(int i=0; i<256; i++)
    {
        fout.write((char*)&freq[i],sizeof(int));
    }

    unsigned char byte=0;
    int bitCount=0;
    int totalBits=0;
    while(fin.get(c))
    {
        for(int i=0; codes[(unsigned char)c][i]!='\0'; i++)
        {
            byte<<=1;
            if(codes[(unsigned char)c][i]=='1')byte|=1;
            bitCount++;
            totalBits++;
            if(bitCount==8)
            {
                fout.put(byte);
                byte=0;
                bitCount=0;
            }
        }
    }
    if(bitCount>0)
    {
        byte<<=(8-bitCount);
        fout.put(byte);
    }
    fin.close();
    fout.close();

    ifstream finput(inputFile,ios::binary);
    ifstream foutput(outputFile,ios::binary);
    finput.seekg(0,ios::end);
    foutput.seekg(0,ios::end);
    int originalBytes=finput.tellg();
    int compressedBytes=foutput.tellg();
    double ratio=100.0*(originalBytes-compressedBytes)/originalBytes;
    cout<<"Original bytes: "<<originalBytes<<"\n";
    cout<<"Compressed bytes: "<<compressedBytes<<"\n";
    cout<<"Compression ratio: "<<ratio<<"%\n";
    finput.close();
    foutput.close();
}

void decompressFile(const char* inputFile,const char* outputFile)
{
    ifstream fin(inputFile,ios::binary);
    if(!fin)
    {
        cout << "Cannot open input file!\n";
        return;
    }
    int freq[256];
    for(int i=0; i<256; i++)
    {
        fin.read((char*)&freq[i], sizeof(int));
    }
    Node* root=buildHuffmanTree(freq);
    ofstream fout(outputFile,ios::binary);
    Node* current=root;
    char byte;

    while(fin.get(byte))
    {
        for(int i=7; i>=0; i--)
        {
            if(byte&(1<<i))
            {
                current=current->right;
            }
            else
            {
                current=current->left;
            }
            if(!current->left&&!current->right)
            {
                fout.put(current->ch);
                current=root;
            }
        }
    }
    fin.close();
    fout.close();
}
int main(int argc,char* argv[])
{
    if(argc!=4)
    {
        cout<<"Usage:\n";
        cout<<"Compress:   ./huffman.exe -c input.txt output.huf\n";
        cout<<"Decompress: ./huffman.exe -d input.huf output.txt\n";
        return 0;
    }
    if(strcmp(argv[1],"-c")==0)
    {
        compressFile(argv[2],argv[3]);
    }
    else if(strcmp(argv[1],"-d")==0)
    {
        decompressFile(argv[2],argv[3]);
    }
    else
    {
        cout<<"Invalid option!\n";
    }
    return 0;
}

//huffman.exe -c Input1.txt outputH.huf
//huffman.exe -d outputH.huf RestoredH.txt