import os

def test_sat():
	work_dir = "../QF_SLID_LC_SAT/" 
	# work_dir = "../QF_SLID_LC_ENTL" 
	beg = len("The result: ")
	count = 0
	with open(work_dir+"/run_result.txt", "w") as fw:
		for parent, dirnames, filenames in os.walk(work_dir):
			for filename in filenames:
				
				if filename.endswith(".smt"):
					print(filename)
					test_case = ("%s/%s" % (work_dir, filename))
					result = os.popen("ComSpen.exe " + test_case).readlines()
					rs = "\n"
					for line in result:
						if line.startswith("The result:"):
							rs = line[beg:]
					# print(result[7][beg:])
					content = ("%s: %s" % (filename, rs))
					fw.write(content)
					count += 1
					# if count > 10:
					#	exit(-1)
					
				


def main():
	test_sat()


if __name__ == '__main__':
	main()