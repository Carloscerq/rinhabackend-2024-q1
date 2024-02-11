create table clientes (
	id SERIAL primary key,
	credito BIGINT not null,
	saldo BIGINT not null default 0
);

create table transacoes (
	id SERIAL primary key,
	quantidade BIGINT not null,
	operacao CHAR(1) not null,
	descricao varchar,
	ttime TIMESTAMP not null default current_timestamp,
	client_id INTEGER references clientes(id)
);
