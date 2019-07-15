const char *wd(int year, int month, int day) {
  return ((const char *[])                                         \
          {"Segunda", "Terça", "Quarta",                           \
           "Quinta", "Sexta", "Sábado", "Domingo"})[               \
      (                                                            \
          day                                                      \
        + ((153 * (month + 12 * ((14 - month) / 12) - 3) + 2) / 5) \
        + (365 * (year + 4800 - ((14 - month) / 12)))              \
        + ((year + 4800 - ((14 - month) / 12)) / 4)                \
        - ((year + 4800 - ((14 - month) / 12)) / 100)              \
        + ((year + 4800 - ((14 - month) / 12)) / 400)              \
        - 32045                                                    \
      ) % 7];
}
/* 
int main(void) {
  printf("%d-%02d-%02d: %s\n", 2011, 5, 19, wd(2011, 5, 19));
  printf("%d-%02d-%02d: %s\n", 2038, 1, 19, wd(2038, 1, 19));
  return 0;
}*/

const char *mes[12] = {"Janeiro", "Fevereiro", "Março", "Abril", "Maio","Junho", "Julho",
                       "Agosto", "Setembro", "Outubro", "Novembro", "Dezembro"};