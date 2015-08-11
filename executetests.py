import os,subprocess
filepath = "benchmarks/"
path     = "Debug/"
tol 	 = "200"
for file in os.listdir(filepath):
 	print file
	result1 =subprocess.Popen("timeout " +tol+"s "+path+"./QBF_CPP "+filepath + file, shell=True, stdout=subprocess.PIPE).stdout.read()

	if result1:
		result1s = result1.split(" ")
		if result1s[0] == "RESULT:":
			print result1s
		else:
			print result1
	result2=subprocess.Popen("timeout " +tol+"s "+path+"./QBF_CPP "+ filepath+ file +" -pp", shell = True,stdout=subprocess.PIPE).stdout.read()
		
	if result2:
		result2s = result2.split(" ")
		if result2s[0] == "RESULT:":
			print result2s
		else:
			print result2
			
	if result2 and result1:
		result1s = result1.split(" ")
		result2s = result2.split(" ")
		if(result1s[0] == "RESULT:" and result2s[0] == "RESULT:"):
						if float(result2s[2]) >= float(result1s[2]):
							print "Without preprocessing faster by: " + str(float(result2s[2])-float(result1s[2])) + "\n"
						else:
							#print "Bla"
							print "Preprocessing + Solver faster by: " + str(float(result1s[2])-float(result2s[2])) + "\n"

						if result2s[4] != result1s[4]:
							print "FALSE RESULT"
							break

		


