all: 
	rant all

rt: all

clean:
	rant clean

scenes/test_reflection.xml: all

renders/test_reflection.ppm: all scenes/test_reflection.xml
	./rt -s scenes/test_reflection.xml -o renders/test_reflection.ppm

.sample: renders/test_reflection.ppm
	touch .sample

show-sample: all .sample
	feh renders/test_reflection.ppm &


todo:
	grep -niI TODO ./*.cpp ./*.h plugins/*.cpp plugins/*.h
	grep -niI TODO ./*.cpp ./*.h plugins/*.cpp plugins/*.h | wc
