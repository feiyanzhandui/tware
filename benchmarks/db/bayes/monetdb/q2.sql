insert into train
  select d.label,
         a.attr_id,
         a.val,
         count(a.val)
  from data as d
  join attr as a
    on d.data_id = a.data_id
  group by d.label,
           a.attr_id,
           a.val;
