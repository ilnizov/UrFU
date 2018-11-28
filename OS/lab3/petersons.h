#ifndef LAB3_PETERSONS_H
#define LAB3_PETERSONS_H

extern int n;
extern int *last;
extern int *stage;

void enter_region(int process);
void leave_region(int process);

#endif
