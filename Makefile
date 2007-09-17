all:
	rant all
rt:
	rant rt

renders/reflection_test.png: all
	./trace.sh scenes/test_reflection.xml

sample: renders/reflection_test.png

#TODO: xv didn't work with png files on Cygwin. Determine
# if this a Cygwin-specific issue or if a better viewer 
# is available
show-sample: sample 
	xv renders/reflection_test.ppm


