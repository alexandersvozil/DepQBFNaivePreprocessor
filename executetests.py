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
		print "QUResolution failed after " + tol + "seconds."
	result3=subprocess.Popen("timeout " +tol+"s "+path+"./QBF_CPP "+
                filepath+ file +" -p ", shell = True,stdout=subprocess.PIPE).stdout.read()
	if result3:
		result3s = result3.split(" ")
		if result3s[0] == "RESULT:":
			print result3s
		else:
			print result3
	else:
		print "QUResolution + Subsumption without ss failed after " + tol  + "seconds." 
	result4=subprocess.Popen("timeout " +tol+"s "+path+"./QBF_CPP "+
                filepath+ file +" -l ", shell = True,stdout=subprocess.PIPE).stdout.read()
	if result4:
		result4s = result4.split(" ")
		if result4s[0] == "RESULT:":
			print result4s
		else:
			print result4
	else:
		print "LDR failed after " + tol  + "seconds." 
	result5=subprocess.Popen("timeout " +tol+"s "+path+"./QBF_CPP "+
                filepath+ file +" -l -s", shell = True,stdout=subprocess.PIPE).stdout.read()
	if result5:
		result5s = result5.split(" ")
		if result5s[0] == "RESULT:":
			print result5s
		else:
			print result5
	else:
		print "LDR with subsumption failed after " + tol  + "seconds." 

        min_list = list()
        correc_list = list()

        if result1:
            min_list.append(float(result1s[2]))
            correc_list.append(int(result1s[4]))

        if result2:
            min_list.append(float(result2s[2]))
            correc_list.append(int(result2s[4]))

        if result3:
            min_list.append(float(result3s[2]))
            correc_list.append(int(result3s[4]))

        if result4:
            min_list.append(float(result4s[2]))
            correc_list.append(int(result4s[4]))

        if result5:
            min_list.append(float(result5s[2]))
            correc_list.append(int(result5s[4]))

        methodstring = "n/a"
        minnumber = str(min(min_list))
        if minnumber == result1s[2]:
            methodstring = "No Preprocessor"

        if minnumber == result2s[2]:
            methodstring = "QUResolution"

        if minnumber == result3s[2]:
            methodstring = "QUResolution with Subsumption"

        if minnumber == result4s[2]:
            methodstring = "LDR"

        if minnumber == result5s[2]:
            methodstring = "LDR with subsumption"

        if methodstring == "n/a":
            print "we could not solve the instance"
        else:
            print "Best result: " + minnumber + " by using: " + methodstring + "\n"

        #check for correctness:
        if(min(correc_list) != max(correc_list)):
            print "UNDSOUND RESULT"
            break

#	if result2 and result1:
#		result1s = result1.split(" ")
#		result2s = result2.split(" ")
#		if(result1s[0] == "RESULT:" and result2s[0] == "RESULT:"):
#						if float(result2s[2]) >= float(result1s[2]):
#							print "Without preprocessing faster by: " + str(float(result2s[2])-float(result1s[2])) + "\n"
#						else:
#							print "Preprocessing + Solver faster by: " + str(float(result1s[2])-float(result2s[2])) + "\n"
#
#						if result2s[4] != result1s[4]:
#							print "FALSE RESULT"
#							break

		


