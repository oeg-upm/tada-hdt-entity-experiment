#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */
#include<iostream>
#include<string>
#include<stdio.h>
#include "t2dv2.h"
#include <tabular_parser/parser.h>

using namespace std;






int main(int argc, char* argv[]) {
    long k;
    string hdt_file, classes_file, files_dir, log_file="main.log";
    if(argc<4) {
        cerr << "Usage: " << argv[0] << "hdt_file classes_file files_dir " << std::endl;
//        hdt_file = "/Users/aalobaid/workspaces/Cworkspace/tada-hdt/dbpedia_all.hdt";
//        string class_uri = "http://dbpedia.org/ontology/Mountain";
//        string fname = "/Users/aalobaid/workspaces/Pyworkspace/tada-gam/local_data/t2dv2/99070098_0_2074872741302696997.csv";

//        EntityAnn* ea = new EntityAnn(hdt_file, log_file, 0.0);
//        std::list<string>* candidates;
//        std::list<std::list<string>*>* data;
//        Parser p(fname);
//        data = p.parse_vertical();
//        candidates = ea->annotate_column(data, 1, true, true);
//        ea->get_graph()->print_nodes();
//        delete candidates;
//        candidates = ea->recompute_f(0.1);
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
    }

    return 0;
}
