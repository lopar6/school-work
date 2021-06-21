import re
import string
import os

# using a dictionay to future proof incase there is a need to run longer algorithms 
produceDict = {}


def makeList():
    ''' creates a dictionary containing "item: quanitiy"
    then creates a file "frequency.dat" containing the values''' 
    frequencyFile = open("frequency.dat", "w")
    if not os.stat("frequency.dat").st_size:
        inputFile = open("input.txt", "r");
        for line in inputFile:
            line = line.lower().strip("\n")
            if line in produceDict:
                produceDict[line] += 1
            else:
                produceDict[line] = 1
        inputFile.close()
        for key in produceDict:
            frequencyFile.write(f"{key} {produceDict[key]}\n")
    else:
        for line in frequencyFile:
            key, value = line.split(" ")
            produceDict[key] = value


def getNumTimesPurchased(item : string) -> int:
    ''' returns number of times a particular item was purchased'''
    file = open("input.txt", "r")
    total = 0
    print(item.lower())
    for line in file:
        if item.lower() == line.lower() or item.lower() == line.rstrip("s"):
            total += 1
    file.close()
    return total
    

def printProduceList():
    '''prints a list of items and the quantity sold'''
    makeList()
    for key in produceDict:
        print(f"{key:>14}: {produceDict[key]}")
    print("")
    

def printHistogram():
    makeList()
    bar = ""
    for item in produceDict:
        for i in range(produceDict[item]): 
            bar += '*'
        print(f"{item:>14}: {bar}")
        bar = ''


    
