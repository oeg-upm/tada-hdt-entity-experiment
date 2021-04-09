import requests
import os
import traceback


alphas_fdir = "results/optimal_alpha/original/alpha_leaveout_alpha_log.csv"
acc_fdir = "results/optimal_alpha/original/acc_pred_class.csv"
opt_alpha_fdir = "results/optimal_alpha/original/opt_alphas.csv"
gs_fdir = "datasets/t2dv2/classes_with_col_GS_2016_04.csv"

# single_alphas_fdir = "results/predicted_alpha/single_alphas_title_case.csv"
single_alphas_fdir = "results/predicted_alpha/single_alphas_original.csv"


# Contains the csv files of t2dv2
t2dv2_dir = os.environ["T2Dv2_dir"]
# the api for semantic annotation
TADA_HOST = os.environ['TADA_WB']


def fetch_alphas_and_files(alphas_file, gs_file):
    """
    :param alphas_file:
    :param gs_file:
    :return:
    """
    j = dict()
    f = open(alphas_file)

    for idx, line in enumerate(f.readlines()):
        if idx == 0:
            continue
        class_uri, fname, alpha, iscorr = line.strip().split(",")

        j[fname] = {
            "class_uri": class_uri,
            "alpha": alpha.strip(),
            "iscorr": iscorr
        }

    f.close()
    f = open(gs_file)

    for line in f.readlines():
        fname, colid, class_uri = line.split(",")
        if fname in j:
            j[fname]["col_id"] = colid
        else:
            j[fname] = {
                "col_id": colid,
                "class_uri": class_uri
            }

    f.close()
    return j


## Copied from OME
def annotate(fname, fdata):
# def annotate(source_dir, subject_col_id, top_k=3, logger=None):
    """
    :param source_dir: the directory of the source file
    :param subject_col_id: the index of the subject column
    :param top_k: the number of suggested classes to return
    :return: list of string (classes)
    """
    source_dir = os.path.join(t2dv2_dir, fname)
    print("fdata: ")
    print(fdata)

    try:
        data = {
            'col_id': fdata["col_id"],
            'alpha': fdata["alpha"],
            'k': 1
        }
    except Exception as e:
        print("Exception: ")
        print(str(e))
        traceback.print_exc()
        raise Exception("ABC")

    files = [
        ('source', (
            source_dir.split(os.sep)[-1],
            open(source_dir),
            'text/plain'))
    ]
    # print("==========================\n\nfdata: ")
    # print(fdata)
    # print("fname: ")
    # print(fname)
    response = requests.request("POST", TADA_HOST+'subject', data=data, files=files)

    if response.status_code == 200:
        # print("-- entities: ")
        # print(response.json())
        entities = response.json()['entities']
    else:
        print("-- ERROR: status code: "+str(response.status_code))
        entities = []

        try:
            print(response.json())
        except:
            print("No JSON")
            traceback.print_exc()
            raise Exception("No JSON")

    # if entities[0] == fdata["class_uri"]:
    #     print("Correct")
    # else:
    if entities[0] != fdata["class_uri"]:
        print("\n====================")
        print("Incorrect")
        print("entities: ")
        print(entities)
        print("fname: ")
        print(fname)
        print("data: ")
        print(fdata)

# if len(entities) == 0:
    #     return False
    return entities[0] == fdata["class_uri"]
    return entities


def validate():
    """

    :return:
    """
    corr = 0
    incorr = 0
    nottested = 0
    singleclass_files = []
    j = fetch_alphas_and_files(alphas_fdir, gs_fdir)
    class_scores = dict()
    for fname in j.keys():
        if 'alpha' in j[fname]:
            class_uri = j[fname]["class_uri"]
            ann_corr = annotate(fname, j[fname])
            if class_uri not in class_scores:
                class_scores[class_uri] = {"correct": [], "incorrect": [], "mismatch1": [], "mismatch2": []}
            if ann_corr:
                corr += 1
                if j[fname]["iscorr"] != "1":
                    print("mismatch1: "+fname)
                    print(j[fname])
                    class_scores[class_uri]["mismatch1"].append(fname)
                else:
                    class_scores[class_uri]["correct"].append(fname)
            else:
                incorr += 1
                if j[fname]["iscorr"] != "0":
                    print("mismatch2: "+fname)
                    print(j[fname])
                    class_scores[class_uri]["mismatch2"].append(fname)
                else:
                    class_scores[class_uri]["incorrect"].append(fname)

        else:
            # print(j[fname])
            nottested += 1
            singleclass_files.append(fname)

    print("corr: %d\n incorr: %d\nnot tested: %d\n" % (corr, incorr, nottested))
    print("single class files: ")
    print(singleclass_files)

    f = open(acc_fdir)
    for line in f.readlines():
        class_uri, acc = line.strip().split(",")
        if class_uri in class_scores:
            corr = len(class_scores[class_uri]["correct"])
            incorr = len(class_scores[class_uri]["incorrect"])
            tot = corr+incorr
            if tot > 0:
                comp_acc = (corr * 1.0) / tot
            else:
                comp_acc = -1.0
            class_scores[class_uri]["f_acc"] = round(float(acc), 2) # file-stored accuracy
            class_scores[class_uri]["c_acc"] = round(comp_acc, 2)  # computed accuracy
            class_scores[class_uri]["total"] = tot
            if class_scores[class_uri]["f_acc"] != class_scores[class_uri]["c_acc"]:
                print("Accuracy mismatch: "+class_uri)
                print(class_scores[class_uri])
                raise Exception("Mismatch")

    f.close()

    f = open(opt_alpha_fdir)
    for line in f.readlines():
        class_uri, opt_alpha = line.strip().split(",")
        if class_uri in class_scores:
            class_scores[class_uri]["opt_alpha"] = round(float(opt_alpha), 2)

    print("\n\n===================")
    for class_uri in class_scores.keys():
        print(class_uri)
        print("\t correct: "+str(class_scores[class_uri]["correct"]))
        print("\t incorrect: "+str(class_scores[class_uri]["incorrect"]))
        print("\t accuracy: " + str(class_scores[class_uri]["c_acc"]))
        print("\t total: " + str(class_scores[class_uri]["total"]))
        print("\t opt_alpha: " + str(class_scores[class_uri]["opt_alpha"]))

        # print("\t mismatch1: "+str(class_scores[class_uri]["mismatch1"]))
        # print("\t mismatch2: "+str(class_scores[class_uri]["mismatch2"]))
        print("\n")

    print("\n\n%%%%%%%%%%%%%%%%%%% Latex")
    tot_alpha = 0.0
    tot_acc = 0.0
    tot_files = 0
    for class_uri in class_scores.keys():
        cls_uri = class_uri.replace("http://dbpedia.org/ontology/", "dbo:")
        print("%s & %s & %s & %d \\\\ " % (cls_uri, str(class_scores[class_uri]["c_acc"]), str(class_scores[class_uri]["opt_alpha"]), class_scores[class_uri]["total"]))
        tot_acc += class_scores[class_uri]["c_acc"]
        tot_alpha += class_scores[class_uri]["opt_alpha"]
        tot_files += class_scores[class_uri]["total"]
        # print(class_uri)
        # print("\t correct: "+str(class_scores[class_uri]["correct"]))
        # print("\t incorrect: "+str(class_scores[class_uri]["incorrect"]))
        # print("\t accuracy: " + str(class_scores[class_uri]["c_acc"]))
        # print("\t total: " + str(class_scores[class_uri]["total"]))
        # print("\t mismatch1: "+str(class_scores[class_uri]["mismatch1"]))
        # print("\t mismatch2: "+str(class_scores[class_uri]["mismatch2"]))
    print("\n")
    print("total acc: "+str(tot_acc)+" and avg: "+str(tot_acc/len(class_scores.keys())))
    print("total alpha: "+str(tot_alpha)+" and avg: "+str(tot_alpha/len(class_scores.keys())))
    print("total files: "+str(tot_files)+" and avg: "+str(tot_files/len(class_scores.keys())))


def compute_scores_for_ks(ks=[0,2,4]):
    """
    k is starting from 0 and not from one
    :param ks:
    :return:
    """
    f = open(single_alphas_fdir, "r")
    d = dict()
    for k in ks:
        d[k] = {
            'corr': 0,
            'incorr': 0,
            'notfound': 0
        }
    for idx, line in enumerate(f.readlines()):
        if idx == 0:
            continue

        class_uri, fname, colid, alpha, str_k=  line.strip().split(',')
        pred_k = float(str_k)
        for k in ks:
            if pred_k == -1:
                d[k]['notfound'] += 1
            elif pred_k == -2 or pred_k > k:
                d[k]['incorr'] += 1
            elif pred_k <= k and pred_k >=0:
                d[k]['corr'] += 1
            else:
                print("ERROR: ")
                print(pred_k)
                raise Exception("un handeled case")
    for k in ks:
        print("\n==================")
        print("k: %d: " % (k+1))
        print("\tcorr: %d" % d[k]['corr'])
        print("\tincorr: %d" % d[k]['incorr'])
        print("\tnotfound: %d" % d[k]['notfound'])

        prec = d[k]['corr'] * 1.0 /(d[k]['corr'] + d[k]['incorr'])
        rec = d[k]['corr'] * 1.0 /(d[k]['corr'] + d[k]['notfound'])
        f1 = 2.0 * prec * rec / (rec+prec)

        # prec = round(prec, 2)
        # rec = round(rec, 2)
        # f1 = round(f1, 2)
        print("\tprecision: %f (%f)" % (prec, round(prec, 2)))
        print("\trecall: %f (%f)" % (rec, round(rec, 2)))
        print("\tF1: %f (%f)" % (f1, round(f1, 2)))


if __name__ == '__main__':
    # data = {'class_uri': 'http://dbpedia.org/ontology/Lake', 'alpha': '0.368182', 'col_id': '0'}
    # fname = "3887681_0_7938589465814037992.csv"
    # annotate(fname, data)
    # validate()
    compute_scores_for_ks()
