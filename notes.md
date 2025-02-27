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

我们也可以使用 `getline()` 代替运算符，这样我们就可以忽略空格读入一整行了，`getline` 是以换行符为分界，每次消耗掉一个换行符。主要作用在于确保我们可以清空缓存区，不会让我们读入变得差。

对于与文件交互，我们使用 `ifstream` 和 `ofstream`

```cpp
int var1;
ifstream in("in.txt",ifstream::in);
ofstream out("out.txt",ofstream::out);
in >> var1;
out << var1;
```

这段代码实现了从 `in.txt` 中读入，并且输出到 `out.txt` 中。