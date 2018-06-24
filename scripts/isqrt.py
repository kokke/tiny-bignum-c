#isqrt.py

import math

def isqrt(n):
	if n == 0: return 0
	high = n
	low = 0
	calcMid = lambda: (high - low) / 2 + low + 1
	mid = calcMid()
	while high > low:
		sq = mid**2
		if sq > n:
			high = mid - 1
		else:
			low = mid
		mid = calcMid()
	return low

if __name__ == "__main__":
	for i in range(10000000):
		sq = isqrt(i)
		if sq != int(math.sqrt(i)):
			print "Failed on {}: {}".format(i, sq)
		elif i % 100000==0: print i 
