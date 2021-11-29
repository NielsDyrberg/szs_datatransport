

def analyze_files():
    f = open("test_files/send_file_64.bin", "rb")
    print(f.read(64))
    print("---")
    print(f.read(64))
    print("---")
    print(f.read(64))
    print("---")
    print(f.read(64))
    print("---")
    print(f.read(64))


def main():
    # todo download all files
    analyze_files()
    pass


if __name__ == "__main__":
    main()
