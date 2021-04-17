import sys
import os

outfdir = "semtab_properties.csv"

def clean_str(s):
    """
    :param s:
    :return:
    """
    start_idx = 0
    l = len(s)

    if s[0] == '"':
        start_idx += 1

    if s[-1] == '"':
        l -= 1

    clean_s = s[start_idx:l]
    return clean_s


def get_properties(files, pdir):
    """
    :param files:
    :param pdir:
    :return:
    """
    f = open(pdir)
    # properties = []
    for line in f.readlines():
        fname, subj_id, col_id, prop_uri = line.strip().split(",")
        fname = clean_str(fname)+".csv"
        col_id = clean_str(col_id)
        prop_uri = clean_str(prop_uri)
        j = {
            'property_uri': prop_uri,
            'col_id': col_id
        }
        if fname in files:
            if 'cols' not in files[fname]:
                files[fname]['cols'] = []
                files[fname]['cols'].append(j)

    return files

    #     j = {
    #         'property_uri': prop_uri,
    #         'fname': fname,
    #         'col_id': col_id
    #     }
    #     properties.append(j)
    # return properties


def get_classes(cdir):
    """
    :param cdir:
    :return:
    """
    f = open(cdir)
    files = dict()
    for line in f.readlines():
        fname, col_id, class_uri = line.strip().split(",")
        fname = clean_str(fname)+".csv"
        col_id = clean_str(col_id)
        class_uri = clean_str(class_uri)
        j = {
            'class_uri': class_uri,
            # 'fname': fname,
            'col_id': col_id
        }
        files[fname] = j
        # classes.append(j)
    return files


def workflow(pdir, cdir):
    """
    :param pdir:
    :param cdir:
    :return:
    """
    files = get_classes(cdir)
    files = get_properties(files, pdir)
    transform(files)


def transform(files):
    """
    :param files:
    :return:
    """
    f = open(outfdir, 'w')
    f.close()
    for fname in files.keys():
        if 'cols' in files[fname] and len(files[fname]['cols']) > 0:
            for colj in files[fname]['cols']:
                line = ",".join([fname, files[fname]['class_uri'], colj['property_uri'], files[fname]['col_id'],
                                 colj['col_id']])
                append(line)
        else:
            print("fname is not in: "+fname)


def append(line):
    f = open(outfdir, 'a+')
    f.write(line)
    f.write('\n')
    f.close()


if __name__ == '__main__':
    if len(sys.argv) == 3:
        _, pfdir, cfdir = sys.argv
        workflow(pfdir, cfdir)
    else:
        print("expects <properties dir from semtab> <classes dir from semtab>")


