import matplotlib.pyplot as plt

#-----------------------------------------------------------------------
# One instruction
#----------------------------------------------------------------------- 
def analyze_1inst():
    text = open("ir.log", "r")
    inst_d  = dict()

    inst_name = ""

    for line in text: 
        line = line.strip()  
        line = line.lower() 
        words = line.split(" ")           
    
        inst_name = words[-1] 

        # Skip start of func and end of func logs
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

    plt.rcParams["figure.figsize"] = (15, 17)
    plt.xlabel('Number of occurrences')
    plt.barh(inst_names, occur_nums, color='forestgreen')
    plt.subplots_adjust(left=0.25)
    plt.savefig('./statistics/1_inst_statistics_bar.png')
    plt.clf()

#-----------------------------------------------------------------------
# Two instructions
#-----------------------------------------------------------------------
def analyze_2inst():
    text = open("ir.log", "r")
    two_inst_d = dict()

    first_inst_name = ""
    second_inst_name = ""

    for line in text:
        line = line.strip()  
        line = line.lower() 
        words = line.split(" ")           
        
        first_inst_name = second_inst_name
        second_inst_name = words[-1]

        # Skip start of func and end of func logs
        if second_inst_name[0] != '[' or second_inst_name[-1] != ']':
            continue;
        if first_inst_name[0] != '[' or first_inst_name[-1] != ']':
            continue;         

        two_inst_name = first_inst_name+' + '+second_inst_name

        if two_inst_name in two_inst_d: 
            two_inst_d[two_inst_name] += 1
        else:
            two_inst_d[two_inst_name] = 1

    two_inst_names = []
    occur_nums = []

    for two_inst in list(two_inst_d.keys()):
        two_inst_names.append(two_inst)
        occur_nums.append(two_inst_d[two_inst])

    plt.rcParams["figure.figsize"] = (15, 17)
    plt.xlabel('Number of occurrences')
    plt.barh(two_inst_names, occur_nums, color='lightcoral')
    plt.subplots_adjust(left=0.25)
    plt.savefig('./statistics/2_inst_statistics_bar.png')
    plt.clf()

#-----------------------------------------------------------------------
# Three instructions
#-----------------------------------------------------------------------
def analyze_3inst():
    text = open("ir.log", "r")
    three_inst_d = dict()

    first_inst_name = ""
    second_inst_name = ""
    third_inst_name = ""

    for line in text:
        line = line.strip()  
        line = line.lower() 
        words = line.split(" ")           
        
        first_inst_name = second_inst_name
        second_inst_name = third_inst_name
        third_inst_name = words[-1]

        # Skip start of func and end of func logs
        if third_inst_name[0] != '[' or third_inst_name[-1] != ']':
            continue;
        if second_inst_name[0] != '[' or second_inst_name[-1] != ']':
            continue;
        if first_inst_name[0] != '[' or first_inst_name[-1] != ']':
            continue;         

        three_inst_name = first_inst_name+' + '+second_inst_name+' + '+third_inst_name

        if three_inst_name in three_inst_d: 
            three_inst_d[three_inst_name] += 1
        else:
            three_inst_d[three_inst_name] = 1

    three_inst_names = []
    occur_nums = []

    for three_inst in list(three_inst_d.keys()):
        three_inst_names.append(three_inst)
        occur_nums.append(three_inst_d[three_inst])

    plt.rcParams["figure.figsize"] = (15, 17)
    plt.xlabel('Number of occurrences')
    plt.barh(three_inst_names, occur_nums, color='darkcyan')
    plt.subplots_adjust(left=0.25)
    plt.savefig('./statistics/3_inst_statistics_bar.png')
    plt.clf()

#-----------------------------------------------------------------------
# Four instructions
#-----------------------------------------------------------------------
def analyze_4inst():
    text = open("ir.log", "r")
    four_inst_d = dict()

    first_inst_name = ""
    second_inst_name = ""
    third_inst_name = ""
    forth_inst_name = ""

    for line in text:
        line = line.strip()  
        line = line.lower() 
        words = line.split(" ")           
        
        first_inst_name = second_inst_name
        second_inst_name = third_inst_name
        third_inst_name = forth_inst_name
        forth_inst_name = words[-1]

        # Skip start of func and end of func logs
        if forth_inst_name[0] != '[' or forth_inst_name[-1] != ']':
            continue;
        if third_inst_name[0] != '[' or third_inst_name[-1] != ']':
            continue;
        if second_inst_name[0] != '[' or second_inst_name[-1] != ']':
            continue;
        if first_inst_name[0] != '[' or first_inst_name[-1] != ']':
            continue;         

        four_inst_name = first_inst_name+' + '+second_inst_name+' + '+third_inst_name+' + '+forth_inst_name

        if four_inst_name in four_inst_d: 
            four_inst_d[four_inst_name] += 1
        else:
            four_inst_d[four_inst_name] = 1

    four_inst_names = []
    occur_nums = []

    for four_inst in list(four_inst_d.keys()):
        four_inst_names.append(four_inst)
        occur_nums.append(four_inst_d[four_inst])

    plt.rcParams["figure.figsize"] = (15, 17)
    plt.xlabel('Number of occurrences')
    plt.barh(four_inst_names, occur_nums, color='peru')
    plt.subplots_adjust(left=0.25)
    plt.savefig('./statistics/4_inst_statistics_bar.png')
    plt.clf()

#-----------------------------------------------------------------------
# Five instructions
#-----------------------------------------------------------------------
def analyze_5inst():
    text = open("ir.log", "r")
    five_inst_d = dict()

    first_inst_name = ""
    second_inst_name = ""
    third_inst_name = ""
    forth_inst_name = ""
    fifth_inst_name = ""

    for line in text:
        line = line.strip()  
        line = line.lower() 
        words = line.split(" ")           
        
        first_inst_name = second_inst_name
        second_inst_name = third_inst_name
        third_inst_name = forth_inst_name
        forth_inst_name = fifth_inst_name
        fifth_inst_name = words[-1]

        # Skip start of func and end of func logs
        if fifth_inst_name[0] != '[' or fifth_inst_name[-1] != ']':
            continue;
        if forth_inst_name[0] != '[' or forth_inst_name[-1] != ']':
            continue;
        if third_inst_name[0] != '[' or third_inst_name[-1] != ']':
            continue;
        if second_inst_name[0] != '[' or second_inst_name[-1] != ']':
            continue;
        if first_inst_name[0] != '[' or first_inst_name[-1] != ']':
            continue;         

        five_inst_name = first_inst_name+' + '+second_inst_name+' + '+third_inst_name+' + '+forth_inst_name+' + '+fifth_inst_name

        if five_inst_name in five_inst_d: 
            five_inst_d[five_inst_name] += 1
        else:
            five_inst_d[five_inst_name] = 1

    five_inst_names = []
    occur_nums = []

    for five_inst in list(five_inst_d.keys()):
        five_inst_names.append(five_inst)
        occur_nums.append(five_inst_d[five_inst])

    plt.rcParams["figure.figsize"] = (15, 17)
    plt.xlabel('Number of occurrences')
    plt.barh(five_inst_names, occur_nums, color='darkgray')
    plt.subplots_adjust(left=0.25)
    plt.savefig('./statistics/5_inst_statistics_bar.png')
    plt.clf()


#-----------------------------------------------------------------------
# Analyze ir.log
#-----------------------------------------------------------------------
analyze_1inst()
analyze_2inst()
analyze_3inst()
analyze_4inst()
analyze_5inst()