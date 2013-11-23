// compile and link: gcc <program file> -lformw -lncursesw -lstdc++


# include <locale.h>

//# include "ncurses.h"
//# include "form/form.h"
# include <test.priv.h>

# if USE_WIDEC_SUPPORT && USE_LIBFORM

# include <form.h>

int main()
{
  setlocale(LC_ALL, "");

  FIELD *field[3];
	FORM  *my_form;

	/* Initialize curses */
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	/* Initialize the fields */
	field[0] = new_field(1, 10, 4, 18, 0, 0);
	field[1] = new_field(1, 10, 6, 18, 0, 0);
	field[2] = NULL;

	/* Set field options */
	set_field_back(field[0], A_UNDERLINE); 	/* Print a line for the option 	*/
	field_opts_off(field[0], O_AUTOSKIP);  	/* Don't go to next field when this */
						/* Field is filled up 		*/
	set_field_back(field[1], A_UNDERLINE);
	field_opts_off(field[1], O_AUTOSKIP);

	/* Create the form and post it */
	my_form = new_form(field);
	post_form(my_form);
	refresh();

	mvprintw(4, 10, "Value 1:");
	mvprintw(6, 10, "Value 2:");
	refresh();

	/* Loop through to get user requests */
	while(true)
	{
    wint_t ch;
    int ret = get_wch (&ch);

    char out_[100];
    char *out = out_;
    sprintf(out, "Got %d (%c), type: %d              ", ch, ch, ret);
    mvprintw (8, 10, out);

    switch (ret)
    {
      case KEY_CODE_YES:
        {
          switch(ch)
          {
            case KEY_DOWN:
              /* Go to next field */
              form_driver(my_form, REQ_NEXT_FIELD);
              /* Go to the end of the present buffer */
              /* Leaves nicely at the last character */
              form_driver(my_form, REQ_END_LINE);
              break;
            case KEY_UP:
              /* Go to previous field */
              form_driver(my_form, REQ_PREV_FIELD);
              form_driver(my_form, REQ_END_LINE);
              break;
            default:
              /* If this is a normal character, it gets */
              /* Printed				  */
              //form_driver(my_form, ch);
              //wadd_wch (my_form->current->working, ch);

              break;
          }
        }
        break;
      case OK:
        {
          form_driver_w (my_form, OK, ch);
        }
        break;
    }
	}

	/* Un post form and free the memory */
	unpost_form(my_form);
	free_form(my_form);
	free_field(field[0]);
	free_field(field[1]);

	endwin();
	return 0;
}

#else
int
main(void)
{
  printf("This program requires the wide-ncurses and forms library\n");
  ExitProgram(EXIT_FAILURE);
  return 1;
}
#endif // USE_WIDEC_SUPPORT

