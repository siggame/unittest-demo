"""
USAGE: c-mutate <mut> <srcdir> [options]

Options
    --config=<config>
"""
import sys
import random
import docopt
import os

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

def candidate_mutations(sym):
    symboltype = BINOP_MUTATION_TYPE[sym]
    return BINOP_MUTATION_TRANSITION[symboltype]
    

MAX_PER_OPERATOR = 2
MAX_CHANGE_PER_MUTATION = 1

def mutate_line(line, col:int, sym:str, used:set):
    candidates = candidate_mutations(sym)
    repl = random.choice([ s for s in candidates if s not in used ])
    used.add(repl)
    front = line[:col]
    back = line[col:].replace(sym, repl, 1)
    return front + back, repl

def main():
    args = docopt.docopt(__doc__)
    srcdir = args["<srcdir>"]
    mut = args["<mut>"]    
    with open(mut, "r") as fp:
        filename = fp.readline().strip()
        db = [ line.strip().split() for line in fp ]

    with open(filename, "r") as fp:
        code = fp.read()
    
    for i, (line, col, func, op, sym) in enumerate(db):
        line = int(line) - 1 # line numbers start at 1
        col = int(col)
    
        source = code.split("\n")
        used = {sym}

        # Generate a mutation for each operator, up to MAX_PER_OPERATOR or until
        # we've exhausted the set of candidates
        for j in range(MAX_PER_OPERATOR):
            if len(used) == len(candidate_mutations(sym)):
                break
            source[line], alt = mutate_line(source[line][:], col, sym, used)
            mutfile = os.path.join(srcdir, f"{i}-{j}-{filename}")
            with open(os.path.join(srcdir, f"{i}-{j}-{filename}"), "w") as fp:
                print("\n".join(source), file=fp)

            print(line+1, col, func, op, sym, alt, mutfile)

             
if __name__ == "__main__":
    main()