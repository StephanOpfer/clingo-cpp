#script(python)
from gringo import Fun
def main(prg):
    prg.ground([("base", [])])
    prg.assign_external(Fun("p", [3]), True)
    prg.solve()
    prg.assign_external(Fun("p", [3]), False)
    print prg.solve()
    prg.ground([("succ", [1]), ("succ", [2])])
    prg.assign_external(Fun("p", [4]), True)
    prg.solve()
    
    #prg.ground([("succ", [3])])
    #prg.solve()
#end.

