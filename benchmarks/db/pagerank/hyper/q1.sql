update page
set page_rank = t1.r
from (
  select p_in.id as p_id,
         0.15+0.85*sum(p_out.page_rank/p_out.out_count) as r
  from page as p_in
  join link as l
    on p_in.id = l.in_id
  join page as p_out
    on l.out_id = p_out.id
  group by p_in.id
  ) as t1
where id = t1.p_id
