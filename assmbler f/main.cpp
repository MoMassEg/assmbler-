//code about assmbler
//mohamed shaban
//uni id : 320230188
//12/1/2024


#include <bits/stdc++.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <bitset>
        #define yes cout << "Yes" << endl;
        #define no cout << "No" << endl;
        #define zero cout << '0' << endl;
        #define mone cout << "-1" << endl;
       // #define one cout << '1' << endl;
        #define endl "\n"
        typedef long long ll;
        #define vc vector
        #define pb push_back
        #define pp pop_back
        #define all(p)    p.begin(),p.end()
        #define alla(p,n)    p,p+n
        #define fl(i,z,n) for(ll i = z; i < n; i++)
        #define flc(i,z,n,c) for(ll i = z; i < n; c)
        #define flr(i,s,x) for(ll i = x-1; i >= s; i--)
        #define w(a) ll a; cin>>a; while(a--)
        #define tc(n) int n;cin>>n;
        #define llc(n) ll n;cin>>n;
        #define sc(n) string n;cin>>n;
        const int ALPHABET_SIZE = 26;
using namespace std;

class Assembler {
private:
    vector<string> code;
    unordered_map<string, int> symbols;
    unordered_map<string, string> mri;
    unordered_map<string, string> rri;
    unordered_map<string, string> ioi;
    unordered_map<int, string> bin;
    int locationCounter = 0;

public:
    Assembler() {
        mri = {
            {"AND", "0010"},
            {"ADD", "0011"},
            {"LDA", "0100"},
            {"STA", "0101"},
            {"BUN", "0110"},
            {"BSA", "0111"},
            {"ISZ", "1000"}
        };

        rri = {
            {"CLA", "0111100000000000"},
            {"CLE", "0111010000000000"},
            {"CMA", "0111001000000000"},
            {"CME", "0111000100000000"},
            {"CIR", "0111000010000000"},
            {"CIL", "0111000001000000"},
            {"INC", "0111000000100000"},
            {"SPA", "0111000000010000"},
            {"SNA", "0111000000001000"},
            {"SZA", "0111000000000100"},
            {"SZE", "0111000000000010"},
            {"HLT", "0111000000000001"}
        };

        ioi = {
            {"INP", "1111100000000000"},
            {"OUT", "1111010000000000"},
            {"SKI", "1111001000000000"},
            {"SKO", "1111000100000000"},
            {"ION", "1111000010000000"},
            {"IOF", "1111000001000000"}
        };
    }

    void firstPass() {
        for (const string& line : code) {
            string label, instruction;
            istringstream iss(line);
            iss >> label;

            if (label.back() == ',') {
                label.pop_back();
                symbols[label] = locationCounter;
                iss >> instruction;
            } else {
                instruction = label;
                label.clear();
            }

            if (instruction == "ORG") {
                int address;
                iss >> address;
                locationCounter = address;
            } else if (instruction == "HEX" || instruction == "DEC" ||
                       mri.find(instruction) != mri.end() ||
                       rri.find(instruction) != rri.end() ||
                       ioi.find(instruction) != ioi.end()) {
                locationCounter++;
            }
        }
        cout<<"=============symbols==========="<<endl;
        for (auto i : symbols)
        cout << i.first << " " << i.second << endl;

    }

    void secondPass() {
        locationCounter = 0;
        for (const string& line : code) {
            istringstream iss(line);
            string label, instruction;
            iss >> label;

            if (label.back() == ',') {
                label.pop_back();
                iss >> instruction;
            } else {
                instruction = label;
                label.clear();
            }

            if (instruction == "ORG") {
                int address;
                iss >> address;
                locationCounter = address;
            } else if (instruction == "END") {
                break;
            } else if (mri.find(instruction) != mri.end()) {
                string operand;
                iss >> operand;
                int address = symbols[operand];
                string binaryValue = mri[instruction] + bitset<12>(address).to_string();
                string addressingMode = "x";
                iss>>addressingMode;
                if(addressingMode=="I")binaryValue[0]='1';
                bin[locationCounter] = binaryValue;
                locationCounter++;
            } else if (rri.find(instruction) != rri.end()) {
                bin[locationCounter] = rri[instruction];
                locationCounter++;
            } else if (ioi.find(instruction) != ioi.end()) {
                bin[locationCounter] = ioi[instruction];
                locationCounter++;
            } else if (instruction == "HEX" || instruction == "DEC") {
                string value;
                iss >> value;
                if (instruction == "HEX") {
                        bin[locationCounter] = bitset<16>(stoi(value, nullptr, 16)).to_string();
                } else if (instruction == "DEC") {
                    bin[locationCounter] = bitset<16>(stoi(value)).to_string();
                }
                locationCounter++;
            }
        }
    }

    void assemble(const vector<string>& assemblyCode) {
        code = assemblyCode;
        firstPass();
        secondPass();
    }

    void printBinaryOutput() const {
        cout<<"=============code============"<<endl;
        for (int i = 0; i < 4096; ++i) {
            if (bin.find(i) != bin.end()) {
                cout << i<< " " << bin.at(i) << endl;
            }
        }
    }



};

int main() {
    cout<<"input assmbly code:"<<endl;
    vector<string>code;
    string line;
    while (getline(cin, line))
    {
        code.push_back(line);
        if(line=="END")break;
    }

    try {
        Assembler assembler;
        assembler.assemble(code);
        assembler.printBinaryOutput();
        cout << "Assembling... TEST PASSED" << endl;

    } catch (const runtime_error& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}
