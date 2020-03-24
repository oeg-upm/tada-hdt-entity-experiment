#include<iostream>
#include<string>
#include "t2dv2.h"

using namespace std;

int main(int argc, char* argv[]) {
    long k;
    string hdt_file, classes_file, files_dir, log_file="main.log";
    if(argc<4) {
        cerr << "Usage: " << argv[0] << "hdt_file classes_file files_dir " << std::endl;
    }
    else {
        hdt_file = argv[1];
        classes_file = argv[2];
        files_dir = argv[3];
        T2Dv2* t2d = new T2Dv2(hdt_file, log_file, classes_file, files_dir);
        t2d->run_test(0.0, 1.0, 0.05);
        k=0;
        cout << "====> K = " <<k+1<<endl;
        t2d->compute_scores(k);
        k+=2;
        cout << "====> K = " <<k+1<<endl;
        t2d->compute_scores(k);
        k+=2;
        cout << "====> K = " <<k+1<<endl;
        t2d->compute_scores(k);
    }
    return 0;
}
