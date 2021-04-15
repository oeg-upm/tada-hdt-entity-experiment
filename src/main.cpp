#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */
#include<iostream>
#include<string>
#include<stdio.h>
#include "t2dv2.h"
#include <tabular_parser/parser.h>
#include <chrono>

using namespace std::chrono;
using namespace std;


int main(int argc, char* argv[]) {
	long k;
    auto start = high_resolution_clock::now();
    string hdt_fpath, log_fpath, classes_fpath, files_dir;
    //hdt_fpath = "/home/ahmad/datasets/wikidata20200309.hdt";
    hdt_fpath = "/home/aalobaid/workspaces/Datasets/dbpedia_all.hdt";
    //    hdt_fpath = "/Users/aalobaid/workspaces/Cworkspace/tada-hdt/dbpedia_all.hdt";
    log_fpath = "testlog.log";
    //    classes_fpath = "/home/aalobaid/workspaces/Datasets/semtab2019/Round 2/gt/CTA_Round2_gt.csv";
    classes_fpath = "/home/aalobaid/workspaces/Cworkspace/tada-hdt-entity-experiment/datasets/semtab-2019/classes_with_col_GS_semtab_2019_CTA_Round1_gt.csv";
    //    classes_fpath = "/home/ahmad/tada-hdt-entity-experiment/datasets/t2dv2/classes_with_col_GS_2016_04_wikidata.csv";
    //classes_fpath = "/home/aalobaid/workspaces/Cworkspace/tada-hdt-entity-experiment/datasets/t2dv2/classes_with_col_GS_2016_04.csv";
    //classes_fpath = "datasets/t2dv2/classes_with_col_GS_2016_04.csv";
    //    files_dir = "/Users/aalobaid/workspaces/Pyworkspace/tada-gam/local_data/t2dv2";
    //files_dir = "/home/ahmad/tada-gam/local_data/t2dv2";
    //files_dir = "/home/aalobaid/workspaces/Datasets/t2dv2";
    //files_dir = "/home/aalobaid/workspaces/Datasets/t2dv2";
    //files_dir ="/home/aalobaid/workspaces/Datasets/semtab2019/Round 2/tables";
    files_dir ="/home/aalobaid/workspaces/Datasets/semtab2019/Round 1/tables";
    T2Dv2 t2d(hdt_fpath, log_fpath, classes_fpath, files_dir);
    t2d.set_lang_tag("@en");
    //    t2d.set_title_case(false);
    t2d.set_title_case(true);
    //    t2d.m_ambiguitity_penalty=2;
    //t2d.set_title_case(false);
    t2d.m_fname_additional_prefix=".csv";  // for sem tab
    t2d.run_entity_test_left_one_out_all("alpha_leaveout_alpha_log.csv", "opt_alphas.csv", "acc_pred_class.csv");
    //t2d.run_entity_test_alpha(0.45, "single_alphas_title_case.csv");
    // Wikidata
    //    string rdf_type = "http://www.wikidata.org/prop/direct/P31";
    //    string rdfs_subclassof = "http://www.wikidata.org/prop/direct/P279";
    //    t2d.rdf_type = rdf_type;
    //    t2d.rdfs_subclassof = rdfs_subclassof;
    //t2d.run_entity_test_left_one_out_all("wiki_alpha_leaveout_alpha_log.csv", "wiki_opt_alphas.csv", "wiki_acc_pred_class.csv");
    //    t2d.run_test(0.0, 1.0, 0.01);
    //    k=0;
    //    cout << "====> K = " <<k+1<<endl;
    //    t2d.compute_scores(k);
    //    k+=2;
    //    cout << "====> K = " <<k+1<<endl;
    //    t2d.compute_scores(k);
    //    k+=2;
    //    cout << "====> K = " <<k+1<<endl;
    //    t2d.compute_scores(k);
    //    t2d.subclassof_uri = "http://www.wikidata.org/prop/direct/P279";
    //    t2d.type_uri = "http://www.wikidata.org/prop/direct/P31";
    // end
    //    string fname ="33242247_1_7232384061498591611.csv";
    //    string class_uri="http://dbpedia.org/ontology/Election";
    //    long k;
    //    k=t2d.run_entity_test_on_a_file_with_alpha(class_uri, fname, 0, 0.45);
    //    cout << "k: "<<k<<endl;
    //    long k;
    //    unsigned int col_id=0;
    //    string fname="19073331_0_2742992342272078110.csv";
    //    EntityAnn* ea = t2d.get_ea_model(fname, 0, false);
    //    string class_uri="http://dbpedia.org/ontology/PoliticalParty";
    //   k = t2d.evaluate_column_get_k(ea, class_uri, 0.41);
    //    cout << "k: "<<k<<endl;
    //    t2d.run_entity_test_on_a_file(class_uri, fname, col_id, 0, 1, 0.01);
    //ea->
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
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<minutes>(stop - start);
    cout << "Total run took "<< duration.count() << " minutes "<< endl;
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
