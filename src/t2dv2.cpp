#include "t2dv2.h"
#include<tada_hdt_entity/entity.h>
#include<tabular_parser/parser.h>
#include <climits>
#include <fstream>
#include <iostream>

T2Dv2::T2Dv2(string hdt_dir, string log_file_dir, string classes_file_dir, string files_dir) {
    m_classes_file_dir = classes_file_dir;
    m_files_dir = files_dir;
    m_correct = m_incorrect = m_notfound = 0;
    m_hdt = HDTManager::mapIndexedHDT(hdt_dir.c_str());
    m_logger = new EasyLogger(log_file_dir);
    m_k = new unordered_map<string, long>;
    m_lang_tag = "";
}

void T2Dv2::set_file_sep(string sep) {
    m_file_sep = sep;
}

string T2Dv2::get_file_sep() {
    return m_file_sep;
}

EntityAnn* T2Dv2::get_ea_model(string fname, unsigned int col_idx, bool context) {
    //    cout << "HEADER get_ea_model\n";
    cout << "get EA Model fname: <"<<fname<<">\n";
    cout << "col_idx: <"<<col_idx<<">\n";
    //    cout << "context: <"<<context<<">\n";
    EntityAnn* ea = new EntityAnn(m_hdt, "entity.log");
    //ea->set_language_tag("@en");
    ea->set_title_case(m_title_case);
    ea->set_language_tag(m_lang_tag);
    string file_dir;
    std::list<string>* candidates;
    file_dir = m_files_dir;
    if(file_dir.substr(m_files_dir.length()-m_file_sep.length()) != m_file_sep) {
        file_dir += m_file_sep;
    }
    file_dir += fname;
    Parser p(file_dir);
    if(context) {
        //        cout << "title case: <"<<m_title_case<<">"<<endl;
        //        cout << "m_lang_tag: <"<<m_lang_tag<<">"<<endl;
        //        m_logger->log("m_title_case: ");
        //        m_logger->log("get_ea_model>  with context");
        //        m_logger->log("get_ea_model>  file_dir: "+file_dir);
        //        m_logger->log("get_ea_model>  col_idx: "+to_string(col_idx));
        //        cout << "file_dir: "<<file_dir<<endl;
        //        cout << "col_idx: "<<col_idx<<endl;
        //        cout << "pre parse:\n";
        //        p.parse_vertical();
        //        cout <<" parse vertical\n";
        candidates = ea->annotate_column(p.parse_vertical(), col_idx, context, context);
        //        cout << "11out candidate: annotate_column\n";
        //        m_logger->log("11get_ea_model>  annotate_column post");
    }
    else {
        //        cout << "000 pre No context out candidate: annotate_column\n";
        //        m_logger->log("000 get_ea_model>  without context");
        candidates = ea->annotate_column(p.parse(), col_idx);
        //        cout << "000 post No context out candidate: annotate_column\n";
    }
    //    cout << "000  finished\n";
    //    m_logger->log("000get_ea_model> num of candidates: "+to_string(candidates->size()));
    //    cout << "000 finished\n";
    //    ea->get_graph()->print_nodes();
    //    delete ea;
    delete candidates;
    return ea;
}

std::list<string>* T2Dv2::annotate_column(string fname, unsigned int col_idx, bool context, double alpha) {
    EntityAnn* ea = new EntityAnn(m_hdt, "entity.log", alpha);
    ea->set_title_case(m_title_case);
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
    if(candidates->size() == 0) {
        k = -1; // not found
    }
    else {
        k=-2; // incorrect
    }
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

// starts with k.
long T2Dv2::evaluate_column_get_k(EntityAnn* ea, string class_uri, double alpha) {
    long k=0;
    std::list<string>* candidates;
    candidates = ea->recompute_f(alpha);
    m_logger->log("evaluate_column_get_k> number of candidates: "+to_string(candidates->size()));
    for(auto it = candidates->cbegin(); it!=candidates->cend(); it++, k++) {
        m_logger->log("evaluate_column_at_k> candidate "+to_string(k)+" is: "+((*it)));
        if((*it)==class_uri) {
            delete candidates;
            return k;
        }
        else {
        }
    }
    if(candidates->size() == 0) {
        k = -1; // not found
    }
    else {
        k=-2; // incorrect
    }
    //    cout<< "\n\nevaluate_column_get_k> \n with size"<< candidates->size()<<"\n";
    //    cout <<"class uri<"<<class_uri<<"> and candidates: \n";
    //    ea->get_graph()->print_nodes();
    //    k=-1;
    delete candidates;
    return k;
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


void T2Dv2::print_k(long k) {
    std::list<string>* not_found = new std::list<string>;
    std::list<string>* incorrect = new std::list<string>;
    for(auto it=m_k->cbegin(); it!=m_k->cend(); it++) {
        if(it->second == -1) {
            not_found->push_back(it->first);
        }
        else if(it->second > k || it->second==-2) {
            incorrect->push_back(it->first);
        }
    }
    cout << "\n\nNot found: "<<endl;
    for(auto it=not_found->cbegin(); it!=not_found->cend(); it++) {
        cout << (*it) << endl;
    }
    cout << "\nIncorrect: "<<endl;
    for(auto it=incorrect->cbegin(); it!=incorrect->cend(); it++) {
        cout << (*it) << "  (k = " << m_k->at((*it)) << ")" <<endl;
    }
    delete not_found;
    delete incorrect;
}










void T2Dv2::compute_scores(long k) {
    m_correct=m_incorrect=m_notfound = 0;
    m_prec=m_rec=m_f1 = 0.0;
    for(auto it=m_k->cbegin(); it!=m_k->cend(); it++) {
        if(it->second == -1) {
            m_notfound++;
        }
        else if(it->second > k || it->second==-2) {
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
        ea = get_ea_model(fname, col_id, m_inner_context);
        //        ea = get_ea_model(fname, col_id, true);
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
    m_logger->log("run_test> with classes file: "+m_classes_file_dir);
    for(auto it=data->cbegin(); it!=data->cend(); it++) {
        m_logger->log("run_test> in for loop with length "+to_string((*it)->size()));
        //        cout << "\n\n---------------\n";
        col_iter = (*it)->begin();
        fname = clean_str(*col_iter);
        col_iter++;
        col_id_str = clean_str(*col_iter);
        col_id = static_cast<unsigned int>(stoul(col_id_str));
        col_iter++;
        class_uri = clean_str(*col_iter);
        //        cout << "class_uri: <" << class_uri << ">"<<endl;
        //        cout << "col_idx: <" << col_id_str <<">" <<endl;
        //        cout << "fname: <"<<fname << ">"<<endl;
        ea = get_ea_model(fname, col_id, true);
        //        ea->get_graph()->print_nodes();
        m_logger->log("run_test> got model of: "+fname);
        from_a = to_a = -1;
        k = kmin = LONG_MAX;
        for(double a=from_alpha; a<=to_alpha; a+=step) {
            k = evaluate_column_get_k(ea, class_uri, a);
            m_logger->log("run_test> got k of alpha "+to_string(a)+" k="+to_string(k));
            if(k<kmin && k>=0) {
                kmin = k;
                from_a = a;
                to_a = a;
                //from_a_list->push_back(from_a);
            }
            else if(k==kmin) {
                to_a = a;
            }
            if(k<=0) { // the best k value is 0, so no need to continue, if k is -1 or -2 , then not found/ incorrect
                //                cout<< "within break, k: "<<k<<endl;
                break;
            }
        }
        m_logger->log("run_test> kmin="+to_string(kmin));
        if(from_a >= 0.0 && kmin==0) {
            for(double a=to_alpha; a>=from_alpha; a-=step) {
                k = evaluate_column_get_k(ea, class_uri, a);
                if(k==kmin) {
                    to_a = a;
                    to_a_list->push_back(to_a);
                    from_a_list->push_back(from_a);
                    break;
                }
            }
        }
        else if(from_a>= 0.0) { // kmin != 0
            to_a_list->push_back(to_a);
            from_a_list->push_back(from_a);
        }
        else { // k=-2 or -1
            kmin=k;
            m_logger->log("run_test> set negative k: "+to_string(k));
        }
        m_logger->log("run_test> insert "+fname+" and kmin="+to_string(kmin));
        m_k->insert({fname, kmin});
        delete ea;
        m_logger->log("run_test> ea is deleted");
    }
    m_logger->log("run_test> outside the for");
    cout << "Alpha from median: "<<get_median(from_a_list)<<endl;
    cout << "Alpha to median: "<<get_median(to_a_list)<<endl;
    delete from_a_list;
    delete to_a_list;
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


double T2Dv2::get_median(std::list<double>* a) {
    unsigned long len;
    std::list<double>::iterator b;
    double res;
    if(a->size() == 0) {
        cout<<"\nget_median> size is zero\n";
        return 0.0;
    }
    a->sort();
    b = a->begin();
    //    cout << "\n\nget_median\n";
    //    cout << "size: " << a->size() << endl;
    //    for(auto it=a->begin();it!=a->end();it++){
    //        cout<< *it << " - ";
    //    }
    //    cout<<endl;
    len = a->size();
    if(len%2==1) { //odd
        std::advance(b, len/2);
        res = *b;
        //        cout << "odd: "<<res<<endl;
    }
    else {
        std::advance(b, (len-1)/2);
        //        cout << "front: ["<<*b <<" --- ";
        res = *b;
        std::advance(b, 1);
        //        cout << ""<<*b <<" ] ";
        res+= *b;
        res = res/2;
        //        cout << "even: "<<res<<endl;
    }
    return res;
}

void T2Dv2::set_lang_tag(string t) {
    m_lang_tag = t;
}

void T2Dv2::set_title_case(bool t) {
    m_title_case = t;
}

void T2Dv2::set_inner_context(bool t) {
    m_inner_context = t;
}


void T2Dv2::generate_properties_file(string input_fdir, string output_fdir) {
    std::list<std::list<string>*>* data;
    std::list<string>::iterator col_iter;
    ofstream output_file;
    Parser p(input_fdir);
    data = p.parse_vertical();
    string fname, class_uri, property_uri, col_id_str;
    unsigned long col_id;
    output_file.open(output_fdir);
    long num_exist, num_not;
    num_exist = num_not = 0;
    for(auto it=data->cbegin(); it!=data->cend(); it++) {
        col_iter = (*it)->begin();
        fname = clean_str(*col_iter);
        col_iter++;
        class_uri = clean_str(*col_iter);
        col_iter++;
        property_uri = clean_str(*col_iter);
        col_iter++;
        col_id_str = clean_str(*col_iter);
        col_id = static_cast<unsigned long>(stoul(col_id_str));
        //        cout << "fname: "<<fname<<endl;
        //        cout << "class uri: "<<class_uri<<endl;
        //        cout << "property uri: "<<property_uri<<endl;
        //        cout << "col id: "<<col_id<<endl;
        if(property_class_exist(class_uri, property_uri)) {
            output_file << fname << "," << class_uri << "," << property_uri << "," << col_id_str <<endl;
            num_exist++;
        }
        else {
            cout << "fname: "<<fname<<endl;
            cout << "class uri: "<<class_uri<<endl;
            cout << "property uri: "<<property_uri<<endl;
            cout << "col id: "<<col_id<<endl;
            num_not++;
        }
    }
    cout << "exist: "<<num_exist<<endl;
    cout << "not: "<<num_not<<endl;
    cout << "percentage: "<<(num_exist*100.0/(num_not+num_exist))<<endl;
    output_file.close();
}

bool T2Dv2::property_class_exist(string class_uri, string property_uri) {
    IteratorTripleString* itt;
    TripleString* triple;
    IteratorTripleString* itt2;
    TripleString* triple2;
    bool found = false;
    string entity;
    // first check if the property exists
    itt = m_hdt->search("", property_uri.c_str(), "");
    m_logger->log("property_class_exist> search properties class: <"+class_uri+"> ----- property: <"+property_uri+">");
    while(itt->hasNext()) {
        triple = itt->next();
        m_logger->log("property_class_exist> property exists: "+property_uri);
        found = true;
        break;
    }
    delete itt;
    if(!found) {
        return false;
    }
    itt = m_hdt->search("", rdf_type.c_str(), class_uri.c_str());
    m_logger->log("property_class_exist> get entities: <"+class_uri+"> ----- property: <"+property_uri+">");
    while(itt->hasNext()) {
        triple = itt->next();
        entity = triple->getSubject();
        itt2 = m_hdt->search(entity.c_str(), property_uri.c_str(), "");
        while(itt2->hasNext()) {
            m_logger->log("property_class_exist> found pair: <"+entity+"> --- <"+property_uri+">");
            delete itt2;
            delete  itt;
            return true;
        }
        delete itt2;
    }
    delete itt;
    return false;
}



void T2Dv2::run_test_properties(string properties_fdir, char mode) {
    std::list<std::list<string>*>*  data;
    std::list<string>* properties;
    string class_uri, col_id_str, fname, property_uri, key;
    long col_id, prop_id, k;
    bool added=false;
    std::list<string>::iterator col_iter;
    EntityAnn* ea =  new EntityAnn(m_hdt, "entity.log", 0);
    ea->set_title_case(m_title_case);
    ea->set_language_tag(m_lang_tag);
    Parser p(properties_fdir);
    data = p.parse_vertical();
    Parser* p2;
    for(auto it=data->cbegin(); it!=data->cend(); it++) {
        m_logger->log("run_test> in for loop with length "+to_string((*it)->size()));
        //        cout << "\n\n---------------\n";
        col_iter = (*it)->begin();
        fname = clean_str(*col_iter);
        col_iter++;
        class_uri = clean_str(*col_iter);
        col_iter++;
        property_uri = clean_str(*col_iter);
        col_iter++;
        col_id_str = clean_str(*col_iter);
        col_id = static_cast<unsigned int>(stol(col_id_str));
        col_iter++;
        col_id_str = clean_str(*col_iter);
        prop_id = static_cast<unsigned int>(stol(col_id_str));
        p2 = new Parser(m_files_dir+m_file_sep+fname);
        if(mode==PERMISSIVE_MODE) {
            properties = ea->annotate_entity_property_heuristic(p2->parse_vertical(), class_uri, prop_id);
        }
        else if(mode==RESTRICTIVE_MODE || mode==HEURISTIC_MODE) { // restrictive or heuristic
            properties = ea->annotate_entity_property_column(p2->parse_vertical(), col_id, prop_id);
        }
        else {
            cout << "ERROR: Wrong mode: <"<<mode<<">\n";
            return ;
        }
        k=0;
        added=false;
        key = fname+"--"+col_id_str;
        for(auto it2=properties->cbegin(); it2!=properties->cend(); it2++, k++) {
            if((*it2) ==  property_uri) {
                m_k->insert({key, k});
                //                cout << "run_test_properties> fname: "+fname+" col_id: "+col_id_str+" top property"+properties->front() <<endl;
                m_logger->log("run_test_properties> restrictive -- found: "+key);
                added=true;
                break;
            }
        }
        if(mode==HEURISTIC_MODE && added==false) {
            properties = ea->annotate_entity_property_heuristic(p2->parse_vertical(), class_uri, prop_id);
            k=0;
            for(auto it2=properties->cbegin(); it2!=properties->cend(); it2++, k++) {
                if((*it2) ==  property_uri) {
                    m_k->insert({key, k});
                    m_logger->log("run_test_properties> heuristic -- found: "+key);
                    added=true;
                    break;
                }
            }
        }//if heuristic
        if(added==false) {
            m_logger->log("run_test_properties> notfound fname: "+fname+" col_id: "+col_id_str+" top property: "+properties->front());
            m_k->insert({key, -1});
        }
        delete properties;
        delete p2;
    }
}




























void T2Dv2::get_classes_and_columns() {
    std::list<std::list<string>* >*  data;
    std::list<string>::iterator col_iter;
    string class_uri, col_id_str, fname;
    unsigned int col_id;
    Parser p(m_classes_file_dir);
    data = p.parse_vertical();
    for(auto it=data->cbegin(); it!=data->cend(); it++) {
        //        m_logger->log("run_test> in for loop with length "+to_string((*it)->size()));
        col_iter = (*it)->begin();
        fname = clean_str(*col_iter);
        col_iter++;
        col_id_str = clean_str(*col_iter);
        col_id = static_cast<unsigned int>(stoul(col_id_str));
        col_iter++;
        class_uri = clean_str(*col_iter);
        //        m_logger->log("fname: "+fname+" | col_id: "+col_id_str+" | class_uri: "+class_uri);
        //        m_logger->log("class: "+class_uri);
        if(m_classes_col_names.find(class_uri)==m_classes_col_names.cend()) {
            //            m_logger->log("new");
            m_classes_col_names.insert(std::make_pair(class_uri, vector < pair< string, unsigned int > >()));
        }
        //        else {
        //            m_logger->log("already exists");
        //        }
        m_classes_col_names[class_uri].push_back(std::make_pair(fname, col_id));
    }
}

void T2Dv2::run_entity_test_on_a_file(string class_uri, string fname, unsigned int col_id, double from_alpha, double to_alpha, double step) {
    string func_name = __func__;
    long k = -2;
    EntityAnn* ea;
    double from_a, to_a;
    m_logger->log(func_name+"> pre get_ea_model");
    ea = get_ea_model(fname, col_id, true);
    m_logger->log(func_name+"> post get_ea_model");
    m_logger->log("run_entity_test_on_a_file> got model of: "+fname);
    m_logger->log("fname: "+fname+" | col_id:"+to_string(col_id)+" | class_uri: "+class_uri);
    from_a = to_a = -1;
    std::list<pair<double, double> > alphas_ranges;
    for(double a=from_alpha; a<=to_alpha; a+=step) {
        k = evaluate_column_get_k(ea, class_uri, a);
        m_logger->log("run_entity_test_on_a_file> got k of alpha "+to_string(a)+" k="+to_string(k));
        if(k==0) {
            if(from_a<0) {
                from_a = a;
            }
            to_a = a;
        }
        else if(k<0) {
            break;
        }
        else if(from_a >=0) { // going away from the optimal alpha
            alphas_ranges.push_back(make_pair(from_a, to_a));
            from_a = to_a = -1;
        }
    }
    m_logger->log("run_entity_test_on_a_file> number of ranges: "+to_string(alphas_ranges.size()));
    for(auto it=alphas_ranges.cbegin(); it!=alphas_ranges.cend(); it++) {
        m_logger->log("run_entity_test_on_a_file> "+to_string(it->first)+" - "+to_string(it->second)+" : "+to_string((it->first+it->second)/2.0));
    }
    if(alphas_ranges.size()==0) {
        m_logger->log(func_name+"> unable to find a label for the entity column of the file: "+fname);
    }
    if(alphas_ranges.size()>1) {
        m_logger->log("run_entity_test_on_a_file> alphabingo");
    }
    //    if(m_files_alpha.find(fname)==m_files_alpha.cend()){
    //        m_logger->log("run_entity_test_on_a_file> Error: the file <"+fname+"> is already processed");
    //        m_logger->log("with alpha: "+to_string(m_files_alpha[fname]));
    //        m_logger->log("and current computed alpha is: "+to_string(alphas_ranges.cbegin()->first + alphas_ranges.cbegin()->second / 2.0));
    //        throw 123;
    //    }
    //    if(k < 0){
    //        m_files_alpha[fname]
    //    }
    //    else{
    if(k >=0 ) {
        m_files_alpha[fname] = (alphas_ranges.cbegin()->first + alphas_ranges.cbegin()->second) / 2.0;
    }
    delete ea;
}


void T2Dv2::run_entity_and_compute_alphas() {
    string class_uri;
    string fname;
    unsigned int col_id;
    string func_name = __func__;
    if(m_classes_col_names.size()==0) {
        m_logger->log(func_name+"> classes and files and not yet fetched. Will be fetched now");
        get_classes_and_columns();
        //        m_logger->log(func_name+"> fetched");
    }
    for(auto it=m_classes_col_names.cbegin(); it!=m_classes_col_names.cend(); it++) {
        class_uri = it->first;
        m_logger->log("=================================");
        m_logger->log(class_uri+"\n");
        //cout<< endl << class_uri <<"\n";
        if(it->second.size() > 1) {
            for(auto it2=it->second.cbegin(); it2!=it->second.cend(); it2++) {
                fname = it2->first;
                col_id = it2->second;
                m_logger->log("\t\t"+fname+", "+to_string(col_id)+"\n");
                run_entity_test_on_a_file(class_uri, fname, col_id, 0, 1, 0.01);
                //                cout << "\t\t" << it2->first << ", " << it2->second << endl;
            }
        }
        else {
            m_logger->log(func_name+"> skip: ("+to_string(it->second.size())+") -- "+class_uri);
        }
    }
}


void T2Dv2::run_entity_test_left_one_out_class(string class_uri, string alphas_out_log) {
    // alphas_out_log: is the file which contain the alpha applied to each one of the files
    string func_name = __func__;
    double alpha_sum, alpha_avg;
    string fname, tfname;
    double out_alpha, class_alpha, acc_sum;
    unsigned int col_id;
    unsigned int corr, incorr, num_training, num_files_with_opt_alpha;
    vector < pair < string, unsigned int > >* vec_ptr = &m_classes_col_names[class_uri];   // to speed up
    class_alpha = 0;
    num_files_with_opt_alpha=0;
    acc_sum=0;
    for(size_t i=0; i<vec_ptr->size(); i++) { // leave out index
        corr = incorr = 0;
        fname = vec_ptr->at(i).first;
        col_id = vec_ptr->at(i).second;
        if(m_files_alpha.find(fname)!=m_files_alpha.cend()) { // out file has a correct alpha
            out_alpha = m_files_alpha[fname]; // store a copy of the alpha
            class_alpha += out_alpha;  // to compute the optimal alpha for the class
            num_files_with_opt_alpha +=1;
            m_files_alpha[fname] = -1; // to check later if the passed alpha resulted in a correct class
            alpha_sum = 0; // sum of the training alphas
            num_training = 0;
            for(size_t j=0; j<vec_ptr->size(); j++) {
                tfname = vec_ptr->at(j).first;
                //ensure that the training file is different than the testing file (out file)
                if(i!=j && m_files_alpha.find(tfname)!=m_files_alpha.cend()) { // ensure the training file has an optimal alpha
                    alpha_sum += m_files_alpha[tfname];
                    num_training +=1;
                }
            }
            if(num_training>0) {
                alpha_avg = alpha_sum / num_training; // get average alpha
                this->append_to_file(alphas_out_log, class_uri+","+fname+","+to_string(alpha_avg)+"\n");
                //                this->append_to_file("alpha_leaveout_alpha_log.csv", class_uri+","+fname+","+to_string(alpha_avg)+"\n");
                run_entity_test_on_a_file(class_uri, fname, col_id, alpha_avg, alpha_avg+0.01, 2); // so it will just test on alpha_avg only
                if(m_files_alpha[fname] >= 0) {
                    corr +=1;
                }
                else {
                    incorr +=1;
                }
                m_files_alpha[fname] = out_alpha;// return the original alpha
                acc_sum += (corr * 1.0)/(corr+incorr);
            } // training > 0
            else {
                m_logger->log(func_name+"> SINGLE: There is a single file with optimal alpha for the class: "+class_uri);
                return; // this mean that there is only a single file in this class with an optimal alpha
            }
        } // if out file has a correct alpha
    }// leaf out for
    if(vec_ptr->size() > 1) {
        class_alpha = class_alpha / (num_files_with_opt_alpha);
        m_classes_opt_alpha[class_uri] = class_alpha;
        m_classes_pred_acc[class_uri] = acc_sum / num_files_with_opt_alpha;
    }
}


void T2Dv2::run_entity_test_left_one_out_all(string alphas_out_log, string alphas_opt, string class_acc) {
    // alphas_out contains the alphas tested for each file
    // the alphas_opt contains the optimal alpha for each class
    // class_acc contains alpha prediction accuracy for leaving one out
    string class_uri;
    string func_name = __func__;
    if(m_classes_col_names.size()==0) {
        m_logger->log(func_name+"> classes and files and not yet fetched. Will be fetched now");
        get_classes_and_columns();
        //        m_logger->log(func_name+"> fetched");
    }
    //    this->append_to_file("alpha_leaveout_alpha_log.csv", "class_uri,fname,alpha\n");
    this->append_to_file(alphas_out_log, "class_uri,fname,alpha\n");
    run_entity_and_compute_alphas();
    cout << "=================alphas===============\n";
    for(auto it=m_classes_col_names.cbegin(); it!=m_classes_col_names.cend(); it++) {
        class_uri = it->first;
        cout<< endl << class_uri <<"\n";
        run_entity_test_left_one_out_class(class_uri, alphas_out_log);
    }
    string sep=",";
    //    ofstream opt_alphas("opt_alphas.csv");
    ofstream opt_alphas(alphas_opt);
    opt_alphas << "class" << sep << "alpha" << endl;
    for(auto it=m_classes_opt_alpha.cbegin(); it!=m_classes_opt_alpha.cend(); it++) {
        opt_alphas << it->first << sep << it->second << endl;
    }
    opt_alphas.close();
    //    ofstream acc_class("acc_pred_class.csv");
    ofstream acc_class(class_acc);
    acc_class << "class" << sep << "accuracy" << endl;
    for(auto it=m_classes_pred_acc.cbegin(); it!=m_classes_pred_acc.cend(); it++) {
        acc_class << it->first << sep << it->second << endl;
    }
    acc_class.close();
}

void T2Dv2::append_to_file(string fdir, string line) {
    ofstream f(fdir, ofstream::out | ofstream::app);
    f << line;
    f.close();
}
