#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */
#include<iostream>
#include<string>
#include<stdio.h>
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
        cout<< "hdt_file: "<<hdt_file<<endl;
        cout<< "classes_file: "<<classes_file<<endl;
        cout<< "files_dir: "<<files_dir<<endl;
        clock_t t=clock();
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
        t = clock() - t;
        printf ("It took %.2f seconds.\n",(static_cast<double>(t))/CLOCKS_PER_SEC);
//        printf ("It took me %ul clicks (%.2f seconds).\n",t,(static_cast<double>(t))/CLOCKS_PER_SEC);
//        printf ("It took me %d clicks (%.2f seconds).\n",t,((float)t)/CLOCKS_PER_SEC);
    }
    return 0;
}
