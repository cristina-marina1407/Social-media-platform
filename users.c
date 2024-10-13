#include "users.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *db_path = "users.db";

static char **users;
static uint16_t users_number;

void init_users(void)
{
	FILE *users_db = fopen(db_path, "r");

	if (!users_db) {
		perror("Error reading users.db");
		return;
	}

	fscanf(users_db, "%hu", &users_number);

	users = malloc(users_number * sizeof(char *));

	char temp[32];
	for (uint16_t i = 0; i < users_number; i++) {
		fscanf(users_db, "%s", temp);
		int size = strlen(temp);

		users[i] = malloc(size + 1);
		strcpy(users[i], temp);
	}

	fclose(users_db);
}

uint16_t get_user_id(char *name)
{
	if (!users)
		return -1;

	for (uint16_t i = 0; i < users_number; i++)
		if (!strcmp(users[i], name))
			return i;

	return -1;
}

char *get_user_name(uint16_t id)
{
	if (id >= users_number)
		return NULL;

	return users[id];
}

void free_users(void)
{
	for (size_t i = 0; i < users_number; i++)
		free(users[i]);

	free(users);
}
