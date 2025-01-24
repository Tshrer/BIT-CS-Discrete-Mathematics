# BIT--
北理工离散数学研究性学习  

内容为对RSA加解密算法的简单复现，包含两段测试代码与两个类
## bigint类
为了解决不可避免的超出int范围的问题而设立  

存放于calu.hpp,定义了在进行加解密时需要的的大整数类  

包括同类之间的加减乘除，乘方，模n的乘方  

重载了输出，+-*/%等运算符  

//简单实现了==，！=等比较（没有<,>等）  
现在有了
## RSA类
定义了一个用于保存于生成密钥的类，  

操作时可以直接将对应成员传入函数  

编写了对std::string的加解密函数  

以及std::vector<bigint>之间相互加解密的函数  

## 声名
测试不周，不保证程序能正确处理所有情况  

参看代码注释（十分简陋）  

