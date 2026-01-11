import argparse
import gzip
import os
import sys
from glob import glob

parser = argparse.ArgumentParser()
parser.add_argument('input', help='Path to input files to be compressed')
parser.add_argument('-o', '--output', default='', help='Output directory')

args = parser.parse_args()

input_dir = os.path.abspath(args.input)
output_dir = os.path.abspath(args.output) if args.output else input_dir

if not os.path.isdir(input_dir):
    print(f'Error: Input directory does not exist: {input_dir}', file=sys.stderr)
    sys.exit(1)

# Ensure output directory exists
os.makedirs(output_dir, exist_ok=True)

files = (
    glob('**/*.htm', root_dir=input_dir, recursive=True)
    + glob('**/*.css', root_dir=input_dir, recursive=True)
    + glob('**/*.svg', root_dir=input_dir, recursive=True)
)

if not files:
    print(f'Warning: No files found to compress in {input_dir}', file=sys.stderr)

for file in files:
    input_path = os.path.join(input_dir, file)
    try:
        with open(input_path, 'rb') as f:
            input_data = f.read()

        output_path = os.path.join(output_dir, file + '.gz')
        output_path_dir = os.path.dirname(output_path)
        if output_path_dir != '':
            os.makedirs(output_path_dir, exist_ok=True)

        print(f'Compressing {input_path} -> {output_path}', file=sys.stderr)

        with gzip.open(output_path, 'wb') as f:
            f.write(input_data)
    except Exception as e:
        print(f'Error processing {input_path}: {e}', file=sys.stderr)
        sys.exit(1)
