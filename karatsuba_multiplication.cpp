#include <iostream>
#include <vector>
#include <cassert>
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

using namespace std;

// using Karatsuba's multiplication
// assumption number of digits are power of 2
void print_vector(vector<int>& nums1, string label){
  std::cout << "Size of " << label << " : " << nums1.size() << "\t" << "[";
  for(int i = 0 ; i < nums1.size()-1; i++)
    std::cout << nums1[i];
  std::cout << nums1[nums1.size()-1] << "]" << std::endl;
}

void reverseSign(vector<int>& nums1){
  for(auto it = nums1.begin(); it!= nums1.end(); it++)
    *it = -1 * (*it);
  return;
}

vector<int> addTwoNumbers(vector<int>& nums1, vector<int>& nums2){
  vector<int> sum_val(std::max(nums1.size(),nums2.size()),0);
  auto r_it_1 = nums1.rbegin();
  auto r_it_2 = nums2.rbegin();
  auto r_it_out = sum_val.rbegin();
  
  int carry = 0;
  while(1){
    if(r_it_1 == nums1.rend() && r_it_2 == nums2.rend())
      break;
    int val = carry;
    if(r_it_1 != nums1.rend()) {
      val += *r_it_1;
      r_it_1++;
    }
    if(r_it_2 != nums2.rend()) {
      val += *r_it_2;
      r_it_2++;
    }
    *r_it_out = val % 10;
    carry = val / 10;
    r_it_out++;
  }
  if(carry == 1) sum_val.insert(sum_val.begin(),carry);
  return sum_val;
}

vector<int> multiplyTwoNumbers(vector<int>& nums1, vector<int>& nums2){
  vector<int> out_val;
  if(nums1.size() == 1 && nums2.size()==1){// single digit
    assert(nums1[0] >= 0);
    assert(nums2[0] >= 0); 
    int val = nums1[0]*nums2[0];
    if(val/10 != 0)
      out_val.push_back(val/10);
    out_val.push_back(val%10);
    return out_val; // return two digit number
  } 

  vector<int> a(nums1.begin(),nums1.begin()+nums1.size()/2);
  vector<int> b(nums1.begin()+nums1.size()/2, nums1.end());
  vector<int> c(nums2.begin(),nums2.begin()+nums2.size()/2);
  vector<int> d(nums2.begin()+nums2.size()/2, nums2.end());
  
  int factor = std::min(nums1.size(),nums2.size())/2;
  
  // compute products
  vector<int> ac = multiplyTwoNumbers(a,c); // ac
  vector<int> bd = multiplyTwoNumbers(b,d); // bd
  vector<int> ad = multiplyTwoNumbers(a,d); // ad
  vector<int> bc = multiplyTwoNumbers(b,c); // bc
  
  vector<int> ad_bc = addTwoNumbers(ad,bc);
  
  // compute 1st, 2nd and 3rd terms
  vector<int> first_term = ac;
  for(int i = 0 ; i < 2*factor; i++)
    first_term.push_back(0);
  
  vector<int> second_term = ad_bc;
  for(int i = 0 ; i < factor; i++)
    second_term.push_back(0);
  
  vector<int> third_term = bd;
  
  // perform addition - 10^(2m) * ac + 10^m * (ad + bc) + bd
  vector<int> first_second_terms = addTwoNumbers(first_term,second_term);
  out_val = addTwoNumbers(first_second_terms,third_term);
  
  return out_val;
}

// TEST cases
TEST_CASE("Reverse Sign","[reverseSign]"){
  SECTION("Single"){
    vector<int> nums1{1};
    reverseSign(nums1);
    REQUIRE(nums1[0]==-1);
  }
  SECTION("Two Element"){
    vector<int> nums1{1,2};
    reverseSign(nums1);
    REQUIRE(nums1==vector<int>{-1,-2});
  }
  
  SECTION("Three Element"){
    vector<int> nums1{1,2,-4};
    reverseSign(nums1);
    REQUIRE(nums1==vector<int>{-1,-2,4});
  }
  
  SECTION("Four Element"){
    vector<int> nums1{1,2,-200,0};
    reverseSign(nums1);
    REQUIRE(nums1==vector<int>{-1,-2,200,0});
  }
}

TEST_CASE("Add Two Numbers","[addTwoNumbers]"){
  SECTION("Single"){
    vector<int> nums1{1};
    vector<int> nums2{2};
    vector<int> sum_val = addTwoNumbers(nums1,nums2);
    REQUIRE(sum_val==vector<int>{3});
  }
  
  SECTION("Two Digit Number"){
    vector<int> nums1{1,2};
    vector<int> nums2{3,4};
    vector<int> sum_val = addTwoNumbers(nums1,nums2);
    REQUIRE(sum_val==vector<int>{4,6});
  }
  
  SECTION("Three Digit Number with overflow in between"){
    vector<int> nums1{1,2,3};
    vector<int> nums2{4,5,7};
    vector<int> sum_val = addTwoNumbers(nums1,nums2);
    REQUIRE(sum_val==vector<int>{5,8,0});
  }
  
  SECTION("Three Digit Number with overflow through out"){
    vector<int> nums1{9,9,9};
    vector<int> nums2{9,9,9};
    vector<int> sum_val = addTwoNumbers(nums1,nums2);
    REQUIRE(sum_val==vector<int>{1,9,9,8});
  }
  
  SECTION("Four digit numbers with zero"){
    vector<int> nums1{9,0,9};
    vector<int> nums2{3,9,9};
    vector<int> sum_val = addTwoNumbers(nums1,nums2);
    REQUIRE(sum_val==vector<int>{1,3,0,8});
  }
  
}

TEST_CASE("Multiply Two Numbers","[multiplyTwoNumbers]"){
  
  SECTION("Multiply single digit numbers"){
    vector<int> nums1{3};
    vector<int> nums2{2};
    vector<int> mul_val = multiplyTwoNumbers(nums1,nums2);
    REQUIRE(mul_val == vector<int>{6});
  }
  
  SECTION("Multiply single digit numbers - 2"){
    vector<int> nums1{3};
    vector<int> nums2{4};
    vector<int> mul_val = multiplyTwoNumbers(nums1,nums2);
    REQUIRE(mul_val == vector<int>{1,2});
  }
  
  SECTION("Multiply single digit numbers with zero"){
    vector<int> nums1{3};
    vector<int> nums2{0};
    vector<int> mul_val = multiplyTwoNumbers(nums1,nums2);
    REQUIRE(mul_val == vector<int>{0});
  }
  
  SECTION("Multiply two digit numbers - equal size, no carry"){
    vector<int> nums1{1,2};
    vector<int> nums2{3,4};
    vector<int> mul_val = multiplyTwoNumbers(nums1,nums2);
    REQUIRE(mul_val == vector<int>{4,0,8});
  }
  
  SECTION("Multiply four digit numbers - equal size, no carry"){
    vector<int> nums1{1,2,3,4};
    vector<int> nums2{5,6,7,8};
    vector<int> mul_val = multiplyTwoNumbers(nums1,nums2);
    REQUIRE(mul_val == vector<int>{7,0,0,6,6,5,2});
  }
  
  SECTION("Multiply 8 digit numbers"){
    vector<int> nums1{9,9,9,9,9,9,9,9};
    vector<int> nums2{8,8,8,8,8,8,8,8};
    vector<int> mul_val = multiplyTwoNumbers(nums1,nums2);
    REQUIRE(mul_val == vector<int>{8,8,8,8,8,8,8,7,
                                   1,1,1,1,1,1,1,2});
  }
  
  SECTION("Multiply 64 digit numbers - DS algo question"){
    vector<int> nums1{3,1,4,1,5,9,2,6,5,3,5,8,9,7,9,3,2,3,8,4,6,2,6,4,3,3,8,3,2,7,9,5,0,2,8,8,4,1,9,7,1,6,9,3,9,9,3,7,5,1,0,5,8,2,0,9,7,4,9,4,4,5,9,2};
    vector<int> nums2{2,7,1,8,2,8,1,8,2,8,4,5,9,0,4,5,2,3,5,3,6,0,2,8,7,4,7,1,3,5,2,6,6,2,4,9,7,7,5,7,2,4,7,0,9,3,6,9,9,9,5,9,5,7,4,9,6,6,9,6,7,6,2,7};
    vector<int> mul_val = multiplyTwoNumbers(nums1,nums2);
    REQUIRE(mul_val.size()>0);
    print_vector(mul_val,"answer");
  }
}
