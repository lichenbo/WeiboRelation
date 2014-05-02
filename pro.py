f = open('console_pro.txt','r')
result = {}
for i in range(0,13):
	result[i] = 0
for line in f:
	count = line.split()[2]
	result[int(count)] = result[int(count)]+1;
f.close()
print(result)
