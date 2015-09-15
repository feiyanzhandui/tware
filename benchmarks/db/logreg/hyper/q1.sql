update model
set val = val-t1.g
from (
  select a1.attr_id as a_id,
         sum(a1.val*t2.s) as g
  from attr as a1
  join (
    select a2.data_id as d_id,
           (1/(1+exp(d.label*sum(a2.val*w.val)))-1)*d.label as s
    from data as d
    join attr as a2
      on d.data_id = a2.data_id
    join model as w
      on a2.attr_id = w.attr_id
    group by d.label,
             a2.data_id
  ) as t2
    on a1.data_id = t2.d_id
  group by a1.attr_id
) as t1
where attr_id = t1.a_id
