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


show-planes: all renders/test_planes.ppm
	feh renders/test_planes.ppm &

renders/test_planes2.ppm: all scenes/test_planes2.xml
	./rt -s scenes/test_planes2.xml -o renders/test_planes2.ppm


show-planes2: all renders/test_planes2.ppm
	feh renders/test_planes2.ppm &

todo:
	grep -niI TODO ./*.cpp ./*.h plugins/*.cpp plugins/*.h
	grep -niI TODO ./*.cpp ./*.h plugins/*.cpp plugins/*.h | wc
