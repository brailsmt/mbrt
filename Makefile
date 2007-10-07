all: 
	rant all

rt: all

clean:
	rant clean

scenes/test_defect.xml: all

renders/test_defect.ppm: all scenes/test_defect.xml
	./rt -s scenes/test_defect.xml -o renders/test_defect.ppm


sample: renders/test_defect.ppm


show-sample: sample 
	feh renders/test_defect.ppm &


