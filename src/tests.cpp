#include <gtest/gtest.h>
#include "t2dv2.h"
#include<tada_hdt_entity/entity.h>
#include <tabular_parser/parser.h>

string input_file = "test.ttl";
string log_file = "test.log";
string hdt_file = "test.hdt";

string dbr_prefix = "http://dbpedia.org/resource/";
string dbo_prefix = "http://dbpedia.org/ontology/";


void ttl_to_hdt(string ttl_dir) {
    HDTSpecification spec("");
    string baseUri="<file://"+ttl_dir+">";
    string output_file;
    HDT* hdt = HDTManager::generateHDT(ttl_dir.c_str(), baseUri.c_str(), TURTLE, spec, NULL);
    output_file = ttl_dir.substr(0, ttl_dir.find_last_of(".")) +  ".hdt"; // same name but with .hdt instread of .ttl
    ofstream out;
    // Save HDT
    out.open(output_file.c_str(), ios::out | ios::binary | ios::trunc);
    if(!out.good()) {
        throw std::runtime_error("Could not open output file.");
    }
    hdt->saveToHDT(out, NULL);
    out.close();
}


namespace {

    TEST(T2Dv2, Hello) {
        T2Dv2* ea = new T2Dv2(hdt_file, log_file, "", "");
        ASSERT_NE(ea, nullptr);
        ea->set_file_sep("--");
        string sp;
        sp = ea->get_file_sep();
        ASSERT_STREQ("--", sp.c_str());
        ea->get_file_sep();
        ASSERT_STREQ(ea->clean_str("\"abc\"").c_str(), "abc");
        ASSERT_STREQ(ea->clean_str("\"abc").c_str(), "abc");
        ASSERT_STREQ(ea->clean_str("abc\"").c_str(), "abc");
        ASSERT_STREQ(ea->clean_str("abc").c_str(), "abc");
        delete ea;
    }

    TEST(T2Dv2, Median) {
        T2Dv2* t2d = new T2Dv2(hdt_file, log_file, "", "");
        std::list<double>* a= new std::list<double>;
        a->push_front(10.0);
        ASSERT_DOUBLE_EQ(10.0, t2d->get_median(a));
        delete  a;
        a= new std::list<double>;
        a->push_front(10.0);
        a->push_front(20.0);
        ASSERT_DOUBLE_EQ(15.0, t2d->get_median(a));
        delete  a;
        a= new std::list<double>;
        a->push_front(10.0);
        a->push_front(20.0);
        a->push_front(1.0);
        ASSERT_DOUBLE_EQ(10.0, t2d->get_median(a));
        delete t2d;
    }

    TEST(T2Dv2, AmateurBoxer) {
        bool annotated_corr;
        long k;
        string fname="AmateurBoxer1.csv";
        string class_uri = dbo_prefix+"AmateurBoxer";
        T2Dv2* t2d = new T2Dv2(hdt_file, log_file, "test_files/classes_test.csv", "test_files");
        annotated_corr = t2d->evaluate_column(fname, class_uri, 0, true, 0.4);
        ASSERT_TRUE(annotated_corr);
        k = t2d->get_k(fname);
        ASSERT_EQ(k, 0);
        delete t2d;
    }

    TEST(T2Dv2, Boxer) {
        bool annotated_corr;
        long k;
        string fname="Boxer1.csv";
        string class_uri = dbo_prefix+"Boxer";
        T2Dv2* t2d = new T2Dv2(hdt_file, log_file, "test_files/classes_test.csv", "test_files");
        annotated_corr = t2d->evaluate_column(fname, class_uri, 0, true, 0.45);
        ASSERT_TRUE(annotated_corr);
        k = t2d->get_k(fname);
        //        t2d->print_k();
        ASSERT_EQ(k, 0);
        delete t2d;
    }

    TEST(T2Dv2, Athlete) {
        bool annotated_corr;
        long k;
        string fname="Athletes1.csv";
        string class_uri = dbo_prefix+"Athlete";
        T2Dv2* t2d = new T2Dv2(hdt_file, log_file, "test_files/classes_test.csv", "test_files");
        annotated_corr = t2d->evaluate_column(fname, class_uri, 0, true, 0.8);
        ASSERT_TRUE(annotated_corr);
        k = t2d->get_k(fname);
        ASSERT_EQ(k, 0);
        delete t2d;
    }

    TEST(T2Dv2, EvaluateatK) {
        unsigned long k=1;
        string fname="Athletes1.csv";
        string class_uri = dbo_prefix+"Athlete";
        T2Dv2* t2d = new T2Dv2(hdt_file, log_file, "test_files/classes_test.csv", "test_files");
        EntityAnn* ea = t2d->get_ea_model(fname, 0, true);
        ASSERT_NE(ea, nullptr);
        ea->get_graph()->print_nodes();
        t2d->evaluate_column_at_k(ea, class_uri, 0.1, 1);
        delete t2d;
    }

    TEST(T2Dv2, Multi1) {
        unsigned long k=1;
        string fname="Athletes1.csv";
        string class_uri = dbo_prefix+"Athlete";
        T2Dv2* t2d = new T2Dv2(hdt_file, log_file, "test_files/classes_test.csv", "test_files");
        t2d->run_test(0.0, 1.0, 0.05, k);
        delete t2d;
    }

    TEST(T2Dv2, MultiWithScore) {
        T2Dv2* t2d = new T2Dv2(hdt_file, log_file, "test_files/classes_test.csv", "test_files");
        t2d->run_test(0.0, 1.0, 0.05);
        t2d->compute_scores(1);
        ASSERT_DOUBLE_EQ(t2d->get_prec(), 1.0);
        ASSERT_DOUBLE_EQ(t2d->get_rec(), 1.0);
        ASSERT_DOUBLE_EQ(t2d->get_f1(), 1.0);
        delete t2d;
    }

    TEST(T2Dv2, MultiWrongCases) {
        T2Dv2* t2d = new T2Dv2(hdt_file, log_file, "test_files/classes_test_wrong.csv", "test_files");
        t2d->run_test(0.0, 1.0, 0.05);
        t2d->compute_scores(1);
        //        t2d->print_k();
        ASSERT_DOUBLE_EQ(t2d->get_prec(), 1.0/3.0);
        ASSERT_DOUBLE_EQ(t2d->get_rec(), 0.5);
        ASSERT_DOUBLE_EQ(t2d->get_f1(), 2.0/5.0);
        delete t2d;
    }

    TEST(T2Dv2, PropertyCheck) {
        T2Dv2* t2d = new T2Dv2(hdt_file, log_file, "test_files/classes_test_wrong.csv", "test_files");
        bool found;
        string class_uri, property_uri;
        property_uri = "http://dbpedia.org/property/city";
        class_uri = "http://dbpedia.org/ontology/FootballPlayer";
        found = t2d->property_class_exist(class_uri, property_uri);
        ASSERT_TRUE(found);
        property_uri = "http://dbpedia.org/property/country";
        found = t2d->property_class_exist(class_uri, property_uri);
        ASSERT_TRUE(found);
        property_uri = "http://dbpedia.org/property/Notfound12321";
        found = t2d->property_class_exist(class_uri, property_uri);
        ASSERT_FALSE(found);
        property_uri = "http://dbpedia.org/property/city";
        class_uri = "http://dbpedia.org/ontology/BasketballPlayer";
        found = t2d->property_class_exist(class_uri, property_uri);
        ASSERT_FALSE(found);
        delete t2d;
    }

    TEST(T2Dv2, PropertiesFile) {
        string class_uri, property_uri, key, fname;
        long k;
        T2Dv2* t2d = new T2Dv2(hdt_file, log_file, "", "test_files");
        t2d->run_test_properties("test_files/properties_test.csv", T2Dv2::RESTRICTIVE_MODE);
        t2d->compute_scores(0);
        t2d->print_k();
        class_uri = "http://dbpedia.org/ontology/Wrestler";
        property_uri = "http://dbpedia.org/property/country";
        fname = "Wrestler1.csv";
        key = fname+"--2";
        k = t2d->get_k(key);
        ASSERT_EQ(k, 0);
        key = fname+"--0";
        k = t2d->get_k(key);
        ASSERT_EQ(k, -1);
        delete t2d;
    }


    TEST(LeaveOneOut, GroupByClass) {
        string classes_fpath = "test_files/classes_test_leaveoneout.csv";
        T2Dv2 t2d(hdt_file, log_file, classes_fpath, "test_files");
        t2d.set_lang_tag("@en");
        t2d.set_title_case(true);
        t2d.get_classes_and_columns();
        ASSERT_EQ(t2d.m_classes_col_names.size(), 3);
        ASSERT_EQ(t2d.m_classes_col_names["http://dbpedia.org/ontology/VolleyballPlayer"].size(), 3);
        ASSERT_EQ(t2d.m_classes_col_names["http://dbpedia.org/ontology/Boxer"].size(), 2);
        ASSERT_EQ(t2d.m_classes_col_names["http://dbpedia.org/ontology/AmateurBoxer"].size(), 1);
    }

    TEST(LeaveOneOut, TestSingleFileInvalid) {
        string classes_fpath = "test_files/classes_test_leaveoneout.csv";
        T2Dv2 t2d(hdt_file, log_file, classes_fpath, "test_files");
        //the test.hdt does not includ the language tag but it is used her to insure that the predicted k is -1
        // and hence, no alpha is created for it
        t2d.set_lang_tag("@en");
        t2d.set_title_case(true);
        t2d.get_classes_and_columns();
        string class_uri = dbo_prefix+"AmateurBoxer";
        string fname = "AmateurBoxer1.csv";
        t2d.run_entity_test_on_a_file(class_uri, fname, 0, 0.0, 1.0, 0.5);
        ASSERT_EQ(t2d.m_files_alpha.find(fname), t2d.m_files_alpha.cend());
    }

    TEST(LeaveOneOut, TestSingleFile) {
        string classes_fpath = "test_files/classes_test_leaveoneout.csv";
        T2Dv2 t2d(hdt_file, log_file, classes_fpath, "test_files");
        t2d.set_title_case(true);
        t2d.get_classes_and_columns();
        string class_uri = dbo_prefix+"AmateurBoxer";
        string fname = "AmateurBoxer1.csv";
        t2d.run_entity_test_on_a_file(class_uri, fname, 0, 0.0, 1.0, 0.5);
        cout << "alpha: "<<t2d.m_files_alpha[fname] <<endl;
        ASSERT_NE(t2d.m_files_alpha.find(fname), t2d.m_files_alpha.cend());
        ASSERT_GT(t2d.m_files_alpha[fname], 0.1);
        ASSERT_EQ(t2d.m_files_alpha[fname], 0.25); // to test the middle of the alpha range
    }


    TEST(LeaveOneOut, TestMoreThanOnePerClass) {
        string classes_fpath = "test_files/classes_test_leaveoneout.csv";
        T2Dv2 t2d(hdt_file, log_file, classes_fpath, "test_files");
        t2d.set_title_case(true);
        t2d.get_classes_and_columns();
        string class_uri = dbo_prefix+"AmateurBoxer";
        string fname = "AmateurBoxer1.csv";
        t2d.run_entity_and_compute_alphas();
        ASSERT_EQ(t2d.m_files_alpha.find("AmateurBoxer1.csv"), t2d.m_files_alpha.cend());
        ASSERT_NE(t2d.m_files_alpha.find("boxer1.csv"), t2d.m_files_alpha.cend());
        ASSERT_NE(t2d.m_files_alpha.find("volleyball1.csv"), t2d.m_files_alpha.cend());
    }

    TEST(LeaveOneOut, TestOptimalAlphaSingleClassSingleFile) {
        string classes_fpath = "test_files/classes_test_leaveoneout.csv";
        T2Dv2 t2d(hdt_file, log_file, classes_fpath, "test_files");
        t2d.set_title_case(true);
        t2d.get_classes_and_columns();
        string class_uri = dbo_prefix+"AmateurBoxer";
        string fname = "AmateurBoxer1.csv";
        string alpha_log = "test_alpha_log.csv";
        t2d.run_entity_and_compute_alphas();
        t2d.append_to_file(alpha_log, "=============== TestOptimalAlphaSingleClassSingleFile =============");
        t2d.run_entity_test_left_one_out_class(class_uri, alpha_log);
        ASSERT_EQ(t2d.m_files_alpha.find(fname), t2d.m_files_alpha.cend());
        ASSERT_EQ(t2d.m_classes_opt_alpha.find(class_uri), t2d.m_classes_opt_alpha.cend());
        ASSERT_EQ(t2d.m_classes_pred_acc.find(class_uri), t2d.m_classes_pred_acc.cend());
    }

    TEST(LeaveOneOut, TestOptimalAlphaSingleClassMulti) {
        string classes_fpath = "test_files/classes_test_leaveoneout.csv";
        T2Dv2 t2d(hdt_file, log_file, classes_fpath, "test_files");
        t2d.set_title_case(true);
        t2d.get_classes_and_columns();
        string class_uri = dbo_prefix+"VolleyballPlayer";
        string fname = "volleyball1.csv";
        string alpha_log = "test_alpha_log.csv";
        t2d.run_entity_and_compute_alphas();
        t2d.append_to_file(alpha_log, "=============== TestOptimalAlphaSingleClassMulti =============");
        t2d.run_entity_test_left_one_out_class(class_uri, alpha_log);
        ASSERT_NE(t2d.m_files_alpha.find(fname), t2d.m_files_alpha.cend());
        ASSERT_NE(t2d.m_classes_opt_alpha.find(class_uri), t2d.m_classes_opt_alpha.cend());
        ASSERT_NE(t2d.m_classes_pred_acc.find(class_uri), t2d.m_classes_pred_acc.cend());
        ASSERT_GT(t2d.m_classes_opt_alpha[class_uri], 0);
        ASSERT_LT(t2d.m_classes_opt_alpha[class_uri], 1);
        ASSERT_FLOAT_EQ(t2d.m_classes_pred_acc[class_uri], 1.0);
        //ASSERT_LE(t2d.m_classes_pred_acc[class_uri], 1);
    }

    TEST(LeaveOneOut, TestOptimalAlphaSingleClassMultiWrong) {
        string classes_fpath = "test_files/classes_test_leaveoneout.csv";
        T2Dv2 t2d(hdt_file, log_file, classes_fpath, "test_files");
        t2d.set_title_case(true);
        t2d.get_classes_and_columns();
        string class_uri = dbo_prefix+"Boxer";
        string fname_good = "boxer1.csv";
        string fname_bad = "boxer2.csv";
        string alpha_log = "test_alpha_log.csv";
        t2d.append_to_file(alpha_log, "=============== TestOptimalAlphaSingleClassMultiWrong =============");
        t2d.run_entity_test_left_one_out_all("test_alpha_leaveout_alpha_log.csv", "test_opt_alphas.csv", "test_acc_pred_class.csv");
        ASSERT_NE(t2d.m_files_alpha.find(fname_good), t2d.m_files_alpha.cend());
        ASSERT_EQ(t2d.m_files_alpha.find(fname_bad), t2d.m_files_alpha.cend());
        ASSERT_EQ(t2d.m_classes_pred_acc.find(class_uri), t2d.m_classes_pred_acc.cend());
    }

    TEST(LeaveOneOut, AbsenceOfOptAlpha) {
        string classes_fpath = "test_files/classes_test_notfound.csv";
        T2Dv2 t2d(hdt_file, log_file, classes_fpath, "test_files");
        t2d.set_title_case(true);
        t2d.get_classes_and_columns();
        string class_uri = dbo_prefix+"Boxer";
        string fname1 = "notfound1.csv";
        string fname2 = "notfound2.csv";
        string alpha_log = "test_alpha_log.csv";
        t2d.append_to_file(alpha_log, "=============== AbsenceOfOptAlpha =============");
        t2d.run_entity_test_left_one_out_all("test_alpha_leaveout_alpha_log.csv", "test_opt_alphas.csv", "test_acc_pred_class.csv");
        ASSERT_NE(t2d.m_classes_pred_acc.find(class_uri), t2d.m_classes_pred_acc.cend());
        ASSERT_EQ(t2d.m_classes_pred_acc[class_uri], 0.0);
        ASSERT_EQ(t2d.m_files_alpha.find(fname1), t2d.m_files_alpha.cend());
        ASSERT_EQ(t2d.m_files_alpha.find(fname2), t2d.m_files_alpha.cend());
    }
}//namespace




int main(int argc, char** argv) {
    ttl_to_hdt(input_file);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
