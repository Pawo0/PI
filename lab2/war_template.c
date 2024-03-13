#include <stdio.h>
#include <stdlib.h>

#define NUMBER_OF_CARDS 52
#define NUMBER_OF_COLORS 4

int random_from_interval(int a, int b)
{
	if (a > b)
		return INT_MIN;
	if ((b - a) > RAND_MAX)
		return INT_MAX;
	if (a == b)
		return a;
	return (rand() % ((b - a + 1))) + a;
}
void shuffle(int t[], int size)
{
	for (int i = 0; i < size; i++)
		t[i] = i;
	for (int i = 0; i < size - 1; i++)
	{
		int k = random_from_interval(i, size - 1);
		int tmp = t[k];
		t[k] = t[i];
		t[i] = tmp;
	}
}

int take_card(int player[], int first_card, int card, int *len)
{
	int id = (int)(first_card + len) % NUMBER_OF_CARDS;
	player[id] = card;
	len++;
}

int use_card(int player[], int *first_card, int *len)
{
	*len = *len - 1;
	int res = player[*first_card];
	*first_card = (*first_card + 1) % NUMBER_OF_CARDS;
	return res;
}

void game(int n, int player_A[], int player_B[], int max_conflicts, int simplified_mode)
{
	int conflicts = 0;
	int first_card_a = 0, first_card_b = 0;
	int len_a = NUMBER_OF_CARDS / 2, len_b = NUMBER_OF_CARDS / 2;

	switch (simplified_mode)
	{
	case 0: // standardowa

		break;
	case 1: // uproszczona
		while (conflicts < max_conflicts && len_a > 0 && len_b > 0)
		{
			int a = use_card(player_A, &first_card_a, &len_a);
			int b = use_card(player_B, &first_card_b, &len_b);
		}

		break;
	default:
		break;
	}
}
void first_takes_cards(int number_of_cards_on_table, int player_1[], int *pout_1, int *p_cards_1,
					   const int player_2[], int *pout_2, int *p_cards_2, int size)
{
}
void both_take_cards(int player_1[], int *pout_1, const int *p_cards_1,
					 int player_2[], int *pout_2, const int *p_cards_2, int size)
{
}

int main(void)
{
	int player_A[NUMBER_OF_CARDS], player_B[NUMBER_OF_CARDS];
	int deck[NUMBER_OF_CARDS];
	int max_conflicts;
	int simplified_mode;
	int seed;
	scanf("%d", &seed);
	scanf("%d", &simplified_mode);
	scanf("%d", &max_conflicts);

	for (int i = 0; i < NUMBER_OF_CARDS; i++)
		deck[i] = i;
	srand((unsigned)seed);
	shuffle(deck, NUMBER_OF_CARDS);
	for (int i = 0; i < NUMBER_OF_CARDS / 2; i++)
	{
		player_A[i] = deck[i];
		player_B[i] = deck[i + NUMBER_OF_CARDS / 2];
	}
	game(NUMBER_OF_CARDS, player_A, player_B, max_conflicts, simplified_mode);
	return 0;
}
