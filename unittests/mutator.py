import sys
import random
BINOP_MUTATION_TYPE = {
    "&&": "LCR",
    "||": "LCR",

    "==": "ROR",
    "!=": "ROR",
    "<": "ROR",
    "<=": "ROR",
    ">": "ROR",
    ">=": "ROR",
}

BINOP_MUTATION_TRANSITION = {
    "LCR": { "&&", "||" },
    "ROR": { "==", "!=", "<", "<=", ">", ">=" }
}

def main():
    src, mut = sys.argv[1], sys.argv[2]
    with open(mut, "r") as fp:
        db = [ line.strip().split() for line in fp ]
    

    
    while True:
        try:
            executed_line = input()
        except OSError:
            break
        else:
            match = [ line for line in db if line[0] == executed_line ]

            with open(src, "r") as fp:
                code = fp.read()
            
            for i, matched_operator in enumerate(match):
                lines = [ line for line in code.split("\n") ]
                _, col, _, op, sym = matched_operator
                row = int(executed_line)
                col = int(col)
                symboltype = BINOP_MUTATION_TYPE[sym]
                candidates = BINOP_MUTATION_TRANSITION[symboltype]
                repl = random.choice([ s for s in candidates if s != sym ])
                front = lines[row - 1][:col + 1]
                back = lines[row - 1][col + 1:].replace(sym, repl, 1)
                lines[row - 1] = front + back
                new_source = "\n".join(lines)

                with open(f".mut{i}-{row}.{col}-{src}", "w") as fp: 
                    print(new_source, file=fp)

                print(row, col, op, sym, repl)

if __name__ == "__main__":
    main()