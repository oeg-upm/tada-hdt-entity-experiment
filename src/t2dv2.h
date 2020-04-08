#ifndef T2DV2_H
#define T2DV2_H
 // pair
#include <utility>
#include<iostream>
#include<string>
#include<list>
#include<unordered_map>

#include<tada_hdt_entity/entity.h>
#include <easy_logger/easy_logger.h>
#include <HDTManager.hpp>

using namespace std;
using namespace hdt;

class T2Dv2 {
    public:
        T2Dv2(string hdt_dir, string log_file, string classes_file_dir, string files_dir);
        void set_file_sep(string);
        string get_file_sep();
        std::list<string>* annotate_column(string fname, unsigned int col_idx, bool context, double alpha);
        EntityAnn* get_ea_model(string fname, unsigned int col_idx, bool context);
        bool evaluate_column(string fname,string class_uri, unsigned int col_idx, bool context, double alpha);
        bool evaluate_column_at_k(EntityAnn* ea,string class_uri, double alpha, unsigned long k_max);
        long evaluate_column_get_k(EntityAnn* ea,string class_uri, double alpha);
        void run_test(double from_alpha, double to_alpha, double step, unsigned long k); // focusing on the alphas range
        void run_test(double from_alpha, double to_alpha, double step); // focusing on the best k
        long get_k(string fname);
        void print_k();
        void print_k(long);
        void compute_scores(long k);
        double get_prec();
        double get_rec();
        double get_f1();
        string clean_str(string);
        double get_median(std::list<double>* a);
        void set_lang_tag(string);
        void set_title_case(bool);
        void set_inner_context(bool);
        void generate_properties_file(string input_file, string output_file);
        bool property_class_exist(string class_uri, string property_uri);

    private:
        string m_classes_file_dir;
        string m_files_dir;
        unsigned long m_correct, m_incorrect, m_notfound;
        string m_file_sep="/";
        HDT * m_hdt;
        EasyLogger* m_logger;
        std::unordered_map<string,long>* m_k;// the result of each file
//        std::list<pair<double,double>>* alpha_ranges; // from_alpha, to_alpha
        double m_prec, m_rec, m_f1;
        string m_lang_tag;
        bool m_title_case = false;
        bool m_inner_context = true;

};


#endif
