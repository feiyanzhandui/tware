update cent
set x = t1.x,
    y = t1.y
from (
  select t2.c_id as c_id,
         avg(d1.x) as x,
         avg(d1.y) as y
  from data as d1
  join (
    select t3.d_id as d_id,
           t3.c_id as c_id
    from (
      select d2.id as d_id,
             c.id as c_id,
             (c.x-d2.x)*(c.x-d2.x)+(c.y-d2.y)*(c.y-d2.y) as dist
      from data as d2
      cross join cent as c
    ) as t3
    join (
      select d2.id as d_id,
             min((c.x-d2.x)*(c.x-d2.x)+(c.y-d2.y)*(c.y-d2.y)) as min_dist
      from data as d2
      cross join cent as c
      group by d2.id
    ) as t4
      on t3.d_id = t4.d_id and t3.dist = t4.min_dist
  ) as t2
    on d1.id = t2.d_id
  group by t2.c_id
) as t1
where id = t1.c_id
