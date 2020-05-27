typedef struct Grave
{
  int64_t row, col;
  char info[100];
  bool reservedFor15years, reservedForever;
  time_t passedAway;
} Grave;
