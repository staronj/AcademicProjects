/**
 * Copyright Jakub Staroń 2015
 * numer indeksu 361025
 */

#ifndef PROJECT_ACTIVITYSUSPENDER_H
#define PROJECT_ACTIVITYSUSPENDER_H

/**
 * Monitor pomagający w zawieszaniu działalności przez firmę
 */
struct ActivitySuspender;
typedef struct ActivitySuspender* ActivitySuspender_pointer;

ActivitySuspender_pointer ActivitySuspender_create();
void ActivitySuspender_pass(ActivitySuspender_pointer this);
void ActivitySuspender_suspend(ActivitySuspender_pointer this);
void ActivitySuspender_resume(ActivitySuspender_pointer this);
void ActivitySuspender_free(ActivitySuspender_pointer this);

#endif //PROJECT_ACTIVITYSUSPENDER_H
