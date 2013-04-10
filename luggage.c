// By Jan Wrobel <wrobel@blues.ath.cx>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LUGGAGE_ID_LENGTH 8
#define FLIGHT_ID_LENGTH 6
#define AIRPORT_CODE_LENGTH 3
#define COMMENTS_LENGTH 80

typedef struct RoutingDirective {
	unsigned int time_stamp;
	char luggage_id[LUGGAGE_ID_LENGTH + 1];
	char flight_id[FLIGHT_ID_LENGTH + 1];
	char departure[AIRPORT_CODE_LENGTH + 1];
	char arrival[AIRPORT_CODE_LENGTH + 1];
	char comments[COMMENTS_LENGTH + 1];
	struct RoutingDirective *next_directive;
} RoutingDirective;

void addRoutingDirectiveToList(RoutingDirective *previous_directive,
			       RoutingDirective *new_directive) {
	RoutingDirective *next_directive = previous_directive->next_directive;
	if (next_directive == NULL ||
	    next_directive->time_stamp > new_directive->time_stamp) {
		new_directive->next_directive = next_directive;
		previous_directive->next_directive = new_directive;
	} else {
		addRoutingDirectiveToList(next_directive, new_directive);
	}
}

int supersedes(RoutingDirective *directive, char *luggage_id, char *departure) {
	if (directive == NULL) {
		return 0;
	}
	if (strcmp(directive->luggage_id, luggage_id) != 0) {
		return supersedes(directive->next_directive,
				  luggage_id,
				  departure);
	}
	if (strcmp(directive->departure, departure) == 0) {
		return 1;
	}
	return 0;
}

int superseded(RoutingDirective *directive) {
	return supersedes(directive->next_directive,
			  directive->luggage_id,
			  directive->departure);
}


int matches(char* expected, char* actual) {
	return (expected[0] == '-' || strcmp(expected, actual) == 0);
}

void printMatchingDirectives(RoutingDirective* first_directive,
			     char *expected_luggage_id,
			     char *expected_flight_id,
			     char *expected_departure,
			     char *expected_arrival) {
	RoutingDirective *directive;
	for (directive = first_directive; directive != NULL;
	     directive = directive->next_directive) {
		if (!superseded(directive) &&
		    matches(expected_luggage_id, directive->luggage_id) &&
		    matches(expected_flight_id, directive->flight_id) &&
		    matches(expected_departure, directive->departure) &&
		    matches(expected_arrival, directive->arrival)) {
                        printf("%010u %s %s %s %s %s\n",
			       directive->time_stamp,
			       directive->luggage_id,
			       directive->flight_id,
			       directive->departure,
			       directive->arrival,
			       directive->comments);
		}
	}
}

int main(int argc, char *argv[]) {
        if (argc != 5) {
                fprintf(stderr, "Command line error: 4 arguments expected\n");
                exit(1);
        }
	RoutingDirective directive_list_head;
	directive_list_head.time_stamp = 0;
	directive_list_head.next_directive = NULL;

	while (1) {
		unsigned int time_stamp;
		char luggage_id[LUGGAGE_ID_LENGTH + 1];
		char flight_id[FLIGHT_ID_LENGTH + 1];
		char departure[AIRPORT_CODE_LENGTH + 1];
		char arrival[AIRPORT_CODE_LENGTH + 1];
		char comments[COMMENTS_LENGTH + 1];
		comments[0] = 0; // comments are optional.

		if (scanf("%d ", &time_stamp) == EOF) {
			break;
		}
		if (scanf("%8[A-Z0-9] %6[A-Z0-9] ",
			  luggage_id, flight_id) == EOF) {
			break;
		}
		if (scanf("%3[A-Z] %3[A-Z]", departure, arrival) == EOF) {
			break;
		}
		if (scanf("%80[^\n]", comments) == EOF) {
			break;
		}

		RoutingDirective *new_directive =
			(RoutingDirective*) calloc(1, sizeof(RoutingDirective));
		new_directive->time_stamp = time_stamp;
		strcpy(new_directive->luggage_id, luggage_id);
		strcpy(new_directive->flight_id, flight_id);
		strcpy(new_directive->departure, departure);
		strcpy(new_directive->arrival, arrival);
		strcpy(new_directive->comments, comments);
		new_directive->next_directive = NULL;

		addRoutingDirectiveToList(&directive_list_head, new_directive);
	}
	printMatchingDirectives(directive_list_head.next_directive,
				argv[1], argv[2], argv[3], argv[4]);
        exit(0);
}
