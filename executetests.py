import os,subprocess
filepath = "benchmarks/"
path     = "Debug/"
tol 	 = "100"
for file in os.listdir(filepath):
 	print file
	result1 =subprocess.Popen("timeout " +tol+"s "+path+"./QBF_CPP "+filepath + file, shell=True, stdout=subprocess.PIPE).stdout.read()

	if result1:
		result1s = result1.split(" ")
		if result1s[0] == "RESULT:":
			print result1s
		else:
			print result1
	else:
		print "Solver failed after " + tol + "seconds."
	result2=subprocess.Popen("timeout " +tol+"s "+path+"./QBF_CPP "+
                filepath+ file +" -p -s", shell = True,stdout=subprocess.PIPE).stdout.read()
		
	if result2:
		result2s = result2.split(" ")
		if result2s[0] == "RESULT:":
			print result2s
		else:
			print result2
	else:
		print "PP + Solver failed after " + tol + "seconds."

	result3=subprocess.Popen("timeout " +tol+"s "+path+"./QBF_CPP "+
                filepath+ file +" -p ", shell = True,stdout=subprocess.PIPE).stdout.read()
		
	if result3:
		result3s = result3.split(" ")
		if result3s[0] == "RESULT:":
			print result3s
		else:
			print result3
	else:
		print "PP + Solver without ss failed after " + tol  + "seconds." 
			
        
	result4=subprocess.Popen("timeout " +tol+"s "+path+"./QBF_CPP "+
                filepath+ file +" -l ", shell = True,stdout=subprocess.PIPE).stdout.read()
	if result4:
		result4s = result4.split(" ")
		if result4s[0] == "RESULT:":
			print result4s
		else:
			print result4
	else:
		print "PP + Solver without ss failed after " + tol  + "seconds." 

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

		


