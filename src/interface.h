#ifndef INTERFACE_H
#define INTERFACE_H

#include "vector.h"
#include "student.h"

int readStringFixLength(char *strp, int length);
int flushScan(void);
int scanAreYouSure(char *message);
void waitForEnter();
int scanMenu(void (*printMenu)(const void *a), const void *);
void displayStudentAssessments(const student_t student);
void sortStudentAssessmentFile(student_t *student);
int scanFilename(char *filename, char *message);

#endif