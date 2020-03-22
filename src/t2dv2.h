#ifndef T2DV2_H
#define T2DV2_H

#include<iostream>
#include<string>
#include<list>
#include<unordered_map>

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
        bool evaluate_column(string fname,string class_uri, unsigned int col_idx, bool context, double alpha);
        long get_k(string fname);
        void print_k();
        void computer_scores(long k);
        float get_prec();
        float get_rec();
        float get_f1();
    private:
        string m_classes_file_dir;
        string m_files_dir;
        unsigned long m_correct, m_incorrect, m_notfound;
        string m_file_sep="/";
        HDT * m_hdt;
        EasyLogger* m_logger;
        std::unordered_map<string,long>* m_k;// the result of each file
        float m_prec, m_rec, m_f1;

};


#endif
