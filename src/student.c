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
    init_vector(&student->assessments, 10, sizeof(assessment_t));
}

void freeStudent(student_t *student)
{
    free_vector(student->assessments);
}

student_t *searchStudent(const vector_t studentList, int studentId)
{
    int maybeStudent = linearSearch(studentList, hasStudentId, &studentId);
    printf("maybeStudent: %d\n", maybeStudent);
    if (maybeStudent != -1)
        return vector_get(studentList, maybeStudent);
    return NULL;
}

void getAllAssessments(const vector_t student_list, vector_t *assessment_list)
{
    init_vector(assessment_list, 10, sizeof(assessment_student_t));
    int i, j;
    for (i = 0; i < student_list.size; i++)
    {
        student_t *student = (student_t *)vector_get(student_list, i);
        for (j = 0; j < student->assessments.size; j++)
        {
            assessment_student_t assessment;
            assessment.studentp = student;
            assessment.assessmentp = vector_get(student->assessments, i);
            vector_push_back(assessment_list, &assessment);
        }
    }
}

void getAllFilenames(const vector_t student_list, vector_t *filenames)
{
    init_vector(filenames, 10, sizeof(char) * MAX_FILENAME_SIZE);
    int i, j;
    for (i = 0; i < student_list.size; i++)
    {
        student_t *student = (student_t *)vector_get(student_list, i);
        for (j = 0; j < student->assessments.size; j++)
        {
            assessment_t *assessment = vector_get(student->assessments, j);
            vector_push_back(filenames, assessment->filename);
        }
    }
}