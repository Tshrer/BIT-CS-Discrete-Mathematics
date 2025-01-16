#include<iostream>
#include<vector>
#include<cmath>
#include<algorithm>
#include<windows.h>
#include<iostream>
#include<map>
#include<random>
#include"calu.hpp"
#ifndef _RSA_CLASS_
#define _RSA_CLASS_
/*
一对RSA密钥以及生成密钥相关的函数
一些bigint与string之间的加解密函数
一些bigint之间的加解密函数
*/

class RSA{
  private:

  int generateRandomNumber(int lowerBound, int upperBound) {
  // 使用随机设备生成种子
  std::random_device rd;
  std::mt19937 gen(rd()); // 使用 Mersenne Twister 作为随机数生成器
  std::uniform_int_distribution<> dis(lowerBound, upperBound); // 定义范围

  return dis(gen); // 生成随机数
  }

  // 埃拉托色尼筛法生成小质数
  std::vector<int> simpleSieve(int limit) {
    std::vector<bool> isPrime(limit + 1, true);
    isPrime[0] = isPrime[1] = false;
    for(int i = 2; i * i <= limit; ++i) {
      if(isPrime[i]) {
        for(int j = i * i; j <= limit; j += i) {
          isPrime[j] = false;
        }
      }
    }
  std::vector<int> primes;
    for(int i = 2; i <= limit; ++i) {
      if(isPrime[i]) {
        primes.push_back(i);
      }
    }
    return primes;
  }

  // 分段埃拉托色尼筛法生成大范围质数
  std::vector<int> segmentedSieve(int start, int count) {
    std::vector<int> result;
    int limit = std::sqrt(start) + 1;
    std::vector<int> primes = simpleSieve(limit);
    int current = start;
    while(result.size() < (size_t)count) {
      int range = std::max(10, limit * 2); // 增加筛选范围
      std::vector<bool> isPrime(range, true);
      for(int prime : primes) {
        int base = std::max(prime * prime, (current + prime - 1) / prime * prime);
        for(int j = base; j < current + range; j += prime) {
          isPrime[j - current] = false;
        }
      }
      for(int i = 0; i < range && result.size() < (size_t)count; ++i) {
        if(isPrime[i]) {
          result.push_back(current + i);
        }
      }
      current += range;
    }
    return result;
  }

  void extendedGCD(bigint a, bigint b, bigint &gcd, bigint &x, bigint &y) {
    if (b == 0) {
      gcd = a;
      x = bigint(1);
      y = bigint(0);
      return;
    }
    bigint x1, y1;
    extendedGCD(b, a % b, gcd, x1, y1);
    x = y1;
    y = x1 - ((a / b) * y1);
  }

  // 计算模逆
  bigint modularInverse(bigint &e, bigint phi) {
    bigint gcd, x, y;
    extendedGCD(e, phi, gcd, x, y);
    while (gcd != 1) {
        e+bigint(2);
        extendedGCD(e, phi, gcd, x, y);
    }
    return (x % phi + phi) % phi; // 确保结果为正
  }

  bigint _e = bigint(7);
  int key1, key2;
  int __n = 7, __s = 1;
  bigint _n, _phi, _d;
  friend std::ostream& operator<<(std::ostream& _os, const RSA& _rsa);
  
  public:
  std::pair<bigint, bigint> public_key, private_key;
  RSA(int n): __n(n){};
  RSA(int rsa_n,int rsa_s): __n(rsa_n), __s(rsa_s){};
  
  bool create(){
    if(__n>9) __n=9;
    int b0, b1, b2;
    b0 = std::pow(10, __n-1);
    if(__n == 9) b2 = (int)2 * b0;
    else b2 = 10 * b0 - 1;
    b1 = (b2 - b0) / 2;
    b1 += b0;
    std::vector<int> keys1 = segmentedSieve(generateRandomNumber(b0, b1), __s);
    std::vector<int> keys2 = segmentedSieve(generateRandomNumber(b1, b2) ,__s);
    key1 = keys1[generateRandomNumber(0, __s - 1)];
    key2 = keys2[generateRandomNumber(0, __s - 1)];
    bigint p(key1), q(key2), _p(key1 - 1), _q(key2 - 1);
    _n = p * q;
    _phi = _p * _q;
    _d = modularInverse(_e, _phi);
    public_key.first = _e;//写入
    public_key.second = _n;
    private_key.first = _d;
    private_key.second = _n;
  return true;
  }
  
  bool setPublicKey(int __k){
    _e = bigint(__k);
    return true;
  }
  
  bool setNumOfPrimeToChoose(int r){
    __s = r;
    return true;
  }
};

std::ostream& operator<<(std::ostream& _os, const RSA& _rsa){
  _os << "public_key:\n";
  _os <<  _rsa.public_key.first << _rsa.public_key.second << std::endl;
  _os << "private_key:\n";
  _os << _rsa.private_key.first << _rsa.private_key.second << std::endl;
  return _os;
}

std::vector<bigint>encrypt(const std::pair<bigint,bigint>& public_key, const std::string& message){
  std::vector<bigint> crypt;
  for(char chr : message){
    bigint t((int)chr);
    t = t.powmod(public_key.first, public_key.second);
    crypt.push_back(t);
  }
  return crypt;
}
std::string decrypt(const std::pair<bigint,bigint>&private_key,const std::vector<bigint>& crypt){
  std::string message;
  for(bigint t : crypt){
    char chr;
    t = t.powmod(private_key.first, private_key.second);
    chr = t.trans();
    message.push_back(chr);
  }
  return message;
}
std::vector<bigint> encrypt(const std::pair<bigint,bigint>& public_key, const std::vector<bigint>& message){
  std::vector<bigint> crypt;
  for(auto t : message){
    t = t.powmod(public_key.first, public_key.second);
    crypt.push_back(t);
  }
  return crypt;
}
std::vector<bigint> decrypt2(const std::pair<bigint,bigint>&private_key,const std::vector<bigint>& crypt){
  std::vector<bigint> message;
  for(auto t : crypt){
    t = t.powmod(private_key.first, private_key.second);
    message.push_back(t);
  }
  return message;
}

// std::string num2str(std::vector<int> a){
//   std::string str;
//   for(auto t:a){
//     str.push_back((char)t);
//   }
//   return str;
// }
// std::vector<int> str2num(std::string a){
//   std::vector<int> num;
//   for(auto s:a){
//     num.push_back((int)s);
//   }
//   return num;
// }

#endif