#include "student.h"
#include "vector.h"
#include "search.h"
#include <stdio.h>
#include <string.h> /* strcmp */

int hasStudentId(const void *a, const void *b)
{
    student_t *student = (student_t *)a;
    int *studentId = (int *)b;
    return student->studentId == *studentId;
}

int hasFilename(const void *a, const void *b)
{
    assessment_t *assessment = (assessment_t *)a;
    char *filename = (char *)b;
    return !(strcmp(assessment->filename, filename));
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

int searchStudentAssessmentIndex(const student_t student, char *filename)
{
    return search(student.assessments, hasFilename, filename);
}

void getAllAssessments(const vector_t studentList, vector_t *assessmentList)
{
    getAllAssessmentsFilter(studentList, assessmentList, NULL);
}
void getAllAssessmentsFilter(const vector_t studentList, vector_t *assessmentList, char *subject)
{
    initVector(assessmentList, sizeof(assessment_student_t));
    int i, j;
    for (i = 0; i < studentList.size; i++)
    {
        student_t *student = (student_t *)vectorGet(studentList, i);
        for (j = 0; j < student->assessments.size; j++)
        {
            assessment_student_t assessment;
            assessment.assessmentp = vectorGet(student->assessments, j);

            if (subject != NULL &&
                strcmp(assessment.assessmentp->subject, subject))
                continue;

            assessment.studentp = student;
            vectorPushBack(assessmentList, &assessment);
        }
    }
}

void getAllFilenames(const vector_t studentList, vector_t *filenames)
{
    initVector(filenames, sizeof(char) * MAX_FILENAME_SIZE);
    int i, j;
    for (i = 0; i < studentList.size; i++)
    {
        student_t *student = (student_t *)vectorGet(studentList, i);
        for (j = 0; j < student->assessments.size; j++)
        {
            assessment_t *assessment = vectorGet(student->assessments, j);
            vectorPushBack(filenames, assessment->filename);
        }
    }
}

int compareStudentDesc(const void *a, const void *b)
{
    student_t *studentap = (student_t *)a;
    student_t *studentbp = (student_t *)b;
    return studentap->studentId - studentbp->studentId;
}

int compareStudentAsc(const void *a, const void *b)
{
    return -compareStudentDesc(a, b);
}

int compareAssessmentAsc(const void *a, const void *b)
{

    assessment_t *assessmentap = (assessment_t *)a;
    assessment_t *assessmentab = (assessment_t *)b;
    return assessmentap->mark - assessmentab->mark;
}

int compareAssessmentDesc(const void *a, const void *b)
{
    return -compareAssessmentAsc(a, b);
}

int compareAssessmentStudentAsc(const void *a, const void *b)
{

    assessment_student_t *stuAssap = (assessment_student_t *)a;
    assessment_student_t *stuAssab = (assessment_student_t *)b;
    return compareAssessmentAsc(stuAssap->assessmentp, stuAssab->assessmentp);
}

int compareAssessmentStudentDesc(const void *a, const void *b)
{
    return -compareAssessmentStudentAsc(a, b);
}
