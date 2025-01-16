#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<map>
#ifndef __CALU_
#define __CALU_
/*
定义了一个bigint类以及一些相关运算
大部分涉及同一类间的运算已定义
涉及与int之间的运算，如bigint+int，需要为int类型创建一个bigint***
*/
class bigint{
  private:
  bool negative=false;
  std::vector<int> value;
  friend std::ostream& operator<<(std::ostream& _out,const bigint& __a);
  bool init_from_vect(std::vector<int>&number){//从内部位数数组构造
    value.clear();
    for(auto v : number){//与原数组顺序不变，倒序
      //if(v>=10){std::cout<<"vect is big!\n";}
      value.push_back(v);
    }
    return true;
  }
  // std::vector<int> subvalue(size_t beg_idx,size_t end_idx){//没用
  //   std::vector<int> result(end_idx-beg_idx,0);
  //   for(size_t idx = 0;idx < end_idx - beg_idx ;idx++){
  //     result[idx]=value[beg_idx+idx];
  //   }
  //   return result;
  // }
  
  static bigint bint_add(const std::vector<int>& __x,const std::vector<int>& __y){//加法运算都为正数，调用需检验
    size_t cur=0, x_size=__x.size(), y_size=__y.size();
    int carry=0;//进位
    std::vector<int> result(std::max(x_size,y_size)+2, 0);//保存结果，倒序
    bigint answer;
    while(cur < x_size || cur < y_size){//按位计算
      result[cur] += carry;
      if(cur < x_size) result[cur] += __x[cur];
      if(cur < y_size) result[cur] += __y[cur];
      carry = result[cur]/10;//处理进位
      result[cur] %= 10;
      cur++;
    }
    answer.init_from_vect(result);//构造
    answer.del_lead_zero();
    return answer;
  }
  
  static int compare(const std::vector<int>& __x,const std::vector<int>& __y){//比较绝对值
    if(__x.size() != __y.size()){
      return __x.size() - __y.size();
    }else{
      for(size_t i = __x.size();i > 0;i--){
        if(__x[i-1] != __y[i-1]){
          return __x[i-1] - __y[i-1];
        }
      }
    }
    return 0;//x==y
  }

  static bigint bint_sub(const std::vector<int>& __x,const std::vector<int>& __y){//减法，x-y,需保证x>y
    int borrow = 0;
    std::vector<int> result;
    bigint answer;
    for (size_t i = 0; i < __x.size(); ++i) {
        int sub = __x[i] - (i<__y.size() ? __y[i]:0) - borrow;
        if(sub < 0){
          sub += 10;
          borrow = 1;
        }else{
          borrow = 0;
        }
        result.push_back(sub);
    }
    answer.init_from_vect(result);
    answer.del_lead_zero();
    return answer;
  }
  
  static bigint bint_mtp(const std::vector<int>& __x,const std::vector<int>& __y){//乘法
    size_t x_size = __x.size(), y_size = __y.size();
    std::vector<int> result(x_size+y_size+2, 0);
    bigint answer;
    for(size_t i=0; i < x_size; i++){
      for(size_t j=0; j < y_size; j++){
        result[i+j] += __x[i] * __y[j];
        result[i+j+1] += result[i+j] / 10;
        result[i+j] %= 10;
      }
    }
    answer.init_from_vect(result);
    answer.del_lead_zero();
    return answer;
  }
  
  static std::pair<bigint, bigint> bint_div(const bigint& dividend, const bigint& divisor){//除法
    bigint quotient, remainder;//商和余数
    //quotient.value.resize(dividend.value.size(), 0);
    for (int i = dividend.value.size() - 1; i >= 0; --i) {
      remainder.value.insert(remainder.value.begin(), dividend.value[i]);
      remainder.del_lead_zero();
      //remainder.dbg_show();
      int l=0, r=9, m=0, x=0; 
      while (l <= r) {//二分查找
        m = (l + r) / 2;
        bigint t = divisor.mtp(m);
        if (compare(t.value, remainder.value) < 0){
          x = m;
          l = m + 1;
        }else if(compare(t.value, remainder.value) > 0){
          r = m - 1;
        }else{
          x = m;
          break;
        }
      }
      quotient.value.insert(quotient.value.begin(),x);//在前方添加元素
      quotient.del_lead_zero();
      //quotient.dbg_show();
      remainder = remainder - divisor.mtp(x);
      remainder.del_lead_zero();
      //remainder.dbg_show();    
    }
    return {quotient, remainder};
  }
  
  bool dbg_show(void)const{//cout
    if(negative && value.size() != 0)std::cout << '-';
    size_t size = value.size();
    for(int i = size-1; i >= 0; i--){
      std::cout << value[i];
    }
    std::cout << std::endl;
    return true;
  }

  public:
  // size_t size(void){
  //   return value.size();
  // }
  bigint(): negative(false){};

  bigint(std::string& number){//从std::string构造
    if(number[0] == '-'){
      negative = true;
      value = std::vector<int>(number.begin()+1, number.end());
    }
    else{
      value = std::vector<int>(number.begin(), number.end());
    }
    std::reverse(value.begin(), value.end());
    for(auto &v : value){
      v -= '0';
    }
  }
  
  bigint(int number){//从int构造
    if(number < 0){
      negative = true;
      number *= -1;
    }else if(number == 0){
      value.push_back(0);
    }
    while(number > 0){
      value.push_back(number % 10);
      number /= 10;
    }
  }//初始化
  
  bigint(const char * ptr){//从c字符串构造
    if(*ptr == '-'){
      negative = true;
      ptr++;
    }
    while(*ptr != '\0'){
      value.push_back(*(ptr++) - '0');
    }
    std::reverse(value.begin(), value.end());
  }
  
  bool del_lead_zero(void){//删去前导零
    size_t size = value.size();
    while(size-1 > 0 && value[size-1] == 0){
      value.pop_back();
      size--;
    }
    return true;
  }
  
  bool iszero(void){//为零？
    del_lead_zero();
    return value.size() == 1 && value[0] == 0;
  }
  // bigint operator=(const int num)const{//
  //   bigint answer(num);
  //   return answer;
  // } 
  
  bigint& operator=(const bigint& other){
    this->negative = other.negative;
    this->value = other.value;
    return *this;
  }
  
  bigint operator+(const bigint& other)const{
    bigint answer;
    if(negative == other.negative){//同号
      answer = bint_add(value, other.value);
      answer.negative = negative;
      return answer;
    }
    else{//异号，调用相减
      if(compare(value, other.value) > 0){
        answer = bint_sub(value, other.value);
        answer.negative = negative;
        return answer;
      }else if(compare(value, other.value) < 0){
        answer = bint_sub(other.value, value);
        answer.negative = other.negative;
        return answer;
      }else{
        return bigint(0);
      }
    }
  }
  
  bigint operator-(const bigint& other)const{
    bigint answer;
    if(negative == other.negative){//同号
      if(compare(value, other.value) > 0){
        answer = bint_sub(value, other.value);
        answer.negative = negative;
        return answer;
      }else if(compare(value, other.value) < 0){
        answer = bint_sub(other.value, value);
        answer.negative = !negative;
        return answer;
      }else return bigint(0);
    }else{//异号
      answer = bint_add(value, other.value);
      answer.negative = negative;//异号两数相减，与被减数同号
      return answer;
    }
  }
  
  bigint operator-()const{//一元减
    bigint answer = *this;
    answer.negative = !(this -> negative);
    return answer;
  }
  
  bigint operator*(const bigint& other)const{
    bigint answer = bint_mtp(value, other.value);
    answer.negative = negative ^ other.negative;
    return answer;
  }
  
  bigint operator/(const bigint& other)const{
    bigint answer;
    if(compare(value, other.value)<0){
      answer = bigint(0);
    }else if(compare(value, other.value)==0){
      answer = bigint(1);
    }else{
      answer = bint_div(*this, other).first;
    }
    answer.negative = negative ^ other.negative;
    return answer;
  }
  
  bigint operator%(const bigint& other)const{
    bigint answer;
    if(compare(value, other.value)<0){
      answer = *this;
    }else if(compare(value, other.value)==0){
      answer = bigint(0);
    }else{
      answer = bint_div(*this, other).second;
    }
    answer.negative = negative;//^other.negative;
    return answer;
  }
  
  bool operator==(const bigint& other)const{
    if(negative != other.negative) return false;
    if(value.size() != other.value.size()) return false;
    for(size_t i=0; i<value.size(); ++i){
      if(value[i] != other.value[i]) return false;
    }
    return true;
  }
  
  bool operator==(const int a)const{
    return *this == bigint(a);
  }
  
  bool operator!=(const bigint& other)const{
    return !(*this == other);
  }
  
  bool operator!=(const int a)const{
    return !(*this == a);
  }
  //bigint pow(const int __a){}
  
  bigint pow(const bigint& __a){
    std::vector<int> zero(1,0), one(1,1), two(1,2);
    bigint __two, __fl, temp, __b  =__a, answer(1);
    temp.init_from_vect(value);
    std::pair<bigint, bigint> __t;
    __two.init_from_vect(two);
    while(compare(__b.value, zero)>0){//快速幂
      __t = bint_div(__b, __two);
      __b = __t.first;
      __fl = __t.second;
      //__b.dbg_show();__fl.dbg_show();
      if(compare(__fl.value, one)==0){
        answer = bint_mtp(answer.value, temp.value);
      }
      temp = bint_mtp(temp.value, temp.value);
      //temp.dbg_show();answer.dbg_show();
    }
    return answer;
  };
  
  bigint powmod(const bigint &__a,const bigint & _mod){
    std::vector<int> zero(1,0), one(1,1), two(1,2);
    bigint __two, __fl, temp, __b = __a, answer(1);
    temp.init_from_vect(value);
    std::pair<bigint, bigint> __t;
    __two.init_from_vect(two);
    while(compare(__b.value, zero) > 0){//快速幂
      __t = bint_div(__b, __two);
      __b = __t.first;
      __fl = __t.second;
      //__b.dbg_show();__fl.dbg_show();
      if(compare(__fl.value, one) == 0){
        answer = answer * temp;
        answer = answer % _mod;
      }
      temp = temp * temp;
      temp = temp % _mod;
      //temp.dbg_show();answer.dbg_show();
    }
    return answer;
  };
  
  int trans(void)const{//将较小数字转为int
    int t, base=1;
    for(auto a : value){
      t += a * base;
      base *= 10;
    }
    return t;
  }

  bigint mtp(const int __a)const{//与小int相乘
    bigint a(__a);
    return bint_mtp(value, a.value);
  }

};//class

  std::ostream& operator<<(std::ostream& _os, const bigint& __a){
    //__a.dbg_show();
    if(__a.negative && __a.value.size() != 0)_os << '-';
    size_t size = __a.value.size();
    for(int i = size-1; i >= 0; i--){
      _os << __a.value[i];
    }
    _os << std::endl;
    return _os;
  }
#endif