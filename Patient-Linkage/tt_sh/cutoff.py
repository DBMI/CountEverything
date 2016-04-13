#!/usr/bin/env python

# count the number of common SNPs in two files

import sys
import operator
import argparse   


def calcHamm(args):
	limit= args.input3
	mHamm = {}
	count=0
      	int1= open("intermediate1.txt",'w')
	with open(args.input1, 'r') as f:
		for l in f:
			l = l.strip()
			if l.startswith('#'):
				continue
			r = l.split()
			if ('INS' in r[-1]) | ('DEL' in r[-1]):
				continue
			if len(r[3])>1:
				continue
			if count>=limit:
				break
			s=""
			for i in range(0,len(r)):
				s += "{0}\t".format(r[i])
			s += "\n"
			count+=1
			int1.write(s)
		int1.close()	
	count=0	
        int2= open("intermediate2.txt",'w')
	with open(args.input2, 'r') as f:
		for l in f:
			l = l.strip()
			if l.startswith('#'):
				continue
			r = l.split()
			if ('INS' in r[-1]) | ('DEL' in r[-1]):
                                continue
			if len(r[3])>1:
				continue
			if count>=limit:
				break
			s=""
                        for i in range(0,len(r)):
                                s += "{0}\t".format(r[i])
                        s += "\n"    
			count+=1           
                        int2.write(s)
                int2.close()



if __name__ == '__main__':
	parser = argparse.ArgumentParser(description='count # of common SNPs')

	parser.add_argument('-i1', '--input1', help='data 1')
	parser.add_argument('-i2', '--input2', help='data 2')
	parser.add_argument('-i3', '--input3', type=int, help='data 3')
	parser.add_argument('--v', help='print records', action='store_true')
	parser.set_defaults(func=calcHamm)

	args = parser.parse_args()
	args.func(args)


