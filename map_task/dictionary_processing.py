import time
import matplotlib.pyplot as plt

def process_cpp_data():
    """Reads data from iostream, builds figure with 2 graphs:
        1) Memory usage of C++ Map
        2) Elements adding Time Usage
    """
    cppData = [float(value) for value in input().split()]
    map_memory_usage =  [float(cppData[i]) for i in range(len(cppData)) if i%2]
    map_adding_time =  [float(cppData[i]) for i in range(len(cppData)) if not i%2]
    elements_count = [pow(10, i) for  i in range(0, 8)]
    figure, (tm, mem) = plt.subplots(1, 2)
    
    figure.suptitle('C++ Map Graphs', fontsize=16)
    
    tm.plot(elements_count, map_adding_time)
    tm.set_title("C++ Map Elements Adding Graph")
    tm.set_xlabel("Amount of Elements")
    tm.set_ylabel("Time, Milliseconds")
    
    mem.plot(elements_count, map_memory_usage)
    mem.set_title("C++ Map's Memory Usage")
    mem.set_xlabel("Amount of Elements")
    mem.set_ylabel("Memory, Kb")
    plt.show()
    

def py_memory_allocation(amount_of_elements: int) -> float:
    """
    Creates dictionary with 10^n elements and records how much time does it need
    """
    t = time.perf_counter()
    dict = {i: i for i in range(amount_of_elements)}
    elapsed = (time.perf_counter()-t)
    dict.clear()
    return elapsed

    
def process_python_data():
    """Does the same things as process_cpp_data but for python Dictionary """
    elements_count = [pow(10, i) for  i in range(0, 6)]
    memloc_times = []
    memory_usage = []
    for i in elements_count:
        memloc_times.append(py_memory_allocation(i) * 1000) #Time usage in milliseconds
        memory_usage.append(8 * i / 1024.0) #memory usage in Kbs
    figure, (tm, mem) = plt.subplots(1, 2)
    
    figure.suptitle('Python Dictionary Graphs', fontsize=16)
    
    tm.plot(elements_count, memloc_times)
    tm.set_title("Python Dict. Elements Adding Graph")
    tm.set_xlabel("Amount of Elements")
    tm.set_ylabel("Time, Milliseconds")
    
    mem.plot(elements_count, memory_usage)
    mem.set_title("Python Dict's Memory Usage")
    mem.set_xlabel("Amount of Elements")
    mem.set_ylabel("Memory, Kb")
    plt.show()
    

def main():
     """The Main Function"""
     process_cpp_data()
     process_python_data()
    

if __name__=='__main__':
    main();