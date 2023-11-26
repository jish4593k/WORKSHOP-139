#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cmath>

// Data structure to represent a point
struct Point {
    double age;
    double income;
    int cluster;

    Point(double a, double i) : age(a), income(i), cluster(-1) {}
};

// Function to calculate Euclidean distance between two points
double calculateDistance(const Point& p1, const Point& p2) {
    return sqrt(pow(p1.age - p2.age, 2) + pow(p1.income - p2.income, 2));
}

// Function to perform k-means clustering
void kMeansClustering(std::vector<Point>& data, int k) {
    // Initialize centroids randomly
    std::random_shuffle(data.begin(), data.end());
    for (int i = 0; i < k; ++i) {
        data[i].cluster = i;
    }

    // Perform k-means clustering
    bool changed = true;
    while (changed) {
        changed = false;

        // Assign each point to the nearest centroid
        for (Point& point : data) {
            int oldCluster = point.cluster;
            double minDistance = std::numeric_limits<double>::max();

            for (const Point& centroid : data) {
                double distance = calculateDistance(point, centroid);
                if (distance < minDistance) {
                    minDistance = distance;
                    point.cluster = centroid.cluster;
                }
            }

            if (oldCluster != point.cluster) {
                changed = true;
            }
        }

        // Update centroids
        for (int i = 0; i < k; ++i) {
            double sumAge = 0.0, sumIncome = 0.0;
            int count = 0;

            for (const Point& point : data) {
                if (point.cluster == i) {
                    sumAge += point.age;
                    sumIncome += point.income;
                    ++count;
                }
            }

            if (count > 0) {
                data[i].age = sumAge / count;
                data[i].income = sumIncome / count;
            }
        }
    }
}

int main() {
    // Read data from CSV file
    std::ifstream inputFile("/content/K MEANS2.csv");
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file." << std::endl;
        return 1;
    }

    std::vector<Point> data;
    std::string line;
    getline(inputFile, line);  // Skip header line
    while (getline(inputFile, line)) {
        std::istringstream ss(line);
        std::string token;

        double age, income;
        getline(ss, token, ',');
        age = std::stod(token);
        getline(ss, token, ',');
        income = std::stod(token);

        data.emplace_back(age, income);
    }

    // Perform k-means clustering
    int k = 2;  // Number of clusters
    kMeansClustering(data, k);

    // Plot the clusters
    for (const Point& point : data) {
        if (point.cluster == 0) {
            std::cout << point.age << "," << point.income << ",green" << std::endl;
        } else {
            std::cout << point.age << "," << point.income << ",red" << std::endl;
        }
    }

    return 0;
}
