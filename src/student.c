#include <stdio.h>  /* printf */
#include <string.h> /* strcmp */
#include "student.h"
#include "vector.h"
#include "search.h"

/*****************************************************************************
 * This function compares a students ID with a given ID.
 * Input:
 *   a - A void pointer to a student of type student_t.
 *       Must be this type to satisfy search() and sort()
 *   b - A void pointer to a int. Value to compare with student ID.
 *       Must be this type to satisfy search() and sort()
 * Return:
 *   1 - If the ID of the student matches the ID passed to compare with. 
 *   0 - Otherwise.
*****************************************************************************/
int hasStudentId(const void *a, const void *b)
{
    /* Cast type both arguments to their type. */
    student_t *student = (student_t *)a;
    int *studentId = (int *)b;
    /* Now compare them. */
    return student->studentId == *studentId;
}

/*****************************************************************************
 * This function compares an assessments file name with a given filename.
 * Input:
 *   a - A void pointer to an assessment of type assessment_t.
 *       Must be this type to satisfy search()
 *   b - A void pointer to a string to compare with assessments filename.
 *       Must be this type to satisfy search()
 * Return:
 *   1 - If the assessment filename matches the filename passed. 
 *   0 - Otherwise.
*****************************************************************************/
int hasFilename(const void *a, const void *b)
{
    /* Cast type both arguments to their type. */
    assessment_t *assessment = (assessment_t *)a;
    char *filename = (char *)b;
    /* Now compare them. */
    return !(strcmp(assessment->filename, filename));
}

/*****************************************************************************
 * This function initialises a student. Setting the default values and 
 * initialises its assessment list.
 * Input:
 *   student - Pointer to the student to be initialised.
 * Post:
 *   student will now be initialised with default values.
*****************************************************************************/
void initStudent(student_t *student)
{
    /* Set to the initial values. */
    student->studentId = 0;
    student->firstName[0] = '\0';
    student->lastName[0] = '\0';
    /* initialises assessment vector. */
    initVector(&student->assessments, sizeof(assessment_t));
}

/*****************************************************************************
 * This function copies a students information to another student struct.
 * Input:
 *   dest - Pointer to the destination student.
 *   src - student to be copied from.
 * Post:
 *   dest will now contain all the values of student src.
*****************************************************************************/
void copyStudent(student_t *dest, const student_t src)
{
    /* Copy the student information */
    dest->studentId = src.studentId;
    strcpy(dest->firstName, src.firstName);
    strcpy(dest->lastName, src.lastName);
    /* Copy the assessment vector. */
    copyVector(&dest->assessments, src.assessments);
}

/*****************************************************************************
 * This function frees a student. 
 * Input:
 *   student - Pointer to the student to be freed.
 * Post:
 *   student will now be freed. Only the assessment vector is dynamically
 *   allocated so only that is freed.
*****************************************************************************/
void freeStudent(student_t *student)
{
    /* Free the asseessment vector */
    freeVector(student->assessments);
}

/*****************************************************************************
 * This function search for a students index in a student list.
 * Input:
 *   studentList - The list of students to search through.
 *   studentId - The ID of the student to search for.
 * Return:
 *   The index of the student in the student list.
 *   -1 if it cannot be found.
*****************************************************************************/
int searchStudentIndex(const vector_t studentList, int studentId)
{
    /* Call the search function using the hasStudentId comparator */
    return search(studentList, hasStudentId, &studentId);
}

/*****************************************************************************
 * This function search for an assessment in under a student.
 * Input:
 *   student - The student whos assessments are to be searched.
 *   filename - The filename of the assessment to search for.
 * Return:
 *   The index of the assessment in the students assessment list.
 *   -1 if it cannot be found.
*****************************************************************************/
int searchStudentAssessmentIndex(const student_t student, char *filename)
{
    return search(student.assessments, hasFilename, filename);
}

/*****************************************************************************
 * This function converts all the assessments in a student list into a 
 * single vector.
 * Input:
 *   studentList - The list of students.
 *   assessmentList - A pointer to the new list of assessments. All the 
 *                    assessments in the student list will be inserted into.
 * Post:
 *   assessmentList will contain all the assessments.
*****************************************************************************/
void getAllAssessments(const vector_t studentList, vector_t *assessmentList)
{
    /* Call getAllAssessmentsFilter without a filter.  */
    getAllAssessmentsFilter(studentList, assessmentList, NULL);
}

/*****************************************************************************
 * This function converts filters all the assessments in a student list 
 * into a single vector.
 * Input:
 *   studentList - The list of students.
 *   assessmentList - A pointer to the new list of assessments. All the 
 *                    assessments will be inserted into.
 * Post:
 *   assessmentList will contain all the filtered assessments.
*****************************************************************************/
void getAllAssessmentsFilter(const vector_t studentList,
                             vector_t *assessmentList, char *subject)
{
    /* Initialise the assessment list vector. */
    initVector(assessmentList, sizeof(assessment_student_t));
    /* Loop over all students. */
    int i, j;
    for (i = 0; i < studentList.size; i++)
    {
        /* Loop over students assessments. */
        student_t *student = (student_t *)vectorGet(studentList, i);
        for (j = 0; j < student->assessments.size; j++)
        {
            /* Check that the students assessment subject matches 
               the filter */
            assessment_student_t assessment;
            assessment.assessmentp = vectorGet(student->assessments, j);
            if (subject != NULL &&
                strcmp(assessment.assessmentp->subject, subject))
                continue;

            /* Insert it in the the vector if it does match. */
            assessment.studentp = student;
            vectorPushBack(assessmentList, &assessment);
        }
    }
}

/*****************************************************************************
 * This function converts all the assessment filenames in a student list 
 * into a single vector.
 * Input:
 *   studentList - The list of students.
 *   filenames - A pointer to the new list of filename. All the 
 *               filenames in the student list will be inserted into.
 * Post:
 *   filenames will now contain all the filenames.
*****************************************************************************/
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

/*****************************************************************************
 * This function compares a two students by their ID in descending order.
 * Input:
 *   a - A void pointer to a student A of type student_t.
 *       Must be this type to satisfy sort()
 *   b - A void pointer to a student B of type student_t.
 *       Must be this type to satisfy sort()
 * Return:
 *   0 > if student A has a larger student ID
 *   0 < if student B has a larger student ID
*****************************************************************************/
int compareStudentDesc(const void *a, const void *b)
{
    /* Cast type both arguments to student_t. */
    student_t *studentap = (student_t *)a;
    student_t *studentbp = (student_t *)b;
    /* Subtract their IDs to determine which is larger. */
    return studentap->studentId - studentbp->studentId;
}

/*****************************************************************************
 * This function compares a two students by their ID in ascending order.
 * Input:
 *   a - A void pointer to a student A of type student_t.
 *       Must be this type to satisfy sort()
 *   b - A void pointer to a student B of type student_t.
 *       Must be this type to satisfy sort()
 * Return:
 *   0 < if student A has a larger student ID
 *   0 > if student B has a larger student ID
*****************************************************************************/
int compareStudentAsc(const void *a, const void *b)
{
    /* invert the result of a descending order. */
    return -compareStudentDesc(a, b);
}

/*****************************************************************************
 * This function compares a two assessments by their mark in ascending order.
 * Input:
 *   a - A void pointer to a assessment A of type assessment_t.
 *       Must be this type to satisfy sort()
 *   b - A void pointer to a assessment B of type assessment_t.
 *       Must be this type to satisfy sort()
 * Return:
 *   0 < if assessment A has a larger mark
 *   0 > if assessment B has a larger mark
*****************************************************************************/
int compareAssessmentAsc(const void *a, const void *b)
{
    /* Cast type both arguments to assessment_t. */
    assessment_t *assessmentap = (assessment_t *)a;
    assessment_t *assessmentbp = (assessment_t *)b;
    return assessmentbp->mark - assessmentap->mark;
}

/*****************************************************************************
 * This function compares a two assessments by their mark in descending order.
 * Input:
 *   a - A void pointer to a assessment A of type assessment_t.
 *       Must be this type to satisfy sort()
 *   b - A void pointer to a assessment B of type assessment_t.
 *       Must be this type to satisfy sort()
 * Return:
 *   0 > if assessment A has a larger mark
 *   0 < if assessment B has a larger mark
*****************************************************************************/
int compareAssessmentDesc(const void *a, const void *b)
{
    /* invert the result of a ascending order. */
    return -compareAssessmentAsc(a, b);
}

/*****************************************************************************
 * This function compares two student assessments by their mark in 
 * ascending order.
 * Input:
 *   a - A void pointer to a assessment A of type assessment_student_t.
 *       Must be this type to satisfy sort()
 *   b - A void pointer to a assessment B of type assessment_student_t.
 *       Must be this type to satisfy sort()
 * Return:
 *   0 < if assessment A has a larger mark
 *   0 > if assessment B has a larger mark
*****************************************************************************/
int compareAssessmentStudentAsc(const void *a, const void *b)
{
    /* Cast type both arguments to assessment_student_t. */
    assessment_student_t *stuAssap = (assessment_student_t *)a;
    assessment_student_t *stuAssbp = (assessment_student_t *)b;
    /* Compute the compareAssessmentAsc for the assessments. */
    return compareAssessmentAsc(stuAssap->assessmentp, stuAssbp->assessmentp);
}

/*****************************************************************************
 * This function compares two student assessments by their mark in 
 * descending order.
 * Input:
 *   a - A void pointer to a assessment A of type assessment_student_t.
 *       Must be this type to satisfy sort()
 *   b - A void pointer to a assessment B of type assessment_student_t.
 *       Must be this type to satisfy sort()
 * Return:
 *   0 > if assessment A has a larger mark
 *   0 < if assessment B has a larger mark
*****************************************************************************/
int compareAssessmentStudentDesc(const void *a, const void *b)
{
    /* invert the result of a ascending order. */
    return -compareAssessmentStudentAsc(a, b);
}
