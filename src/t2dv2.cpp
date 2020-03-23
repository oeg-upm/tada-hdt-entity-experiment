#include "t2dv2.h"
#include<tada_hdt_entity/entity.h>
#include<tabular_parser/parser.h>

T2Dv2::T2Dv2(string hdt_dir, string log_file_dir ,string classes_file_dir, string files_dir){
    m_classes_file_dir = classes_file_dir;
    m_files_dir = files_dir;
    m_correct = m_incorrect = m_notfound = 0;
    m_hdt = HDTManager::mapIndexedHDT(hdt_dir.c_str());
    m_logger = new EasyLogger(log_file_dir);
    m_k = new unordered_map<string,long>;
}

void T2Dv2::set_file_sep(string sep){
    m_file_sep = sep;
}

string T2Dv2::get_file_sep(){
    return m_file_sep;
}

std::list<string>* T2Dv2::annotate_column(string fname, unsigned int col_idx, bool context, double alpha){
    EntityAnn *ea = new EntityAnn(m_hdt, "entity.log", alpha);
    string file_dir;
    std::list<string> *candidates;
    file_dir = m_files_dir;
    if(file_dir.substr(m_files_dir.length()-m_file_sep.length()) != m_file_sep){
        file_dir += m_file_sep;
    }
    file_dir += fname;
    Parser p(file_dir);
    if(context){
        candidates = ea->annotate_column(p.parse_vertical(),col_idx, context, context);
    }
    else{
        candidates = ea->annotate_column(p.parse(),col_idx);
    }
    ea->get_graph()->print_nodes();
    delete ea;
    return candidates;
}

bool T2Dv2::evaluate_column(string fname,string class_uri, unsigned int col_idx, bool context, double alpha){
    std::list<string> *candidates;
    candidates = this->annotate_column(fname, col_idx, context, alpha);
    long k=0;
    m_logger->log("evaluate_column> fname: "+fname);
    for(auto it = candidates->cbegin();it!=candidates->cend();it++,k++){
        m_logger->log("evaluate_column> candidate "+to_string(k)+" is: "+((*it)));
        if((*it)==class_uri){
            m_k->insert({fname,k});
            delete candidates;
            return true;
        }
        else{
            cout << "not the same: "<<(*it)<<endl;
        }
    }
    k=-1;
    m_k->insert({fname,k});
    delete candidates;
    return false;
}

long T2Dv2::get_k(string fname){
    return m_k->at(fname);
}

void T2Dv2::print_k(){
    cout << "\n\nprint k> \n";
    for(auto it=m_k->cbegin();it!=m_k->cend();it++){
        cout<< it->first << " --- " << it->second <<endl;
    }
}

void T2Dv2::computer_scores(long k){
    m_correct=m_incorrect=m_notfound = 0;
    m_prec=m_rec=m_f1 = 0.0;
    for(auto it=m_k->cbegin();it!=m_k->cend();it++){
        if(it->second == -1){
            m_notfound++;
        }
        else if(it->second > k){
            m_incorrect++;
        }
        else if(it->second <= k){
            m_correct++;
        }
    }
    float corr,incorr, notf;
    corr = static_cast<float>(m_correct);
    incorr = static_cast<float>(m_incorrect);
    notf = static_cast<float>(m_notfound);
    m_prec = corr / (corr+incorr);
    m_rec = corr / (corr+notf);
    m_f1 = 2 * corr * incorr / (corr+incorr);
}

float T2Dv2::get_prec(){
    return m_prec;
}

float T2Dv2::get_rec(){
    return m_rec;
}

float T2Dv2::get_f1(){
    return m_f1;
}










