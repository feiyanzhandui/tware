create table data(
    data_id integer,
    label   integer
);

create table model(
    attr_id integer,
    val     float
);

create table attr(
    data_id integer,
    attr_id integer,
    val     float
);

create index pk_data on data(data_id);
create index pk_model on model(attr_id);
create index pk_attr on attr(data_id,attr_id);

copy data from '/mnt/reg_data1000.csv' delimiter ',';
copy model from '/mnt/reg_model90.csv' delimiter ',';
copy attr from '/mnt/reg_attr1000.csv' delimiter ',';
