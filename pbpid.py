import crypt
import commands

cryptedpasswd = "$1$awesome"

for i in range (1, 32768):
	arg = crypt.crypt(str(i), cryptedpasswd)
	result = commands.getstatusoutput('./ch21 ' + arg)
	if "phail" not in result[1]: 
		print result
		break
	
	