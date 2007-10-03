all: 
	rant all

rt: all

clean:
	rant clean

scenes/test_defect.xml: all

renders/test_defect.ppm: all scenes/test_defect.xml
	./rt -s scenes/test_defect.xml


sample: renders/test_defect.ppm


show-sample: sample 
	feh scene.ppm


