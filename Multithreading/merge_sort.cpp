#include <vector>
#include <random>
#include <fstream>
#include <chrono>
#include <future>


struct RandomGenerator
{
    static std::vector<int> generateRandomVector(int N, int minVal = -10'000, int maxVal = 10'000)
    {
        std::vector<int> result(N);

        std::random_device rd;
        std::mt19937 gen(rd());

        std::uniform_int_distribution<int> dist(minVal, maxVal);

        for (int i = 0; i < N; ++i)
        {
            result[i] = dist(gen);
        }

        return result;
    }

};


struct MergeSort
{
private:
    template<typename T>
    static std::vector<T> merge(const std::vector<T>& left, const std::vector<T>& right)
    {
        std::vector<T> result;
        result.reserve(left.size() + right.size());

        auto itLeft = left.begin();
        auto itRight = right.begin();

        while (itLeft != left.end() && itRight != right.end())
        {
            if (*itLeft < *itRight)
            {
                result.push_back(*itLeft++);
            }
            else
            {
                result.push_back(*itRight++);
            }
        }

        result.insert(result.end(), itLeft, left.end());
        result.insert(result.end(), itRight, right.end());

        return result;
    }

public:
    template<typename T>
    static std::vector<T> mergeSort(const std::vector<T>& vec, int n_threads = 1)
    {
        if (vec.size() <= 1)
        {
            return vec;
        }

        auto middle = vec.size() / 2;
        std::vector<T> left(vec.begin(), vec.begin() + middle);
        std::vector<T> right(vec.begin() + middle, vec.end());

        if (n_threads > 1)
        {
            std::future<std::vector<T>> left_future = std::async(std::launch::async, mergeSort<T>, std::cref(left),
                                                                 n_threads / 2);
            std::future<std::vector<T>> right_future = std::async(std::launch::async, mergeSort<T>, std::cref(right),
                                                                  n_threads / 2);

            left = left_future.get();
            right = right_future.get();
        }
        else
        {
            left = mergeSort(left, 1);
            right = mergeSort(right, 1);
        }

        return merge(left, right);
    }
};


int main()
{
    const unsigned int MAX_THREADS = std::thread::hardware_concurrency();

    std::vector<int> vec = RandomGenerator::generateRandomVector(100'000);

    for (int n_threads = 1; n_threads < MAX_THREADS + 1; n_threads++)
    {
        auto start = std::chrono::high_resolution_clock::now();
        std::vector<int> merged_vec = MergeSort::mergeSort(vec, n_threads);
        auto stop = std::chrono::high_resolution_clock::now();

        auto duration = duration_cast<std::chrono::microseconds>(stop - start);
    }

    return 0;
}
