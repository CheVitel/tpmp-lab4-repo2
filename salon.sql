-- Created by Redgate Data Modeler (https://datamodeler.redgate-platform.com)
-- Last modification date: 2026-04-13 18:50:55.45

-- tables
-- Table: compositions
CREATE TABLE compositions (
    id integer NOT NULL CONSTRAINT compositions_pk PRIMARY KEY,
    disc_id integer NOT NULL,
    title text,
    author varchar(100) NOT NULL,
    performer varchar(100) NOT NULL,
    discs_id integer NOT NULL,
    CONSTRAINT compositions_discs FOREIGN KEY (discs_id)
    REFERENCES discs (id)
);

-- Table: discs
CREATE TABLE discs (
    id integer NOT NULL CONSTRAINT discs_pk PRIMARY KEY,
    manufacturer text NOT NULL,
    price numeric NOT NULL,
    stock_quantity integer NOT NULL
);

-- Table: transactions
CREATE TABLE transactions (
    id integer NOT NULL CONSTRAINT transactions_pk PRIMARY KEY,
    disc_id integer NOT NULL,
    transaction_date datetime NOT NULL,
    quantity integer NOT NULL,
    total_price decimal(10,2) NOT NULL,
    discs_id integer NOT NULL,
    CONSTRAINT transactions_discs FOREIGN KEY (discs_id)
    REFERENCES discs (id)
);

-- Заполнение таблицы дисков
INSERT INTO discs (id, manufacturer, price, stock_quantity) VALUES 
(1, 'Sony Music', 1200.50, 10),
(2, 'Universal Music', 950.00, 15),
(3, 'Warner Records', 1100.00, 5);

-- Заполнение таблицы произведений (связаны с discs_id)
INSERT INTO compositions (id, disc_id, title, author, performer, discs_id) VALUES 
(1, 1, 'Billie Jean', 'Michael Jackson', 'Michael Jackson', 1),
(2, 1, 'Beat It', 'Michael Jackson', 'Michael Jackson', 1),
(3, 2, 'Bohemian Rhapsody', 'Freddie Mercury', 'Queen', 2),
(4, 3, 'Interstellar Theme', 'Hans Zimmer', 'Hans Zimmer', 3);

-- Заполнение таблицы транзакций
INSERT INTO transactions (id, disc_id, transaction_date, quantity, total_price, discs_id) VALUES 
(1, 1, '2026-04-15 10:00:00', -2, 2401.00, 1), -- Продажа 2-х дисков
(2, 2, '2026-04-15 12:30:00', 5, 4750.00, 2);   -- Поступление 5-ти дисков


-- End of file.
