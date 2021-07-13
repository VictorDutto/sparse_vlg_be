import subprocess as sp
import time
import os

# Strategies

strategies = [
    "basic",
    "density",
    "degree"
]

# Function
def csv_bench(times):
    csv = ""
    for strat in strategies:
        if len(times) != len(strategies):
            times.append(0)
        csv += strat + ";"
    csv += "\n"

    for t in times:
        csv += str(round(t,2)) + ";"
    csv += "\n"

    return csv

# Main
if __name__ == "__main__":

    print("VLG Testsuite")

    outputs = []
    times = []

    for strat in strategies:
        t0 = time.time()
        outputs.append(os.popen("./sparse_vlg --" + strat).read())
        t1 = time.time()

        print("Execution of " + strat + " done\n")
        times.append(t1-t0)
        print("Calculated exec time " + str(t1-t0))

    print("That's all, folks !")
    print()
    print(csv_bench(times))


