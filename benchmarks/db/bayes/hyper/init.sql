create table data(
    data_id integer,
    label   integer
);

create table attr(
    data_id integer,
    attr_id integer,
    val     integer
);

create table class(
    label integer,
    total integer
);

create table train(
    label   integer,
    attr_id integer,
    val     integer,
    total   integer
);

create index pk_data on data(data_id);
create index pk_attr on attr(data_id,attr_id);

copy data from '/mnt/bayes_data1000.csv' delimiter ',';
copy attr from '/mnt/bayes_attr1000.csv' delimiter ',';
