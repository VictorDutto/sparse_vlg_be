import subprocess as sp
import time
import os

# Strategies

# Add or remove strategies to try here
strategies = [
    "", #nothing -> trigger multi approche
    "--basic",
    "--density",
    "--degreemin",
    "--degreemax"
]

# Add graph files to test here
files = ["p2p-Gnutella04.txt"]

# Function
def csv_bench(times):
    '''Create csv from times execution list'''
    csv = "files;"
    for strat in strategies:
        if len(times) != len(strategies):
            times.append(0)
        csv += strat + ";"
    csv += "\n"

    for ii, f in enumerate(files):
        t = times[ii]
        csv += f + ";"
        for t_ in t:
            csv += str(round(t_,2)) + ";"
        csv += "\n"

    return csv

# Main
if __name__ == "__main__":

    print("VLG Testsuite")

    outputs = []
    times = []

    for f in files:
        file_times = []
        for strat in strategies:
            # Calculate time to compute using specific strategy
            t0 = time.time()
            outp = os.popen("./sparse_vlg " + strat + " --file " + f).read()
            t1 = time.time()

            # Pretty prind and saving output
            outputs.append(strat+ ": " + outp)
            print("Execution of " + strat + " with file " + f + " done")
            file_times.append(t1-t0)
            print("Calculated exec time " + str(t1-t0) + "\n")
        times.append(file_times)

    for o in outputs:
        print(o, end="")

    print()
    print(csv_bench(times))
    
    # Write output as csv
    f = open("test.csv", "a")
    f.write(csv_bench(times))
    f.close()