import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns

def plot_benchmark_results(filename, benchmark_types):
    try:
        df = pd.read_csv(filename)
        for benchmark_type in benchmark_types:
            plt.figure(figsize=(12, 7))
            shared_df = df[df['Benchmark'].str.contains(benchmark_type) & (
                    df['Benchmark'].str.startswith("RawPtr") |
                    df['Benchmark'].str.startswith("SharedPtr") |
                    df['Benchmark'].str.startswith("Std_SharedPtr")
            )]

            sns.barplot(x='Benchmark', y='Time(ns)', data=shared_df, palette="pastel") # Seaborn barplot
            plt.xlabel("Benchmark", fontsize=12)
            plt.ylabel("Time(ns)", fontsize=12)
            plt.title(f"Benchmark Results (SharedPtr - {benchmark_type})", fontsize=14)
            plt.xticks(rotation=45, ha='right', fontsize=10)
            plt.tight_layout()
            plt.grid(axis='y', linestyle='--', alpha=0.7)
            plt.savefig(f"benchmark_results_{benchmark_type}_shared.png")
            plt.show()


            # --- UniquePtr Graph (Seaborn) ---
            plt.figure(figsize=(12, 7))
            unique_df = df[df['Benchmark'].str.contains(benchmark_type) & (
                    df['Benchmark'].str.startswith("RawPtr") |
                    df['Benchmark'].str.startswith("UniquePtr") |
                    df['Benchmark'].str.startswith("Std_UniquePtr")
            )]

            sns.barplot(x='Benchmark', y='Time(ns)', data=unique_df, palette="pastel")

            plt.xlabel("Benchmark", fontsize=12)
            plt.ylabel("Time(ns)", fontsize=12)
            plt.title(f"Benchmark Results (UniquePtr - {benchmark_type})", fontsize=14)
            plt.xticks(rotation=45, ha='right', fontsize=10)
            plt.tight_layout()
            plt.grid(axis='y', linestyle='--', alpha=0.7)
            plt.savefig(f"benchmark_results_{benchmark_type}_unique.png")
            plt.show()


    except FileNotFoundError:
        print(f"Error: File '{filename}' not found.")
    except pd.errors.EmptyDataError:
        print(f"Error: File '{filename}' is empty or contains invalid data.")
    except Exception as e:
        print(f"An unexpected error occurred: {e}")


results_file = "benchmark_results.csv"
benchmark_types = ["Creation"]
plot_benchmark_results(results_file, benchmark_types)