"""
Parse C source file and extract mutation points
"""
import sys
from pycparser import parse_file
import pycparser.c_ast as c_ast
import os


class NodeVisitor(c_ast.NodeVisitor):
    
    def __init__(self, file):
        super().__init__()
        self.file = file
        self.context = ["<global>"]
    
    def visit_FuncDef(self, node):
        if node.coord.file == "<stdin>":
            self.cur = node.decl.name
            self.context.append(node.decl.name)
            self.visit(node.body)
            self.context.pop()


    def visit_BinaryOp(self, node):
        if node.coord.file == "<stdin>":
            # Since we had to include _undef.h to remove gcc's compiler extensions,
            # remove a line
            line = node.coord.line - 1
            print(line, node.coord.column, self.context[-1], "BinaryOp", node.op, file=self.file)
            self.visit(node.left)
            self.visit(node.right)

def main():
    file = sys.argv[1]
    out, ext = os.path.splitext(file)
    out += ".mut"
    try:
        os.system("( rm /tmp/mutation-registry; )")
    except:
        pass

    # Complicated shell command:
    #   sed '1...' inserts an include directive to the file stream so
    #   #include "unittests/_undefs.h" gets printed out before the rest of the file
    #   Then it gets redirected to the C preprocessor.
    #   The -x <language> manually stipulates we are using C
    #   The -E preserves line info
    #   --std=c99 : Use c99 standard
    #   -o : output file
    os.system(f"sed '1 i\\#include \"unittests/_undefs.h\"' {file} | cpp -x c -E --std=c99 -o /tmp/mutation-registry")
    
    try:
        # Generate abstract syntax tree so we can get the location of
        # arithmetic, relational, augmented assignments, and unary ops
        ast = parse_file("/tmp/mutation-registry")
    except:
        print("err")
    else:
        print("ok")
        
        # Record to .mut file the locations of mutation candidates
        with open(out, "w") as fp:
            visitor = NodeVisitor(fp)
            visitor.visit(ast)

    finally:
        pass

        


if __name__ == "__main__":
    main()
