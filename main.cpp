#include<bits/stdc++.h>
#include "hash.h"

using namespace std;

char* HashName[8] =                                             //  存放哈希函数名称的数组
{
    "ELF", "SDBM", "RS", "JS",
    "PJW", "BKDR", "DJB", "AP"
};
char* ResolutionName[4] =                                       //  存放冲突处理方法名称的数组
{
    "Open Address, Linear Probe",
    "Open Address, Quadratic Probe",
    "Chaining, STL List(Linear Search)",
    "Chaining, STL Set(Red-Black Tree)"
};
unsigned int (*Hash)(const char* str, int TableSize);         //  哈希函数的函数指针，用于选择哈希函数
unsigned int (*HashFun[8])(const char* str, int TableSize) =  //  哈希函数的函数指针数组，用于存放哈希函数的地址名
{
    &ELFHash, &SDBMHash, &RSHash, &JSHash,
    &PJWHash, &BKDRHash, &DJBHash, &APHash,
};

int TableSize;                  //  哈希表长度（哈希地址空间）
int HashMethod;                 //  哈希方法（哈希函数）编号
int CollisionResolutionMethod;  //  冲突处理方法编号
int DictSize;                   //  字典（关键字）的样本容量
double LoadFactor;              //  负载因子 = 关键字样本容量/哈希表长度

struct Dict                     //  字典的结构体Dict
{
    string word;                //  word存放单词字符串
    string note;                //  note存放释义字符串
    bool operator < (const Dict& dict2) const
    {
        return word < dict2.word;       // 重载<运载符
	}
	bool operator == (const Dict& dict2) const
    {
        return word == dict2.word;      // 重载==运载符
	}
};

Dict dict[3500];
vector<Dict> table0;            //  冲突处理方法1/2（开放地址法）哈希表采用Dict数组结构
vector< list<Dict> > table1;    //  冲突处理方法3（拉链法，链表结构）哈希表采用Dict的链表结构
vector< set<Dict> > table2;     //  冲突处理方法4（拉链法，红黑树结构）哈希表采用Dict的红黑树结构
list<Dict>::iterator iter1;
set<Dict>::iterator iter2;

void HashSelect()                       //  提示用户选择哈希方法
{
    printf("\nPlease Choose A Hash Function:\n");
    printf("    1.ELF Hash Function\n");
    printf("    2.SDBM Hash Function\n");
    printf("    3.RS Hash Function\n");
    printf("    4.JS Hash Function\n");
    printf("    5.PJW Hash Function\n");
    printf("    6.BKDR Hash Function\n");
    printf("    7.DJB Hash Function\n");
    printf("    8.AP Hash Function\n");
    printf("    0.Quit\n");
    scanf("%d", &HashMethod);

    if(HashMethod == 0) return;
    else if(HashMethod > 8)
    {
        printf("Invalid Choice! Choose ELF Hash By Default\n");
        HashMethod = 1;
        Hash = HashFun[0];
    }
    else    Hash = HashFun[HashMethod-1];
    printf("Successfully Choose Hash Function.\n");
}

void CollisionResolutionSelect()        //  提示用户选择冲突处理方法
{
    printf("\nPlease Choose A Method For Collision Reso该算法的时间复杂度为O(n2)。与图中边数无关，该算法适合于稠密图。lution:\n");
    printf("    1.Open Address, Linear Probe\n");
    printf("    2.Open Address, Quadratic Probe\n");
    printf("    3.Chaining, STL List(Linear Search)\n");
    printf("    4.Chaining, STL Set(Red-Black Tree)\n");
    scanf("%d", &CollisionResolutionMethod);
    if(CollisionResolutionMethod > 4 || CollisionResolutionMethod <= 0)
    {
        printf("Invalid Choice! Please Choose Again!\n");
        CollisionResolutionSelect();
    }
    else
    {
        printf("You choose %s\n", ResolutionName[CollisionResolutionMethod-1]);
    }
}

void TableSizeSetting()                 //  提示用户设置哈希表大小
{
    printf("\nPlease set the size of hash table(1-200000):\n");
    printf("If you choose open address method for Collision Resolution\n");
    printf("The size should not be less than 3250.\n");
    printf("And a size of prime number is recommended.\n");
    scanf("%d", &TableSize);
    if(TableSize < 3250 && (CollisionResolutionMethod == 1 || CollisionResolutionMethod == 2))
    {
        printf("Table size is too small, it should not be less than 3250.\n");
        TableSizeSetting();
    }
}

void BuildTable_1(int size)             //  冲突处理方法1的哈希表建立函数
{
    table0.clear();
    table0.resize(TableSize+10);
    for(int i = 0; i < size; i++)
    {
        int index = Hash(dict[i].word.c_str(), TableSize);
        while(table0[index].word.size() != 0)
        {
            if(index < TableSize-1)  index++;
            else index = 0;
        }
        table0[index] = dict[i];
    }
}

void BuildTable_2(int size)             //  冲突处理方法2的哈希表建立函数
{
    table0.clear();
    table0.resize(TableSize+10);
    for(int i = 0; i < size; i++)
    {
        int index = Hash(dict[i].word.c_str(), TableSize);
        int step_size = 1;
        while(table0[index].word.size() != 0)
        {
            index += step_size * step_size;
            while(index >= TableSize) index -= TableSize;
            step_size++;
        }
        table0[index] = dict[i];
    }
}

void BuildTable_3(int size)             //  冲突处理方法3的哈希表建立函数
{
    table1.clear();
    table1.resize(TableSize+10);
    for(int i = 0; i < size; i++)
    {
        int index = Hash(dict[i].word.c_str(), TableSize);
        table1[index].push_back(dict[i]);
    }
}

void BuildTable_4(int size)             //  冲突处理方法4的哈希表建立函数
{
    table2.clear();
    table2.resize(TableSize+10);
    for(int i = 0; i < size; i++)
    {
        int index = Hash(dict[i].word.c_str(), TableSize);
        table2[index].insert(dict[i]);
    }
}

void BuildTable()                       //  建立哈希表
{
    DictSize = 0;
    fstream fin;
    fin.open("data\\dict.txt");
    string str;
    while(getline(fin, str))
    {
        dict[DictSize].word = str.substr(0, str.find(' '));
        dict[DictSize].note = str.substr(str.find(' ')+1, str.size());
        DictSize++;
    }
    LoadFactor = double(DictSize)/TableSize;        //  计算负载因数
    void (*BuildTableFun[4])(int size) =            //  哈希表建立函数的函数指针数组
    {
        &BuildTable_1, &BuildTable_2,
        &BuildTable_3, &BuildTable_4
    };
    BuildTableFun[CollisionResolutionMethod-1](DictSize);
    fin.close();
}
//  冲突处理方法1的探测函数
//  线性探测：1.获得关键字的哈希值；2.将哈希地址中的元素与关键字进行比较
//  3.如果相等查找成功，否则当前地址+1继续查找（如果地址超出哈希地址范围则从起始地址继续）
//  直到当前地址中元素为0（查找失败）或者当前地址中元素等于关键字（查找成功）
bool Probe1(string word_to_search, int &TimeOfProbes, int &index)
{
    bool ok = true;
    index = Hash(word_to_search.c_str(), TableSize);
    TimeOfProbes++;
    while(table0[index].word != word_to_search)
    {
        if(table0[index].word.size() == 0)
        {
            ok = false;
            break;
        }
        if(index < TableSize-1)  index++;
        else index = 0;
        TimeOfProbes++;
    }
    return ok;
}
//  冲突处理方法2的探测函数
//  二次探测：1.获得关键字的哈希值；2.将哈希地址中的元素与关键字进行比较
//  3.如果相等查找成功，否则当前地址+步长继续查找（如果地址超出哈希地址范围则从起始地址继续）
//  每次更新地址后步长+1
//  直到当前地址中元素为0（查找失败）或者当前地址中元素等于关键字（查找成功）
bool Probe2(string word_to_search, int &TimeOfProbes, int &index)
{
    bool ok = true;
    index = Hash(word_to_search.c_str(), TableSize);
    TimeOfProbes++;
    int step_size = 1;
    while(table0[index].word != word_to_search)
    {
        if(table0[index].word.size() == 0)
        {
            ok = false;
            break;
        }
        index += step_size * step_size;
        while(index >= TableSize) index -= TableSize;
        step_size++;
        TimeOfProbes++;
    }
    return ok;
}
//  冲突处理方法3的探测函数
//  拉链法（单链表）：1.获得关键字的哈希值；2.将哈希地址中的元素与关键字进行比较
//  3.如果相等查找成功，否则沿着链表继续向下查找
//  直到查找完毕未找到该元素（查找失败）或者当前地址中元素等于关键字（查找成功）
bool Probe3(string word_to_search, int &TimeOfProbes, int &index)
{
    index = Hash(word_to_search.c_str(), TableSize);
    TimeOfProbes++;
    bool ok = false;
    for(iter1 = table1[index].begin(); iter1 != table1[index].end(); ++iter1)
    {
        if((*iter1).word == word_to_search)
        {
            ok = true;
            break;
        }
        TimeOfProbes++;
    }
    return ok;
}
//  冲突处理方法4的探测函数
//  拉链法（红黑树）：1.获得关键字的哈希值；2.将哈希地址中的元素与关键字进行比较
//  3.如果相等查找成功，否则沿着二叉树继续向下查找
//  直到查找完毕未找到该元素（查找失败）或者当前地址中元素等于关键字（查找成功）
bool Probe4(string word_to_search, int &TimeOfProbes, int &index)
{
    index = Hash(word_to_search.c_str(), TableSize);
    bool ok = true;
    Dict dict_to_search;
    dict_to_search.word = word_to_search;
    iter2 = table2[index].find(dict_to_search);
    if(iter2 == table2[index].end()) ok = false;

    if(ok)  TimeOfProbes += int(0.5 * int(log2(table2[index].size())+0.5) + 0.5)+1;
    else
    {
        if(table2[index].size() > 1)   TimeOfProbes += int(log2(table2[index].size())+0.5)+1;
        else    TimeOfProbes++;
    }
    return ok;
}

bool (*Probe[4])(string word_to_search, int &TimeOfProbes, int &index) =    //  探测函数的函数指针数组
{
    &Probe1, &Probe2,
    &Probe3, &Probe4
};

void TestSampleGenerate(int SampleSize, double SuccessRate, string &FileName)   //  自动生成测试样本
{
    fstream fout;
    cout << "Please Enter The File Name:" << endl;
    cin >> FileName;
    string indirectory = "data\\"+FileName;
    fout.open(indirectory.c_str(), ofstream::out);
    fout << SampleSize << endl;
    int success = int(SampleSize * 1.0 * SuccessRate);
    int fail = SampleSize - success;
    char letters[] = "abcdefghijklmnopqrstuvwxyz";
    bool (*probe)(string word_to_search, int &TimeOfProbes, int &index);
    probe = Probe[CollisionResolutionMethod-1];     //  选择探测函数
    for(int i = 0; i < SampleSize; i++)
    {
        if(fail == 0 || (success-- > 0 && rand() % 2 == 0))
        {
            fout << dict[rand() % DictSize].word << endl;
        }
        else
        {
            string word;
            int nop1 = 0, nop2 = 0;
            int length = rand() % 8 + 1;
            for(int i = 0; i < length; i++)
            {
                word = word + letters[rand()%26];
            }
            if((probe(word, nop1, nop2)) == false)
            {
                fout << word << endl;
                fail--;
            }
            else    i--;
        }

    }
}

void Test()                             //  测试
{
    printf("\nPlease Enter The File Name of Your Input File.\n");
    printf("Enter 'auto' to generate test sample automatically.\n");
    string infilename;
    cin >> infilename;
    string indirectory;
    if(infilename != "auto")
        indirectory = "data\\" + infilename;
    else
    {
        while(1)
        {
            int SampleSize;
            double SuccessRate;
            string FileName;
            printf("Please Enter the Size of Test Sample.(0, 50000]\n");
            scanf("%d", &SampleSize);
            printf("Please Enter the Success Rate of Test Sample.[0, 1]\n");
            scanf("%lf", &SuccessRate);
            if(SuccessRate < 0 || SuccessRate >1)
            {
                printf("Success Rate must be a value in [0, 1].\n");
                printf("Please Try Again.\n");
                continue;
            }
            else if(SampleSize <= 0 || SampleSize > 50000)
            {
                printf("Size of Test Sample must be a value in (0, 50000].\n");
                printf("Please Try Again.\n");
                continue;
            }
            else
            {
                TestSampleGenerate(SampleSize, SuccessRate, FileName);
                indirectory = "data\\"+FileName;
                break;
            }
        }
    }
    printf("Please Enter The File Name of Your Output File.\n");
    string outfilename;
    cin >> outfilename;
    string outdirectory = "data\\" + outfilename;
    fstream fin;
    fstream fout;
    fin.open(indirectory.c_str());
    fout.open(outdirectory.c_str(),ofstream::out);
    int num;
    int TimeOfProbes = 0;
    fin >> num;
    int success = 0;
    if(CollisionResolutionMethod == 1)
    {
        for(int i = 0; i < num; i++)
        {
            string word_to_search;
            fin >> word_to_search;
            int index = 0;
            bool ok = Probe1(word_to_search, TimeOfProbes, index);
            fout << word_to_search << ": ";
            if(ok)
            {
                fout << table0[index].note << endl;
                success++;
            }
            else    fout << "Not exist." << endl;
        }
    }
    if(CollisionResolutionMethod == 2)
    {
        for(int i = 0; i < num; i++)
        {
            string word_to_search;
            fin >> word_to_search;
            int index = 0;
            bool ok = Probe2(word_to_search, TimeOfProbes, index);
            fout << word_to_search << ": ";
            if(ok)
            {
                fout << table0[index].note << endl;
                success++;
            }
            else    fout << "Not exist." << endl;
        }
    }
    if(CollisionResolutionMethod == 3)
    {
        for(int i = 0; i < num; i++)
        {
            string word_to_search;
            fin >> word_to_search;
            int index  = 0;
            bool ok = Probe3(word_to_search, TimeOfProbes, index);
            fout << word_to_search << ": ";
            if(ok)
            {
                fout << (*iter1).note << endl;
                success++;
            }
            else    fout << "Not exist." << endl;
        }
    }
    if(CollisionResolutionMethod == 4)
    {
        for(int i = 0; i < num; i++)
        {
            string word_to_search;
            fin >> word_to_search;
            int index  = 0;
            bool ok = Probe4(word_to_search, TimeOfProbes, index);
            fout << word_to_search << ": ";
            if(ok)
            {
                fout << (*iter2).note << endl;
                success++;
            }
            else    fout << "Not exist." << endl;
        }
    }

    printf("\n\n              TEST RESULT");
    printf("\n***************************************\n");
    printf("    Method for Hashing:  ");
    printf("%s", HashName[HashMethod-1]);
    printf("\n***************************************\n");
    printf("    Number of Key Words: ");
    printf("%d", DictSize);
    printf("\n***************************************\n");
    printf("    Size of Hash Table:  ");
    printf("%d", TableSize);
    printf("\n***************************************\n");
    printf("    Load Factor:        ");
    printf("%.2f", LoadFactor);
    printf("\n***************************************\n");
    printf("    Method for Collision Resolution:\n");
    printf("    %s", ResolutionName[CollisionResolutionMethod-1]);
    printf("\n***************************************\n");
    printf("    Number of Test Samples: ");
    printf("%d", num);
    printf("\n***************************************\n");
    printf("    Successful Search Rate: ");
    printf("%.1f%%", double(success)/num*100);
    printf("\n***************************************\n");
    printf("    Average Times Of Probes: ");
    cout << fixed << setprecision(2) << double(TimeOfProbes)/num;
    printf("\n***************************************\n\n");
    fin.close();
    fout.close();
}

int main()                              //主函数
{
    while(1)
    {
        cout << "Welcome use my Hash Test Program" << endl;
        cout << "This program is developed by Eric Xian." << endl;
        cout << "电子与信息工程学院，SYSU，2017" << endl;
        HashSelect();
        if(HashMethod == 0) break;
        CollisionResolutionSelect();
        TableSizeSetting();
        BuildTable();
        Test();
        cout << "Continue?(Y/N)";
        char YorN;
        cin >> YorN;
        if(YorN == 'N') break;
    }
    return 0;
}

