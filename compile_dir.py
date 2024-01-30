import os
import sys
import subprocess


def compile_files_in_directory(compiler_path, input_dir, output_dir):
    if not os.path.exists(input_dir) or not os.path.exists(output_dir):
        print("Error: Input or output directory does not exist.")
        return

    for input_file in os.listdir(input_dir):
        if input_file.endswith(".imp"):
            input_file_path = os.path.join(input_dir, input_file)
            output_file_path = os.path.join(output_dir, f"{os.path.splitext(input_file)[0]}.mr")

            compile_command = [f"./{compiler_path}", input_file_path, output_file_path]
            subprocess.run(compile_command)


if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("Usage: python script.py compiler_path input_dir output_dir")
        sys.exit(1)

    compiler_path, input_dir, output_dir = sys.argv[1:]
    compile_files_in_directory(compiler_path, input_dir, output_dir)
