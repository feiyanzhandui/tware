create local temporary table newmodel as
  select a1.attr_id,
         w1.val-sum(a1.val*t1.s)
  from attr as a1
  join model as w1
    on a1.attr_id = w1.attr_id
  join (
    select a2.data_id as d_id,
           (1/(1+exp(d1.label*sum(a2.val*w2.val)))-1)*d1.label as s
    from data as d1
    join attr as a2
      on d1.data_id = a2.data_id
    join model as w2
      on a2.attr_id = w2.attr_id
    group by d1.label,
             a2.data_id
  ) as t1
    on a1.data_id = t1.d_id
  group by a1.attr_id,
           w1.val
with data
on commit preserve rows;

delete from model;
insert into model select * from newmodel;
drop table newmodel;
