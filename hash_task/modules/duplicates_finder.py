import os
import time


def find_duplicates(files: list[str], hash_function: callable) -> tuple[list[str], float]:
    """Finds duplicates among the files list with the hash function preset by user

    :param files: list of text files to process
    :param hash_function: callable, hash function we'll use

    :returns: tuple of duplicated files and the time the utility spent processing the files
    """
    hash_to_file = {}
    duplicates = []
    total_time = 0

    for file_path in files:
        with open(file_path, 'rb') as file:
            file_data = file.read()
            start_time = time.perf_counter()
            file_hash = hash_function(file_data)
            end_time = time.perf_counter()
            total_time += (end_time - start_time)
            if file_hash in hash_to_file:
                duplicates.append(file_path)
            else:
                hash_to_file[file_hash] = file_path

    return duplicates, total_time
