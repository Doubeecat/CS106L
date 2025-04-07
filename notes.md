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

# L8 - Classes & objects

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

# L9 - Operators

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

经验法则：
1. 对于 `[],(),->,=` 等必须是成员函数，直接操作的是对象状态
2. 对于 `<<,>>` 等必须为非成员函数，且最好定义为友元函数（因为很多时候没有办法正常访问他们），因为左操作数是流对象，无法添加成员函数
3. 对于一元运算符，例如 `++,--` 必须实现为成员函数
4. 对于二元运算符，如果对两个操作数的处理是一样的（比如 `+,<`） 那么尽量实现为非成员函数
5. 对于二元运算符，如果对两个操作数的处理是不一样的（比如 `+=`） 那么必须实现为成员函数（对于左操作数的访问更加容易！）

