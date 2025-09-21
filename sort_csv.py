import csv

def sort_csv(filename, sort_column):
    with open(filename, 'r') as csvfile:
        reader = csv.reader(csvfile)
        data = list(reader)

    # Sort the data based on the specified column
    data = sorted(data, key=lambda row: row[sort_column])

    return data

def write_sorted_csv(filename, sorted_data):
    with open(filename, 'w', newline='') as csvfile:
        writer = csv.writer(csvfile)
        writer.writerows(sorted_data)

filename = "tests/dataset_100.csv"
sort_column = 2  # Index of "Official Name Suburb"

sorted_data = sort_csv(filename, sort_column)
write_sorted_csv("tests/dataset_100sort.csv", sorted_data)
