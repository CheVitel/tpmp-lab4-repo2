#include <stdio.h>
#include "salon.h"

int main() {
    init_db("salon.db");

    int choice;
    while (1) {
        printf("\n--- МЕНЮ САЛОНА ---\n");
        printf("1. Баланс склада \n");
        printf("2. Самый популярный CD \n");
        printf("3. Сформировать спец. отчет за период \n");
        printf("4. Результаты продажи диска за период \n");
        printf("0. Выход\n");
        printf("Выбор: ");
        scanf("%d", &choice);

        if (choice == 0) break;

        switch(choice) {
            case 1: report_stock_balance(); break;
            case 2: report_most_popular_cd(); break;
            case 3: 
                run_period_summary("2023-01-01", "2023-12-31"); 
                break;
            case 4:
                get_cd_sales_info("CD001", "2023-01-01", "2023-12-31");
                break;
        }
    }

    close_db();
    return 0;
}
