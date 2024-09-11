import argparse
import json
import glob
import matplotlib.pyplot as plt
from collections import deque

parser = argparse.ArgumentParser()
parser.add_argument("-path", "--path", type=str,
                    help="Input the data's path, if not specified=D:\\\\combined/*.jpg",
                    default="D:\\\\combined/*.json")
parser.add_argument("-func", "--func", type=str,
                    help="Input the traversal algorithm (po/r/cmp), if not specified: Post Order",
                    default="diff", choices=['po', 'r', 'cmp'])
args = parser.parse_args()

OBJECT = "children"


def recursive_sonSearch(searchlist):
    max_sons = 0
    if isinstance(searchlist, dict) and OBJECT in searchlist:
        cur_list = searchlist[OBJECT]
        max_sons = len(cur_list)
        for i in range(max_sons):
            tmp_sons = recursive_sonSearch(cur_list[i])
            if max_sons < tmp_sons:
                max_sons = tmp_sons
    return max_sons


def postorder_traversal(searchlist):
    max_sons = 0
    if not searchlist:
        return 0
    child_mas = deque([searchlist])

    while child_mas:
        cur_son = child_mas.popleft()
        if isinstance(cur_son, dict) and OBJECT in cur_son:
            cur_list = cur_son[OBJECT]
            child_mas.append(len(cur_list))
            for tmp_son in cur_list:
                if tmp_son is not None:
                    child_mas.appendleft(tmp_son)
        elif isinstance(cur_son, int):
            if max_sons < cur_son:
                max_sons = cur_son
    return max_sons


histlist = []
cmp_output_fl = 1
for filename in glob.glob(args.path):
    data = json.load(open(filename, 'r'))["activity"]["root"]
    if args.func == "po":
        histlist.append(postorder_traversal(data))
        tmp_max = 0
    elif args.func == "r":
        sons = recursive_sonSearch(data)
        histlist.append(sons)
    else:
        if cmp_output_fl:
            print("CMP START, see DIFFs below:")
            cmp_output_fl = 0
        val1 = recursive_sonSearch(data)
        val2 = postorder_traversal(data)
        if val1 != val2:
            print(f"{val1} vs {val2}")
        tmp_max = 0
if not cmp_output_fl:
    print("CMP END")

plt.hist(histlist, bins=10, color='blue', edgecolor='black')
plt.title('MAX_GUI_SONS')
plt.ylabel('Nodes')
plt.xlabel('MAX_SONS')
plt.show()
