import sys
f = open(sys.argv[1],'r')
fromList = []
for line in f:
	elem = line.split()[0]
	if elem not in fromList:
		fromList.append(elem)
f.close()

f = open(sys.argv[1],'r')
g = open(sys.argv[2],'r+')
for line in f:
	to = line.split()[1]
	if to in fromList:
		g.write(line)
f.close()
g.close()
