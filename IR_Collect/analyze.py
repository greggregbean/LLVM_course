import matplotlib.pyplot as plt

text = open("ir.log", "r")  
inst_d = dict() 

for line in text: 
    line = line.strip()  
    line = line.lower() 
    words = line.split(" ")           
  
    inst_name = words[-1] 

    if inst_name[0] != '[' or inst_name[-1] != ']':
        continue;

    if inst_name in inst_d: 
        inst_d[inst_name] += 1
    else:
        inst_d[inst_name] = 1

inst_names = []
occur_nums = []

for inst in list(inst_d.keys()):
    inst_names.append(inst)
    occur_nums.append(inst_d[inst])

plt.rcParams["figure.figsize"] = (20, 5)
plt.xlabel('Instruction')
plt.ylabel('Number of occurrences')
plt.bar(inst_names, occur_nums)
plt.savefig('statistics_bar.png')
  
 