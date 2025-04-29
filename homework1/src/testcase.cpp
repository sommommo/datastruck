#include "header.h"

// �H������ 0~n-1 ������
std::vector<int> generateRandomData(int n) {
    std::vector<int> data(n);
    for (int i = 0; i < n; ++i) data[i] = i;
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(data.begin(), data.end(), g);
    return data;
}

// ���Ƽg�iTXT��
void writeDataToFile(const std::string& filename, const std::vector<int>& data) {
    std::ofstream fout(filename);
    if (!fout) {
        std::cerr << "Failed to open file for writing.\n";
        return;
    }
    for (size_t i = 0; i < data.size(); ++i) {
        fout << data[i];
        if (i != data.size() - 1) fout << ' '; // �Ů���j
    }
    fout.close();
    std::cout << "File \"" << filename << "\" created successfully!\n";
}

int main() {
    int n;
    std::cout << "Enter number of elements: ";
    std::cin >> n;

    auto data = generateRandomData(n);

    std::string filename = "testcase" + std::to_string(n) + ".txt"; // �۰ʲեX�ɦW
    writeDataToFile(filename, data);

    return 0;
}
