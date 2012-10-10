import searcher as ac
s = ac.Searcher(rulefile="../../banned.txt")
#s = ac.Searcher(imgfile="./compiled.gz")
filename = "D:/system_access.log"
fp = open(filename,"r")
content = fp.read()
def callback(s,start,end,level):
	#v = from_buffer(s)
	#print(s[start:end+1],level)
	pass
s.search(content,2)