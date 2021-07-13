import subprocess as sp
import os

# Strategies

strategies = [
    "basic",
    "density",
    "degree"
]

# Functions
def run_shell_file(arg, t):
    '''Run file as bash posix'''
    args = ["build/sparse_vlg", "--" + arg]

    print(args)
    if t != 0:
        try:
            ret = sp.run(args, capture_output=True, text=True, timeout = t)
        except sp.TimeoutExpired:
            raise RuntimeError
    else:
        ret = sp.run(args, capture_output=True, text=True)

    return ret

# Main
if __name__ == "__main__":

    print("VLG Testsuite")

    outputs = []

    for strat in strategies:
        #print(run_shell_file(strat, 0))
        output.append(os.popen("./sparse_vlg --" + strat).read())
        print("Execution of " + strat + " done\n")
        print()

    print("That's all, folks !")


