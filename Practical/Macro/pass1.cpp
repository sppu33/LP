#include<bits/stdc++.h>
using namespace std;

int main() {
    ifstream br("input.txt");
    ofstream mnt("mnt.txt");
    ofstream mdt("mdt.txt");
    ofstream kpdt("kpdt.txt");
    ofstream pnt("pntab.txt");
    ofstream ir("intermediate.txt");

    if (!br.is_open() || !mnt.is_open() || !mdt.is_open() || !kpdt.is_open() || !pnt.is_open() || !ir.is_open()) {
        cerr << "Error opening one or more files!" << endl;
        return 1;
    }

    map<string, int> pntab;
    string line;
    string Macroname;
    int mdtp = 1, kpdtp = 0, paramNo = 1, pp = 0, kp = 0, flag = 0;

    while (getline(br, line)) {
        istringstream iss(line);
        vector<string> parts((istream_iterator<string>(iss)), istream_iterator<string>());

        if (parts[0] == "MACRO" || parts[0] == "macro") {
            flag = 1;
            if (!getline(br, line)) break;
            istringstream iss2(line);
            vector<string> parts2((istream_iterator<string>(iss2)), istream_iterator<string>());
            Macroname = parts2[0];

            if (parts2.size() <= 1) {
                mnt << parts2[0] << "\t" << pp << "\t" << kp << "\t" << mdtp << "\t" << (kp == 0 ? kpdtp : (kpdtp + 1)) << "\n";
                continue;
            }

            for (size_t i = 1; i < parts2.size(); ++i) {
                string param = parts2[i];
                // param.erase(remove_if(param.begin(), param.end(), [](char c) { return c == '&' || c == ','; }), param.end());
                string result="";
                for (char c : param) {
                    if (c != '&' && c != ',') {
                        result += c;
                    }
                }
                param=result;

                if (param.find('=') != string::npos) {
                    ++kp;
                    size_t pos = param.find('=');
                    string keywordParam = param.substr(0, pos);
                    string value = param.substr(pos + 1);
                    pntab[keywordParam] = paramNo++;
                    kpdt << keywordParam << "\t" << value << "\n";
                } else {
                    pntab[param] = paramNo++;
                    ++pp;
                }
            }

            mnt << parts2[0] << "\t" << pp << "\t" << kp << "\t" << mdtp << "\t" << (kp == 0 ? kpdtp : (kpdtp + 1)) << "\n";
            kpdtp += kp;
            kp = 0;
            pp = 0;

        } else if (parts[0] == "MEND" || parts[0] == "mend") {
            mdt << line << "\n";
            flag = kp = pp = 0;
            ++mdtp;
            paramNo = 1;

            pnt << Macroname << ":\t";
            for (const auto& pair : pntab) {
                pnt << pair.first << "\t";
            }
            pnt << "\n";
            pntab.clear();

        } else if (flag == 1) {
            for (const auto& part : parts) {
                if (part.find('&') != string::npos) {
                    string param = part;
                    //param.erase(remove_if(param.begin(), param.end(), [](char c) { return c == '&' || c == ','; }), param.end());
                    string result="";
                    for (char c : param) {
                        if (c != '&' && c != ',') {
                            result += c;
                        }
                    }
                    param=result;
                    mdt << "(P," << pntab[param] << ")\t";
                } else {
                    mdt << part << "\t";
                }
            }
            mdt << "\n";
            ++mdtp;
        } else {
            ir << line << "\n";
        }
    }

    br.close();
    mnt.close();
    mdt.close();
    kpdt.close();
    pnt.close();
    ir.close();

    cout << "Macro Pass1 Processing done. :)" << endl;
    return 0;
}