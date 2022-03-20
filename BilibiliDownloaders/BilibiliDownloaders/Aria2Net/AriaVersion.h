#pragma once

#define _STR(A) #A
#define STR(A) _STR(A)

#define ARIA_MAJOR 0
#define ARIA_MINOR 0
#define ARIA_PATCH 1

#define GIT_HASH   1

#define ARIA_VERSION 10000*ARIA_MAJOR + 100*ARIA_MINOR + ARIA_PATCH
#define ARIA_VERSION_STR ARIA_MAJOR.#ARIA_MINOR#.ARIA_PATCH
#define ARIA_VERSION_STR_WITH_GIT ARIA_VERSION_STR.#GIT_HASH

#define Build_YEAR
#define Build_MONTH
#define Build_DAY

#define Build_DATE Build_YEAR.#Build_MONTH#.Build_DAY

#define Build_HOUR
#define Build_MINTUE
#define Build_SECOND

#define Build_TIME Build_HOUR.#Build_MINTUE#.Build_SECOND

#define Build_STR Build_DATE.#Build_TIME
