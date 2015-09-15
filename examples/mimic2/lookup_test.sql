select label, itemid, count(*) as ct
from d_chartitems natural join chartevents
where lower(label) like '%na%' or lower(label) like '%sodium%'
group by label, itemid
order by ct

select test_name, itemid, count(*) as ct
from d_labitems natural join labevents
where lower(test_name) like '%proth%' or lower(test_name) like '%pt%'
group by test_name, itemid
order by ct

select test_name, itemid, count(*) as ct
from d_labitems natural join labevents
where lower(test_name) like '%neut%' or lower(test_name) like '%lymp%' or lower(test_name) like '%mono%' or lower(test_name) like '%bas%'
group by test_name, itemid
order by ct


select test_name, itemid, count(*) as ct
from d_labitems natural join labevents
where lower(test_name) like '%hdl%'
group by test_name, itemid
order by ct



select test_name, itemid, count(*) as ct
from d_labitems natural join labevents
group by test_name, itemid
order by ct