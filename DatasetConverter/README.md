# How to use 

## 1 Open a terminal in the directory of source files:

	mkdir build
	cd build
	cmake ..

## 2 If cmake can generate files and result is successfull then compile:

	make
	./ConvertDarknetDataset [inputFilePath] [outputFile]
	
## what we Convert format?
If you have trained your network using darknet repository then your [inputFilePath] is like the below:

	data/obj/img1.jpg
	data/obj/img2.jpg
	data/obj/img3.jpg
	
In this format each image is labeled like the below:

	<object-class> <x_center> <y_center> <width> <height>

For example `data/obj/img1.txt`:

	1 0.716797 0.395833 0.216406 0.147222
	0 0.687109 0.379167 0.255469 0.158333
	1 0.420312 0.395833 0.140625 0.166667

But in this repository your format is not same.

