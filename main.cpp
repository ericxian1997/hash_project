#include<bits/stdc++.h>
#include "hash.h"

using namespace std;

char* HashName[8] =                                             //  ��Ź�ϣ�������Ƶ�����
{
    "ELF", "SDBM", "RS", "JS",
    "PJW", "BKDR", "DJB", "AP"
};
char* ResolutionName[4] =                                       //  ��ų�ͻ���������Ƶ�����
{
    "Open Address, Linear Probe",
    "Open Address, Quadratic Probe",
    "Chaining, STL List(Linear Search)",
    "Chaining, STL Set(Red-Black Tree)"
};
unsigned int (*Hash)(const char* str, int TableSize);         //  ��ϣ�����ĺ���ָ�룬����ѡ���ϣ����
unsigned int (*HashFun[8])(const char* str, int TableSize) =  //  ��ϣ�����ĺ���ָ�����飬���ڴ�Ź�ϣ�����ĵ�ַ��
{
    &ELFHash, &SDBMHash, &RSHash, &JSHash,
    &PJWHash, &BKDRHash, &DJBHash, &APHash,
};

int TableSize;                  //  ��ϣ���ȣ���ϣ��ַ�ռ䣩
int HashMethod;                 //  ��ϣ��������ϣ���������
int CollisionResolutionMethod;  //  ��ͻ���������
int DictSize;                   //  �ֵ䣨�ؼ��֣�����������
double LoadFactor;              //  �������� = �ؼ�����������/��ϣ����

struct Dict                     //  �ֵ�Ľṹ��Dict
{
    string word;                //  word��ŵ����ַ���
    string note;                //  note��������ַ���
    bool operator < (const Dict& dict2) const
    {
        return word < dict2.word;       // ����<���ط�
	}
	bool operator == (const Dict& dict2) const
    {
        return word == dict2.word;      // ����==���ط�
	}
};

Dict dict[3500];
vector<Dict> table0;            //  ��ͻ������1/2�����ŵ�ַ������ϣ�����Dict����ṹ
vector< list<Dict> > table1;    //  ��ͻ������3��������������ṹ����ϣ�����Dict������ṹ
vector< set<Dict> > table2;     //  ��ͻ������4����������������ṹ����ϣ�����Dict�ĺ�����ṹ
list<Dict>::iterator iter1;
set<Dict>::iterator iter2;

void HashSelect()                       //  ��ʾ�û�ѡ���ϣ����
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

void CollisionResolutionSelect()        //  ��ʾ�û�ѡ���ͻ������
{
    printf("\nPlease Choose A Method For Collision Reso���㷨��ʱ�临�Ӷ�ΪO(n2)����ͼ�б����޹أ����㷨�ʺ��ڳ���ͼ��lution:\n");
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

void TableSizeSetting()                 //  ��ʾ�û����ù�ϣ���С
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

void BuildTable_1(int size)             //  ��ͻ������1�Ĺ�ϣ��������
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

void BuildTable_2(int size)             //  ��ͻ������2�Ĺ�ϣ��������
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

void BuildTable_3(int size)             //  ��ͻ������3�Ĺ�ϣ��������
{
    table1.clear();
    table1.resize(TableSize+10);
    for(int i = 0; i < size; i++)
    {
        int index = Hash(dict[i].word.c_str(), TableSize);
        table1[index].push_back(dict[i]);
    }
}

void BuildTable_4(int size)             //  ��ͻ������4�Ĺ�ϣ��������
{
    table2.clear();
    table2.resize(TableSize+10);
    for(int i = 0; i < size; i++)
    {
        int index = Hash(dict[i].word.c_str(), TableSize);
        table2[index].insert(dict[i]);
    }
}

void BuildTable()                       //  ������ϣ��
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
    LoadFactor = double(DictSize)/TableSize;        //  ���㸺������
    void (*BuildTableFun[4])(int size) =            //  ��ϣ���������ĺ���ָ������
    {
        &BuildTable_1, &BuildTable_2,
        &BuildTable_3, &BuildTable_4
    };
    BuildTableFun[CollisionResolutionMethod-1](DictSize);
    fin.close();
}
//  ��ͻ������1��̽�⺯��
//  ����̽�⣺1.��ùؼ��ֵĹ�ϣֵ��2.����ϣ��ַ�е�Ԫ����ؼ��ֽ��бȽ�
//  3.�����Ȳ��ҳɹ�������ǰ��ַ+1�������ң������ַ������ϣ��ַ��Χ�����ʼ��ַ������
//  ֱ����ǰ��ַ��Ԫ��Ϊ0������ʧ�ܣ����ߵ�ǰ��ַ��Ԫ�ص��ڹؼ��֣����ҳɹ���
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
//  ��ͻ������2��̽�⺯��
//  ����̽�⣺1.��ùؼ��ֵĹ�ϣֵ��2.����ϣ��ַ�е�Ԫ����ؼ��ֽ��бȽ�
//  3.�����Ȳ��ҳɹ�������ǰ��ַ+�����������ң������ַ������ϣ��ַ��Χ�����ʼ��ַ������
//  ÿ�θ��µ�ַ�󲽳�+1
//  ֱ����ǰ��ַ��Ԫ��Ϊ0������ʧ�ܣ����ߵ�ǰ��ַ��Ԫ�ص��ڹؼ��֣����ҳɹ���
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
//  ��ͻ������3��̽�⺯��
//  ����������������1.��ùؼ��ֵĹ�ϣֵ��2.����ϣ��ַ�е�Ԫ����ؼ��ֽ��бȽ�
//  3.�����Ȳ��ҳɹ���������������������²���
//  ֱ���������δ�ҵ���Ԫ�أ�����ʧ�ܣ����ߵ�ǰ��ַ��Ԫ�ص��ڹؼ��֣����ҳɹ���
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
//  ��ͻ������4��̽�⺯��
//  �����������������1.��ùؼ��ֵĹ�ϣֵ��2.����ϣ��ַ�е�Ԫ����ؼ��ֽ��бȽ�
//  3.�����Ȳ��ҳɹ����������Ŷ������������²���
//  ֱ���������δ�ҵ���Ԫ�أ�����ʧ�ܣ����ߵ�ǰ��ַ��Ԫ�ص��ڹؼ��֣����ҳɹ���
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

bool (*Probe[4])(string word_to_search, int &TimeOfProbes, int &index) =    //  ̽�⺯���ĺ���ָ������
{
    &Probe1, &Probe2,
    &Probe3, &Probe4
};

void TestSampleGenerate(int SampleSize, double SuccessRate, string &FileName)   //  �Զ����ɲ�������
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
    probe = Probe[CollisionResolutionMethod-1];     //  ѡ��̽�⺯��
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

void Test()                             //  ����
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

int main()                              //������
{
    while(1)
    {
        cout << "Welcome use my Hash Test Program" << endl;
        cout << "This program is developed by Eric Xian." << endl;
        cout << "��������Ϣ����ѧԺ��SYSU��2017" << endl;
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

