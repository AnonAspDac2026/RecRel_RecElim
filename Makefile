# Compiler and Flags
CXX = gcc
CXXFLAGS = -I/usr/rose/include/rose -std=c++17
LDFLAGS = -L/usr/rose/lib -lrose -ldl -lpthread -lstdc++

CXX2 = g++
CXXFLAGS2 = -std=c++17 -Wall -Wextra
LDFLAGS2 = -lm 

# Source files and headers
SRC = src/transformation_helper.cpp src/transformation_divconq.cpp src/tail.cpp src/helper.cpp src/analysis.cpp
HEADERS = src/transformation_helper.h src/transformation_divconq.h src/tail.h src/helper.h
OBJ = $(SRC:.cpp=.o)

# Executable name
TARGET = analysis_tool

# Directories
TEST_DIR = test_programs
RESULT_DIR = result_programs
WRAPPED_DIR = wrapped_programs

# Paths and directories
INPUT_DIR = inputs
OUTPUT_DIR = outputs
TRUE_OUTPUT_DIR = $(OUTPUT_DIR)/true
RESULT_OUTPUT_DIR = $(OUTPUT_DIR)/result

PYTHON = python3

# Build executable
all: $(TARGET) create_result_dir

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ) $(LDFLAGS)

# Create the result directory
create_result_dir:
	mkdir -p $(RESULT_DIR)

# Compile source files into object files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -f $(OBJ) $(TARGET)
	rm -rf $(RESULT_DIR)

# Rebuild the project
rebuild: clean all


# Run the tool on all test programs
run: all create_result_dir
	@echo "Running analysis_tool on test programs..."
	@for file in $(TEST_DIR)/*.cpp; do \
		prog=$$(basename $$file); \
		echo "Processing $$file..."; \
		./$(TARGET) $$file $(RESULT_DIR)/$$prog; \
	done
	@echo "All test programs processed. Output stored in $(RESULT_DIR)."

generate_inputs:
	@echo "Generating test inputs..."
	@$(PYTHON) generate_inputs.py

# Run original programs and collect outputs
run_original:
	@echo "Running original test programs..."
	@mkdir -p $(TRUE_OUTPUT_DIR)
	@for file in $(TEST_DIR)/*.cpp; do \
		prog=$$(basename $$file .cpp); \
		for input in $(INPUT_DIR)/$${prog}_input*.txt; do \
			$(CXX2) $(CXXFLAGS2) $$file -o $${prog}_orig $(LDFLAGS2) && chmod +x $${prog}_orig; \
			bash -c "./$${prog}_orig < $$input > $(TRUE_OUTPUT_DIR)/$${prog}_true_$$(basename $$input)"; \
		done; \
		rm -f $${prog}_orig; \
	done
	@echo "Original programs' outputs stored."

# Wrap main functions for transformed files
wrap_main:
	@echo "Adding main functions to transformed files..."
	@$(PYTHON) wrap_main.py

# Run transformed programs and collect outputs
run_transformed: 
	@echo "Running transformed programs..."
	@mkdir -p $(RESULT_OUTPUT_DIR)
	@for file in $(RESULT_DIR)/*.cpp; do \
		prog=$$(basename $$file .cpp | sed 's/^rose_//'); \
		for input in $(INPUT_DIR)/$${prog}_input*.txt; do \
			$(CXX2) $(CXXFLAGS2) $$file -o $${prog}_transformed $(LDFLAGS2) && chmod +x $${prog}_transformed; \
			bash -c "./$${prog}_transformed < $$input > $(RESULT_OUTPUT_DIR)/$${prog}_result_$$(basename $$input)"; \
		done; \
		rm -f $${prog}_transformed; \
	done
	@echo "Transformed programs' outputs stored."

# Compare outputs
compare_outputs:
	@echo "Comparing outputs..."
	@for file in $(TRUE_OUTPUT_DIR)/*; do \
		result_file=$(RESULT_OUTPUT_DIR)/$$(basename $$file | sed 's/true/result/'); \
		if cmp -s $$file $$result_file; then \
			echo "PASSED: $$(basename $$file)"; \
		else \
			echo "FAILED: $$(basename $$file)"; \
		fi; \
	done

# Full test pipeline
test_all: generate_inputs run_original run_transformed compare_outputs
	@echo "Testing completed successfully."


.PHONY: all clean rebuild create_result_dir run generate_inputs run_original run_transformed compare_outputs test_all
