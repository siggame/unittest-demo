"""
Parse C source file and extract mutation points
"""
import sys
from pycparser import parse_file
import pycparser.c_ast as c_ast
import os
import collections

class NodeVisitor(c_ast.NodeVisitor):
    
    def __init__(self, file):
        super().__init__()
        self.file = file
        self.context = ["<global>"]
        self.lines = collections.defaultdict(list)
  
    def visit_FuncDef(self, node):
        if node.coord.file == "<stdin>":
            self.cur = node.decl.name
            self.context.append(node.decl.name)
            self.visit(node.body)
            self.context.pop()


    def visit_BinaryOp(self, node):
        # During preprocessing, the part of the file that included the headers
        # is tagged with <stdin>.
        if node.coord.file == "<stdin>":
            # Since we had to include _undef.h to remove gcc's compiler extensions,
            # remove a line
            line = node.coord.line - 1
            self.lines[line].append([line, node.coord.column, self.context[-1], "BinaryOp", node.op])
            self.visit(node.left)
            self.visit(node.right)

def main():
    file = sys.argv[1]
    out, ext = os.path.splitext(file)
    out += ".mut"
  
    # Complicated shell command:
    #   sed '1...' inserts an include directive to the file stream so
    #   #include "unittests/_undefs.h" gets printed out before the rest of the file
    #   Then it gets redirected to the C preprocessor.
    #   The -x <language> manually stipulates we are using C
    #   The -E preserves line info
    #   --std=c99 : Use c99 standard
    #   -o : output file
    os.system(f"sed '1 i\\#include \"unittests/src/_undefs.h\"' {file} | cpp -x c -E --std=c99 -o /tmp/mutation-registry")


    # Generate abstract syntax tree so we can get the location of
    # arithmetic, relational, augmented assignments, and unary ops
    ast = parse_file("/tmp/mutation-registry")

    
    with open(file, "r") as fp:
        source = fp.read()
    
    src_lines = source.split("\n")

    # Record to .mut file the locations of mutation candidates
    
    with open(out, "w") as fp:
        print(file, file=fp)
        visitor = NodeVisitor(fp)
        visitor.visit(ast)

        # Annoyingly, the "node.coord" attribute only captures the root node
        # coordinate. So if you have more than one operation per line, you
        # end up with multiple entries in the mutation registry. So we need
        # to manually search each line for the specified symbol and update
        for line in visitor.lines:
            src_line = src_lines[line - 1]
            visited = set()
            for _, col, func, op, sym in visitor.lines[line]:
                j = col
                symlen = len(sym)
                while True:
                    if (src_line[j:j+symlen]) == sym:
                        coord = (line, j, func, op, sym)
                        if coord not in visited:
                            visited.add(coord)
                            break
                    j+=1
            for line in sorted(visited, key=lambda x: x[1]):
                print(*line, file=fp)



if __name__ == "__main__":
    main()
