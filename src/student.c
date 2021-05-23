#include "student.h"
#include "vector.h"
#include "search.h"
#include <stdio.h>

int hasStudentId(const void *a, const void *b)
{
    student_t *student = (student_t *)a;
    int *studentId = (int *)b;
    return student->studentId == *studentId;
}

void initStudent(student_t *student)
{
    student->studentId = 0;
    student->firstName[0] = '\0';
    student->lastName[0] = '\0';
    initVector(&student->assessments, sizeof(assessment_t));
}

void freeStudent(student_t *student)
{
    freeVector(student->assessments);
}

student_t *searchStudent(const vector_t studentList, int studentId)
{
    int maybeStudent = search(studentList, hasStudentId, &studentId);
    printf("maybeStudent: %d\n", maybeStudent);
    if (maybeStudent != -1)
        return vectorGet(studentList, maybeStudent);
    return NULL;
}

int searchStudentIndex(const vector_t studentList, int studentId)
{
    return search(studentList, hasStudentId, &studentId);
}

void getAllAssessments(const vector_t student_list, vector_t *assessment_list)
{
    initVector(assessment_list, sizeof(assessment_student_t));
    int i, j;
    for (i = 0; i < student_list.size; i++)
    {
        student_t *student = (student_t *)vectorGet(student_list, i);
        for (j = 0; j < student->assessments.size; j++)
        {
            assessment_student_t assessment;
            assessment.studentp = student;
            assessment.assessmentp = vectorGet(student->assessments, j);
            vectorPushBack(assessment_list, &assessment);
        }
    }
}

void getAllFilenames(const vector_t student_list, vector_t *filenames)
{
    initVector(filenames, sizeof(char) * MAX_FILENAME_SIZE);
    int i, j;
    for (i = 0; i < student_list.size; i++)
    {
        student_t *student = (student_t *)vectorGet(student_list, i);
        for (j = 0; j < student->assessments.size; j++)
        {
            assessment_t *assessment = vectorGet(student->assessments, j);
            vectorPushBack(filenames, assessment->filename);
        }
    }
}