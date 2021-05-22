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