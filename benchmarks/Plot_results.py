import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt  # Для дополнительных настроек, если потребуется

def plot_benchmark_results(filename, benchmark_types):
    try:
        df = pd.read_csv(filename)

        for benchmark_type in benchmark_types:
            plt.figure(figsize=(14, 8))
            shared_df = df[df['Benchmark'].str.contains(benchmark_type) & (
                    df['Benchmark'].str.startswith("RawPtr") |
                    df['Benchmark'].str.startswith("SharedPtr") |
                    df['Benchmark'].str.startswith("Std_SharedPtr")
            )]

            sns.lineplot(x='Iterations', y='Time(ns)', hue='Benchmark', data=shared_df, palette="pastel", marker='o')
            plt.title(f"Benchmark Results (SharedPtr - {benchmark_type})", fontsize=14)
            plt.grid(axis='y', linestyle='--', alpha=0.7)
            plt.xlabel("Iterations", fontsize=12)
            plt.ylabel("Time (ns)", fontsize=12)
            plt.xticks(rotation=45, ha='right', fontsize=10)
            plt.tight_layout()
            plt.savefig(f"benchmark_results_{benchmark_type}_shared.png")
            plt.show()

            plt.figure(figsize=(14, 8))
            unique_df = df[df['Benchmark'].str.contains(benchmark_type) & (
                    df['Benchmark'].str.startswith("RawPtr") |
                    df['Benchmark'].str.startswith("UniquePtr") |
                    df['Benchmark'].str.startswith("Std_UniquePtr")
            )]

            sns.lineplot(x='Iterations', y='Time(ns)', hue='Benchmark', data=unique_df, palette="pastel", marker='o')
            plt.title(f"Benchmark Results (UniquePtr - {benchmark_type})", fontsize=14)
            plt.grid(axis='y', linestyle='--', alpha=0.7)
            plt.xlabel("Iterations", fontsize=12)
            plt.ylabel("Time (ns)", fontsize=12)
            plt.xticks(rotation=45, ha='right', fontsize=10)
            plt.tight_layout()
            plt.savefig(f"benchmark_results_{benchmark_type}_unique.png")
            plt.show()

    except FileNotFoundError:
        print(f"Error: File '{filename}' not found.")
    except pd.errors.EmptyDataError:
        print(f"Error: File '{filename}' is empty or contains invalid data.")
    except Exception as e:
        print(f"An unexpected error occurred: {e}")

if __name__ == "__main__":
    results_file = "benchmark_results.csv"
    benchmark_types = ["Creation"]
    plot_benchmark_results(results_file, benchmark_types)