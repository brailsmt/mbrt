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

renders/test_planes.ppm: all scenes/test_planes.xml
	./rt -s scenes/test_planes.xml -o renders/test_planes.ppm

.planes: renders/test_planes.ppm
	touch .sample

show-planes: all .planes
	feh renders/test_planes.ppm &

todo:
	grep -niI TODO ./*.cpp ./*.h plugins/*.cpp plugins/*.h
	grep -niI TODO ./*.cpp ./*.h plugins/*.cpp plugins/*.h | wc
