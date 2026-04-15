#ifndef SALON_H
#define SALON_H

#include <sqlite3.h>

extern sqlite3 *db;

int init_db(const char *db_name);
void close_db();
int authenticate(const char *username, const char *password); // Убедись, что это тут есть
void add_cd_with_cover(const char *code, const char *manufacturer, double price, const char *image_path);
void report_stock_balance();
void report_most_popular_cd();
void run_period_summary(const char* start, const char* end);
void get_cd_sales_info(const char* code, const char* start, const char* end);

#endif
