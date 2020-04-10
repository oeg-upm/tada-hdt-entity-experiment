#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */
#include<iostream>
#include<string>
#include<stdio.h>
#include "t2dv2.h"
#include <tabular_parser/parser.h>

using namespace std;


int main(int argc, char* argv[]) {
    long k;
    char mode;
    string hdt_file, classes_file, files_dir, properties_file, log_file="main.log", mode_str;
    if(argc<4) {
        cerr << "Usage: " << argv[0] << "hdt_file classes_file files_dir " << std::endl;
        cerr << "Or: " << argv[0] << "hdt_file classes_file files_dir properties_file mode " << std::endl;
    }
    else if(argc==4) {
        hdt_file = argv[1];
        classes_file = argv[2];
        files_dir = argv[3];
        cout<< "hdt_file: "<<hdt_file<<endl;
        cout<< "classes_file: "<<classes_file<<endl;
        cout<< "files_dir: "<<files_dir<<endl;
        clock_t t=clock();
        T2Dv2* t2d = new T2Dv2(hdt_file, log_file, classes_file, files_dir);
        t2d->set_lang_tag("@en");
        t2d->set_title_case(true);
//        t2d->set_title_case(true);
//        t2d->set_inner_context(false);
        t2d->set_inner_context(true);
//        t2d->run_test(0.0, 1.0, 0.05);
        t2d->run_test(0.0, 1.0, 0.01);
        k=0;
        cout << "====> K = " <<k+1<<endl;
        t2d->compute_scores(k);
        k+=2;
        cout << "====> K = " <<k+1<<endl;
        t2d->compute_scores(k);
        k+=2;
        cout << "====> K = " <<k+1<<endl;
        t2d->compute_scores(k);
//        t2d->print_k();
        t = clock() - t;
        printf ("It took %.2f seconds.\n",(static_cast<double>(t))/CLOCKS_PER_SEC);
//        printf ("It took me %ul clicks (%.2f seconds).\n",t,(static_cast<double>(t))/CLOCKS_PER_SEC);
//        printf ("It took me %d clicks (%.2f seconds).\n",t,((float)t)/CLOCKS_PER_SEC);
        t2d->print_k(1);
    }
    else if(argc>=5) {
        hdt_file = argv[1];
        classes_file = argv[2];
        files_dir = argv[3];
        properties_file = argv[4];
        if(argc==5){
            mode = T2Dv2::RESTRICTIVE_MODE;
        }
        else{
            mode_str = argv[5];
            if(mode_str=="heuristic" || mode_str[0]==T2Dv2::HEURISTIC_MODE){
                mode = T2Dv2::HEURISTIC_MODE;
            }
            else if (mode_str=="permissive" || mode_str[0]==T2Dv2::PERMISSIVE_MODE){
                mode = T2Dv2::PERMISSIVE_MODE;
            }
            else{
                cerr << "Invalid mode, it should be either restrictive, permissive, or heuristic \n";
                return 0;
            }
        }
        cout<< "hdt_file: "<<hdt_file<<endl;
        cout<< "classes_file: "<<classes_file<<endl;
        cout<< "files_dir: "<<files_dir<<endl;
        cout << "properties_file: "<<properties_file<<endl;
        cout << "mode: "<<mode<<endl;
        clock_t t=clock();
        T2Dv2* t2d = new T2Dv2(hdt_file, log_file, classes_file, files_dir);
        t2d->set_lang_tag("@en");
        t2d->set_title_case(true);
        t2d->run_test_properties(properties_file,mode);
        k=0;
        cout << "====> K = " <<k+1<<endl;
        t2d->compute_scores(k);
        k+=2;
        cout << "====> K = " <<k+1<<endl;
        t2d->compute_scores(k);
        k+=2;
        cout << "====> K = " <<k+1<<endl;
        t2d->compute_scores(k);
//        t2d->print_k();
        t = clock() - t;
        printf ("It took %.2f seconds.\n",(static_cast<double>(t))/CLOCKS_PER_SEC);
//        printf ("It took me %ul clicks (%.2f seconds).\n",t,(static_cast<double>(t))/CLOCKS_PER_SEC);
//        printf ("It took me %d clicks (%.2f seconds).\n",t,((float)t)/CLOCKS_PER_SEC);
        t2d->print_k(1);
    }

    return 0;
}
