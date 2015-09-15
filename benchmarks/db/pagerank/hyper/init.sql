create table page(
    id        integer,
    out_count integer,
    page_rank float
);

create table link(
    out_id integer,
    in_id  integer
);

create index pk_page on page(id);
create index pk_link on link(out_id,in_id);

copy page from '/mnt/pages.csv' delimiter ',';
copy link from '/mnt/links.csv' delimiter ',';
