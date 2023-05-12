import glob

import modules.hash_functions
import modules.duplicates_finder

INPUT_DIR = 'input_data/'
HASH_FUNCTIONS = [modules.hash_functions.hash_buz, modules.hash_functions.hash_crc, modules.hash_functions.hash_pjw, modules.hash_functions.hash_default]


def process_duplicates(files_dir: str, extension='txt'):
    """Finds duplicates hash with all the functions for the input files and prints some data"""
    if not files_dir.endswith("/"):
        files_dir += '/'
    files_finding_mask = files_dir + '*.' + extension
    files = glob.glob(files_finding_mask)

    for i in HASH_FUNCTIONS:
        dup, time1 = modules.duplicates_finder.find_duplicates(files, i)
        print(f"Function Name: \t{i.__name__}")
        print(f"Duplicates: {len(dup)}")
        print(f"Execution time: {time1} sec \n\n")
        #print(f"List of duplicates: {dup} \n\n")


def main():
    process_duplicates(INPUT_DIR)


if __name__ == '__main__':
    main()
