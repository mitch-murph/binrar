/*****************************************************************************
 * 48430 Fundamentals of C Programming - Assignment 3
 * Student
 * Functions for the student information.
*****************************************************************************/
#ifndef STUDENT_H
#define STUDENT_H

#include "vector.h"
#include "database.h"

/*****************************************************************************
 * Preprocessing directives (Public)
*****************************************************************************/
#define MAX_NAME_SIZE 15

/*****************************************************************************
 * Structs
*****************************************************************************/
/* Struct to store information about a student */
struct student
{
    int studentId;
    char firstName[MAX_NAME_SIZE + 1], lastName[MAX_NAME_SIZE + 1];
    vector_t assessments;
};
typedef struct student student_t;

/* Struct to store information about a an assessment */
struct assessment
{
    char subject[MAX_NAME_SIZE];
    int mark;
    char filename[MAX_FILENAME_SIZE];
};
typedef struct assessment assessment_t;

/* Struct to store a point to an assessment and the corresponding student. */
struct assessment_student
{
    assessment_t *assessmentp;
    student_t *studentp;
};
typedef struct assessment_student assessment_student_t;

/*****************************************************************************
 * Public function prototypes
*****************************************************************************/
void initStudent(student_t *student);
void copyStudent(student_t *dest, const student_t src);
void freeStudent(student_t *student);
int searchStudentIndex(const vector_t studentList, int studentId);
int searchStudentAssessmentIndex(const student_t student, char *filename);
void getAllAssessments(const vector_t studentList, vector_t *assessmentList);
void getAllFilenames(const vector_t studentList, vector_t *filenames);
void getAllAssessmentsFilter(const vector_t studentList,
                             vector_t *assessmentList, char *subject);
int compareStudentAsc(const void *a, const void *b);
int compareStudentDesc(const void *a, const void *b);
int compareAssessmentAsc(const void *a, const void *b);
int compareAssessmentDesc(const void *a, const void *b);
int compareAssessmentStudentAsc(const void *a, const void *b);
int compareAssessmentStudentDesc(const void *a, const void *b);

#endif