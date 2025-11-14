# ============================================================================ #
#                                PRINTF PROJECT                                #
#                           42 Madrid | by <laviles>                           #
# ============================================================================ #

# == Compilation Settings ==================================================== #

NAME		= 	libftprintf.a

CC			= 	cc

CFLAGS		=	-Wall -Wextra -Werror 

# == Sources ================================================================= #

SRCS		=	ft_printf.c ft_printf_utils.c
OBJS		= 	$(SRCS:.c=.o)

# == Rules =================================================================== #

all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

clean:
	rm -f $(OBJS) $(BONUS_OBJS)

fclean: clean
	rm -f $(NAME)

re:	fclean all

.PHONY: $(NAME) all clean fclean re
