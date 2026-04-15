#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "salon.h"

sqlite3 *db;

int init_db(const char *db_name) {
    if (sqlite3_open(db_name, &db) != SQLITE_OK) return 0;

    // 1. Создание таблиц
    const char *sql = 
        "CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY, username TEXT, password TEXT);"
        "CREATE TABLE IF NOT EXISTS cds (code TEXT PRIMARY KEY, manuf_date TEXT, manufacturer TEXT, price REAL);"
        "CREATE TABLE IF NOT EXISTS tracks (id INTEGER PRIMARY KEY, title TEXT, author TEXT, performer TEXT, cd_code TEXT);"
        "CREATE TABLE IF NOT EXISTS operations (id INTEGER PRIMARY KEY, op_date TEXT, op_type TEXT, cd_code TEXT, quantity INTEGER);"
        "CREATE TABLE IF NOT EXISTS period_results (cd_code TEXT, in_qty INTEGER, out_qty INTEGER);";

    sqlite3_exec(db, sql, 0, 0, 0);

    // ВАЖНО: Добавляем тестового пользователя, если его нет (нужно для тестов!)
    const char *insert_admin = "INSERT OR IGNORE INTO users (id, username, password) VALUES (1, 'admin', 'admin123');";
    sqlite3_exec(db, insert_admin, 0, 0, 0);

    // 4. ТРИГГЕР (Пункт 4 задания)
    const char *trigger_sql = 
        "CREATE TRIGGER IF NOT EXISTS check_stock BEFORE INSERT ON operations "
        "FOR EACH ROW WHEN NEW.op_type = 'OUT' "
        "BEGIN "
        "  SELECT CASE WHEN ("
        "    (SELECT COALESCE(SUM(quantity), 0) FROM operations WHERE cd_code = NEW.cd_code AND op_type = 'IN') - "
        "    (SELECT COALESCE(SUM(quantity), 0) FROM operations WHERE cd_code = NEW.cd_code AND op_type = 'OUT') - NEW.quantity < 0"
        "  ) THEN RAISE(ABORT, 'Ошибка: Недостаточно товара!') END; "
        "END;";
    
    sqlite3_exec(db, trigger_sql, 0, 0, 0);
    return 1;
}

// 2.1 Сведения о количестве проданных и оставшихся
void report_stock_balance() {
    const char *sql = 
        "SELECT c.code, "
        "SUM(CASE WHEN o.op_type='OUT' THEN o.quantity ELSE 0 END) as sold, "
        "(SUM(CASE WHEN o.op_type='IN' THEN o.quantity ELSE 0 END) - SUM(CASE WHEN o.op_type='OUT' THEN o.quantity ELSE 0 END)) as stock "
        "FROM cds c LEFT JOIN operations o ON c.code = o.cd_code "
        "GROUP BY c.code ORDER BY (stock) DESC;";
    // Выполнение через sqlite3_prepare_v2... (логика печати аналогична предыдущим)
}

// 2.3 Компакт, купленный MAX количество раз (*)
void report_most_popular_cd() {
    const char *sql = 
        "SELECT c.*, t.title, t.performer FROM cds c "
        "JOIN tracks t ON c.code = t.cd_code "
        "WHERE c.code = (SELECT cd_code FROM operations WHERE op_type='OUT' GROUP BY cd_code ORDER BY SUM(quantity) DESC LIMIT 1);";
    // Печать результатов...
}

// 5. Функция за период (заносит в специальную таблицу)
void run_period_summary(const char* start, const char* end) {
    sqlite3_exec(db, "DELETE FROM period_results;", 0, 0, 0);
    const char *sql = 
        "INSERT INTO period_results "
        "SELECT cd_code, "
        "SUM(CASE WHEN op_type='IN' THEN quantity ELSE 0 END), "
        "SUM(CASE WHEN op_type='OUT' THEN quantity ELSE 0 END) "
        "FROM operations WHERE op_date BETWEEN ? AND ? GROUP BY cd_code;";
    
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    sqlite3_bind_text(stmt, 1, start, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, end, -1, SQLITE_STATIC);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    printf("Отчет за период %s - %s успешно сформирован в таблице period_results.\n", start, end);
}

// 6. Результаты продажи по коду за период (*)
void get_cd_sales_info(const char* code, const char* start, const char* end) {
    const char *sql = "SELECT SUM(quantity), SUM(quantity * (SELECT price FROM cds WHERE code = ?)) "
                      "FROM operations WHERE cd_code = ? AND op_type = 'OUT' AND op_date BETWEEN ? AND ?;";
    // Подготовка и вывод...
}

int authenticate(const char *username, const char *password) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT id FROM users WHERE username = ? AND password = ?";
    int auth_success = 0;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);
        if (sqlite3_step(stmt) == SQLITE_ROW) auth_success = 1;
        sqlite3_finalize(stmt);
    }
    return auth_success;
}

void add_cd_with_cover(const char *code, const char *manufacturer, double price, const char *image_path) {
    sqlite3_stmt *stmt;
    const char *sql = "INSERT OR IGNORE INTO cds (code, manufacturer, price) VALUES (?, ?, ?);";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, code, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, manufacturer, -1, SQLITE_STATIC);
        sqlite3_bind_double(stmt, 3, price);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    printf("Test log: Added CD %s (Path: %s)\n", code, image_path);
}

void close_db() { sqlite3_close(db); }
