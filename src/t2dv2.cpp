#include "t2dv2.h"
#include<tada_hdt_entity/entity.h>
#include<tabular_parser/parser.h>
#include <climits>

T2Dv2::T2Dv2(string hdt_dir, string log_file_dir, string classes_file_dir, string files_dir) {
    m_classes_file_dir = classes_file_dir;
    m_files_dir = files_dir;
    m_correct = m_incorrect = m_notfound = 0;
    m_hdt = HDTManager::mapIndexedHDT(hdt_dir.c_str());
    m_logger = new EasyLogger(log_file_dir);
    m_k = new unordered_map<string, long>;
}

void T2Dv2::set_file_sep(string sep) {
    m_file_sep = sep;
}

string T2Dv2::get_file_sep() {
    return m_file_sep;
}

EntityAnn* T2Dv2::get_ea_model(string fname, unsigned int col_idx, bool context) {
    EntityAnn* ea = new EntityAnn(m_hdt, "entity.log", 0.0);
    string file_dir;
    std::list<string>* candidates;
    file_dir = m_files_dir;
    if(file_dir.substr(m_files_dir.length()-m_file_sep.length()) != m_file_sep) {
        file_dir += m_file_sep;
    }
    file_dir += fname;
    Parser p(file_dir);
    if(context) {
        candidates = ea->annotate_column(p.parse_vertical(), col_idx, context, context);
    }
    else {
        candidates = ea->annotate_column(p.parse(), col_idx);
    }
    //    ea->get_graph()->print_nodes();
    //    delete ea;
    delete candidates;
    return ea;
}

std::list<string>* T2Dv2::annotate_column(string fname, unsigned int col_idx, bool context, double alpha) {
    EntityAnn* ea = new EntityAnn(m_hdt, "entity.log", alpha);
    string file_dir;
    std::list<string>* candidates;
    file_dir = m_files_dir;
    if(file_dir.substr(m_files_dir.length()-m_file_sep.length()) != m_file_sep) {
        file_dir += m_file_sep;
    }
    file_dir += fname;
    Parser p(file_dir);
    if(context) {
        candidates = ea->annotate_column(p.parse_vertical(), col_idx, context, context);
    }
    else {
        candidates = ea->annotate_column(p.parse(), col_idx);
    }
    //    ea->get_graph()->print_nodes();
    delete ea;
    return candidates;
}

bool T2Dv2::evaluate_column(string fname, string class_uri, unsigned int col_idx, bool context, double alpha) {
    std::list<string>* candidates;
    candidates = this->annotate_column(fname, col_idx, context, alpha);
    long k=0;
    m_logger->log("evaluate_column> fname: "+fname);
    for(auto it = candidates->cbegin(); it!=candidates->cend(); it++, k++) {
        m_logger->log("evaluate_column> candidate "+to_string(k)+" is: "+((*it)));
        if((*it)==class_uri) {
            m_k->insert({fname, k});
            delete candidates;
            return true;
        }
        else {
            //            cout << "not the same: "<<(*it)<<endl;
        }
    }
    k=-1;
    m_k->insert({fname, k});
    delete candidates;
    return false;
}


bool T2Dv2::evaluate_column_at_k(EntityAnn* ea, string class_uri, double alpha, unsigned long k_max) {
    long k=0, kkmax;
    std::list<string>* candidates;
    //    cout<< "ea will get candidates : "<<endl;
    candidates = ea->recompute_f(alpha);
    //    cout<< "got candidates\n";
    //    cout<< "num of candidates: "<<candidates->size()<<endl;
    kkmax = static_cast<long> (k_max);
    for(auto it = candidates->cbegin(); it!=candidates->cend() && k<=kkmax; it++, k++) {
        m_logger->log("evaluate_column_at_k> candidate "+to_string(k)+" is: "+((*it)));
        if((*it)==class_uri) {
            delete candidates;
            return true;
        }
        else {
            //            cout << "not the same: "<<(*it)<<endl;
        }
    }
    k=-1;
    delete candidates;
    return false;
}


long T2Dv2::evaluate_column_get_k(EntityAnn* ea, string class_uri, double alpha) {
    long k=0;
    std::list<string>* candidates;
    candidates = ea->recompute_f(alpha);
    for(auto it = candidates->cbegin(); it!=candidates->cend(); it++, k++) {
        m_logger->log("evaluate_column_at_k> candidate "+to_string(k)+" is: "+((*it)));
        if((*it)==class_uri) {
            delete candidates;
            return k;
        }
        else {
        }
    }
    k=-1;
    delete candidates;
    return false;
}


long T2Dv2::get_k(string fname) {
    return m_k->at(fname);
}


void T2Dv2::print_k() {
    cout << "\n\nprint k> \n";
    for(auto it=m_k->cbegin(); it!=m_k->cend(); it++) {
        cout<< it->first << " --- " << it->second <<endl;
    }
}


void T2Dv2::compute_scores(long k) {
    m_correct=m_incorrect=m_notfound = 0;
    m_prec=m_rec=m_f1 = 0.0;
    for(auto it=m_k->cbegin(); it!=m_k->cend(); it++) {
        if(it->second == -1) {
            m_notfound++;
        }
        else if(it->second > k) {
            m_incorrect++;
        }
        else if(it->second <= k) {
            m_correct++;
        }
    }
    double corr, incorr, notf;
    corr = static_cast<double>(m_correct);
    incorr = static_cast<double>(m_incorrect);
    notf = static_cast<double>(m_notfound);
    m_prec = corr / (corr+incorr);
    m_rec = corr / (corr+notf);
    m_f1 = 2.0 * m_prec * m_rec / (m_prec+m_rec);
    cout << "correct: "<<m_correct<<endl;
    cout << "incorrect: "<<m_incorrect<<endl;
    cout << "notfound: " << m_notfound<<endl;
    cout << "Precision: "<<m_prec<<endl;
    cout << "Recall: "<<m_rec<<endl;
    cout << "F1: " << m_f1<<endl;
}


double T2Dv2::get_prec() {
    return m_prec;
}


double T2Dv2::get_rec() {
    return m_rec;
}


double T2Dv2::get_f1() {
    return m_f1;
}


void T2Dv2::run_test(double from_alpha, double to_alpha, double step, unsigned long k) {
    std::list<std::list<string>*>*  data;
    string class_uri, col_id_str, fname;
    EntityAnn* ea;
    unsigned int col_id;
    //    double from_a,to_a;
    std::list<double>* from_a_list = new std::list<double>;
    std::list<double>* to_a_list = new std::list<double>;
    double from_a, to_a;
    std::list<string>::iterator col_iter;
    Parser p(m_classes_file_dir);
    data = p.parse_vertical();
    for(auto it=data->cbegin(); it!=data->cend(); it++) {
        //        for(auto it2=(*it)->cbegin(); it2!=(*it)->cend();it2++){
        //            cout << (*it2) << "| ";
        //        }
        cout << "---------------\n";
        col_iter = (*it)->begin();
        fname = clean_str(*col_iter);
        col_iter++;
        col_id_str = clean_str(*col_iter);
        col_id = static_cast<unsigned int>(stoul(col_id_str));
        col_iter++;
        class_uri = clean_str(*col_iter);
        cout << "class_uri: " << class_uri<<endl;
        cout<< "col_idx: " << col_id_str <<endl;
        cout<<"fname: "<<fname<<endl;
        ea = get_ea_model(fname, col_id, true);
        //        ea->get_graph()->print_nodes();
        from_a = to_a = -1;
        for(double a=from_alpha; a<=to_alpha; a+=step) {
            if(evaluate_column_at_k(ea, class_uri, a, k)) {
                from_a = a;
                from_a_list->push_back(a);
                cout<< "from a: "<<a;
                //from_a = a;
                break;
            }
        }
        if(from_a >= 0.0) {
            for(double a=to_alpha; a>=from_alpha; a-=step) {
                if(evaluate_column_at_k(ea, class_uri, a, k)) {
                    to_a = a;
                    to_a_list->push_back(a);
                    //                to_a = a;
                    cout << "  to a: "<<a<<endl;
                    break;
                }
            }
        }
        delete ea;
    }
    cout << "Alpha from median: "<<get_median(from_a_list)<<endl;
    cout << "Alpha to median: "<<get_median(to_a_list)<<endl;
}


void T2Dv2::run_test(double from_alpha, double to_alpha, double step) {
    std::list<std::list<string>*>*  data;
    string class_uri, col_id_str, fname;
    long k, kmin;
    EntityAnn* ea;
    unsigned int col_id;
    //    double from_a,to_a;
    std::list<double>* from_a_list = new std::list<double>;
    std::list<double>* to_a_list = new std::list<double>;
    double from_a, to_a;
    std::list<string>::iterator col_iter;
    Parser p(m_classes_file_dir);
    data = p.parse_vertical();
    for(auto it=data->cbegin(); it!=data->cend(); it++) {
        //        for(auto it2=(*it)->cbegin(); it2!=(*it)->cend();it2++){
        //            cout << (*it2) << "| ";
        //        }
        cout << "---------------\n";
        col_iter = (*it)->begin();
        fname = clean_str(*col_iter);
        col_iter++;
        col_id_str = clean_str(*col_iter);
        col_id = static_cast<unsigned int>(stoul(col_id_str));
        col_iter++;
        class_uri = clean_str(*col_iter);
        cout << "class_uri: " << class_uri<<endl;
        cout << "col_idx: " << col_id_str <<endl;
        cout << "fname: "<<fname<<endl;
        ea = get_ea_model(fname, col_id, true);
        //        ea->get_graph()->print_nodes();
        from_a = to_a = -1;
        kmin = LONG_MAX;
        for(double a=from_alpha; a<=to_alpha; a+=step) {
            k = evaluate_column_get_k(ea,class_uri,a);

            if(k<kmin){
                kmin = k;
                from_a = a;
                to_a = a;
                from_a_list->push_back(from_a);
            }
            else if(k==kmin){
                to_a = a;
            }
            if(k<=0){// the best k value is 0, so no need to continue, if k is -1 , then not found
                break;
            }
        }
        if(from_a >= 0.0 && kmin==0) {
            for(double a=to_alpha; a>=from_alpha; a-=step) {
                k = evaluate_column_get_k(ea,class_uri,a);
                if(k==kmin){
                    to_a = a;
                    to_a_list->push_back(to_a);
                    break;
                }
            }
        }
        else if(from_a>= 0.0){ // kmin != 0
            to_a_list->push_back(to_a);
        }
        m_k->insert({fname,kmin});
        delete ea;
    }
    cout << "Alpha from median: "<<get_median(from_a_list)<<endl;
    cout << "Alpha to median: "<<get_median(to_a_list)<<endl;
}


string T2Dv2::clean_str(string s) {
    string c;
    unsigned long start_idx=0, len;
    if(s[0]=='"') {
        start_idx=1;
    }
    if(s[s.length()-1]=='"') {
        len = s.length()-1-start_idx;
    }
    else {
        len = s.length();
    }
    c = s.substr(start_idx, len);
    //    cout << "<"<<s<<"> to <"<<c<<">\n";
    return c;
}


double T2Dv2::get_median(std::list<double>* a){
    unsigned long len;
    std::list<double>::iterator b;
    double res;
    a->sort();
    b = a->begin();
//    for(auto it=a->cbegin();it!=a->cend();it++){
//        cout<< *it << " - ";
//    }
//    cout<<endl;
    len = a->size();
    if(len%2==1){//odd
        std::advance(b,len/2);
        res = *b;
//        cout << "odd: "<<res<<endl;
    }
    else{
        std::advance(b,(len-1)/2);
//        cout << "front: ["<<*b <<" --- ";
        res = *b;
        std::advance(b,1);
//        cout << ""<<*b <<" ] ";
        res+= *b;
        res = res/2;
//        cout << "even: "<<res<<endl;
    }
    return res;
}




