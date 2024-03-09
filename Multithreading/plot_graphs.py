import matplotlib.pyplot as plt
import pandas as pd
import glob
import os


def read_and_aggregate_data(file_pattern):
    files = glob.glob(file_pattern)
    aggregated_data = pd.DataFrame()

    for file in files:
        # Read each file and remove 'us' suffix from 'Time' values and convert to int
        df = pd.read_csv(file, header=None, names=['Threads', 'Time'])
        df['Time'] = df['Time'].str.rstrip('us').astype(int)

        # Extract test identifier from file name
        test_id = os.path.splitext(os.path.basename(file))[0]

        # Add test_id as a column to df
        df['TestID'] = test_id

        # Append to the aggregated data
        aggregated_data = pd.concat([aggregated_data, df], ignore_index=True)

    return aggregated_data


def plot_all_tests(data, dest_folder):
    plt.figure(figsize=(10, 6))
    for test_id, group in data.groupby('TestID'):
        plt.plot(group['Threads'], group['Time'], marker='o', linestyle='-', label=test_id)

    plt.title('Merge Sort Performance Across All Tests')
    plt.xlabel('Number of Threads')
    plt.ylabel('Execution Time (Microseconds)')
    plt.legend()
    plt.grid(True)
    plt.xticks(data['Threads'].unique())
    plt.ylim(0, data['Time'].max() * 1.1)

    if not os.path.exists(dest_folder):
        os.makedirs(dest_folder)
    plt.savefig(os.path.join(dest_folder, 'all_tests_performance.png'))
    plt.close()


def plot_average_performance(data, dest_folder):
    avg_data = data.groupby('Threads')['Time'].mean().reset_index()
    plt.figure(figsize=(10, 6))
    plt.plot(avg_data['Threads'], avg_data['Time'], marker='o', linestyle='-', color='r', label='Average Performance')

    plt.title('Average Merge Sort Performance Across Tests')
    plt.xlabel('Number of Threads')
    plt.ylabel('Average Execution Time (Microseconds)')
    plt.legend()
    plt.grid(True)
    plt.xticks(avg_data['Threads'].unique())
    plt.ylim(0, data['Time'].max() * 1.1)

    plt.savefig(os.path.join(dest_folder, 'average_performance_across_tests.png'))
    plt.close()


def main():
    file_pattern = "TestResults/test_*.txt"
    dest_folder = "Plots"

    data = read_and_aggregate_data(file_pattern)
    plot_all_tests(data, dest_folder)
    plot_average_performance(data, dest_folder)


if __name__ == '__main__':
    main()
