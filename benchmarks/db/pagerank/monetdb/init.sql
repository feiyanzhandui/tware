create table page(
    id        integer,
    out_count integer,
    page_rank float
);

create table link(
    out_id integer,
    in_id  integer,
    primary key(out_id,in_id)
);

create table temp(
    id        integer,
    out_count integer,
    page_rank float
);

copy into page from '/mnt/data/pages.csv' using delimiters ',';
copy into link from '/mnt/data/links.csv' using delimiters ',';
