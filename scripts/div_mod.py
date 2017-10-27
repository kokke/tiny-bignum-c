

#
#
# Code for testing division algorithm with remainder in Python
# 
# I want to verify the alghorithm before implementing in C :)
#
#



from random import Random

def pow_mod2(b, e, m):
  if m == 1:
    return 0
  c = 1
  ep = e
  while ep > 0:
    c = (c * b) % m
    ep -= 1
  return c



def pow_mod(b, e, m):
  if m == 1:
    return 0
  c = 1
  for _ in range(1, e+1):
    c = (c * b) % m
  return c


def divide(x, y):
  """ Idiomatic / Pythonic recursive division with remainder """
  if x == 0:
    return (0,0)
  
  q, r = divide(x / 2, y)
  q *= 2
  r *= 2
  
  if x & 1:
    r += 1
  
  if r >= y:
    r -= y
    q += 1
  return (q,r)


def divide2(x, y):
  if x == 0:
    return (0,0)
  elif x == y:
    return (1,0)
  elif x < y:
    return (0, x)
  else:
    q, r = divide2(x / 2, y)
    q *= 2
    r *= 2
    if x & 1:
      r += 1
    if r >= y:
      r -= y
      q += 1
    return (q,r)





if __name__ == "__main__":
  NTESTS = 100000
  r = Random()  

  print("\nRunning %d random tests...\n" % NTESTS)

  for i in xrange(NTESTS):
    x = r.randint(0, 0xffffffff)
    y = r.randint(1, 0xffffffff)
    e = x / y
    res = divide(x,y)[0]
    assert(res == e)
    assert(divide(x,y) == divide2(x,y))

  print("All tests succeeded.\n")
