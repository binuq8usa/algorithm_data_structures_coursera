#include <iostream>
#include <algorithm>
#include <vector>
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <utility>
#include <limits>

using namespace std;

typedef uint64_t CData;
typedef vector<CData>::iterator VecIt;

enum PivotIdx{
  START=0,
  END=1,
  MEDIAN=2
};

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

// returns index as -1
VecIt FindMedianOfThree(VecIt start, VecIt end){
  CData arr_len = end-start;
  auto mid = start + arr_len/2;
  if(arr_len % 2 == 0) mid--;
  CData small_val = std::numeric_limits<CData>::max();
  CData large_val = std::numeric_limits<CData>::min();
  if(small_val > *start) small_val = *start;
  if(small_val > *mid) small_val = *mid;
  if(small_val > *(end-1)) small_val = *(end-1);
  if(large_val < *start) large_val = *start;
  if(large_val < *mid) large_val = *mid;
  if(large_val < *(end-1)) large_val = *(end-1);
  if(*start > small_val && *start < large_val) return start;
  if(*mid > small_val && *mid < large_val) return mid;
  if(*(end-1) > small_val && *(end-1) < large_val) return (end-1);
}

void PrintNums(vector<CData>& nums){
  std::cout << "[";
  for(auto it = nums.begin(); it!= nums.end()-1; it++)
    std::cout << *it << ",";
  std::cout << *(nums.end()-1) << "]" << std::endl;
}

void PrintNums(VecIt start, VecIt end){
  std::cout << "[";
  for(auto it = start; it!= end-1; it++)
    std::cout << *it << ",";
  std::cout << *(end-1) << "]" << std::endl;
}

CData num_comparisons_total = 0;

CData QuickSort(VecIt start, VecIt end, PivotIdx& pivot_idx){
    // base cases - case 1 or case 2
    CData arr_len = end-start;
    num_comparisons_total += arr_len-1;
    if(arr_len == 2){
        if(*start > *(start+1)) iter_swap(start,start+1);
        return 1;
    }

    // swap if first element is not pivot idx
    if(pivot_idx == END) iter_swap(start,end-1);
    else if(pivot_idx == MEDIAN){
        vector<CData> nums;
        auto it_pivot = FindMedianOfThree(start,end);
        iter_swap(start,it_pivot);
    }
    else if(pivot_idx != START) return -1;
    CData pivot_val = *start;

    // get the split
    auto it_split = start+1;
    for(auto it = it_split; it!= end; it++)
        if(*it < pivot_val){// less than pivot value
            iter_swap(it,it_split);
            it_split++;
        }
    // place the pivot in the correct idx by swapping the first element with
    // index to the left of the it_split.
    iter_swap(start,it_split-1);
    CData num_left_comps = (it_split-1-start > 1)? QuickSort(start,it_split-1,pivot_idx) : 0;
    CData num_right_comps = (end-it_split > 1)? QuickSort(it_split,end,pivot_idx):0;
    return num_left_comps + num_right_comps + arr_len-1;
}

CData QuickSort(vector<CData>& nums, PivotIdx pivot_idx = START){
  if(nums.size() == 0) return 0;
  num_comparisons_total = 0;
  return QuickSort(nums.begin(), nums.end(), pivot_idx);
}

TEST_CASE("Find median of three","[FindMedianOfThree]"){
  SECTION("Test case 1"){
    vector<CData> nums{3,4,5};
    VecIt it_median = FindMedianOfThree(nums.begin(), nums.end());
    REQUIRE(*it_median == 4);
  }

  SECTION("Test case 2"){
    vector<CData> nums{3,5,4};
    VecIt it_median = FindMedianOfThree(nums.begin(), nums.end());
    REQUIRE(*it_median == 4);
  }

  SECTION("Test case 3"){
    vector<CData> nums{4,5,3};
    VecIt it_median = FindMedianOfThree(nums.begin(), nums.end());
    REQUIRE(*it_median == 4);
  }

  SECTION("Test case 4"){
    vector<CData> nums{4,3,5};
    VecIt it_median = FindMedianOfThree(nums.begin(), nums.end());
    REQUIRE(*it_median == 4);
  }

  SECTION("Test case 5"){
    vector<CData> nums{5,3,4};
    VecIt it_median = FindMedianOfThree(nums.begin(), nums.end());
    REQUIRE(*it_median == 4);
  }

  SECTION("Test case 6"){
    vector<CData> nums{5,4,3};
    VecIt it_median = FindMedianOfThree(nums.begin(), nums.end());
    REQUIRE(*it_median == 4);
  }

  SECTION("Test case from assignment"){
    vector<CData> nums{8,2,4,5,7,1};
    VecIt it_median = FindMedianOfThree(nums.begin(), nums.end());
    REQUIRE(*it_median == 4);
  }

  SECTION("Test case from quick sort"){
    vector<CData> nums{3,8,2,5,1,4,7,6};
    VecIt it_median = FindMedianOfThree(nums.begin(), nums.end());
    REQUIRE(*it_median == 5);
  }

  SECTION("Test case from quick sort-2"){
    vector<CData> nums{9,8,7,1,3,10,16,18,4,5};
    VecIt it_median = FindMedianOfThree(nums.begin(), nums.end());
    REQUIRE(*it_median == 5);
  }

  SECTION("Test case from quick sort-3"){
    vector<CData> nums{9,8,7,1,3,10,16,18,4,5,13};
    VecIt it_median = FindMedianOfThree(nums.begin(), nums.end());
    REQUIRE(*it_median == 10);
  }
}

TEST_CASE("Quick sort - 1st element as pivot","[QuickSort]"){
  SECTION("Simple test case"){
    vector<CData> nums{3,8,2,5,1,4,7,6};
    CData num_comps = QuickSort(nums);
    std::cout << "Num of comps : " << num_comps << std::endl;
    REQUIRE(nums == vector<CData>{1,2,3,4,5,6,7,8});
  }

  SECTION("Simple test case 2"){
    vector<CData> nums{9,8,7,1,3,10,16,18,4,5};
    CData num_comps = QuickSort(nums);
    std::cout << "Num of comps : " << num_comps << std::endl;
    REQUIRE(nums == vector<CData>{1,3,4,5,7,8,9,10,16,18});
  }

  SECTION("Simple test case 3"){
    vector<CData> nums{9,8,7,1,3,10,16,18,4,5,13};
    CData num_comps = QuickSort(nums);
    std::cout << "Num of comps : " << num_comps << std::endl;
    REQUIRE(nums == vector<CData>{1,3,4,5,7,8,9,10,13,16,18});
  }
}

TEST_CASE("Quick sort - Last element as pivot","[QuickSort]"){
  SECTION("Simple test case"){
    vector<CData> nums{3,8,2,5,1,4,7,6};
    CData num_comps = QuickSort(nums,END);
    std::cout << "Num of comps : " << num_comps << std::endl;
    REQUIRE(nums == vector<CData>{1,2,3,4,5,6,7,8});
  }

  SECTION("Simple test case 2"){
    vector<CData> nums{9,8,7,1,3,10,16,18,4,5};
    CData num_comps = QuickSort(nums, END);
    std::cout << "Num of comps : " << num_comps << std::endl;
    REQUIRE(nums == vector<CData>{1,3,4,5,7,8,9,10,16,18});
  }

  SECTION("Simple test case 3"){
    vector<CData> nums{9,8,7,1,3,10,16,18,4,5,13};
    CData num_comps = QuickSort(nums, END);
    std::cout << "Num of comps : " << num_comps << std::endl;
    REQUIRE(nums == vector<CData>{1,3,4,5,7,8,9,10,13,16,18});
  }
}

TEST_CASE("Quick sort - Median of three","[QuickSort]"){
  SECTION("Simple test case"){
    vector<CData> nums{3,8,2,5,1,4,7,6};
    CData num_comps = QuickSort(nums,MEDIAN);
    std::cout << "Num of comps : " << num_comps << std::endl;
    REQUIRE(nums == vector<CData>{1,2,3,4,5,6,7,8});
  }

  SECTION("Simple test case 2"){
    vector<CData> nums{9,8,7,1,3,10,16,18,4,5};
    CData num_comps = QuickSort(nums, MEDIAN);
    std::cout << "Num of comps : " << num_comps << std::endl;
    REQUIRE(nums == vector<CData>{1,3,4,5,7,8,9,10,16,18});
  }

  SECTION("Simple test case 3"){
    vector<CData> nums{9,8,7,1,3,10,16,18,4,5,13};
    CData num_comps = QuickSort(nums, MEDIAN);
    std::cout << "Num of comps : " << num_comps << std::endl;
    REQUIRE(nums == vector<CData>{1,3,4,5,7,8,9,10,13,16,18});
  }

  SECTION("Large set of elements - Case 1"){
      std::string filename = "../nums.txt";
      vector<CData> nums = readFromTextFile(filename);
      nums.pop_back();
      CData num_comps = QuickSort(nums);
      std::cout << "Num of comps (Pivot - Start) : " << num_comps << std::endl;
      std::cout << "Num of comps global : " << num_comparisons_total << std::endl;
      std::cout << "nums size : " << nums.size() << std::endl;
      vector<CData> outNums;
      for(CData i = 1 ; i <= 10000; i++)
          outNums.push_back(i);
      REQUIRE(nums == outNums);
  }

  SECTION("Large set of elements - Case 2"){
      std::string filename = "../nums.txt";
      vector<CData> nums = readFromTextFile(filename);
      nums.pop_back();
      CData num_comps = QuickSort(nums, END);
      std::cout << "Num of comps (Pivot - End) : " << num_comps << std::endl;
      std::cout << "Num of comps global : " << num_comparisons_total << std::endl;
      std::cout << "nums size : " << nums.size() << std::endl;
      vector<CData> outNums;
      for(CData i = 1 ; i <= 10000; i++)
          outNums.push_back(i);
      REQUIRE(nums == outNums);
  }

  SECTION("Large set of elements - Case 3"){
      std::string filename = "../nums.txt";
      vector<CData> nums = readFromTextFile(filename);
      nums.pop_back();
      CData num_comps = QuickSort(nums, MEDIAN);
      std::cout << "Num of comps (Pivot - Median) : " << num_comps << std::endl;
      std::cout << "Num of comps global : " << num_comparisons_total << std::endl;
      std::cout << "nums size : " << nums.size() << std::endl;
      vector<CData> outNums;
      for(CData i = 1 ; i <= 10000; i++)
          outNums.push_back(i);
      REQUIRE(nums == outNums);
  }
}
