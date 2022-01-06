def q0(symbol):
    if symbol in ['1', '0', '2']:
        return symbol, 1, 'q0'
    if symbol == '+':
        return '+', 1, 'q1'
    if symbol == '-':
        return '-', 1, 'q2'
    if symbol == ' ':
        return ' ', 1, 'q0'


def q1(symbol):
    if symbol == '0':
        return '0', 0, 'qn'
    if symbol == '1':
        return '1', -1, 'q3'
    if symbol == '2':
        return '2', -1, 'q4'


def q2(symbol):
    if symbol == '0':
        return '0', 0, 'qn'
    if symbol == '1':
        return '1', -1, 'q5'
    if symbol == '2':
        return '2', -1, 'q6'


def q3(symbol):
    if symbol == ' ':
        return '1', 0, 'qn'
    if symbol in ['1', '0']:
        return str(int(symbol) + 1), 0, 'qn'
    if symbol == '2':
        return '0', -1, 'q3'
    if symbol == '+':
        return '+', -1, 'q3'


def q4(symbol):
    if symbol == '0':
        return '2', 0, 'qn'
    if symbol in ['1', '2']:
        return str(int(symbol) - 1), -1, 'q3'
    if symbol == '+':
        return '+', -1, 'q4'


def q5(symbol):
    if symbol == '0':
        return '2', -1, 'q5'
    if symbol == '1':
        return '0', 0, 'q7'
    if symbol == '2':
        return '1', 0, 'qn'
    if symbol == '-':
        return '-', -1, 'q5'


def q6(symbol):
    if symbol in ['1', '0']:
        return str(int(symbol) + 1), -1, 'q5'
    if symbol == '2':
        return '0', 0, 'qn'
    if symbol == '-':
        return '-', -1, 'q6'


def q7(symbol):
    if symbol in ['1', '2', '0']:
        return symbol, -1, 'q7'
    if symbol == ' ':
        return ' ', 1, 'q8'


def q8(symbol):
    if symbol == '0':
        return ' ', 1, 'q8'
    if symbol in ['1', '2']:
        return symbol, 0, 'qn'
    if symbol == '+':
        return '+', -1, 'q8'
    if symbol == '-':
        return '-', -1, 'q8'
    if symbol == ' ':
        return '0', 0, 'qn'


tape = list(input())
dictionary = {'q0': q0, 'q1': q1, 'q2': q2, 'q3': q3, 'q4': q4, 'q5': q5, 'q6': q6, 'q7': q7, 'q8': q8}
state = 'q0'
i = 0
while state != 'qn':
    new_symbol, direction, state = dictionary[state](tape[i])
    tape[i] = new_symbol
    i += direction
print(''.join(tape))


