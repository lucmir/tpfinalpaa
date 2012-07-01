#!/usr/bin/python

import sys
import getopt


def get_ratings_by_item(in_file_name, out_file_name, users):
	ratings = {}

	file = open(in_file_name, "r" )

	for line in file:
		content = line.split()

		user_id = int(content[0]) - 1
		item_id = int(content[1])
		rating = int(content[2])

		if item_id not in ratings:
			ratings[item_id] = []
			for i in range(0,users):
				ratings[item_id].append("?")

		ratings[item_id][user_id] = rating

	file.close()

	file = open(out_file_name, "w")

	header = "id"
	for i in range(0,users):
		header += "," + str(i+1)
	file.write(header + "\n")

	for item in ratings:
		line = str(item)
		for rating in ratings[item]:
			line += "," + str(rating)
		file.write(line + "\n")

	file.close()



if __name__ == '__main__':
	in_file_name = None
	out_file_name = None
	users = 0
	try:
		opts, args = getopt.getopt(sys.argv[1:], "i:o:u:")
		for o,p in opts:
	  		if o in ['-i','--input']:
				in_file_name = p
	  		elif o in ['-o','--output']:
	  			out_file_name = p
	  		elif o in ['-u','--users']:
	  			users = int(p)
	except getopt.GetoptError:
		print "option not recognised"
		exit(2)
	
	get_ratings_by_item(in_file_name, out_file_name, users)