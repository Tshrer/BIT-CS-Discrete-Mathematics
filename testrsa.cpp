#include"calu.hpp"
#include"RSA.hpp"
#include<iostream>
#include<vector>
#include<string>
#include<cstdlib>
//测试RSA的加法同态性
int main(){
  RSA rsa(8);
  rsa.setPublicKey(65537);
  rsa.setNumOfPrimeToChoose(1);
  rsa.create();//生成一对RSA密钥
  std::cout<<rsa;

  bigint a(1234),b(5678);//选择两个明文

  std::vector<bigint> mes1, mes2, crypt, message;

  mes1.push_back(a);
  mes2.push_back(b);//放入待加密的容器中
  std::cout<<mes1[0]<<mes2[0];//输出一下

  std::vector<bigint>crypt1 = encrypt(rsa.public_key,mes1);
  std::vector<bigint>crypt2 = encrypt(rsa.public_key,mes2);//分别加密

  bigint c=crypt1[0]*crypt2[0];//将密文相乘

  crypt.push_back(c);
  std::cout<<crypt1[0]<<crypt2[0]<<crypt[0];//输出一下密文

  message = decrypt2(rsa.private_key,crypt);//得到的新密文进行解密

  std::cout<<message[0];//将解密得到的明文输出
  std::cout<<a*b;//与a*b对比
  system("pause");
  return 0;
}