NAME := test_decimal

TEST_SRCS := test_decimal_creation.cpp test_sum.cpp main.cpp
TEST_OBJS := ${TEST_SRCS:.cpp=.o}

TEST_SUBJ_SRCS := ../Decimal.cpp
TEST_SUBJ_HEDS := ${TEST_SUBJ_SRCS:.cpp=.hpp}
TEST_SUBJ_OBJS := ${TEST_SUBJ_SRCS:.cpp=.o}
TEST_SUBJ_OBJS := $(subst ../,,$(TEST_SUBJ_OBJS))

BUILD := ./build/
CFLGS := -Wall -Wextra -Werror -std=c++98 -ggdb3 -DDEBUG -O0

OBJS := $(foreach OBJ, ${TEST_OBJS} ${TEST_SUBJ_OBJS}, $(BUILD)$(OBJ))

all: $(NAME)

$(NAME): $(OBJS)
	c++ $(CFLGS) -o $(NAME) $^

$(BUILD)%.o: %.cpp $(TEST_SUBJ_HEDS)
	mkdir -p ${@D}
	c++ $(CFLGS) -c $< -o $@

$(BUILD)%.o: ../%.cpp $(TEST_SUBJ_HEDS)
	mkdir -p ${@D}
	c++ $(CFLGS) -c $< -o $@

clean:
	rm -rf $(BUILD)
fclean: clean
	rm -f $(NAME)

test: all
	./$(NAME)

retest: fclean test
re: fclean all

.PHONY: all fclean clean test retest re
