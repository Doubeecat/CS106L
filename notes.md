# L2-Streams

C++ 中的字符串：

```cpp
string str = "Hello world";
```

采用下标访问，0-based.本质上是一个字符数组。

## 流的存在？

与 console 和 files 等等对象进行交互的方式。很酷的在于流提供了**一个通用接口**.

举个例子，`cout` 如何输出一个东西到控制台？程序中的数据对象 $\stackrel{\text{类型转化}}{\rightarrow}$ `string` 表示 $\stackrel{\text{输出}}{\rightarrow}$ 控制台，第二部分就是流所做的事情了。

### `stringstream`

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

`stream` 比 `buffer` 更厉害的在于，你可以直接从 `stream` 中读入数字或者其他什么东西。

