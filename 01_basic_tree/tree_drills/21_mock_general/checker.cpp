// checker.cpp: compares YOUR output with the EXPECTED output, exactly (like a judge would).
//
//   g++ -std=c++17 -o checker checker.cpp
//   ./checker your_output.txt expected_output.txt
//
// Spaces, blank lines and the newline at the very end all count. Only "\r" (Windows line endings) is ignored.
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

static bool readAll(const char *path, string &out)
{
    ifstream f(path, ios::binary);
    if (!f) return false;
    stringstream ss;
    ss << f.rdbuf();
    out.clear();
    for (char c : ss.str()) if (c != '\r') out += c;
    return true;
}

static vector<string> lines(const string &s)
{
    vector<string> v;
    string cur;
    for (char c : s)
    {
        if (c == '\n') { v.push_back(cur); cur.clear(); }
        else cur += c;
    }
    if (!cur.empty()) v.push_back(cur);
    return v;
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        cout << "usage: checker <your_output.txt> <expected_output.txt>" << endl;
        return 2;
    }
    string got, want;
    if (!readAll(argv[1], got))  { cout << "can't open " << argv[1] << endl; return 2; }
    if (!readAll(argv[2], want)) { cout << "can't open " << argv[2] << endl; return 2; }

    if (got == want) { cout << "PASS" << endl; return 0; }

    vector<string> g = lines(got), w = lines(want);
    size_t n = g.size() < w.size() ? g.size() : w.size();
    for (size_t i = 0; i < n; i++)
    {
        if (g[i] != w[i])
        {
            cout << "FAIL at line " << (i + 1) << endl;
            cout << "  yours:    [" << g[i] << "]" << endl;
            cout << "  expected: [" << w[i] << "]" << endl;
            return 1;
        }
    }
    if (g.size() < w.size())
    {
        cout << "FAIL: your output stops early. Missing line " << (g.size() + 1) << ": [" << w[g.size()] << "]" << endl;
        return 1;
    }
    if (g.size() > w.size())
    {
        cout << "FAIL: your output has extra lines. First extra line " << (w.size() + 1) << ": [" << g[w.size()] << "]" << endl;
        return 1;
    }
    cout << "FAIL: same lines, but the output ends differently (missing or extra newline at the very end)" << endl;
    return 1;
}
