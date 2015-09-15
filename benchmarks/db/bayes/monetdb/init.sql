create table data(
    data_id integer,
    label   integer,
    primary key(data_id)
);

create table attr(
    data_id integer,
    attr_id integer,
    val     integer,
    primary key(data_id,attr_id),
    foreign key(data_id) references data(data_id)
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

copy into data from '/mnt/bayes_data1.csv' using delimiters ',';
copy into attr from '/mnt/bayes_attr1.csv' using delimiters ',';
