create local temporary table newranks as
  select p_in.id as p_id,
         p_in.out_count as p_count,
         sum(p_out.page_rank/p_out.out_count) as r
  from page as p_in
  join link as l
    on p_in.id = l.in_id
  join page as p_out
    on l.out_id = p_out.id
  group by p_in.id,
           p_in.out_count
with data
on commit preserve rows;

drop table newranks;

delete from page;
insert into page select * from newranks;
