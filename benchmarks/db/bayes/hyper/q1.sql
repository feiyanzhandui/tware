insert into class
  select label,
         count(label)
  from data
  group by label
