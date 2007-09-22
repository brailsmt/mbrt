all:
	rant all


rt:
	rant rt


clean:
	rant clean


renders/reflection_test.png: all
	./trace.sh scenes/test_reflection.xml


sample: renders/reflection_test.png


# 
show-sample: sample 
	gtksee renders/reflection_test.png


