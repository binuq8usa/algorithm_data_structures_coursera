#include <iostream>
#include <algorithm>
#include <vector>
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <utility>
#include <limits>

using namespace std;

typedef uint16_t CData;

enum PivotIdx{
  START=0,
  END=1,
  MEDIAN=2,
  RANDOM=3
};

std::default_random_engine random_generator;
CData num_comparisons_total = 0;

template <class T>
struct IsWithinRange{
  T a_;
  T b_;
  IsWithinRange(const T a, const T b)
    :a_(a), b_(b) {}
  bool operator()(const T key){
    return (key >= a_ && key <= b_);
  }
};

template <class T>
struct IsOutOfRange{
  T a_;
  T b_;
  IsOutOfRange(const T a, const T b)
    :a_(a), b_(b) {}
  bool operator()(const T key){
    return (key < a_ && key > b_);
  }
};

template <typename T>
vector<T> readFromTextFile(string filename){
    fstream inFile;
    inFile.open(filename,std::ios_base::in);
    vector<T> nums;
    while(!inFile.eof()){
        T num = 0;
        inFile >> num;
        nums.push_back(num);
    }
    inFile.close();
    return nums;
}

// returns index as -1
template <typename T>
typename vector<T>::iterator FindMedianOfThree(typename vector<T>::iterator start, typename vector<T>::iterator end){
  T arr_len = end-start;
  auto mid = start + arr_len/2;
  if(arr_len % 2 == 0) mid--;
  T small_val = std::numeric_limits<T>::max();
  T large_val = std::numeric_limits<T>::min();
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

template <typename T>
void PrintNums(vector<T>& nums){
  std::cout << "[";
  for(auto it = nums.begin(); it!= nums.end()-1; it++)
    std::cout << *it << ",";
  std::cout << *(nums.end()-1) << "]" << std::endl;
}

template <typename T>
void PrintNums(typename vector<T>::iterator start, typename vector<T>::iterator end){
  std::cout << "[";
  if(start-end == 0) {
    std::cout << ",]" << std::endl;
    return;
  }
  for(auto it = start; it!= end-1; it++)
    std::cout << *it << ",";
  std::cout << *(end-1) << "]" << std::endl;
}

template <class T>
T GetRandomValue(T a, T b){
  std::uniform_int_distribution<T> distribution(a,b);
  return distribution(random_generator);
}

template <typename T>
typename vector<T>::iterator PartitionArray(typename vector<T>::iterator start, typename vector<T>::iterator end){
    T pivot_val = *start;
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
    return it_split;
}

template <class T>
T QuickSort(typename vector<T>::iterator start, typename vector<T>::iterator end, PivotIdx& pivot_idx){
  // base cases - case 1 or case 2
  T arr_len = end-start;
  if(arr_len == 2){
    if(*start > *(start+1)) iter_swap(start,start+1);
    return 1;
  }
  // swap if first element is not pivot idx
  if(pivot_idx == END) iter_swap(start,end-1);
  else if(pivot_idx == MEDIAN){
    auto it_pivot = FindMedianOfThree<T>(start,end);
    iter_swap(start,it_pivot);
  }
  else if(pivot_idx == RANDOM){
    auto it_pivot = start + GetRandomValue<T>(0,end-start-1);
    iter_swap(start,it_pivot);
  }
  else if(pivot_idx != START) return -1;
  auto it_split = PartitionArray<T>(start,end);
  T num_left_comps = (it_split-1-start > 1)?
    QuickSort<T>(start,it_split-1,pivot_idx):0;
  T num_right_comps = (end-it_split > 1)?
    QuickSort<T>(it_split,end,pivot_idx):0;
  return num_left_comps + num_right_comps + arr_len-1;
}

template <typename T>
T QuickSort(vector<T>& nums, PivotIdx pivot_idx = START){
  if(nums.size() == 0) return 0;
  return QuickSort<T>(nums.begin(), nums.end(), pivot_idx);
}

template <class T>
T FindIthOrderStatistic(typename vector<T>::iterator start, typename vector<T>::iterator end, T ith){
  auto it_pivot = start + GetRandomValue<T>(0,end-start-1);
  iter_swap(start,it_pivot);
  auto it_split = PartitionArray<T>(start,end);
  auto it_ith_order_stat = start + ith;

  if(it_split-1 == it_ith_order_stat)
    return *(it_split-1);
  else if(it_split-1 > it_ith_order_stat)
    return FindIthOrderStatistic<T>(start,it_split-1,ith);
  else if(it_split-1 < it_ith_order_stat)
    return FindIthOrderStatistic<T>(it_split,end,ith-(it_split-start));
}

template <typename T>
T FindMedian(vector<T>& nums){
  T ith_order = (nums.size()%2 == 0)? nums.size()/2-1: nums.size()/2;
  return FindIthOrderStatistic<T>(nums.begin(), nums.end(), ith_order);
}

TEST_CASE("Random number generation","[GetRandomValue]"){
  CData a = 0;
  CData b = 9;
  vector<CData> gen_values;
  for(int i = 0; i < 100; i++)
    gen_values.push_back(GetRandomValue<CData>(a,b));

  REQUIRE(std::all_of(gen_values.begin(), gen_values.end(),IsWithinRange<CData>(a,b))==true);
  REQUIRE(std::any_of(gen_values.begin(),gen_values.end(),IsOutOfRange<CData>(a,b))==false);
}

TEST_CASE("Find median of three","[FindMedianOfThree]"){
  SECTION("Test case 1"){
    vector<CData> nums{3,4,5};
    vector<CData>::iterator it_median = FindMedianOfThree<CData>(nums.begin(), nums.end());
    REQUIRE(*it_median == 4);
  }

  SECTION("Test case 2"){
    vector<CData> nums{3,5,4};
    vector<CData>::iterator it_median = FindMedianOfThree<CData>(nums.begin(), nums.end());
    REQUIRE(*it_median == 4);
  }

  SECTION("Test case 3"){
    vector<CData> nums{4,5,3};
    vector<CData>::iterator it_median = FindMedianOfThree<CData>(nums.begin(), nums.end());
    REQUIRE(*it_median == 4);
  }

  SECTION("Test case 4"){
    vector<CData> nums{4,3,5};
    vector<CData>::iterator it_median = FindMedianOfThree<CData>(nums.begin(), nums.end());
    REQUIRE(*it_median == 4);
  }

  SECTION("Test case 5"){
    vector<CData> nums{5,3,4};
    vector<CData>::iterator it_median = FindMedianOfThree<CData>(nums.begin(), nums.end());
    REQUIRE(*it_median == 4);
  }

  SECTION("Test case 6"){
    vector<CData> nums{5,4,3};
    vector<CData>::iterator it_median = FindMedianOfThree<CData>(nums.begin(), nums.end());
    REQUIRE(*it_median == 4);
  }

  SECTION("Test case from assignment"){
    vector<CData> nums{8,2,4,5,7,1};
    vector<CData>::iterator it_median = FindMedianOfThree<CData>(nums.begin(), nums.end());
    REQUIRE(*it_median == 4);
  }

  SECTION("Test case from quick sort"){
    vector<CData> nums{3,8,2,5,1,4,7,6};
    vector<CData>::iterator it_median = FindMedianOfThree<CData>(nums.begin(), nums.end());
    REQUIRE(*it_median == 5);
  }

  SECTION("Test case from quick sort-2"){
    vector<CData> nums{9,8,7,1,3,10,16,18,4,5};
    vector<CData>::iterator it_median = FindMedianOfThree<CData>(nums.begin(), nums.end());
    REQUIRE(*it_median == 5);
  }

  SECTION("Test case from quick sort-3"){
    vector<CData> nums{9,8,7,1,3,10,16,18,4,5,13};
    vector<CData>::iterator it_median = FindMedianOfThree<CData>(nums.begin(), nums.end());
    REQUIRE(*it_median == 10);
  }
}

TEST_CASE("Quick sort - 1st element as pivot","[QuickSort]"){
  SECTION("Simple test case"){
    vector<CData> nums{3,8,2,5,1,4,7,6};
    CData num_comps = QuickSort<CData>(nums);
    std::cout << "Num of comps : " << num_comps << std::endl;
    REQUIRE(nums == vector<CData>{1,2,3,4,5,6,7,8});
  }

  SECTION("Simple test case 2"){
    vector<CData> nums{9,8,7,1,3,10,16,18,4,5};
    CData num_comps = QuickSort<CData>(nums);
    std::cout << "Num of comps : " << num_comps << std::endl;
    REQUIRE(nums == vector<CData>{1,3,4,5,7,8,9,10,16,18});
  }

  SECTION("Simple test case 3"){
    vector<CData> nums{9,8,7,1,3,10,16,18,4,5,13};
    CData num_comps = QuickSort<CData>(nums);
    std::cout << "Num of comps : " << num_comps << std::endl;
    REQUIRE(nums == vector<CData>{1,3,4,5,7,8,9,10,13,16,18});
  }
}

TEST_CASE("Quick sort - Last element as pivot","[QuickSort]"){
  SECTION("Simple test case"){
    vector<CData> nums{3,8,2,5,1,4,7,6};
    CData num_comps = QuickSort<CData>(nums,END);
    std::cout << "Num of comps : " << num_comps << std::endl;
    REQUIRE(nums == vector<CData>{1,2,3,4,5,6,7,8});
  }

  SECTION("Simple test case 2"){
    vector<CData> nums{9,8,7,1,3,10,16,18,4,5};
    CData num_comps = QuickSort<CData>(nums, END);
    std::cout << "Num of comps : " << num_comps << std::endl;
    REQUIRE(nums == vector<CData>{1,3,4,5,7,8,9,10,16,18});
  }

  SECTION("Simple test case 3"){
    vector<CData> nums{9,8,7,1,3,10,16,18,4,5,13};
    CData num_comps = QuickSort<CData>(nums, END);
    std::cout << "Num of comps : " << num_comps << std::endl;
    REQUIRE(nums == vector<CData>{1,3,4,5,7,8,9,10,13,16,18});
  }
}

TEST_CASE("Quick sort - Median of three","[QuickSort]"){
  SECTION("Simple test case"){
    vector<CData> nums{3,8,2,5,1,4,7,6};
    CData num_comps = QuickSort<CData>(nums,MEDIAN);
    std::cout << "Num of comps : " << num_comps << std::endl;
    REQUIRE(nums == vector<CData>{1,2,3,4,5,6,7,8});
  }

  SECTION("Simple test case 2"){
    vector<CData> nums{9,8,7,1,3,10,16,18,4,5};
    CData num_comps = QuickSort<CData>(nums, MEDIAN);
    std::cout << "Num of comps : " << num_comps << std::endl;
    REQUIRE(nums == vector<CData>{1,3,4,5,7,8,9,10,16,18});
  }

  SECTION("Simple test case 3"){
    vector<CData> nums{9,8,7,1,3,10,16,18,4,5,13};
    CData num_comps = QuickSort<CData>(nums, MEDIAN);
    std::cout << "Num of comps : " << num_comps << std::endl;
    REQUIRE(nums == vector<CData>{1,3,4,5,7,8,9,10,13,16,18});
  }
}

TEST_CASE("Quick sort - Random Pivot","[QuickSort]"){
  SECTION("Simple test case"){
    vector<CData> nums{3,8,2,5,1,4,7,6};
    CData num_comps = QuickSort<CData>(nums,RANDOM);
    std::cout << "Num of comps : " << num_comps << std::endl;
    REQUIRE(nums == vector<CData>{1,2,3,4,5,6,7,8});
  }

  SECTION("Simple test case 2"){
    vector<CData> nums{9,8,7,1,3,10,16,18,4,5};
    CData num_comps = QuickSort<CData>(nums, RANDOM);
    std::cout << "Num of comps : " << num_comps << std::endl;
    REQUIRE(nums == vector<CData>{1,3,4,5,7,8,9,10,16,18});
  }

  SECTION("Simple test case 3"){
    vector<CData> nums{9,8,7,1,3,10,16,18,4,5,13};
    CData num_comps = QuickSort<CData>(nums, RANDOM);
    std::cout << "Num of comps : " << num_comps << std::endl;
    REQUIRE(nums == vector<CData>{1,3,4,5,7,8,9,10,13,16,18});
  }
}

TEST_CASE("Quick sort - Algo data course cases","[QuickSort]"){
  SECTION("Large set of elements - Case 1"){
      std::string filename = "../nums.txt";
      vector<CData> nums = readFromTextFile<CData>(filename);
      nums.pop_back();
      CData num_comps = QuickSort<CData>(nums);
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
      vector<CData> nums = readFromTextFile<CData>(filename);
      nums.pop_back();
      CData num_comps = QuickSort<CData>(nums, END);
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
      vector<CData> nums = readFromTextFile<CData>(filename);
      nums.pop_back();
      CData num_comps = QuickSort<CData>(nums, MEDIAN);
      std::cout << "Num of comps (Pivot - Median) : " << num_comps << std::endl;
      std::cout << "Num of comps global : " << num_comparisons_total << std::endl;
      std::cout << "nums size : " << nums.size() << std::endl;
      vector<CData> outNums;
      for(CData i = 1 ; i <= 10000; i++)
          outNums.push_back(i);
      REQUIRE(nums == outNums);
  }

  SECTION("Large set of elements - Case 4"){
      std::string filename = "../nums.txt";
      vector<CData> nums = readFromTextFile<CData>(filename);
      nums.pop_back();
      CData num_comps = QuickSort<CData>(nums, RANDOM);
      std::cout << "Num of comps (Pivot - Random) : " << num_comps << std::endl;
      std::cout << "Num of comps global : " << num_comparisons_total << std::endl;
      std::cout << "nums size : " << nums.size() << std::endl;
      vector<CData> outNums;
      for(CData i = 1 ; i <= 10000; i++)
          outNums.push_back(i);
      REQUIRE(nums == outNums);
  }
}

TEST_CASE("Find median of array using linear scan","[FindMedian]"){
  SECTION("Simple test case"){
    vector<CData> nums{3,8,2,5,1,4,7,6};
    REQUIRE(FindMedian<CData>(nums) == 4);
  }

  SECTION("Simple test case"){
    vector<CData> nums{3,2,5,1,4,7,6};
    REQUIRE(FindMedian<CData>(nums) == 4);
  }
}
