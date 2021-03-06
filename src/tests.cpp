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
        annotated_corr = t2d->evaluate_column(fname, class_uri, 0, true, 0.7);
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
        annotated_corr = t2d->evaluate_column(fname, class_uri, 0, true, 0.65);
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
        bool annotated_corr;
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
        found = t2d->property_class_exist(class_uri,property_uri);
        ASSERT_TRUE(found);
        property_uri = "http://dbpedia.org/property/country";
        found = t2d->property_class_exist(class_uri,property_uri);
        ASSERT_TRUE(found);
        property_uri = "http://dbpedia.org/property/Notfound12321";
        found = t2d->property_class_exist(class_uri,property_uri);
        ASSERT_FALSE(found);
        property_uri = "http://dbpedia.org/property/city";
        class_uri = "http://dbpedia.org/ontology/BasketballPlayer";
        found = t2d->property_class_exist(class_uri,property_uri);
        ASSERT_FALSE(found);
        delete t2d;
    }

    TEST(T2Dv2, PropertiesFile) {
        string class_uri, property_uri, key, fname;
        long k;
        T2Dv2* t2d = new T2Dv2(hdt_file, log_file, "", "test_files");
        t2d->run_test_properties("test_files/properties_test.csv",T2Dv2::RESTRICTIVE_MODE);
        t2d->compute_scores(0);
        t2d->print_k();
        class_uri = "http://dbpedia.org/ontology/Wrestler";
        property_uri = "http://dbpedia.org/property/country";
        fname = "Wrestler1.csv";
        key = fname+"--2";
        k = t2d->get_k(key);
        ASSERT_EQ(k,0);
        key = fname+"--0";
        k = t2d->get_k(key);
        ASSERT_EQ(k,-1);
        delete t2d;
    }

}//namespace




int main(int argc, char** argv) {
    ttl_to_hdt(input_file);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
