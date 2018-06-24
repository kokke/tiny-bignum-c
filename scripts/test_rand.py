
#
#
# Can take one command-line parameter: number of tests to run
#
# Runs NTESTS random tests of selecting an operand from + - *
# and applying it on two random operands and comparing the result
# to the one Python can calculate
#
# In effect, this verifies the C implementation against Python's
#
#


from random import Random, choice
import subprocess
import sys
import os
import math


TEST_BINARY = "./build/test_random"


def run_shell(runthis):
    """ Helper, runs a command string as if given to the shell """
    #print("trying to run '%s'" % runthis)
    osstdout = subprocess.Popen(runthis, shell=True, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, close_fds=True)
    theInfo = osstdout.communicate()[0].strip()
    #return (theInfo, osstdout.returncode)
    return osstdout.returncode


# Check for command-line arguments - default to 100 tests
if len(sys.argv) == 2:
  NTESTS = int(sys.argv[1])
else:
  NTESTS = 100

# Give the operands names
ADD = 0
SUB = 1
MUL = 2
DIV = 3
AND = 4
OR  = 5
XOR = 6
POW = 7
MOD = 8
RSHIFT = 9
LSHIFT = 10
ISQRT = 11
NUM_OPERATIONS = 12 # this variable should be 1 larger than the last supported operation ^^

# Instantiate object of Random-class for choosing an operand and two operators
rand = Random()

# List of command-strings leading to failures - expected to be empty if no bugs are triggered
failures = list()

print("\nRunning %d random tests (parsed using from_string):\n" % NTESTS)

# Iterate NTESTS times
i = 0
while i < NTESTS:
  i += 1
  # Choose random operand
  while 1:
    operation = choice(range(NUM_OPERATIONS))
    if operation != POW:
      break

  expected = 0      

  # Generate two large operators
  if operation in [LSHIFT, RSHIFT]:
    oper1 = rand.randint(0, 0xFF)
    oper2 = rand.randint(0, 32)
  else:
    oper1 = rand.randint(0, 0xFFFFFF)
    oper2 = rand.randint(0, 0xFFFFFF)

  # Calculate expected value
  if operation == ADD:
    expected = oper1 + oper2
  elif operation == SUB:
    # bignum only supports unsigned, so if B > A
    # we swap operands to avoid the underflow / wrap-around
    if oper2 > oper1:
      tmp = oper1
      oper1 = oper2
      oper2 = tmp
    expected = oper1 - oper2
  elif operation == MUL:
    expected = oper1 * oper2
  elif operation == DIV:
    if oper2 > oper1:
      tmp = oper1
      oper1 = oper2
      oper2 = tmp
    # avoid dividing by 0
    if oper2 == 0:
      oper2 += 1
    expected = oper1 / oper2
  elif operation == AND:
    expected = oper1 & oper2
  elif operation == OR:
    expected = oper1 | oper2
  elif operation == XOR:
    expected = oper1 ^ oper2
  elif operation == POW:
    expected = oper1 ** oper2
  elif operation == MOD:
    # avoid dividing by 0
    if oper2 == 0:
      oper2 += 1
    expected = oper1 % oper2
  elif operation == LSHIFT:
    expected = oper1 << oper2
  elif operation == RSHIFT:
    expected = oper1 >> oper2
  elif operation == ISQRT:
		expected = int(math.sqrt(oper1));


  # Convert to string to pass to C program
  # NOTE: bignum_from_string requires hex format and even no. of chars in str
  oper1 = "%.0x" % oper1
  oper2 = "%.0x" % oper2
  expected = "%.0x" % expected

  # bignum_from_string requires string-length to be an even number
  # so we zero-pad the number if the length is an odd number of bytes
  while (len(oper1) & 15) != 0:
    oper1 = "0" + oper1
  while (len(oper2) & 15) != 0:
    oper2 = "0" + oper2
  while (len(expected) & 15) != 0:
    expected = "0" + expected

  # Create the command-string to run in shell
  cmd_string = "%s %s %s %s %s" % (TEST_BINARY, operation, oper1, oper2, expected)
  if len([e for e in cmd_string.split(" ") if e]) < 5:
    # something has gone wrong with making the command-string - abort and retry
    i -= 1
    continue 
  

  # Expect return code to be ZERO for successful comparison
  success = (0 == run_shell(cmd_string))

  # Add command-string to list in case of failure
  # this allowis the user to re-create the failure
  if not success:
    failures.append(cmd_string)
    sys.stdout.write("x")
    try:
      f = open("error_log.txt", "a+")
      f.write(cmd_string + os.linesep)
      f.close()
    except:
      import traceback
      print("\n\nEXCEPTION:\n\n" + traceback.format_exc())
  else:
   sys.stdout.write(".")
   sys.stdout.flush()
    

sys.stdout.write("\n")

# After running the tests, give user feedback
print("")
print("%d/%d random tests passed." % (NTESTS - len(failures), NTESTS))
print("")


# Output the commands leading to failure, if any occurred:
if len(failures) > 0:
  print("")
  print("\n".join(failures))
  print("")




