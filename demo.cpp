#include"calu.hpp"
#include"RSA.hpp"
#include<iostream>
#include<vector>
#include<string>
#include<cstdlib>
//尝试RSA加解密
int main(){
  RSA rsa(8);//八位质数（不找过int范围内）
  rsa.setPublicKey(65537);//选定一个常用公钥
  //在符合要求的两段质数中
  //每段从前1个中随机选择一个
  //若设为50，则从前50个中随机选择一个
  rsa.setNumOfPrimeToChoose(1);

  rsa.create();//生成一对RSA密钥
  std::cout<<rsa;

  std::string mes("Hello_world!");
  for(auto c : mes){//逐字符输出加密前的明文
    std::cout << (int)c<<std::endl;
  }

  std::vector<bigint>crypt = encrypt(rsa.public_key,mes);//加密得到密文

  for(auto b : crypt){
    std::cout << b;//输出密文
  }

  std::string decry = decrypt(rsa.private_key,crypt);//解密
  
  std::cout << decry << std::endl;//输出解密的内容
  system("pause");
  return 0;
}








//  std::vector<int> mes1=str2num(mes);
//   for(auto t:mes1){
//     std::cout<<t<<std::endl;
//   }
//   for(auto b:crypt){
//     b.dbg_show();
//   }
//   std::vector<int> mes2=str2num(decry);
//   for(auto t:mes2){
//     std::cout<<t<<std::endl;
//   }