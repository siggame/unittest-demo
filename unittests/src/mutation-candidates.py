"""
Parse C source file and extract mutation points
"""
import sys
from pycparser import parse_file
import pycparser.c_ast as c_ast
import os
import collections
from typing import *


class StaticCallNode:

    def __init__(self, name):
        self.name = name
        self.calls: List[StaticCallNode] = []




class NodeVisitor(c_ast.NodeVisitor):
    
    def __init__(self, file):
        super().__init__()
        self.file = file
        self.context = ["<global>"]
        self.lines = collections.defaultdict(list)
        # Set of functions with static declaration. Since static functions
        # are not exported (and cannot be unit tested), we need to add static
        # functions to the call graph of a extern function
        self.static_functions = set()
        self.extern_functions = set()        
        self.function_call_graph: Dict[str, List[str]] = collections.defaultdict(list)
        self.function_to_lines: Dict[str, List[Tuple]] = collections.defaultdict(list)

    def visit_FuncDecl(self, node):
        if node.coord.file == "<stdin>":
            if node.decl.storage and node.decl.storage[0] == "static":
                self.static_functions.add(node.decl.name)

    def visit_FuncDef(self, node):
        if node.coord.file == "<stdin>":
            if node.decl.storage and node.decl.storage[0] == "static":
                self.static_functions.add(node.decl.name)
            else:
                self.extern_functions.add(node.decl.name)
            
            self.cur = node.decl.name
            self.context.append(node.decl.name)
            self.visit(node.body)
            self.context.pop()
    
    def visit_FuncCall(self, node):
        if node.coord.file == "<stdin>":
            if node.name.name in self.static_functions:
                self.function_call_graph[self.context[-1]].append(node.name.name)

    def visit_BinaryOp(self, node):
        # During preprocessing, the part of the file that included the headers
        # is tagged with <stdin>.
        if node.coord.file == "<stdin>":
            # Since we had to include _undef.h to remove gcc's compiler extensions,
            # remove a line
            line = node.coord.line - 1

            data = [line, node.coord.column, self.context[-1], "BinaryOp", node.op]
            self.lines[line].append(data)
            self.function_to_lines[self.context[-1]].append(data)
            self.visit(node.left)
            self.visit(node.right)
    
    def traverse_call_graph(self, function, output, visited):
        if function in visited:
            return
        output.append(function)
        for call in self.function_call_graph[function]:
            self.traverse_call_graph(call, output, visited)


def match_symbol(src_line, symbols_in_line):
    visited = set()
    for line, col, func, op, sym in symbols_in_line:
        j = col
        symlen = len(sym)
        while True:
            if (src_line[j:j+symlen]) == sym:
                coord = (line, j, func, op, sym)
                if coord not in visited:
                    visited.add(coord)
                    break
            j+=1
    return sorted(visited, key=lambda x: x[1])





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
            visitor.lines[line][:] = match_symbol(src_line, visitor.lines[line])
            
            for line in visitor.lines[line]:
                print(*line, file=fp)
        
        for extern in visitor.extern_functions:
            calls = []
            visited = set()
            visitor.traverse_call_graph(extern, calls, visited)
            
            for func in calls[1:]:
                print(0, 0, extern, "SYMLINK", func, file=fp)

            

        


if __name__ == "__main__":
    main()
