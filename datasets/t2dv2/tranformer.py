import os
import sys

def clean(b):
    a = b.strip()
    if a == "":
        return a
    start_idx = 0
    end_idx = len(a)
    if a[0] == '"':
        start_idx = 1
    if a[-1] == '"':
        end_idx -= 1
    return a[start_idx:end_idx]
    
        
def get_properties_of_a_file(fdir,class_uri):
    f = open(fdir)
    fname = fdir.split(os.path.sep)[-1]
    lines = []
    for line in f.readlines():
#        print("line: "+line)
        attributes_ = line.split(',')
        attributes = [clean(a) for a in attributes_]
#        print("attributes: ")
#        print(attributes)
        if len(attributes) <3:
            print("fdir: "+fdir)
            print("will skip this line: <"+line.strip()+">")
            continue
        if attributes[2].upper() == 'FALSE':
#            print("in false")
            property_uri = attributes[0].strip()
            p = [fname,class_uri,property_uri,attributes[3]]
#            p = [fname,class_uri,property_uri,str(int(attributes[3])-1)]
            lines.append(",".join(p))
#            print("p: ")
#            print(p)
#            print("len: lines: ")
#            print(len(lines))
#        else:
#            print("another: <"+attributes[2]+">")
#    print("lines: ")
#    print(lines)
    if len(lines) == 0:
        print("no properties in file: "+fdir)
    return lines


def write_properties(p_base_fdir,classes_fdir,output_fdir):
    f = open(classes_fdir)
    lines = []
    for line_ in f.readlines():
        line = line_.strip()
        if line == "":
            continue
        attributes = line.split(',')
        fname = attributes[0]
        class_uri = attributes[2]
#        print("fname: "+fname)
        lines += get_properties_of_a_file(os.path.join(p_base_fdir,fname),class_uri)
    f.close()
#    print("lines: ")
#    print(lines)
    f = open(output_fdir,'w')
    f.write("\n".join(lines))
    f.close()


    
if __name__ == "__main__":
    args = sys.argv
    if len(args) == 4:
        write_properties(args[1],args[2],args[3])
    else:
        print("Args: properties_base_dir, classes_file_dir, output_file_dir")
#        a = "/Users/aalobaid/Downloads/extended_instance_goldstandard (6)/property/99070098_0_2074872741302696997.csv"
#        get_properties_of_a_file(a,"something")
