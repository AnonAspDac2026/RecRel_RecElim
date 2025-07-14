# RecRel_RecElim

This is an anonymous repository for a submission made to **ASPDAC 2026**. For the paper:

**"Automatic Recursion Elimination using Recurrence Relations for Synthesis of Stack-free Hardware"**

This repository contains:
- An implementation of the method described in the paper.
- The benchmark programs used for evaluation.

## Building the Tool

To build the tool, run:

    make

## Running the Benchmarks

To run the selected set of benchmarks:

    make test

## General Usage

The tool can be run manually as follows:

    ./analysis_tool [optional: -I include_path] input_file output_file

## Notes

For some files, when running the tool, the following error can be printed:

    undefined reference to `main'
    collect2: error: ld returned 1 exit status

This is a **ROSE-specific warning** and has **no impact** on the output of the tool. It can be safely ignored.
