import random

varNum = 26
mintermNum = 100
posProbability = 0.01


charArr = [chr(ord('a') + i) for i in range(varNum)]

mintermStr = ""

for i in range(mintermNum):
    num = random.randint(1,varNum)
    mintermItem = random.sample(charArr,num)
    for item in mintermItem:
        if(random.random() < posProbability):
            mintermStr += item.upper()
        else:
            mintermStr += item 
    mintermStr+="+"

mintermStr = mintermStr[:-1]
mintermStr += ".\n"

for char in charArr:
    mintermStr += f'{char} {random.random()}\n'



outfile = open("bigcase.txt","w")
outfile.write(mintermStr)
outfile.close()
