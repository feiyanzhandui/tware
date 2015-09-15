create local temporary table dists as 
  select d.id as d_id,
         c.id as c_id,
         (c.x-d.x)*(c.x-d.x)+(c.y-d.y)*(c.y-d.y) as dist
  from data as d
  cross join cent as c
with data
on commit preserve rows;

create local temporary table newcents as
  select t1.c_id as id,
         avg(t3.x) as x,
         avg(t3.y) as y
  from dists as t1
  join (
    select d_id,
           min(dist) as mindist
    from dists
    group by d_id
  ) as t2
    on t2.d_id = t1.d_id and t2.mindist = t1.dist
  join data as t3
    on t3.id = t1.d_id
  group by t1.c_id
with data
on commit preserve rows;

delete from cent where id in (select id from newcents);
insert into cent select * from newcents;
drop table dists;
drop table newcents;