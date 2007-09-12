all:
	rant all
rt:
	rant rt

renders/reflection_test.png:
	./trace.sh scenes/test_reflection.xml

sample: all renders/reflection_test.png

#TODO- make this platform independant
#TODO- figure out how to do that
show-sample: sample 
	cygstart renders/reflection_test.png


