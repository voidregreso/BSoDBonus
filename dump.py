import pefile
import sys

def dump_exported_symbols(pe_file, output_file):
    try:
        pe = pefile.PE(pe_file)

        if not hasattr(pe, 'DIRECTORY_ENTRY_EXPORT'):
            print("No exported symbols found in the PE file.")
            return

        with open(output_file, 'w') as file:
            for exp in pe.DIRECTORY_ENTRY_EXPORT.symbols:
                if exp.name is not None:
                    file.write(f"{exp.name.decode()}\n")

        print(f"Exported symbols have been written to {output_file}")

    except FileNotFoundError:
        print(f"File not found: {pe_file}")
    except Exception as e:
        print(f"An error occurred: {e}")

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python script.py <PE file> <output file>")
    else:
        dump_exported_symbols(sys.argv[1], sys.argv[2])
