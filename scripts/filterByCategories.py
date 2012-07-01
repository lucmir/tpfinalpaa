#!/usr/bin/python

import sys
import getopt


def read_item_cluster_map(item_cluster_file_name):
	file = open(item_cluster_file_name, "r" )

	item_cluster_map = []
	item_cluster_map.append(0)

	max_cluster_id = 0;

	for line in file:
		cluster_id = int(line.strip())
		item_cluster_map.append(cluster_id)
		if cluster_id > max_cluster_id:
			max_cluster_id = cluster_id

	file.close()

	num_clusters = max_cluster_id + 1

	return item_cluster_map, num_clusters


def get_ratings_by_item(in_file_name, out_file_name, item_cluster_map, num_clusters):
	ratings = {}

	file = open(in_file_name, "r" )

	out_files = []
	for i in range(1, num_clusters+1):
		out_file = open(out_file_name + "_" + str(i), "w")
		out_files.append(out_file)

	for line in file:
		
		content = line.split()

		first = True
		for i in content:
			if first:
				for f in out_files:
					f.write(i)
				first = False
			else:
				entry = i.split(':')
				item_id = int(entry[0])
				cluster_id = item_cluster_map[item_id]
				f = out_files[cluster_id]
				f.write(" " + i)

		for f in out_files:
			f.write("\n")

	file.close()
	for f in out_files:
		f.close()


if __name__ == '__main__':
	in_file_name = None
	out_file_name = None
	item_cluster_file_name = 0
	
	try:
		opts, args = getopt.getopt(sys.argv[1:], "i:c:o:")
		for o,p in opts:
	  		if o in ['-i','--input']:
				in_file_name = p
	  		elif o in ['-o','--output']:
	  			out_file_name = p
  			elif o in ['-c','--clusters']:
	  			item_cluster_file_name = p
	except getopt.GetoptError:
		print "option not recognised"
		exit(2)

	item_cluster_map, num_clusters = read_item_cluster_map(item_cluster_file_name)
	
	get_ratings_by_item(in_file_name, out_file_name, item_cluster_map, num_clusters)