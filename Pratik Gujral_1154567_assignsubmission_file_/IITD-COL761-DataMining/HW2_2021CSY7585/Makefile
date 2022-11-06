all: inpConv help

inpConv:
	g++ conv_input.cpp -o convertor
	./convertor sample_dataset.txt output.txt 90

help:
	g++ helper.cpp -o myhelper
	./myhelper sample_dataset.txt output.txt.fp queries.txt

clean:
	rm myhelper convertor output.txt output.txt.fp output_2021CSY7585.txt
