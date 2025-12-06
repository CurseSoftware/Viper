#!/usr/bin/python3

import argparse
import subprocess

valid_build_targets = [
    "toolchain"
]


def handle_build(args):
    mode = ""
    mode_flag = "-DCMAKE_BUILD_TYPE="

    if args.debug:
        mode = "debug"
        mode_flag += "Debug"
    elif args.release:
        mode = "release"
        mode_flag += "Release"
    else:
        raise ValueError("Invalid build mode")

    print(f"Performing {mode} build")

    subprocess.run(["cmake", "-B", f"build_{mode}", "."])
    subprocess.run(["cmake", "--build", f"build_{mode}"])


def handle_run(args):
    print(f"Running {args.target}...")
    mode = ""
    if args.debug:
        mode = "debug"
    elif args.release:
        mode = "release"
    else:
        raise ValueError("Invalid run mode")

    subprocess.run([f"build_{mode}/{args.target}/{args.target}"])
    print("Done.")


def main():
    parser = argparse.ArgumentParser(description="Runner for Viper project")

    subparsers = parser.add_subparsers(dest="build", required=False)

    build_parser = subparsers.add_parser("build")
    mode_group = build_parser.add_mutually_exclusive_group()
    mode_group.add_argument("-d", "--debug", action="store_true", help="Debug build")
    mode_group.add_argument("-r", "--release", action="store_true", help="Release build")
    build_parser.add_argument("target", type=str, help="Target to build", choices=valid_build_targets)
    build_parser.set_defaults(func=handle_build)

    run_parser = subparsers.add_parser("run")
    mode_group = run_parser.add_mutually_exclusive_group()
    mode_group.add_argument("-d", "--debug", action="store_true", help="Debug run")
    mode_group.add_argument("-r", "--release", action="store_true", help="Release run")
    run_parser.add_argument("target", type=str, help="Target to run", choices=valid_build_targets)
    run_parser.set_defaults(func=handle_run)

    args = parser.parse_args()
    args.func(args)


if __name__ == "__main__":
    main()
