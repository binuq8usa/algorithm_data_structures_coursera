#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

using namespace std;

typedef uint32_t CData;
typedef vector<CData>::iterator VecIt;

vector<CData> left_nums;
vector<CData> right_nums;

vector<CData> readFromTextFile(string filename){
    fstream inFile;
    inFile.open(filename,std::ios_base::in);
    vector<CData> nums;
    while(!inFile.eof()){
        CData num = 0;
        inFile >> num;
        nums.push_back(num);
    }
    inFile.close();
    return nums;
}

void PrintNums(vector<CData>& nums){
  std::cout << "[";
  if(nums.size() > 0){
    for(auto it = nums.begin(); it!= nums.end()-1; it++)
      std::cout << *it << ",";
    std::cout << *(nums.end()-1) ;
  }
  std::cout << "]" <<std::endl;
}

void PrintNums(VecIt start, VecIt end){
  std::cout << "[";
  for(auto it = start; it!= end-1; it++)
    std::cout << *it << ",";
  std::cout << *(end-1) << "]" <<std::endl;
}

// merge sort - inplace
CData mergeSort(VecIt start, VecIt end){
  if(end-start <= 1) return 0;
  if(end-start <= 2){
    CData val = (CData)(*start > *(start+1));
    if(val) iter_swap(start,start+1);
    return val;
  }

  // divide into two
  VecIt mid = start + (end-start)/2;
  CData num_inversions_left = mergeSort(start,mid);
  CData num_inversions_right = mergeSort(mid,end);

  // create left and right array copies
  left_nums = vector<CData>(start,mid);
  right_nums = vector<CData>(mid,end);
  CData num_split_inversions = 0;
  for(auto it = start, it_left = left_nums.begin(), it_right = right_nums.begin() ; it!= end; it++){
    if(it_left == left_nums.end() && it_right != right_nums.end()){
      *it = *it_right;
      it_right++;
      continue;
    }
    if(it_right == right_nums.end() && it_left != left_nums.end()){
      *it = *it_left;
      it_left++;
      continue;
    }
    if(*it_left > *it_right){
      num_split_inversions += (left_nums.end() - it_left);
      *it = *it_right;
      it_right++;
    }
    else{
      *it = *it_left;
      it_left++;
    }
  }

  left_nums.clear();
  right_nums.clear();

  return num_split_inversions + num_inversions_left + num_inversions_right;
}

// sort array
 CData sortArray(vector<CData>& nums){
  if(nums.size() == 0) return 0;
  CData num_inversions = mergeSort(nums.begin(),nums.end());
  return num_inversions;
}

// Test cases for merge sort
TEST_CASE("Merge sort","[sortArray]"){
  SECTION("Leet code example 1"){
    vector<CData> nums{5,2,3,1};
    CData num_inversions = sortArray(nums);
    REQUIRE(nums == vector<CData>{1,2,3,5});
    REQUIRE(num_inversions == 5);
    PrintNums(nums);
  }

  SECTION("Leet code example 2"){
    vector<CData> nums{5,1,1,2,0,0};
    CData num_inversions = sortArray(nums);
    REQUIRE(nums == vector<CData>{0,0,1,1,2,5});
    REQUIRE(num_inversions == 11);
    PrintNums(nums);
  }

  SECTION("Odd number of terms"){
    vector<CData> nums{9,8,7,6,5,4,3,2,1};
    CData num_inversions = sortArray(nums);
    REQUIRE(nums == vector<CData>{1,2,3,4,5,6,7,8,9});
    REQUIRE(num_inversions == 36);
    PrintNums(nums);
  }

  SECTION("Repeated terms"){
    vector<CData> nums{9,8,7,6,6,4,3,5,1};
    CData num_inversions = sortArray(nums);
    REQUIRE(nums == vector<CData>{1,3,4,5,6,6,7,8,9});
    PrintNums(nums);
  }

  SECTION("Null array"){
    vector<CData> nums;
    CData num_inversions = sortArray(nums);
    REQUIRE(nums == vector<CData>{});
    PrintNums(nums);
  }

  SECTION("Single element"){
    vector<CData> nums{9};
    CData num_inversions = sortArray(nums);
    REQUIRE(nums == vector<CData>{9});
    PrintNums(nums);
  }

  SECTION("Large set of elements"){
      std::string filename = "../nums.txt";
      vector<CData> nums = readFromTextFile(filename);
      nums.pop_back();
      CData num_inversions = sortArray(nums);
      std::cout << "num inversions : " << num_inversions << std::endl;
      std::cout << "nums size : " << nums.size() << std::endl;
      REQUIRE(nums.size() == 100000);
  }
}
