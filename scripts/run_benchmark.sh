#build/bin/prepare_data --dir --orig data/ --downsized test/downsized/
#build/bin/resize_performance --downsized test/downsized/ --upsized test/upsized/ --results test/results/
#build/bin/quality_evaluation --dir --upsized test/upsized/ --orig data/ --results test/results/

#!/bin/bash

# Image Tool Benchmark Script
# Runs complete benchmark pipeline: prepare data -> resize -> evaluate quality

set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Get script directory and project root
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

# Change to project root
cd "$PROJECT_ROOT"

# Default directories (relative to project root)
DATA_DIR="data"
TEST_DIR="test"
DOWNSIZED_DIR="$TEST_DIR/downsized"
UPSIZED_DIR="$TEST_DIR/upsized"
RESULTS_DIR="$TEST_DIR/results"

echo -e "${BLUE}=== Image Tool Benchmark Pipeline ===${NC}"
echo -e "Working directory: ${BLUE}$(pwd)${NC}"
echo

# Check if build exists
if [ ! -d "build" ]; then
    echo -e "${RED}Error: build/ directory not found. Please run 'make' first.${NC}"
    echo -e "${YELLOW}Hint: Run from project root:${NC}"
    echo -e "  cd $PROJECT_ROOT"
    echo -e "  mkdir build && cd build"
    echo -e "  cmake .. && make"
    exit 1
fi

# Check if data directory exists
if [ ! -d "$DATA_DIR" ]; then
    echo -e "${RED}Error: $DATA_DIR/ directory not found.${NC}"
    exit 1
fi

# Create test directories
echo -e "${YELLOW}Creating test directories...${NC}"
mkdir -p "$TEST_DIR" "$DOWNSIZED_DIR" "$UPSIZED_DIR" "$RESULTS_DIR"

echo -e "${YELLOW}Step 1/3: Preparing test data (downscaling images)...${NC}"
./build/bin/prepare_data --dir --orig "$DATA_DIR/" --downsized "$DOWNSIZED_DIR/"
if [ $? -eq 0 ]; then
    echo -e "${GREEN}✓ Test data prepared${NC}"
else
    echo -e "${RED}✗ Failed to prepare test data${NC}"
    exit 1
fi

echo
echo -e "${YELLOW}Step 2/3: Running resize performance benchmark...${NC}"
./build/bin/resize_performance --downsized "$DOWNSIZED_DIR/" --upsized "$UPSIZED_DIR/" --results "$RESULTS_DIR/"
if [ $? -eq 0 ]; then
    echo -e "${GREEN}✓ Resize benchmark completed${NC}"
else
    echo -e "${RED}✗ Resize benchmark failed${NC}"
    exit 1
fi

echo
echo -e "${YELLOW}Step 3/3: Evaluating image quality...${NC}"
./build/bin/quality_evaluation --dir --upsized "$UPSIZED_DIR/" --orig "$DATA_DIR/" --results "$RESULTS_DIR/"
if [ $? -eq 0 ]; then
    echo -e "${GREEN}✓ Quality evaluation completed${NC}"
else
    echo -e "${RED}✗ Quality evaluation failed${NC}"
    exit 1
fi

echo
echo -e "${GREEN}=== Benchmark Complete! ===${NC}"
echo -e "Results saved to: ${BLUE}$RESULTS_DIR/${NC}"
echo
echo "Generated files:"
echo "  • $RESULTS_DIR/results_time.csv - Performance metrics"
echo "  • $RESULTS_DIR/results.csv - Quality metrics"
