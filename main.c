#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>

struct dlink_node {
	char *data;
	size_t data_cap;
	size_t data_len;
	struct dlink_node *next;
	struct dlink_node *prev;
};

struct dlink_node *new_link() {
	struct dlink_node *out = malloc(sizeof(struct dlink_node));
	out->data_cap = 0;
	out->data_len = 0;
	out->data = NULL;
	out->next = NULL;
	out->prev = NULL;
	return out;
}

void append(struct dlink_node *head, struct dlink_node *next) {
	struct dlink_node *iter = head;
	while (iter->next != NULL) {
		iter = iter->next;
	}
	next->prev = iter;
	iter->next = next;
}

struct dlink_node *prepend(struct dlink_node *head, struct dlink_node *newhead) {
	newhead->next = head;
	head->prev = newhead;
	return newhead;
}

struct dlink_node *last(struct dlink_node *head) {
	struct dlink_node *iter = head;
	while (iter-> next != NULL) {
		iter = iter->next;
	}
	return iter;
}

void free_list(struct dlink_node *head) {
	struct dlink_node *iter = head;
	struct dlink_node *last = iter;

	do {
		if (iter->data != NULL) {
			free(iter->data);
		}
		last = iter;
		if (iter->next != NULL) {
			iter = iter->next;
		} else {
			iter = NULL;
		}
		free(last);
	} while (iter != NULL);
}


void print_reverse(struct dlink_node *tail) {
	struct dlink_node *iter = tail;
	printf("capacity %zu, length %zu : %s\n", iter->data_cap, iter->data_len, iter->data);
	while (iter->prev != NULL) {
		iter = iter->prev;
		printf("capacity %zu, length %zu : %s\n", iter->data_cap, iter->data_len, iter->data);
	}
}


void print_list(struct dlink_node *head) {
	struct dlink_node *iter = head;
	printf("capacity %zu, length %zu : %s\n", iter->data_cap, iter->data_len, iter->data);
	while (iter->next != NULL) {
		iter = iter->next;
		printf("capacity %zu, length %zu : %s\n", iter->data_cap, iter->data_len, iter->data);
	}
}


void alloc_failed() {
	perror("Allocation failure");
	exit(1);
}

struct dlink_node *take_input() {
	int last_c;
	struct dlink_node *out = new_link();
	// Move through any whitespace before the meaningful input
	while (true)
	{
		last_c = getchar();
		// User is finished, early return
		if (last_c == EOF) {
			return out;
		}
		// We found our first non-whitespace character
		// Push it back into stdin so later code can pull it out again
		if (!isspace(last_c)) {
			ungetc(last_c, stdin);
			break;
		}
	}

	//TODO Double allocation size each time
	const size_t ALLOC_SZ = 2; // Troll the allocator, cause lots of allocations
	size_t idx = 0;
	char *buff = (char*) malloc(ALLOC_SZ);
	if (buff == NULL) {
		alloc_failed();
	}
	size_t current_size = ALLOC_SZ;
	while (true) {
		last_c = getchar();
		if (last_c == '\n' || last_c == EOF) {
			buff[idx] = 0;
			break;
		}
		buff[idx++] = last_c;
		if (idx >= current_size - 1) {
			buff = realloc(buff, current_size + ALLOC_SZ);
			if (buff == NULL) {
				alloc_failed();
			}
			current_size += ALLOC_SZ;
		}
	}
	out->data = buff;
	out->data_cap = current_size;
	out->data_len = idx;
	return out;
}

int main(void) {
	struct dlink_node *head = NULL;
	while (true) {
		printf ("Enter a TODO item:\n");
		struct dlink_node *result = take_input();
		if (result->data_cap == 0) {
			free(result);
			break;
		}
		if (head == NULL) {
			head = result;
		} else {
			append(head, result);
		}
		print_list(head);

	}
	print_reverse(last(head));
	free_list(head);
	printf("Done\n");
	return 0;
}
