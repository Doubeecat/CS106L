# L2-Streams

C++ 中的字符串：

```cpp
string str = "Hello world";
```

采用下标访问，0-based.本质上是一个字符数组。

## 流的存在？

与 console 和 files 等等对象进行交互的方式。很酷的在于流提供了**一个通用接口**.

举个例子，`cout` 如何输出一个东西到控制台？程序中的数据对象 $\stackrel{\text{类型转化}}{\rightarrow}$ `string` 表示 $\stackrel{\text{输出}}{\rightarrow}$ 控制台，第二部分就是流所做的事情了。

## `stringstream`

`stringstream` 分为两种，有 `ostringstream` 和 `istringstream`，代表的是输出流和输入流。`ostringstream` 可以使用 `<<` 运算符向里面写入东西。也就相当于一个缓冲区。

我们可以使用 `.str()` 把 `ostringstream` 中的东西转换为字符串。

```cpp
ostringstream oss("it's me");
cout << oss.str() << endl;
//it's me
oss << "888hi!";
cout << oss.str();
//888hi!e
```

想象一下我们有一个字符指针一直在移动，最开始构造这个流的时候，指针从起点开始，所以我们覆盖了缓冲区。如果我们想让字符指针从流末尾开始，我们可以使用 `stringstream::ate` 构造一个流：

```cpp
ostringstream oss("it's me",stringstream::ate);
cout << oss.str() << endl;
//it's me
oss << "888hi!";
cout << oss.str();
//it's me888hi!
```

`stream` 比 `buffer` 更厉害的在于，你可以直接从 `stream` 中读入数字或者其他什么东西。比如我们可以把读入流中的 `string` 也自动变为 `int` 等类型，并且以符号作为分割，特别智能：

```cpp
istringstream iss("114 yuan");
int price;string unit;
iss >> price;
iss >> unit;
cout << price << " " << unit << endl;
//57 yuan
```

`stringstream` 牛逼之处在于，他把一个字符串给划分成了若干个 tokens，比如：

```
16.9 \n Ounces. . \t\n\n -38271
```

`16.9`,`Ounces`,`.`,`-38271` 就是我们分出来的四个 tokens.但是 tokens 的分割也是基于我们如何读入的，比如上面这段，如果我们用 `int` 类型的变量去读入，那么第一个 token 就是 `16`.

我们对于每个 `ostringstream` 的 IO，分为两步：
1. 把对应变量转换为字符串
2. 插入到对应的 `ostringstream` 的缓冲区。


`istringstream` 也是类似：
1. 从缓冲区中读取字符串
2. 把对应字符串转化为所求类型的变量
 
`stringstream` 支持链式调用，也就是可以实现 `iss >> var1 >> var2 >> ...`  的使用。

刚才我们提到了流的字符指针，我们同样可以通过一些函数来知道当前指针的位置，这些东西比较底层：

|功能 |`istringstream`|`ostringstream`|
|---|---|---|
| 获取指针位置 | `oss.tellp()` | `iss.tellg()` |
| 改变指针位置 | `oss.seekp(pos)` | `iss.seekg(pos)` | 
|获取偏移量| `streamoff()`||

```cpp
ostringstream iss("114 yuan",stringstream::ate);
cout << iss.tellp();
//8 这里设置了 stringstream::ate，所以是在末尾
ostringstream iss1("114 yuan");
cout << iss1.tellp();
//0
```

我们可以实现一个简单的函数，实现从字符串到数字的转换啦！

```cpp
int StringToInterger(const string &s) {
    int number;
    istringstream iss(s);
    iss >> number;
    return number;
}
```

这个函数实现了从字符串内读入了一个数的功能，我们也可以实现从字符串自动读入一堆数，然后返回一个 `vector` 的功能：

```cpp
vector<int> StringToVector(const string &s) {
    vector <int> number;
    istringstream iss(s);
    int x;
    while (iss >> x) {
        number.push_back(x);
    }
    return number;
}

int main() {
    vector <int> vec;
    vec = StringToVector("1 2 3 4 5");
    for (auto x : vec) cout << x*x << "\n";
    return 0;
}

//1 4 9 16 25
```

## 状态位

反应流到底发生什么了，分成四种：

- Good bit: 准备好读入/输出了
- Fail bit: 先前操作失败，冻结此后操作
- EOF bit: 先前操作已经到达了缓冲区内容末尾
- Bad bit: 由于流缓冲区上的输入/输出操作失败而导致的错误。（比如你文件被删掉了）

状态位之间并不矛盾，比如 G 和 B 通常同时出现在类型不匹配的时候，正常读入到 `EOF` 也会同时出现 G 和 E.良好和失败并不对立，正如考试一样^_^

```cpp
void PrintStateBits(const istringstream &s) {
    cout << (s.bad() ? "B" : "-");
    cout << (s.good() ? "G" : "-");
    cout << (s.fail() ? "F" : "-");
    cout << (s.eof() ? "E" : "-");
    cout << endl;
}

int StringToInterger(const string &s) {
    int number;
    istringstream iss(s);
    PrintStateBits(iss);
    iss >> number;
    PrintStateBits(iss);
    return number;
}
int main() {
    while (1) {
        string s;
        if (!getline(cin,s)) exit(1);
        StringToInterger(s);
    }
    return 0;
}

```
```
1
-G--
---E
12 2
-G--
-G--
a
-G--
--F-
11111111111111111111111
-G--
--FE
```

状态位的存在帮助我们很好地去检查我们在使用流时发生的错误。

## 标准 `iostream`

以下四个流都是与 console 交互的：

- `cin` : 标准输入流
- `cout` : 标准输出流（buffered）
- `cerr` : 标准错误流（unbuffered）
- `clog` : 标准错误流（buffered）

`cin,cout` 和控制台交互的原则和我们在 `stringstream` 大同小异，但是特点在于，如果缓冲区这时候没有东西，`cin` 会等待用户输入。提取器会帮助我们跳过前导空格，但是不会消耗下一个非空格字符。

注意，`cin` 如果读入失败了，这个时候 Fail bit 为 1，此后所有 `cin` 操作都倒闭了。

我们也可以使用 `getline()` 代替运算符，这样我们就可以忽略空格读入一整行了，`getline` 是以换行符为分界，每次消耗掉一个换行符。主要作用在于确保我们可以清空缓存区，不会让我们读入变得差。使用方法也很简单： 

```cpp
cin >> str;
//更改成 
getline(cin,str);
```

`getline` 函数返回的是 `bool`，指示我们的操作是否成功。基于此我们可以写出一个新的更加完备的 `getInteger()`

upd:实际上 getline 返回的是传入的输出流对象本身的引用，之所以返回值能被用在 bool 求值语境下，是因为 std::cin 提供了一个 operator bool 类型转换函数，这个函数会在合适的语境下将 std::cin 本身转换为一个 bool 值。（感谢知乎评论区的“用户”朋友！）



```cpp
int getInterget() {
    while (1) {
        string line;
        if (!getline(cin,line)) 
            throw domain_error();
        istringstream iss(line);
        int res;char trash;
        if (iss >> res && !(iss >> trash))
            return res;
    }
}
```

对于与文件交互，我们使用 `ifstream` 和 `ofstream`

```cpp
int var1;
ifstream in("in.txt",ifstream::in);
ofstream out("out.txt",ofstream::out);
in >> var1;
out << var1;
```

这段代码实现了从 `in.txt` 中读入，并且输出到 `out.txt` 中。

## 现代 C++ 类型

### `size_t`
在实际编码过程中，经常碰到对于 `size_t` 的 warnings。我们来深究一下这个东西。

在调用 `std::.size()` 的时候我们得到的是一个 `size_t` 类型的无符号整数，它代表一个表示大小的变量，本质上是一个无符号整型。`size_t` 在不同的机器上有微小不同，提供了一种可以移植的方法来声明与系统中可寻址的内存区域一致的长度。

在实际编码中，我们要小心 `arr.size() - 1` 这样的写法！当 `size() = 0` 的时候就会产生很多问题。

### 类型别名

我们可以使用 `using a = b;` 这样的语句来给 `b` 这个类型取一个 `a` 的别名。比如：

```cpp
using map_iter = unordered_map <forward_list<int>,unordered_set> :: const_iterator;

map_iter begin = map1.cbegin();
```

最直接的应用就是 STL 库里的各种 `iterator` 和 `reference`。

### `auto`

自动推导，来点 C++ 笑话：

```cpp
auto f(string& a) {
    auto b = a;
    return b;
}
```

比较需要注意的是，在 C++ 中尽量少用 C 风格字符串。在 lambda 函数，自动推导 `iterator`，定义 `templates` 中我们经常用到 auto.

# L3-Sequences Containers


### `pair`

`pair` 是一个可以储存两个对象的元组。从 C++17 开始，C++ 本身支持了 structured bindings.我们可以通过 `make_pair` 自动推导一个对应的二元组。当然也可以利用 `auto` 自动推导：

```cpp
auto p = make_pair(1.28,"abc"); //返回的是一个 pair<double,string>
auto [a,b] = p;// a 为 double 类型，b 为 string 类型。
```

### `struct`

`struct` 本身是一个更为强大的元组，可以任意储存多种类型的对象。并且每个对象是具有名称的，不需要再用 `first,second` 来写了。

```cpp
struct Time {
    double timestamp;
};

struct Course {
    Time start,end;
    string name;
    vector <string> students;
};
```

`struct` 是 `class` 的一种轻量级形式，可以说我们不涉及 `private/public` 的内容。

当然我们依然可以利用结构化绑定(structured bindings)来简化代码：

```cpp
struct node {
    int l,r;
};

node Construct_node(int l,int r) {
    return node{l,r};
}
```

### 初始化

C++11 开始支持了一种统一初始化：

```cpp
int main() {
    vector <int> pi{3,1,4,1,5,9};
}
```

对于自定义的 `struct` 来说也很适合，你可以按照 `struct` 里面定义的顺序来初始化一个东西。注意对于 0 初始化来说，我们一般只认为对全局变量或者某些东西你可以依赖 C++ 进行空初始化，对于其他情况尽量不要依赖 C++.举个例子，对于 `std::vector` 来说，他在构造函数里有指定某个类型的默认值，所以新定义 `std::vector` 你可以相信！

## 序列容器

是容器的子集，容器又是 STL 的一方面东西。

```cpp
std::vector <T>
std::deque <T>
std::list <T>
std::array <T>
std::forward_list <T>
```

### `vector`

可以使用任意类型定义的线性容器。

| 基本方法 | `std::vector<int>`|
|:---:|:---:|
|创建新 vector| `vector <int> v;`|
|创建新 vector，长度为 n| `vector <int> v(n);`|
|创建新 vector，长度为 n，值为 k| `vector <int> v(n,k);`|
|添加新元素| `v.push_back(k);`|
|清空| `v.clear()`|
|获取下标元素| `v.at(i);v[i];`|

注意，使用 `at(i)` 的时候可以丢出错误 `out_of_range`，但是用中括号就没有了，如果超出范围实际上是一个 undefined behavior。

### `deque`

注意到 `vector` 实际上不支持 `push_front` 操作，或者说这样做的操作效率很低。我们使用 `deque` 来实现一个可以在双端插入删除的东西。但是请注意，在同样操作下 `deque` 的效率总是比 `vector` 低的！

## STL

先来点现代 C++ 的三体人震撼：

```cpp
int main() {
    int n = 15;
    vector <int> vec(n);
    generate(vec.begin(),vec.end(),rand);
    sort(vec.begin(),vec.end());
    copy(vec.begin(),vec.end(),ostream_iterator<int>(cout,"\n"));
}
```

这个代码实现了自动生成 n 个整数并且排序输出的功能，太无敌了。

# L4&5-Associative Containers & Iterators

## Container Adaptor

STL 库里给我们提供了很多常用数据结构的封装完毕的实现！比如 `std::stack,std::queue`，这俩东西实际上是 `vector,deque` 只允许在某一端进行操作的实现。

因为这个原因，所以这俩都被称为**容器适配器(container adaptor)**.就是这俩实际上的实现是把 `vector,deque` 给封装成了其他东西。在声明的时候如果你喜欢给他任意一个满足需求的容器都可以。

为什么要有容器适配器？这就来到了设计 C++ 的哲学之一：直接在代码中表达自己的意图。
 
## Associative Containers

关联容器是一种没有序列概念的数据类型，数据以键值类储存。

```cpp
std::map<T1,T2>
std::set<T>
std::unordered_map<T1,T2>
std::unordered_set<T>
```

前两个是基于顺序排序的储存，所以键值类型必须支持小于号。后两个必须实现 `hash` 函数。所以 `map,set` 在遍历连续的一段区间时显著快，后两者在随机访问中显著快。

```cpp
std::map <string,int> freq;
string word;
freq[word]++;
freq.get(word);//如果不存在则会抛出错误
int a = freq[word];//如果不存在则会新建一个 (word,0) 的元组
freq.count(word);//这个返回0/1 代表是否存在
```

## Iterators

太超模了，这才是新时代 C++！

迭代器允许我们遍历任意容器，无论其是否有序。提供了一种以线性方式遍历容器的方法。

迭代器的类型取决于使用的数据结构的类型，我们可以对一个迭代器类型进行 `++` 的操作，以及使用 `*` 解引用。每个容器都有 `.begin(),.end()` 两个迭代器。比如：

```cpp
set <int>::iterator iter = mySet.begin();
for (;iter != mySet.end();++iter) {
    cout << *iter << "\n";
}
```

当然实际上由于我们现在有了 auto，我们也可以让 auto 直接推导。

### Map iterator

对于 map 的迭代器来说，我们使用的是 `std::pair` 来储存。而且注意到 `make_pair` 的时候，实际上是自动化地决定类型，所以这就是很好的使用 auto 的机会。

```cpp
auto p = make_pair("114514",1919810);
```

而 `map` 的迭代器实际上是一个 `pair<T1,T2>` 的东西，我们可以这样遍历：

```cpp
map <int,int>::iterator iter = m.begin();
for (;iter != m.end();++iter) {
    cout << (*iter).first << " " << (*iter).second << "\n";
}
```

当然由于 `auto` 的存在，我们可以使用更加牛逼也简单 的遍历方法：

```cpp
for (auto e : m) {
    cout << e.first << " " << e.second << endl;
}
```
### iterator-based loop
使用 iterator 的好处在于，我们可以在两个迭代器之间遍历，省掉了具体知道哪个位置的复杂度。比如我们现在想要查询 `lower_bound` 和 `upper_bound` 之间的所有数，我们可以这么写：

```cpp
std::set <int> s = {3,1,4,1,5,9,2,6};
int x = 1;
std::set <int>::iterator st = lower_bound(s.begin(),s.end(),x);
std::set <int>::iterator ed = upper_bound(s.begin(),s.end(),x);
cout << *st << " " << *ed << "\n";
for (auto iter = st;iter != ed;++iter) {
    cout << *iter;
}
console: 
1 2
1
```

这里有一些遍历的表:

||$[a,b]$|$[a,b)$|$(a,b]$|$(a,b)$|
|---|---|---|---|---|
|begin|lower_bound(a)|lower_bound(a)|upper_bound(a)|upper_bound(a)|
|end|upper_bound(b)|lower_bound(b)|upper_bound(b)|lower_bound(b)|

迭代器最大的意义也就在这：它支持我们以一个固定的逻辑形式，遍历不同的数据结构。在后面我们可以把它和 `template` 结合起来。

# L6&7-Templates

## 隐式函数

对于不同的数据类型，我们想要使用同样的一套函数形式处理不同的类型。在 C 语言中我们可能会对所有类型都写一遍，但是实际上我们可以用 template 来做！

template 的本质是我们使用一个统一的类型来编写函数，比如：
```cpp
template <typename T>//先声明模板类型为 T
pair<T,T> my_minmax(T a,T b) {
    if (a < b) return {a,b};
    else return {b,a};
}

pair <double,double> p1 = my_minmax(1.2,3.2);
cout << p1.first << p1.second;
pair <string,string> p2 = my_minmax("a","b");
cout << p2.first << p2.second;
```

我们可以指定一下 template 方法的类型，利用尖括号声明类型。例如：

```cpp
auto [min1,max1] = my_minmax<double>(1.2,3.2);
auto [min2,max2] = my_minmax<string>("Doubee","cat");
```

回顾一下我们的 STL 定义，实际上我们用类似 `vector <int> vec;` 的定义时候就在明确类型，vector 实际上也是一个 template 的对象。好玩的是，`T` 类型的指定实际上是在编译时就完成了，所以编译完的函数和我们最朴素定义是等价的。

如果我们想指定多个不同的模板类型，我们可以这么写：
```cpp
template <typename T,typename U>
```
然后正常使用 T 和 U 即可。 

# L8 - Functions and Algorithms
# L9 - STL

这两节之后有空再补（）因为笔者对这两块了解比较多

# L10 - Classes & objects

## 类

类(class)为我们提供了存取新对象的模板，如果我们想引入新的东西到程序里，我们肯定需要编写一个 class。

组成类的部分分成以下几部分：
- 成员变量(member variables)：每一个对象都具有成员变量的一份拷贝
- 成员函数(member functions)：每个成员函数可以和成员变量进行交互。用 `obj.fun()` 进行交互
- 构造器(constructor)：每个对象最开始是没有东西的，需要通过构造函数对其进行初始化。

C++ 中我们把类分为两种文件：
- `.h` 包含了所有在类中出现的实体的，带分号的原型声明，也就是 `header`
- `.cpp` 包含了所有方法的具体实现。

### 定义

.h 文件一般长这样：

```cpp
#ifndef _BankAccount_h
#define _BankAccount_h
//上面这个文件是被包含两次的

class BankAccount {
    public:
        BankAccount(string name,double val);//constructors 
        void deposit(double amount);// member functions
        void withdraw(double amount);
    private:
        string name;//member variables
        double balance;
};

#endif
```

一个东西设为 `public` 代表可以从外部访问，`private` 代表仅可以从内部访问。`private` 很重要，因为我们很多时候不希望外部函数直接修改变量。举个例子：我肯定不希望别人直接修改我的银行卡余额，但是让银行系统修改银行卡余额是对的（）

在 `.h` 文件中我们给出了定义，在 `.cpp` 文件中我们类似于直接调用客户端一样的进行书写。

```cpp
account::account(string n,double bal) {
    if (bal < 0) throw bal;
    name = n;
    val = bal;
}

void account::withdraw(double amount){
    if (val >= amount) {
        val -= amount;
    }
}

void account::debug() {
    cerr << "name:" << name << "\n";
    cerr << "val:" << val << "\n";
}

int main() {
    string name = "cat";
    account a(name,1.0);
    a.debug();
    a.withdraw(0.5);
    a.debug();
}   
```

如果需要定义成员函数我们这么写(in .cpp)，注意此时 .h 中一定要有对应的定义才能写，但是注意如果是构造函数，前面不写 return type:

```cpp
#include "ClassName.h"

returnType ClassName::methodName(parameters) {
    statements;
}
```

唯一的不同在于我们写一个函数前面需要有 `ClassName::`,并且这函数在 `.h` 中有预先定义。简单理解就是 .h 可以理解为 API。   

我们设计一个东西为 private 的时候，需要考虑每个函数对他的访问是什么形态的：

我们可以让方法对一个变量只读，也就是访问器 (accessors)。可以允许终端去只写，也就是 mutatar.

### 析构函数(Destructor)

当一个对象被程序删除（比如代码结束的时候，或者调用 `delete`）调用的函数。一般写成这样：
```cpp
// spec1_destructors.cpp
#include <string> // strlen()

class String
{
    public:
        String(const char* ch);  // Declare the constructor
        ~String();               // Declare the destructor
    private:
        char* _text{nullptr};
};

// Define the constructor
String::String(const char* ch)
{
    size_t sizeOfText = strlen(ch) + 1; // +1 to account for trailing NULL

    // Dynamically allocate the correct amount of memory.
    _text = new char[sizeOfText];

    // If the allocation succeeds, copy the initialization string.
    if (_text)
    {
        strcpy_s(_text, sizeOfText, ch);
    }
}

// Define the destructor.
String::~String()
{
    // Deallocate the memory that was previously reserved for the string.
    delete[] _text;
}

int main()
{
    String str("We love C++");
}
```

## `const`

为什么使用 const？如果我们使用全局变量，那么全局变量可以在任意地方都被修改，很难去记住为什么使用它，并且这是不安全的！

很多时候，我们不希望我们引入的参数被修改，在函数参数中使用 `const` 可以帮助我们使得函数只读。

例如这个：

```cpp
int f(const int &x) {
    solve(x);//在这个操作之后 x 依然不变
}
```

const 类也是可以的，但是如果我们的调用了一个 const 的成员函数，那么只能以 const 的作用调用对象。

```cpp
struct Planet {
    int countPopulation() const;
    void Deathstar();
};

void destroy(const Planet &p) {
    cout << p.countPopulation() << endl;
    //第一个是可以的
    p.Deathstar();
    //最后一个是不行的，因为 Deathstar 不是 const 函数
}
```

还有 const 指针，比如：
```
int * const p;
```

对这个指针执行 `(*p)++` 是可以的，因为相当于先对指针解引用了，但是执行 `p++` 就不可以了。

注意区分，我们还有一个非 const 指针对 const 变量的引用，总之这些东西就是套娃来去，搞明白 const 约束的是变量本身还是指针就 OK。

让我们看看 const 迭代器是什么，其实本质上和指针一样，如果我们想要一个迭代器只读，我们直接 `const vector <int>::iterator itr`，同样的对于 `++itr` 是无法编译，但是 `*itr = 6` 可以正确赋值。但是要注意，我们还有一种 `const_iterator`：

```cpp
const vector <int>::iterator itr = v.begin();
*itr = 5;//合法的，不能改变迭代器本身，但是可以改变迭代器对应的值
++itr;//不合法

vector <int> const_iterator itr = v.begin();
*itr = 5;//这样就寄了
++itr;//合法
```

# L11 - Operators

我们先来分析一段程序，观察一下 C++ 在实际过程中进行了什么：

```cpp
vector <string> v{"seven","august"};
cout << v[0];
v[1] += "!";
```

它等价于

```cpp
vector <string> v{"seven","august"};
cout.operator << (v.operator[](0));
v.operator[](1).operator += ("!");
```

对于 `<<` 来说，我们应当手写对于每个类型的 <<，令编译器知道我们如何运行。

我们可以通过手写一个分数类来展示一下：参见仓库里的 `fraction.h`

```cpp
//fraction.h
#include <iostream>
using namespace std;
#ifndef _fraction_h
#define _fraction_h

class fraction {
    public:
        fraction(){};
        fraction(int a,int b):base(a),frac(b){};
        fraction operator + (const fraction &a);
        fraction operator + (const int &a);
        fraction operator - (const fraction &a);
        fraction operator * (const fraction &a);
        fraction operator / (const fraction &a);
        fraction& operator += (const fraction &a);
        fraction& operator -= (const fraction &a);
        fraction& operator = (const fraction &a);
        
    private:
        int base,frac;
        friend ostream& operator << (ostream &os,const fraction &a);
};


#endif
```

```cpp
//fraction.cpp
/*
Undo the destiny.
*/
#include <bits/stdc++.h>
#include "fraction.h"
using namespace std;
#define ll long long
fraction fraction::operator + (const fraction &a) {
    int newbase = base * a.base;
    int newfrac = a.base * frac + base * a.frac;
    int g = __gcd(newbase,newfrac);
    return fraction(newbase/g,newfrac/g);
}
fraction fraction::operator + (const int &a) {
    int newbase = base;
    int newfrac = frac + a * base;
    int g = __gcd(newbase,newfrac);
    return fraction(newbase/g,newfrac/g);
}
fraction fraction::operator - (const fraction &a) {
    int newbase = base * a.base;
    int newfrac = a.base * frac - base * a.frac;
    int g = __gcd(newbase,newfrac);
    return fraction(newbase/g,newfrac/g);
}
fraction fraction::operator * (const fraction &a) {
    int newbase = base * a.base;
    int newfrac = frac * a.frac;
    int g = __gcd(newbase,newfrac);
    return fraction(newbase/g,newfrac/g);
}
fraction fraction::operator / (const fraction &a) {
    int newbase = base * a.frac;
    int newfrac = frac * a.base;
    int g = __gcd(newbase,newfrac);
    return fraction(newbase/g,newfrac/g);
}
fraction& fraction::operator += (const fraction &a) {
    
    frac = a.base * frac + base * a.frac;
    base *= a.base;
    int g = __gcd(base,frac);
    base /= g,frac /= g;
    return *this;
}
fraction& fraction::operator -= (const fraction &a) {
    
    frac = a.base * frac - base * a.frac;
    base *= a.base;
    int g = __gcd(base,frac);
    return *this;
}
fraction& fraction::operator = (const fraction &a){
    base = a.base;
    frac = a.frac;
    return *this;
}

ostream& operator << (ostream& os,const fraction &a) {
    os << "(" << a.frac << "/" << a.base << ")";
    return os;
}

int main() {
    fraction a(2,1),b(4,3),c(6,5);
    cout << a << " " << b <<  " " << c << "\n";
    cout << a + b;
    cout << b + c;
    cout << ((a += b) += c);
}

```

注意我们这边在写 `+=,-=` 的时候，都用的是引用，因为我们实际上在返回的时候需要支持连续的写法，比如：

```cpp
fraction a(1,2),b(3,4),c(5,6);
(a += b) += c;
```

如果不是引用，那么将只有第一个运算符可以正常工作。

为什么 `+-*/` 需要是成员函数，但是 `<<` 只能是一个非成员函数？

如果一个运算符重载为成员函数的时候，是由一个操作数调用了另一个操作数：
```cpp
A a,b,c;
c = a + b;
//等价于
c = a.operator+(b);
```
但是如果一个运算符重载为友元函数的时候，参与运算的对象全部成为函数参数：
```cpp
A a,b,c;
c = a + b;
//等价于
c = operator + (a,b);
```

经验法则：

1. 对于 `[],(),->,=` 等必须是成员函数，直接操作的是对象状态
2. 对于 `<<,>>` 等必须为非成员函数，且最好定义为友元函数（因为很多时候没有办法正常访问他们），因为左操作数是不是该类的对象，无法添加成员函数，只能通过友元访问
3. 对于一元运算符，例如 `++,--` 尽量实现为成员函数，如果是非成员函数也可以，当然这样你就要添加引用啥的了，更麻烦一点。
4. 对于二元运算符，如果对两个操作数的处理是一样的（比如 `+,<`） 那么尽量实现为非成员函数，这样两边操作数地位更加对称 
5. 对于二元运算符，如果对两个操作数的处理是不一样的（比如 `+=`） 那么必须实现为成员函数（对于左操作数的访问更加容易！）

# L12 - Special Member Funtions

在实现二元成员运算符的时候，最好把运算符实现为非成员函数。例如 `+`，这样对对象两边的处理才是对等的。

```cpp
vector <string> operator + (const vector <string> & lhs,const vector <string> & rhs) {
    vector <string> copy = lhs;
    copy += rhs;
    return copy;
}
```

同时我们要记得考虑 const 与否，比如我们在考虑 `[]` 运算符的时候我们最好重载一个 const 的和一个非 const 的.因为永远不知道什么时候会用到 const。

## 拷贝函数

一般用到四种：
- 默认构造函数：对象创建时不带参数
- 拷贝构造函数：从一个已经存在的对象赋值
- 拷贝赋值函数：把对象用一个已经存在的对象替换
- 析构函数：删除一个对象

如果没有手写拷贝构造函数的时候，编译器会帮助我们生成一个构造函数。但是比如我们的类中含有指针的话，编译器的构造函数只会帮我们复制指针本身，并不会更深地把指针对应的东西拷贝过去。

拷贝构造函数并不止需要复制某个值的表面（比如单纯复制指针的地址），必须找出一个方法深拷贝他（比如对于 char[]，我们使用 memcpy 这样的，或者使用 std::copy）例如我们这里可以实现一个对于 `StringVector` 类的解析与析构函数：

```cpp
StringVector::StringVector() :
    _logicalSize(0),_allocatedSize(kInitialSize) {
    _elems = new ValueType[_allocatedSize];
}

//这里巧妙使用了初始化列表捏

StringVector::~StringVector() {
    delete _elems;    
}

StringVector::StringVector(const StringVector & other)  :_logicalSize(other._logicalSize),_allocatedSize(other._allocatedSize) {
    _elems = new ValueType[_allocatedSize];
    std::copy(other.begin(),other.end(),begin());
}

StringVector& StringVector::operator=(const StringVector & rhs) {
    if (this != rhs) {
        delete [] _elems;
        _logicalSize = rhs._logicalSize;
        _allocatedSize = rhs._allocatedSize;
        _elems = new ValueType[_allocatedSize];    
        std::copy(rhs.begin(),rhs.end(),begin());
    }
    return *this;
}
```

拷贝赋值本质上是在实现等号罢了。

# L13 - Move Semantics

## `emplace_back`

在使用 vector 的时候，我们发现两个非常相似的函数：`emplace_back` 和  `push_back`。但是只注意到 `emplace_back` 的参数特别奇怪：

> ```cpp
> template< class... Args >
> void emplace_back( Args&&... args );
> (since C++11) (until C++17)
> template< class... Args >
> reference emplace_back( Args&&... args );
> (since C++17)
> (constexpr since C++20)
> ```

`Args&&... args` 比较有魅力的是，你可以丢入任意数量的参数。但是 `&&` 代表的是什么？

这就是 `emplace_back` 有用的一点了，譬如我们在 `push_back` 一个对象的时候，会首先创建一个向量的拷贝，然后再把这个拷贝给丢进去。而 `emplace_back` 允许我们直接把构造一个对象的各种参数丢进去，然后直接创建这个对象。只要我们在每次插入的时候确保所有构造函数中的参数都被满足即可。可以参考 [Cppreference 中的代码](https://en.cppreference.com/w/cpp/container/vector/emplace_back)


```cpp
#include <vector>
#include <cassert>
#include <iostream>
#include <string>

struct President
{
    std::string name;
    std::string country;
    int year;
 
    President(std::string p_name, std::string p_country, int p_year)
        : name(std::move(p_name)), country(std::move(p_country)), year(p_year)
    {
        std::cout << "I am being constructed.\n";
    }
 
    President(President&& other)
        : name(std::move(other.name)), country(std::move(other.country)), year(other.year)
    {
        std::cout << "I am being moved.\n";
    }
 
    President& operator=(const President& other) = default;
};
 
int main()
{
    std::vector<President> elections;
    std::cout << "emplace_back:\n";
    auto& ref = elections.emplace_back("Nelson Mandela", "South Africa", 1994);
 
    std::vector<President> reElections;
    std::cout << "\npush_back:\n";
    reElections.push_back(President("Franklin Delano Roosevelt", "the USA", 1936));
}
```

## 复制问题

我们来分析以下下面这个代码：


```cpp
#include <iostream>
#include "StrVector.h"
using namespace std;
StrVector readNames(size_t size) {
    StrVector names(size,"cat");
    return names;
}

int main() {
    StrVector name1 = readNames(114514);
    StrVector name2;
    name2 = readNames(1919810);
}

```

首先在每个 `readNames` 中，构造每个 names 用了填充构造函数，并且在 `return names;` 的时候创建了一个拷贝，这里用的是拷贝构造函数，最后又调用了析构函数，消除了每个在函数中构造的 names.

在 `main()` 的几个等号中，我们用的都是拷贝构造函数。并且在每个调用 `readNames` 结束后我们都销毁了对应的对象。

## 左值与右值

定义：l-value（左值）是一个具有名字的表达式，可以由取右值得到。r-value（右值）是一个没有名字的表达式，作为临时变量，并且无法寻得地址。

还有一个概念是左值引用与右值引用，左值引用即 `type & it`，，右值引用则是 `type && it`：

```cpp
int val = 2;
int* ptr = 0x3f3f3f3f;
vector <int> v1{1,2,3};

auto &ptr2 = ptr;
auto && v4 = v1 + v2;
```

这里的 `ptr2` 就是左值引用，`v4` 是右值引用。我们不能绑定一个右值到左值引用上，例如这样：

```cpp
auto& ptr3 = &val;
```

同理，我们不能绑定一个右值引用到左值上，例如：

```cpp
auto &&val2 = val;
```

## 移动操作

这一部分将设计两个新的特殊成员函数：移动构造函数和移动赋值函数。二者都是对于右值操作的。

在很多情况我们编写移动赋值的时候，如果我们直接用 `=` 会造成什么问题？因为我们对应的对象是右值，所以实际上资源所有权未转移，只会复制指针值（也就是进行浅拷贝）

我们一般用的是 `std::move` 来规避这种问题。例如：

```cpp
StringVector::StringVector(StringVector && other):elems(std::move(other.elems)),logicalsize(std::move(other.logicalsize)),allocatedsize(std::move(other.allocatedsize))
{
    other.elems = nullptr;
}
```

而这实际上也优化了程序运行效率，因为在移动赋值中，我们不需要新建一个拷贝。例如我们可以写出来一个高效的 `swap`，而这对于某些类类型是尤其重要的（例如 `std::vector`）：

```cpp
template<typename T>
void swap(T& a,T& b){
    T temp = std::move(a);
    a = std::move(b);
    b = std::move(temp);
}
```

并且额外注意的一点是，我们在赋值操作之前需要检查这是否是自我赋值，具体体现在：

```cpp
T& operator = (const T & other) {
    if (this != &other) {
        //do sth
    }
}
```

# L14 - Inheritance

## namespaces（命名空间）

其实我们之前使用的 `vector::,std::` 都是在对应的命名空间下进行，这也不难解释我们为什么用 `using namespace std;` 在许多竞赛代码下（）

C++ 中，许多函数在不同的库中有不同实现，如果我们不加以区分其实根本不知道我们用的究竟是哪个函数（）甚至会产生编译错误，这就是命名空间存在的必要性。相当于划分领域。使用方法也很简单

```cpp
namespace Lecture {
    int count(const vector <int> &v) {
        int cnt = 0;
        for (auto x : v) {
            if (x == 1) ++cnt;
        }
        return cnt;
    }
}
```

这也正是我们在 .cpp 里编写成员函数时候会用 `classx::`

## Inheritance（继承）

对于所有面向对象的过程语言来说，我们都具有继承和接口的概念。我们称被继承的类叫做基类或父类(Base class)，继承的类叫做子类(Derived class)。接口是 C++ 中最简单的继承形式。例如：

```cpp
class Drink {
    public:
        virtual void make() = 0;
}

class Tea:public Drink {
    public:
        void make() {
            //do sth;
        }
};
```

这里涉及了一个新概念，即虚函数。虚函数用 `virtual` 关键字修饰，在最开始仅有一个 `=0` 的参数，这样也被称为纯虚函数。

而等于 0 的作用是：强制**任意**继承该类的类必须实现一个 `make()`，否则不视为一个真正的类。

另外值得注意的是，我们使用 `:public Drink`，以 public 来修饰 Drink。这代表我们把 Drink 中元贝的类型都按照原本的类型处理。如果使用 `protected` 那么就会把 Drink 里所有的 public 转换成 protected，用 `private` 则会变成 private.默认则是 public

如果我们想定义某个函数呢？这就涉及到 **抽象类** 的概念了。

## Abstract Classes（抽象类）

如果一个类包含至少一个纯虚函数，这个类就被称为抽象类。抽象类不能被实例化，也就是我们没法构造一个实际的函数。

```cpp
class Drink {
    public:
        virtual void make() = 0;//纯虚函数
        virtual void make2();//非纯虚函数
        void make3() {ans = 42;};//正常函数
}
```

换句话说，虚函数必须是类的成员函数。实现它是为了实现多态，并且重写常规的函数是合法但不道德的。并且永远记得继承父亲的构造函数，把他作为对应的初始化列表。

```cpp
class Derived : public Base {
    Derived() : Base(args);
}
```

但是注意，析构函数可以是一个虚函数。如果我们想要让一个类可继承，那么我们尽量将类中的析构函数变为虚函数，方便之后继承。这样我们可以保证，如果用户因为某种原因继承这个类的时候，编译器可以正确解析到子类的析构函数。

然后国内好像很喜欢考的一个东西：构造函数的执行顺序是按照继承类的顺序从上到下构造，析构函数的执行顺序是从下到上。

提一下三个关键字，我们经常在类的构造过程中这么写：
- private：只能由本类访问
- protected：只能由本类或派生出来的类访问
- public：可以由任何对象访问

接下来讨论一个话题：到底是用 Template 还是用 继承类？

继承类实际上是使用的动态多态性，Template 是静态多态性。换句话说例如一个基类指针指向一个子类对象时，那么只有运行才知道指针指向的是什么对象。也就是说，**模板在编译时确定了对应类型，继承在运行时才确定对应类型**。

Template 使用时编译器创建了所有类型的对应函数，所以编译时间会长但是运行效率高。

# L15 - Template Classes

我们用函数模板创建的是对不同数据类型的相同函数(长相相似)。同理，我们创建的模板类是对不同数据类型的相同类的构造。例如：

```cpp
template <class T,class Container = std::vector <T> > 
class Stack {
    public:
        Stack() = default;
        ~Stack() = default;
        void push(T x) {...}
    private:
        Container s;
}

Stack <double,std::vector <double> > doubleStack;
```
