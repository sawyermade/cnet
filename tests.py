import sys, os, json, pickle, numpy as np
from tqdm import tqdm
from collections import OrderedDict
from conceptnet import ConceptNet

def main():
	# Args
	noun_list_path = sys.argv[1]
	verb_list_path = sys.argv[2]
	
	# Loads conceptnet 
	if len(sys.argv) > 3:
		cnet_pickle_path = sys.argv[3]
		with open(cnet_pickle_path, 'rb') as fp:
			cnet = pickle.load(fp)

	else:
		cnet = ConceptNet()
		with open('cnet.pkl', 'wb') as fp:
			pickle.dump(cnet, fp)

	# Opens jsons
	with open(noun_list_path) as fp:
		noun_list = json.load(fp)
	with open(verb_list_path) as fp:
		verb_list = json.load(fp)

	# Not sure yet, talk to mauricio
	# print(cnet.get_embedding('headset'))

	# print(cnet.query_concept('headset'))

	# print(cnet.query_concept('headset', directed=True))

	# print(cnet.query_edge('hand', 'arm'))

	# print(cnet.query_edge('hand', 'arm', directed=True))

	# print(cnet.query_edge('arm', 'hand', directed=True))


	# print(cnet.query_concept('spoon'))
	# print(cnet.query_edge('take', 'spoon'))
	# path = cnet.bfs_outbonds('take', 'spoon')

	# print(cnet.query_concept('water'))
	# print(cnet.outbonds['water'])
	# w = cnet.query_concept('water')
	# print(w.keys())
	# i = w['in']
	# o = w['out']
	# print(type(i), type(o))
	# ob = cnet.outbonds['water']
	# print(ob)

	### BFS ###
	# Noun to verb paths
	print('Starting noun to verb path outbonds...')
	noun_to_verb = OrderedDict()
	for noun in tqdm(noun_list[:10]):
		noun_to_verb[noun] = OrderedDict()
		for verb in verb_list[:10]:
			path = cnet.bfs_outbonds(noun, verb)
			noun_to_verb[noun][verb] = path

	# Saves noun to verb path outbonds
	out_file = 'noun_to_verb-outbonds.json'
	print(f'Saving file: {out_file}\n')
	with open(out_file, 'w') as fp:
		json.dump(noun_to_verb, fp, indent=3)


	# Verb to noun paths
	print('Starting verb to noun path outbonds...')
	verb_to_noun = OrderedDict()
	for verb in tqdm(verb_list):
		verb_to_noun[verb] = OrderedDict()
		for noun in noun_list:
			path = cnet.bfs_outbonds(verb, noun)
			verb_to_noun[verb][noun] = path

	# Saves verb to noun path outbonds
	out_file = 'verb_to_noun-outbonds.json'
	print(f'Saving file: {out_file}\n')
	with open(out_file, 'w') as fp:
		json.dump(verb_to_noun, fp, indent=3)

	### Number Batch ###
	# print('Getting Noun numberbatch embeddings...')
	# noun_nbe_list = []
	# for noun in tqdm(noun_list):
	# 	nbe = cnet.get_embedding(noun)
	# 	if nbe is None: print(noun)
	# 	noun_nbe_list.append(nbe.tolist())


	# print('\nGetting Verb numberbatch embeddings...')
	# verb_nbe_list = []
	# for verb in tqdm(verb_list):
	# 	nbe = cnet.get_embedding(verb)
	# 	if nbe is None: print(verb)
	# 	verb_nbe_list.append(nbe.tolist())

	# print('Getting dot products...')
	# nbe_dots = []
	# for i, noun in enumerate(tqdm(noun_list)):
	# 	nnbe = noun_nbe_list[i]
	# 	temp_dots = []
	# 	for j, verb in enumerate(verb_list):
	# 		vnbe = verb_nbe_list[j]
	# 		dp = np.dot(nnbe, vnbe)
	# 		temp_dots.append(dp)
	# 	nbe_dots.append(temp_dots)

	# print('Saving Json')
	# nbe_dict = {
	# 	'noun_list' : noun_list,
	# 	'verb_list' : verb_list,
	# 	'noun_nbe_list' : noun_nbe_list,
	# 	'verb_nbe_list' : verb_nbe_list,
	# 	'nbe_dots' : nbe_dots
	# }
	# json.dump(nbe_dict, open('nbe_dict.json', 'w'), indent=3)


if __name__ == '__main__':
	main()
	