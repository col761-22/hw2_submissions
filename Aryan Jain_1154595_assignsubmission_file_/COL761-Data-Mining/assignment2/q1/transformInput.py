import sys

def transformFsg(origName,fsgName):
    infile = open(origName,"r")
    outfile = open(fsgName,"w")
    line = infile.readline()
    while line:
        if line[0] == '#':
            outfile.write("t " + line.strip() + "\n")
            numNodes = int(infile.readline().split()[0])
            for i in range(numNodes):
                outfile.write("v" + " " + str(i) + " "+  infile.readline().split()[0] + "\n")
            numEdges = int(infile.readline().split()[0])
            for i in range(numEdges):
                outfile.write("u" + " " + " ".join(infile.readline().split()) + "\n")
        line = infile.readline()

    infile.close()
    outfile.close()

def transformGspan(origName,gspanName):
    infile = open(origName,"r")
    outfile = open(gspanName,"w")
    labelInt = 1
    labelToInt = {}
    line = infile.readline()
    numGraphs = 0
    while line:
        if line[0] == '#':
            numGraphs+=1
            outfile.write("t # "+ line.strip() + "\n")
            numNodes = int(infile.readline().split()[0])
            for i in range(numNodes):
                label = infile.readline().split()[0]
                if label in labelToInt:
                    intLabel  = labelToInt[label]
                else:
                    labelToInt[label] = labelInt
                    intLabel = labelInt
                    labelInt += 1
                outfile.write("v" + " " + str(i) + " "+ str(intLabel)  + "\n")
            numEdges = int(infile.readline().split()[0])
            for i in range(numEdges):
                tokens = infile.readline().split()
                source = tokens[0]
                dest = tokens[1]
                if (int(source) > int(dest)):
                    temp = source
                    source = dest
                    dest = temp
                label = tokens[2]
                if label in labelToInt:
                    intLabel  = labelToInt[label]
                else:
                    labelToInt[label] = labelInt
                    intLabel = labelInt
                    labelInt += 1
                outfile.write("e" + " " + " ".join([source,dest,str(intLabel)]) + "\n")
        line = infile.readline()

    infile.close()
    outfile.close()
    numGraphFile = open("numGraphs.numGraphs", "w")
    numGraphFile.write(str(numGraphs))
    numGraphFile.close()

if __name__=="__main__":
    origFile = sys.argv[1]
    transformedFile = sys.argv[2]
    format = sys.argv[3]
    if format=="fsg":
        transformFsg(origFile,transformedFile)
    else:
        transformGspan(origFile,transformedFile)