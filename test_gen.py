import numpy as np

import random

OUT_FILE_PREFIX = 'TEST_7IN_CLASS_A'

COUNT_GEN = 5

INPUT_NAMES = [
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
    'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p'
]

OUTPUT_NAME = 'Y'

INPUT_NUM = 7

def save_to_file(index, truth_table, permutation, negation, output_negation):
    lines = [
        '# Auto generated BLIF FILE\n',
    ]

    string = '#'
    for n in negation:
        if n:
            string += ' 1'
        else:
            string += ' 0'

    if output_negation:
        string += ' 1'
    else:
        string += ' 0'
    lines.append(string + '\n')
    
    lines.append(f'.model {OUT_FILE_PREFIX}_{index}\n')
    string = '.inputs'
    for i in range(INPUT_NUM):
        string += f' {INPUT_NAMES[i]}'
    lines.append(string + '\n')

    lines.append(f'.outputs {OUTPUT_NAME}\n')

    string = '.names'
    for i in range(INPUT_NUM):
        string += f' {INPUT_NAMES[permutation[i]]}'
    string += f' {OUTPUT_NAME}'
    lines.append(string + '\n')

    for minterm in truth_table:
        string = ''
        for i in range(INPUT_NUM):
            if minterm[i]:
                string += '1'
            else:
                string += '0'
        string += ' 1'
        lines.append(string + '\n')

    lines.append('.end\n')

    with open(f'{OUT_FILE_PREFIX}_{index}.blif', 'w+') as f:
        f.writelines(lines)


num_minterms = random.randint(1, ((1 << INPUT_NUM) - 1) // 3)
all_minterms = list(range(1 << INPUT_NUM))
random_minterms = random.sample(all_minterms, num_minterms)
complement = [minterm for minterm in all_minterms if minterm not in random_minterms]

on_set = np.zeros((num_minterms, INPUT_NUM), dtype=bool)
off_set = np.zeros((len(complement), INPUT_NUM), dtype=bool)

for minterm in range(len(random_minterms)):
    for bit in range(INPUT_NUM):
        on_set[minterm, bit] = random_minterms[minterm] & (1 << bit)

for minterm in range(len(complement)):
    for bit in range(INPUT_NUM):
        off_set[minterm, bit] = complement[minterm] & (1 << bit)

save_to_file(0, on_set, list(range(INPUT_NUM)), [0] * INPUT_NUM, False)

for i in range(1, COUNT_GEN):
    if random.randint(0, 100) > 85:
        truth_table_copy = np.copy(off_set)
        output_negation = True
    else:
        truth_table_copy = np.copy(on_set)
        output_negation = False

    negation = []
    for j in range(INPUT_NUM):
        if random.randint(0, 100) > 85:
            truth_table_copy[:, j] = ~truth_table_copy[:, j]
            negation.append(True)
        else:
            negation.append(False)
    
    permutation = list(range(INPUT_NUM))
    random.shuffle(permutation)

    truth_table_copy = truth_table_copy[:, permutation]

    save_to_file(i, truth_table_copy, permutation, negation, output_negation)


    
