import requests
import os
import traceback

alphas_fdir = "results/results-title-case/alpha_leaveout_alpha_log.csv"
alphas_fdir = "alpha_leaveout_alpha_log.csv"
gs_fdir = "datasets/t2dv2/classes_with_col_GS_2016_04.csv"

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
        class_uri, fname, alpha = line.split(",")

        j[fname] = {
            "class_uri": class_uri,
            "alpha": alpha.strip()
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
    data = {
        'col_id': fdata["col_id"],
        'alpha': fdata["alpha"],
        'k': 1
    }

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

    print("\n====================")
    # if entities[0] == fdata["class_uri"]:
    #     print("Correct")
    # else:
    if entities[0] != fdata["class_uri"]:
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
    for fname in j.keys():
        if 'alpha' in j[fname]:
            ann = annotate(fname, j[fname])
            if ann:
                corr += 1
            else:
                incorr += 1
        else:
            # print(j[fname])
            nottested += 1
            singleclass_files.append(fname)

    print("corr: %d\n incorr: %d\nnot tested: %d\n" % (corr, incorr, nottested))
    print("single class files: ")
    print(singleclass_files)


if __name__ == '__main__':
    # data = {'class_uri': 'http://dbpedia.org/ontology/Lake', 'alpha': '0.368182', 'col_id': '0'}
    # fname = "3887681_0_7938589465814037992.csv"
    # annotate(fname, data)
    validate()
