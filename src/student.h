#ifndef STUDENT_H
#define STUDENT_H

#define MAX_NAME_SIZE 15
#define MAX_FILENAME_SIZE 255

#include "vector.h"

struct student
{
    int studentId;
    char firstName[MAX_NAME_SIZE + 1], lastName[MAX_NAME_SIZE + 1];
    vector_t assessments;
};
typedef struct student student_t;

struct assessment
{
    char subject[MAX_NAME_SIZE];
    int mark;
    char filename[MAX_FILENAME_SIZE];
};
typedef struct assessment assessment_t;

struct assessment_student
{
    assessment_t *assessmentp;
    student_t *studentp;
};
typedef struct assessment_student assessment_student_t;

void initStudent(student_t *student);
void freeStudent(student_t *student);
student_t *searchStudent(const vector_t student_list, int studentId);
void getAllAssessments(const vector_t student_list, vector_t *assessment_list);

#endif