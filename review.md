# OOP Review
&copy; LI Haodong

[TOC]

### Chapter 2
1. 使用`inline`声明内联函数时，必须使用函数体和`inline`关键字结合在一起使用，`inline void Coord::setCoord(int, int)`被视为普通函数

2. 对象的定义可以在类声明的时候（全局对象）也可在使用的时候

3. ```c++
   class A {
       protected:
       	int test;
   }
   int main() {
       A a;
       std::cout << a.test << std::endl;	// Error!!!!!!
   }
   ```

### Chapter 3

1. 初始化表`A(int a):x1(a),x2(x1), pi(3.14){} `

2. 如果需要将数据成员存放在堆中或数组中,则应在构造函数中使用赋值语句，而不是初始化表中

3. **类成员是按照它们在类里被声明的顺序初始化的,与它们在初始化表中列出的顺序无关**

   ```c++
   class D {
       public:
       	D(int i): m2(i), m1(m1 + 1) {
               std::cout << m1 << std::endl;
               std::cout << m2 << std::endl;
           }
       private:
       	int m1;
   	    int m2;
   };
   int main() {
       D(15);			// -858993459
   }					// 15
   ```

   

4. 所谓浅拷贝,就是由缺省的拷贝构造函数所实现的数据成员逐一赋值,若类中含有指针类型数据,则会产生错误，此时需要从新定义拷贝构造函数

5. 析构函数没有参数,也没有返回值,而且不能重载，默认析构函数只能释放对象的数据成员所占用的空间,但不包括堆内存空间

6. 类的组合：先调用内嵌对象的构造函数(按内嵌时的声明顺序,先声明者先构造)。然后调用本类的构造函数。(析构函数的调用顺序相反)

7. 类应该先声明,后使用，如果需要在某个类的声明之前,引用该类,则应进行前向引用声明，此时仅仅引入一个符号，具体实现在后面（此时仅仅可以引用，而不能使用！）

### Chapter 4

1. 声明:类型说明符 类名:: *数据成员指针名;
   赋值:数据成员指针名=&类名:: 数据成员名
   使用:对象名. *数据成员指针名
   对象指针名-> *数据成员指针名
2. 声明:类型说明符
   (类名∷ \*指针名)(参数表);
   赋值:成员函数指针名 = 类名∷成员函数名;
   使用:(对象名.*成员函数指针名)(参数表) ;
   (对象指针名 -> *成员函数指针名)(参数表) ;
3. 仅有共有的静态数据成员可以被访问
   可用类名访问: 类名::静态数据成员
   也可用对象访问: 对象名.静态数据成员
   对象指针->静态数据成员
4. 可以通过定义和使用静态成员函数来访问静态数据成员，对公有静态成员函数,可以通过类名或对象名来调用，静态成员函数可以直接访问该类的静态数据成员和函数成员
5. 静态成员函数可以定义成内嵌的,也可以在类外定义,在类外定义时不
   能用`static`前缀
6. **静态成员函数中没有指针this,所以静态成员函数不访问类中的非静态
   数据成员,若确实需要则只能通过对象名(作为参数)访问**
7. 可以通过函数指针或者数据指针访问类的**共有**静态函数与**共有**静态数据成员

### Chapter 5

1. 有元函数可以是多个类的

   ```c++
   class  girl;	// 向前引用
   class boy {
   public:
       friend void prdata(const boy& b, const girl& g);
       ...;
   };
   class girl {
   public:
       friend void prdata(const boy& b, const girl& g);
       ...;
   };
   void prdata(const boy& b, const girl& g) {
       ...;
   }
   ```

2. 友元成员函数不仅可以是一般函数(非成员函数),而且可以是另一个类中的成员函数

3. 一个类的成员函数也可以作为另一个类的友元,这种成员函数不仅可以访问自己所在类对象中的所有成员,还可以访问friend声明语句所在类对象中的所有成员。一个类的成员函数作为另一个类的友元函数时,必须先定义这个类`friend  void Class::Function();`

4. 有元类

   ```c++
   class Y{};
   class X{
       ...;
       friend class Y;	// 此时Y中的所有成员函数均为X的友元函数
       				// Y可以访问X中的数据成员
       ...;
   }
   ```

5. 友元的关系是单向的而不是双向的

6. 友元的关系不能传递

7. 对象成员必须要进行初始化

8. 是用`const`可以保护共有数据成员，定义为` const` 的对象的所有数据成员的值都不能被修改。凡出现调用非`const`的成员函数,将出现编译错误。但构造函数除外。`class const object`

9. 用 `const` 声明的常数据成员,其值是不能改变的。只能通过构造函数的参数初始化表对常数据成员进行初始化。

10. 成员函数声明中包含`const`时称为常成员函数。此时,该函数只能引用本类中的数据成员,而不能修改它们(注意:可以改`mutable`成员),即成员数据不能作为语句的左值。注意`const`的位置在函数名和括号之后.注意:`const`是函数类型的一部分,在声明函数和定义函数时都要有`const`关键字

11. 如果已定义了一个常对象,则**只能调用其中的`const`成员函数**

12. 指向常对象(变量)的指针变量,不能通过它来改变所指向目标对象的值,但指针变量的值是可以改变的。

### Chapter 6

1. 派生类继承了基类中除构造函数和析构函数之外的所有成员

2. 父类中的`private`在所有字类中均为不可访问

3. ![image-20210706080032523](../../.config/Typora/typora-user-images/image-20210706080032523.png)

4. 基类的`public`成员和`protected`成员被**私有**继承后作为派生类的`private`成员,派生类的其他成员可以直接访问它们,但是在类外部通过派生类的对象无法访问。基类的`private`成员在私有派生类中是不可直接访问的

5. `protected`

   1. 对建立其所在类对象的模块来说(水平访问时),它与 `private` 成员的性质相同
   2. 对于其派生类来说(垂直访问时),它与 `public` 成员的性质相同。既实现了数据隐藏,又方便继承,实现代码重用。

6. 虚基类

   ```c++
   class erived:virtual public base{
       //...
   };
   class erived:public virtual base{
       //...
   };
   ```

7. 若同一层次中同时包含虚基类和非虚基类,应先调用虚基类的构造函数,再调用非虚基类的构造函数,最后调用派生类构造函数,若全是虚基类或者非虚基类，则先左后右,自上而下

### Chapter 7

1. 多态分为编译时多态与运行时多态

2. 内联函数不能是虚函数

3. 构造函数不能是虚函数

4. 虚函数必须是其所在类的成员函数,而不能是友元函数,也不能是静态成员函数

5. 一个虚函数无论被公有继承多少次,它仍然保持其虚函数的特性

6. ```c++
   class Grandam
   {
   public:
       Grandam()
       {
       }
       virtual ~Grandam()
       {
           std::cout << "This is Grandam∷~Grandam()." << std::endl;
       }
   };
   
   class Mother : public Grandam
   {
   public:
       Mother()
       {
       }
       ~Mother()
       {
           std::cout << "This is Mother∷~Mother()." << std::endl;
       }
   };
   
   main() {
       Grandam *f;
       f = new Mother;	// This is Mother∷~Mother().
       delete f;		// This is Grandam∷~Grandam().
   }
   ```

7. 如果子类对父类中的虚函数进行重载，则`Base *p = new Dervied`会调用父类的成员函数

8. 纯虚函数是一个在基类中说明的虚函数,它在该基类中没有定义,但要求在它的派生类中必须定义自己的版本,或重新说明为纯虚函数
   `virtual <函数类型> <函数名> ( 参数表 ) = 0;`

9. 如果一个类至少有一个纯虚函数,那么就称该类为抽象类

10. 抽象类只能作为其他类的基类来使用,不能建立抽象类对象,其纯虚函数的实现由派生类给出

11. 派生类中必须重载基类中的纯虚函数,否则它仍将被看作一个抽象类
