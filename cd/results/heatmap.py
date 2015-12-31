import math
import sys

#convert value (0-1) to five-color scale
def convertFractionToColor(value):
    #         blue    cyan    green   yellow  red
    colors = [(0,0,1),(0,1,1),(0,1,0),(1,1,0),(1,0,0)]

    if value <= 0.0:
        return (int(float(colors[0][0]) * 255.0),
                int(float(colors[0][1]) * 255.0),
                int(float(colors[0][2]) * 255.0))
    elif value >= 1:
        return (int(float(colors[-1][0]) * 255.0),
                int(float(colors[-1][1]) * 255.0),
                int(float(colors[-1][2]) * 255.0))

    lowIndex = int(math.floor(value * float(len(colors)-1)))
    highIndex = int(math.ceil(value * float(len(colors)-1)))

    lowValue = float(lowIndex) / float(len(colors)-1)
    highValue = float(highIndex) / float(len(colors)-1)

    lowColor = colors[lowIndex]
    highColor = colors[highIndex]
    colorVec = [highColor[0]-lowColor[0], highColor[1]-lowColor[1], highColor[2]-lowColor[2]]

    blendRatio = (value - lowValue) / (highValue - lowValue)

    return (int((lowColor[0] + blendRatio * colorVec[0]) * 255.0),
            int((lowColor[1] + blendRatio * colorVec[1]) * 255.0),
            int((lowColor[2] + blendRatio * colorVec[2]) * 255.0))



def average(times):
    listSum = 0.0
    for time in times:
        listSum += time
    return listSum / float(len(times))

for filename in sys.argv[1:]:
    filename = filename.split('.')[0]
    f = open(filename + ".csv", 'r')

    #save averages as dictionary of dictionaries
    results = {}

    #fill dictionaries with data (n, t, time)
    for line in f:
        sline = line.split(',');
        if int(sline[0]) not in results.keys():
            results[int(sline[0])] = {}
        if int(sline[1]) not in results[int(sline[0])].keys():
            results[int(sline[0])][int(sline[1])] = []
        results[int(sline[0])][int(sline[1])].append(float(sline[3]))

    #average results
    for n in results.keys():
        for t in results[n].keys():
            results[n][t] = average(results[n][t])

    #sort results
    import operator
    sortedResults = []
    for n in results.keys():
        sortedResults.append([n, sorted(results[n].items(), key=operator.itemgetter(0))])
    sortedResults.sort(key=lambda tup: tup[0])

    #write results to file
    file = open(filename + ".txt", "w")
    for n in sortedResults:
        file.write(" ")
        for t in n[1]:
            file.write(str(t[0]) + " ")
        file.write("\n")
        break
    for n in sortedResults:
        file.write(str(n[0]) + " ")
        for t in n[1]:
            file.write(str(t[1]) + " ")
        file.write("\n")
    file.close()

    #transform results to log
    for n in range(len(sortedResults)):
        for t in range(len(sortedResults[n][1])):
            sortedResults[n][1][t] = list(sortedResults[n][1][t])
            sortedResults[n][1][t][1] = math.log(sortedResults[n][1][t][1])

    #find lowest and highest values in each dimension
    minN = sys.maxint;
    maxN = -sys.maxint;
    minT = sys.maxint;
    maxT = -sys.maxint;
    minTime = sys.float_info.max
    maxTime = sys.float_info.min
    for n in sortedResults:
        if n[0] < minN:
            minN = n[0]
        elif n[0] > maxN:
            maxN = n[0]
        for t in n[1]:
            if t[0] < minT:
                minT = t[0]
            elif t[0] > maxT:
                maxT = t[0]
            if t[1] < minTime:
                minTime = t[1]
            elif t[1] > maxTime:
                maxTime = t[1]

    #change times to colors
    for n in range(len(sortedResults)):
        for t in range(len(sortedResults[n][1])):
            sortedResults[n][1][t][1] = convertFractionToColor((sortedResults[n][1][t][1] - minTime) / (maxTime - minTime))

    #write colors to bitmap
    import bmp
    targetWidth = 640
    targetHeight = 320
    widthScale = targetWidth/len(sortedResults[1][1])
    heightScale = targetHeight/len(sortedResults)
    realWidth = len(sortedResults[1][1])*widthScale
    realHeight = len(sortedResults)*heightScale
    img = bmp.BitMap(realWidth, realHeight, bmp.Color.BLACK)

    for n in range(len(sortedResults)):
        for t in range(len(sortedResults[n][1])):
            for y in range(heightScale):
                for x in range(widthScale):
                    color = sortedResults[n][1][t][1]
                    img.setPenColor(bmp.Color(color[0],color[1],color[2]))
                    img.plotPoint( t*widthScale+x, (len(sortedResults) - n)*heightScale-y-1)
    img.saveFile(filename + ".bmp")