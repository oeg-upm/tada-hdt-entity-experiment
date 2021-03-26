


alphas_fdir = "results/results-title-case/alpha_leaveout_alpha_log.csv"
gs_fdir = "datasets/t2dv2/classes_with_col_GS_2016_04.csv"

def fetch_alphas_and_files(alphas_file, gs_file):
    """
    :param alphas_file:
    :param gs_file:
    :return:
    """
    j = dict()
    f = open(alphas_file)
    headers = []
    for line in f.readlines():
        # headers = line.split(",")
        break

    for line in f.readlines():
        class_uri, fname, alpha = line.split(",")
        j[fname] = {
            "class_uri": class_uri,
            "alpha": alpha
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




