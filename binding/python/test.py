import searcher as ac
s = ac.Searcher(rulefile="../../test.txt")
#s = ac.Searcher(imgfile="./compiled.gz")
filename = "../../banned.txt"
fp = open(filename,"r")
content = fp.read()
def callback(s,start,end,level):
	#v = from_buffer(s)
	print(s[start:end+1],level)
	pass
s.search("abcdefgh",2,callback)