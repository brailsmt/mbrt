all: 
	rant all

rt: all

clean:
	rant clean

scenes/test_reflection.xml: all

renders/reflection_test.png: all scenes/test_reflection.xml
	./trace.sh scenes/test_reflection.xml


sample: renders/reflection_test.png


show-sample: sample 
	gtksee renders/reflection_test.png


