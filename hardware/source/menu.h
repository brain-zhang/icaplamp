#ifndef _MENU_H_
#define _MENU_H_

#define MENU_MESSAGE_BASE_SEG 0x10

void Menu_show();
void Menu_next();
void Menu_entry();
void Menu_exit();
uchar Menu_showing();
uchar Menu_is_exiting();
uchar Menu_get_index();

#endif