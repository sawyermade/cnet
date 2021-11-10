import os, sys, json, torch, numpy as np

def main():
	# Args
	output_network_dir = 'output_networks'
	gt_path = 'data/epic_100_gt-val.json'
	out_k = 'stats/output_top_k.json'
	out_k_counts = 'stats/output_top_k_counts.json'

	# Get paths to output networks
	val_test = 'val'
	network_paths = [os.path.join(output_network_dir, 'trn_rgb-val.pt')]
	# for d in os.listdir(output_network_dir):
	# 	if val_test in d:
	# 		p = os.path.join(output_network_dir, d)
	# 		network_paths.append(p)
	# network_paths.sort()
	# print(network_paths)

	# Open gt json
	with open(gt_path) as fp:
		gt_dict = json.load(fp)

	# Goes through networks
	top_k_dict = {}
	for net_path in network_paths:
		# Load output from network
		net_output = torch.load(net_path)

		# Get network type
		net_type = os.path.split(net_path)[-1][:3] + f'_{val_test}'
		print(f'net_type: {net_type}\n')

		# Add network type to dict
		top_k_dict[net_type] = {}

		# Go through segments
		for seg_dict in net_output:
			# Pull ID, verb, and noun outputs
			seg_id = seg_dict['narration_id']
			verb_output = seg_dict['verb_output']
			noun_output = seg_dict['noun_output']

			# Converts to tensor, softmax, then numpy
			verb_output = torch.Tensor(verb_output).softmax(dim=0).numpy()
			noun_output = torch.Tensor(noun_output).softmax(dim=0).numpy()

			# Gets max indice list
			verb_sorted = np.argsort(verb_output)[::-1]
			noun_sorted = np.argsort(noun_output)[::-1]
			
			# Pull gt verb and noun from segment ID
			verb_gt = gt_dict[seg_id]['verb_class']
			noun_gt = gt_dict[seg_id]['noun_class']

			# Get rank of gt in output sorted
			verb_rank = int(verb_sorted[verb_sorted == verb_gt][0])
			noun_rank = int(noun_sorted[noun_sorted == noun_gt][0])

			# Add segment id to dict
			top_k_dict[net_type][seg_id] = {
				'verb' : verb_rank,
				'noun' : noun_rank
			}

	# Get top k counts
	top_k_counts = {}
	for net_type, seg_dict in top_k_dict.items():
		# Add net type to dict
		top_k_counts[net_type] = {}

		# Count verbs
		total = 97
		key = 'verb'
		top_k_counts[net_type][key] = {}
		for i in range(total):
			top_k = f'top_{i+1}'
			top_k_counts[net_type][key][top_k] = 0

			for seg_id, verb_noun_dict in seg_dict.items():
				if verb_noun_dict[key] < i+1:
					top_k_counts[net_type][key][top_k] += 1

		# Count nouns
		total = 300
		key = 'noun'
		top_k_counts[net_type][key] = {}
		for i in range(total):
			top_k = f'top_{i+1}'
			top_k_counts[net_type][key][top_k] = 0

			for seg_id, verb_noun_dict in seg_dict.items():
				if verb_noun_dict[key] < i+1:
					top_k_counts[net_type][key][top_k] += 1

	# Save count dicts
	with open(out_k, 'w') as fp:
		json.dump(top_k_dict, fp, indent=3)
	with open(out_k_counts, 'w') as fp:
		json.dump(top_k_counts, fp, indent=3)


if __name__ == '__main__':
	main()