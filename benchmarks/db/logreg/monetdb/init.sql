create table data(
    data_id integer,
    label   integer,
    primary key(data_id)
);

create table attr(
    data_id integer,
    attr_id integer,
    val     float,
    primary key(data_id,attr_id),
    foreign key (data_id) references data(data_id)
);

create table model(
    attr_id float,
    val     double
);

copy into data from '/mnt/reg_data1000.csv' using delimiters ',';
copy into attr from '/mnt/reg_attr1000.csv' using delimiters ',';

insert into model
select distinct(attr_id), 0.5
from attr;