import sys, os, json, pickle, numpy as np
from tqdm import tqdm
from collections import OrderedDict
from conceptnet import ConceptNet


def dj_topk(f, t, k):
	pass


def get_paths(cnet, from_list, to_list, k, stop=None):
	# Generate Sub graph
	print('Starting from to to path outbonds...')
	path_dict = OrderedDict()
	for f in tqdm(from_list[:stop]):
		path_dict[f] = OrderedDict()
		for t in to_list[:stop]:
			# path = cnet.bfs_outbonds(noun, verb)
			path = dj_topk(f, t, k)
			path_dict[f][t] = path

	return path_dict


def main():
	# Args
	# noun_list_path = sys.argv[1]
	# verb_list_path = sys.argv[2]
	# cnet_pickle_path = sys.argv[3]
	# k = sys.argv[4]
	noun_list_path = 'data/classes_nouns-fixed.json'
	verb_list_path = 'data/classes_verbs-fixed.json'
	cnet_pickle_path = 'data/cnet.pkl'
	k = 1

	# Open noun and verb jsons
	with open(noun_list_path) as fp:
		noun_list = json.load(fp)
	with open(verb_list_path) as fp:
		verb_list = json.load(fp)

	# Load ConceptNet
	with open(cnet_pickle_path, 'rb') as fp:
		cnet = pickle.load(fp)

	# Create sub graph
	noun_to_verb_paths = get_paths(cnet, noun_list, verb_list, k)
	verb_to_noun_paths = get_paths(cnet, verb_list, noun_list, k)


if __name__ == '__main__':
	main()