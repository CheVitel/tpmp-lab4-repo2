#ifndef SALON_H
#define SALON_H

#include <sqlite3.h>

extern sqlite3 *db;

// Инициализация
int init_db(const char *db_name);
void close_db();

// Пункт 2: Запросы SELECT
void report_stock_balance();         // 2.1 Проданные и оставшиеся
void report_cd_sales_period(const char* code, const char* start, const char* end); // 2.2
void report_most_popular_cd();      // 2.3 (*)
void report_popular_performer();    // 2.4 (*)
void report_author_revenue();       // 2.5

// Пункты 5 и 6: Функционал обработки
void run_period_summary(const char* start, const char* end); // 5 (с занесением в таблицу)
void get_cd_sales_info(const char* code, const char* start, const char* end); // 6 (*)

// Пункт 3: Обновление
void delete_cd(const char* code);

#endif
