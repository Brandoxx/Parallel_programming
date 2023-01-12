#include <iostream>
#include <vector>
#include <algorithm>
#include <execution>
#include <cmath>
#include <omp.h>

int main()
{
    std::vector<float> numbersToStat;
    std::cout << "Enter a list of numbers, followed by Ctrl+D:" << std::endl;
    float num;
    while (std::cin >> num)
    {
        numbersToStat.push_back(num);
    }
    if (numbersToStat.size() == 0)
    {
        std::cout << "No numbers entered." << std::endl;
        return 0;
    }
    float minimum = numbersToStat[0];
    float maximum = numbersToStat[0];
    float average = 0;
    float standardDeviation = 0;
    float median = 0;
    std::sort(std::execution::par, numbersToStat.begin(), numbersToStat.end());
    #pragma omp parallel for reduction(min:minimum) reduction(max:maximum) reduction(+:average) reduction(+:standardDeviation)
    for (auto i = 0; i < numbersToStat.size(); i++)
    {
        if (numbersToStat[i] < minimum)
        {
            minimum = numbersToStat[i];
        }
        if (numbersToStat[i] > maximum)
        {
            maximum = numbersToStat[i];
        }
        average += numbersToStat[i]/numbersToStat.size();
        standardDeviation += (numbersToStat[i] - average) * (numbersToStat[i] - average);
    }
    standardDeviation = std::sqrt(standardDeviation/(numbersToStat.size()-1));

    std::cout << "Minimum: " << minimum << std::endl;
    std::cout << "Maximum: " << maximum << std::endl;
    std::cout << "Median: " << numbersToStat[(numbersToStat.size()/2) - 1] << std::endl;
    std::cout << "Average: " << average << std::endl;
    std::cout << "Standard Deviation: " << standardDeviation << std::endl;
    return 0;
}
