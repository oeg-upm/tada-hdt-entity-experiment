#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */
#include<iostream>
#include<string>
#include<stdio.h>
#include "t2dv2.h"
#include <tabular_parser/parser.h>

using namespace std;


int main(int argc, char* argv[]) {

//    string file_dir1="/Users/aalobaid/workspaces/Pyworkspace/tada-gam/local_data/t2dv2/5873256_0_7795190905731964989.csv";
//    string file_dir2 = "/Users/aalobaid/workspaces/Pyworkspace/tada-gam/local_data/t2dv2/86747932_0_7532457067740920052.csv";
//    unsigned int col_idx= 1;
//    string m_hdt = "/Users/aalobaid/workspaces/Cworkspace/tada-hdt/dbpedia_all.hdt";
//    EntityAnn* ea = new EntityAnn(m_hdt, "entity.log");
//    //ea->set_language_tag("@en");
//    ea->set_title_case(true);
//    ea->set_language_tag("@en");
//    std::list<string>* candidates;
//    Parser p(file_dir1);
//    cout << "file_dir: "<<file_dir1<<endl;
//    cout << "col_idx: "<<col_idx<<endl;
////        cout << "pre parse:\n";
////        p.parse_vertical();
////        cout <<" parse vertical\n";
//    candidates = ea->annotate_column(p.parse_vertical(), col_idx, true, true);
//    delete candidates;

//    ea = new EntityAnn(m_hdt, "entity.log");
//    ea->set_title_case(true);
//    ea->set_language_tag("@en");
//    Parser p2(file_dir2);
//    cout << "file_dir2: "<<file_dir2<<endl;
//    cout << "col_idx: "<<col_idx<<endl;
////        cout << "pre parse:\n";
////        p.parse_vertical();
////        cout <<" parse vertical\n";
//    candidates = ea->annotate_column(p2.parse_vertical(), col_idx, true, true);







//    string fname = "86747932_0_7532457067740920052.csv";

//    string file_dir = "/Users/aalobaid/workspaces/Pyworkspace/tada-gam/local_data/t2dv2/86747932_0_7532457067740920052.csv";
//    unsigned int col_idx= 1;
//    string m_hdt = "/Users/aalobaid/workspaces/Cworkspace/tada-hdt/dbpedia_all.hdt";
//    EntityAnn* ea = new EntityAnn(m_hdt, "entity.log");
//    //ea->set_language_tag("@en");
//    ea->set_title_case(true);
//    ea->set_language_tag("@en");
//    std::list<string>* candidates;
//    Parser p(file_dir);
//    cout << "file_dir: "<<file_dir<<endl;
//    cout << "col_idx: "<<col_idx<<endl;
////        cout << "pre parse:\n";
////        p.parse_vertical();
////        cout <<" parse vertical\n";
//    candidates = ea->annotate_column(p.parse_vertical(), col_idx, true, true);
//    delete candidates;
//    candidates = ea->annotate_column(p.parse_vertical(), col_idx, true, true);
//    delete candidates;





    unsigned int col_idx= 1;
    string fname = "86747932_0_7532457067740920052.csv";

    string hdt_fpath, log_fpath, classes_fpath, files_dir;
    hdt_fpath = "/Users/aalobaid/workspaces/Cworkspace/tada-hdt/dbpedia_all.hdt";
    log_fpath = "testlog.log";
    classes_fpath = "datasets/t2dv2/classes_with_col_GS_2016_04.csv";
    files_dir = "/Users/aalobaid/workspaces/Pyworkspace/tada-gam/local_data/t2dv2";
    T2Dv2 t2d(hdt_fpath, log_fpath, classes_fpath, files_dir);
    t2d.set_lang_tag("@en");
    t2d.set_title_case(true);
    t2d.run_entity_test_left_one_out_all();
//    cout << "\n\n\n123\n";
//    t2d.get_ea_model(fname, col_idx, true);
//    t2d.get_ea_model("5873256_0_7795190905731964989.csv", col_idx, true);

//    t2d.get_ea_model(fname, col_idx, true);
//    t2d.get_ea_model(fname, col_idx, true);


//    t2d.get_classes_and_columns();
//    for(auto it=t2d.m_classes_col_names.cbegin();it!=t2d.m_classes_col_names.cend();it++){
//        cout<< endl << it->first <<"\n";
//        for(auto it2=it->second.cbegin();it2!=it->second.cend();it2++){
//            cout << "\t\t" << it2->first << ", " << it2->second << endl;
//        }
//    }
    //t2d.run_test_on_a_file("http://dbpedia.org/ontology/Mountain", "1146722_1_7558140036342906956.csv", 0, 0, 1, 0.01);
            //double run_test_on_a_file(string class_uri, string fname, unsigned int col_id, double from_alpha, double to_alpha, double step);

    return 0;
}

//int main(int argc, char* argv[]) {
//    long k;
//    char mode;
//    string hdt_file, classes_file, files_dir, properties_file, log_file="main.log", mode_str;
//    if(argc<4) {
//        cerr << "Usage: " << argv[0] << "hdt_file classes_file files_dir " << std::endl;
//        cerr << "Or: " << argv[0] << "hdt_file classes_file files_dir properties_file mode " << std::endl;
//    }
//    else if(argc==4) {
//        hdt_file = argv[1];
//        classes_file = argv[2];
//        files_dir = argv[3];
//        cout<< "hdt_file: "<<hdt_file<<endl;
//        cout<< "classes_file: "<<classes_file<<endl;
//        cout<< "files_dir: "<<files_dir<<endl;
//        clock_t t=clock();
//        T2Dv2* t2d = new T2Dv2(hdt_file, log_file, classes_file, files_dir);
//        t2d->set_lang_tag("@en");
//        t2d->set_title_case(true);
////        t2d->set_title_case(true);
////        t2d->set_inner_context(false);
//        t2d->set_inner_context(true);
////        t2d->run_test(0.0, 1.0, 0.05);
//        t2d->run_test(0.0, 1.0, 0.01);
//        k=0;
//        cout << "====> K = " <<k+1<<endl;
//        t2d->compute_scores(k);
//        k+=2;
//        cout << "====> K = " <<k+1<<endl;
//        t2d->compute_scores(k);
//        k+=2;
//        cout << "====> K = " <<k+1<<endl;
//        t2d->compute_scores(k);
////        t2d->print_k();
//        t = clock() - t;
//        printf ("It took %.2f seconds.\n",(static_cast<double>(t))/CLOCKS_PER_SEC);
////        printf ("It took me %ul clicks (%.2f seconds).\n",t,(static_cast<double>(t))/CLOCKS_PER_SEC);
////        printf ("It took me %d clicks (%.2f seconds).\n",t,((float)t)/CLOCKS_PER_SEC);
//        t2d->print_k(1);
//    }
//    else if(argc>=5) {
//        hdt_file = argv[1];
//        classes_file = argv[2];
//        files_dir = argv[3];
//        properties_file = argv[4];
//        if(argc==5){
//            mode = T2Dv2::RESTRICTIVE_MODE;
//        }
//        else{
//            mode_str = argv[5];
//            if(mode_str=="heuristic" || mode_str[0]==T2Dv2::HEURISTIC_MODE){
//                mode = T2Dv2::HEURISTIC_MODE;
//            }
//            else if (mode_str=="permissive" || mode_str[0]==T2Dv2::PERMISSIVE_MODE){
//                mode = T2Dv2::PERMISSIVE_MODE;
//            }
//            else{
//                cerr << "Invalid mode, it should be either restrictive, permissive, or heuristic \n";
//                return 0;
//            }
//        }
//        cout<< "hdt_file: "<<hdt_file<<endl;
//        cout<< "classes_file: "<<classes_file<<endl;
//        cout<< "files_dir: "<<files_dir<<endl;
//        cout << "properties_file: "<<properties_file<<endl;
//        cout << "mode: "<<mode<<endl;
//        clock_t t=clock();
//        T2Dv2* t2d = new T2Dv2(hdt_file, log_file, classes_file, files_dir);
//        t2d->set_lang_tag("@en");
//        t2d->set_title_case(true);
//        t2d->run_test_properties(properties_file,mode);
//        k=0;
//        cout << "====> K = " <<k+1<<endl;
//        t2d->compute_scores(k);
//        k+=2;
//        cout << "====> K = " <<k+1<<endl;
//        t2d->compute_scores(k);
//        k+=2;
//        cout << "====> K = " <<k+1<<endl;
//        t2d->compute_scores(k);
////        t2d->print_k();
//        t = clock() - t;
//        printf ("It took %.2f seconds.\n",(static_cast<double>(t))/CLOCKS_PER_SEC);
////        printf ("It took me %ul clicks (%.2f seconds).\n",t,(static_cast<double>(t))/CLOCKS_PER_SEC);
////        printf ("It took me %d clicks (%.2f seconds).\n",t,((float)t)/CLOCKS_PER_SEC);
//        t2d->print_k(1);
//    }

//    return 0;
//}
