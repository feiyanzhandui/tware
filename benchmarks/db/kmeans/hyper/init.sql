create table data(
    id integer,
    x  float,
    y  float
);

create table cent(
    id integer,
    x  float,
    y  float
);

create index pk_data on data(id);
create index pk_cent on cent(id);

copy data from '/mnt/kmeans_data240.csv' delimiter ',';
copy cent from '/mnt/kmeans_cent32.csv' delimiter ',';
