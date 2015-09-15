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

copy into data from '/home/ubuntu/data/kmeans_crime_db.csv' using delimiters ',';
copy into cent from '/home/ubuntu/data/kmeans_cent32.csv' using delimiters ',';
