SET search_path = MIMIC2V26, pg_catalog;
COPY (
with icd9 as (
  select subject_id,
         string_agg(code, ' ') as icd9
  from icd9
  group by subject_id),
patient as (
  select subject_id,
         case gender
           when 'M' then 'Male'
           when 'F' then 'Female'
           else null
         end as sex,
         icustay_admit_age as age,
         height as height,
         weight_first as weight,
         (weight_first/(height*height))*703 as bmi,
         sapsi_first as sapsi,
         sofa_first as sofa,
         ROW_NUMBER() over(partition by subject_id order by icustay_id) as i
  from icustay_detail),
demo as (
  select subject_id,
         case ethnicity_descr
           when 'AMERICAN INDIAN/ALASKA NATIVE' then 'Other'
           when 'ASIAN' then 'Asian'
           when 'ASIAN - CHINESE' then 'Asian'
           when 'ASIAN - VIETNAMESE' then 'Asian'
           when 'BLACK/AFRICAN AMERICAN' then 'Black'
           when 'BLACK/CAPE VERDEAN' then 'Black'
           when 'BLACK/HAITIAN' then 'Black'
           when 'HISPANIC OR LATINO' then 'Hispanic'
           when 'HISPANIC/LATINO - PUERTO RICAN' then 'Hispanic'
           when 'MULTI RACE ETHNICITY' then 'Other'
           when 'NATIVE HAWAIIAN OR OTHER PACIFIC ISLAND' then 'Other'
           when 'OTHER' then 'Other'
           when 'PATIENT DECLINED TO ANSWER' then 'Unknown'
           when 'UNABLE TO OBTAIN' then 'Unknown'
           when 'UNKNOWN/NOT SPECIFIED' then 'Unknown'
           when 'WHITE' then 'White'
           when 'WHITE - RUSSIAN' then 'White'
           else null
         end as race,
         case marital_status_descr
           when 'DIVORCED' then 'Divorced'
           when 'MARRIED' then 'Married'
           when 'SEPARATED' then 'Separated'
           when 'SINGLE' then 'Single'
           when 'UNKNOWN (DEFAULT)' then 'Unknown'
           when 'WIDOWED' then 'Widowed'
           else null
         end as marital,
         case religion_descr
           when '7TH DAY ADVENTIST' then 0
           when 'BAPTIST' then 1
           when 'BUDDHIST' then 2
           when 'CATHOLIC' then 3
           when 'CHRISTIAN SCIENTIST' then 4
           when 'EPISCOPALIAN' then 5
           when 'GREEK ORTHODOX' then 6
           when 'HEBREW' then 7
           when 'HINDU' then 8
           when 'JEHOVAH''S WITNESS' then 9
           when 'JEWISH' then 10
           when 'LUTHERAN' then 11
           when 'METHODIST' then 12
           when 'MUSLIM' then 13
           when 'NOT SPECIFIED' then 14
           when 'OTHER' then 15
           when 'PROTESTANT QUAKER' then 16
           when 'ROMANIAN EAST. ORTH' then 17
           when 'UNITARIAN-UNIVERSALIST' then 18
           when 'UNOBTAINABLE' then 19
           else null
         end as religion,
         ROW_NUMBER() over(partition by subject_id order by hadm_id) as i
  from demographic_detail),
temp as (
  select subject_id,
         ROUND(value1num, 1) as temperature,
         ROW_NUMBER() over(partition by subject_id order by charttime) as i
  from chartevents
  where itemid = 678),
heart_rate as (
  select subject_id,
         ROUND(value1num) as heart_rate,
         ROW_NUMBER() over(partition by subject_id order by charttime) as i
  from chartevents
  where itemid = 211),
resp_rate as (
  select subject_id,
         ROUND(value1num) as resp_rate,
         ROW_NUMBER() over(partition by subject_id order by charttime) as i
  from chartevents
  where itemid = 618),
bp as (
  select subject_id,
         ROUND(value1num) as systolic_bp,
         ROUND(value2num) as diastolic_bp,
         ROW_NUMBER() over(partition by subject_id order by charttime) as i
  from chartevents
  where itemid = 51),
spo2 as (
  select subject_id,
         ROUND(value1num) as spo2,
         ROW_NUMBER() over(partition by subject_id order by charttime) as i
  from chartevents
  where itemid = 646),
gcs as (
  select subject_id,
         value1num as gcs,
         ROW_NUMBER() over(partition by subject_id order by charttime) as i
  from chartevents
  where itemid = 198),
braden as (
  select subject_id,
         value1num as braden,
         ROW_NUMBER() over(partition by subject_id order by charttime) as i
  from chartevents
  where itemid = 87),
sodium as (
  select subject_id,
         COALESCE(ROUND(valuenum,2),
           case
             when value like '%>%' then 180
             else null
         end) as sodium,
         ROW_NUMBER() over(partition by subject_id order by charttime) as i
  from labevents
  where itemid = 50159),
potassium as (
  select subject_id,
         COALESCE(ROUND(valuenum,2),
           case
             when value like '%GREATER%' then 10
             when value like '%>%' then 10
             when value like '%LESS%' then 1.5
             else null
           end) as potassium,
         ROW_NUMBER() over(partition by subject_id order by charttime) as i
  from labevents
  where itemid = 50149),
chloride as (
  select subject_id,
         COALESCE(ROUND(valuenum,2),
           case
             when value like '%GREATER%' then 140
             when value like '%>%' then 140
             else null
           end) as chloride,
         ROW_NUMBER() over(partition by subject_id order by charttime) as i
  from labevents
  where itemid = 50083),
magnesium as (
  select subject_id,
         COALESCE(ROUND(valuenum,2),
           case
             when value like '%LESS%' then 1
             else null
           end) as magnesium,
         ROW_NUMBER() over(partition by subject_id order by charttime) as i
  from labevents
  where itemid = 50140),
calcium as (
  select subject_id,
         COALESCE(ROUND(valuenum,2),
           case
             when value like '%LESS%' then 0
             else null
           end) as calcium,
         ROW_NUMBER() over(partition by subject_id order by charttime) as i
  from labevents
  where itemid = 50079),
anion_gap as (
  select subject_id,
         ROUND(valuenum,2) as anion_gap,
         ROW_NUMBER() over(partition by subject_id order by charttime) as i
  from labevents
  where itemid = 50068),
bun as (
  select subject_id,
         ROUND(valuenum,2) as bun,
         ROW_NUMBER() over(partition by subject_id order by charttime) as i
  from labevents
  where itemid = 50177),
creatinine as (
  select subject_id,
         COALESCE(ROUND(valuenum,2),
           case
             when value like '%LESS%' then 0
             when value like '%<%' then 0
             else null
           end) as creatinine,
         ROW_NUMBER() over(partition by subject_id order by charttime) as i
  from labevents
  where itemid = 50090),
glucose as (
  select subject_id,
         COALESCE(ROUND(valuenum,2),
           case
             when value like '%GREATER%' then 1300
             else null
           end) as glucose,
         ROW_NUMBER() over(partition by subject_id order by charttime) as i
  from labevents
  where itemid = 50112),
ph as (
  select subject_id,
         valuenum as ph,
         ROW_NUMBER() over(partition by subject_id order by charttime) as i
  from labevents
  where itemid = 50018),
be as (
  select subject_id,
         valuenum as be,
         ROW_NUMBER() over(partition by subject_id order by charttime) as i
  from labevents
  where itemid = 50002),
total_co2 as (
  select subject_id,
         COALESCE(valuenum,
           case
             when value like '%GREATER%' then 40
             when value like '%>%' then 40
             when value like '%LESS%' then 5
             when value like '%<%' then 5
             else null
           end) as total_co2,
         ROW_NUMBER() over(partition by subject_id order by charttime) as i
  from labevents
  where itemid = 50172),
total_o2 as (
  select subject_id,
         valuenum as total_o2,
         ROW_NUMBER() over(partition by subject_id order by charttime) as i
  from labevents
  where itemid = 50015),
pco2 as (
  select subject_id,
         valuenum as pco2,
         ROW_NUMBER() over(partition by subject_id order by charttime) as i
  from labevents
  where itemid = 50016),
po2 as (
  select subject_id,
         valuenum as po2,
         ROW_NUMBER() over(partition by subject_id order by charttime) as i
  from labevents
  where itemid = 50019),
wbc as (
  select subject_id,
         COALESCE(ROUND(valuenum,1),
           case
             when value like '%<%' then 0
             else null
           end) as wbc,
         ROW_NUMBER() over(partition by subject_id order by charttime) as i
  from labevents
  where itemid = 50468),
rbc as (
  select subject_id,
         ROUND(valuenum,2) as rbc,
         ROW_NUMBER() over(partition by subject_id order by charttime) as i
  from labevents
  where itemid = 50442),
hgb as (
  select subject_id,
         ROUND(valuenum,1) as hgb,
         ROW_NUMBER() over(partition by subject_id order by charttime) as i
  from labevents
  where itemid = 50386),
hct as (
  select subject_id,
         ROUND(valuenum,1) as hct,
         ROW_NUMBER() over(partition by subject_id order by charttime) as i
  from labevents
  where itemid = 50383),
mcv as (
  select subject_id,
         ROUND(valuenum,1) as mcv,
         ROW_NUMBER() over(partition by subject_id order by charttime) as i
  from labevents
  where itemid = 50413),
mch as (
  select subject_id,
         ROUND(valuenum,1) as mch,
         ROW_NUMBER() over(partition by subject_id order by charttime) as i
  from labevents
  where itemid = 50411),
mchc as (
  select subject_id,
         ROUND(valuenum,1) as mchc,
         ROW_NUMBER() over(partition by subject_id order by charttime) as i
  from labevents
  where itemid = 50412),
rdw as (
  select subject_id,
         ROUND(valuenum,1) as rdw,
         ROW_NUMBER() over(partition by subject_id order by charttime) as i
  from labevents
  where itemid = 50444),
plates as (
  select subject_id,
         COALESCE(ROUND(valuenum,1),
           case
             when value like '%<%' then 5
             else null
           end) as plates,
         ROW_NUMBER() over(partition by subject_id order by charttime) as i
  from labevents
  where itemid = 50428),
neuts as (
  select subject_id,
         ROUND(valuenum,1) as neuts,
         ROW_NUMBER() over(partition by subject_id order by charttime) as i
  from labevents
  where itemid = 50419),
lymphs as (
  select subject_id,
         ROUND(valuenum,1) as lymphs,
         ROW_NUMBER() over(partition by subject_id order by charttime) as i
  from labevents
  where itemid = 50408),
monos as (
  select subject_id,
         ROUND(valuenum,1) as monos,
         ROW_NUMBER() over(partition by subject_id order by charttime) as i
  from labevents
  where itemid = 50417),
basos as (
  select subject_id,
         ROUND(valuenum,1) as basos,
         ROW_NUMBER() over(partition by subject_id order by charttime) as i
  from labevents
  where itemid = 50333),
eos as (
  select subject_id,
         ROUND(valuenum,1) as eos,
         ROW_NUMBER() over(partition by subject_id order by charttime) as i
  from labevents
  where itemid = 50373),
pt as (
  select subject_id,
         COALESCE(ROUND(valuenum,1),
           case
             when value like '%GREATER%' then 125
             when value like '%HIGHEST%' then 125
             when value like '%>%' then 125
             else null
           end) as pt,
         ROW_NUMBER() over(partition by subject_id order by charttime) as i
  from labevents
  where itemid = 50439),
inr_pt as (
  select subject_id,
         COALESCE(ROUND(valuenum,1),
           case
             when value like '%GREATER%' then 44
             when value like '%>%' then 44
             else null
           end) as inr_pt,
         ROW_NUMBER() over(partition by subject_id order by charttime) as i
  from labevents
  where itemid = 50399),
ptt as (
  select subject_id,
         COALESCE(ROUND(valuenum,1),
         case
           when value like '%HIGHEST%' then 150
           when value like '%>%' then 150
           else null
         end) as ptt,
         ROW_NUMBER() over(partition by subject_id order by charttime) as i
  from labevents
  where itemid = 50440),
ckmb as (
  select subject_id,
         COALESCE(ROUND(valuenum,2),
           case
             when value like '%GREATER%' then 600
             else null
           end) as ckmb,
         ROW_NUMBER() over(partition by subject_id order by charttime) as i
  from labevents
  where itemid = 50087),
cpkmb as (
  select subject_id,
         ROUND(valuenum,2) as cpkmb,
         ROW_NUMBER() over(partition by subject_id order by charttime) as i
  from labevents
  where itemid = 50086),
ldh as (
  select subject_id,
         ROUND(valuenum,2) as ldh,
         ROW_NUMBER() over(partition by subject_id order by charttime) as i
  from labevents
  where itemid = 50134),
bnp as (
  select subject_id,
         COALESCE(ROUND(valuenum,2),
           case
             when value like '%GREATER%' then 35000
             when value like '%>%' then 35000
             else null
           end) as bnp,
         ROW_NUMBER() over(partition by subject_id order by charttime) as i
  from labevents
  where itemid = 50195),
tropi as (
  select subject_id,
         COALESCE(ROUND(valuenum,1),
           case
             when value like '%GREATER%' then 50
             when value like '%>%' then 50
             else null
           end) as tropi,
         ROW_NUMBER() over(partition by subject_id order by charttime) as i
  from labevents
  where itemid = 50188),
tropt as (
  select subject_id,
         COALESCE(ROUND(valuenum,2),
           case
             when value like '%GREATER%' then 25
             when value like '%>%' then 25
             else null
           end) as tropt,
         ROW_NUMBER() over(partition by subject_id order by charttime) as i
  from labevents
  where itemid = 50189),
total_bili as (
  select subject_id,
         ROUND(valuenum,1) as total_bili,
         ROW_NUMBER() over(partition by subject_id order by charttime) as i
  from labevents
  where itemid = 50170),
direct_bili as (
  select subject_id,
         COALESCE(ROUND(valuenum,1),
           case
             when value like '%LESS%' then 0
             when value like '%<%' then 0
             else null
           end) as direct_bili,
         ROW_NUMBER() over(partition by subject_id order by charttime) as i
  from labevents
  where itemid = 50096),
indirect_bili as (
  select subject_id,
         COALESCE(ROUND(valuenum,1),
           case
             when value like '%LESS%' then 0
             when value like '%<%' then 0
             else null
           end) as indirect_bili,
         ROW_NUMBER() over(partition by subject_id order by charttime) as i
  from labevents
  where itemid = 50127),
albumin as (
  select subject_id,
         COALESCE(ROUND(valuenum,2),
           case
             when value like '%LESS%' then 0
             when value like '%<%' then 0
             else null
           end) as albumin,
         ROW_NUMBER() over(partition by subject_id order by charttime) as i
  from labevents
  where itemid = 50060),
tg as (
  select subject_id,
         ROUND(valuenum,2) as tg,
         ROW_NUMBER() over(partition by subject_id order by charttime) as i
  from labevents
  where itemid = 50174)
select icd9,
       COALESCE(sex,'Unknown') as sex,
       COALESCE(age,-1) as age,--
       COALESCE(race,'Unknown') as race,
       COALESCE(marital,'Unknown') as marital,
       --COALESCE(religion,-1) as religion,
       COALESCE(height,-1) as height,
       COALESCE(weight,-1) as weight,
       COALESCE(bmi,-1) as bmi,
       COALESCE(temperature,98.6) as temperature,
       COALESCE(heart_rate,100) as heart_rate,
       COALESCE(resp_rate,20) as resp_rate,
       COALESCE(systolic_bp,120) as systolic_bp,
       COALESCE(diastolic_bp,80) as diastolic_bp,
       COALESCE(spo2,100) as spo2,
       COALESCE(sapsi,0) as sapsi,--
       COALESCE(sofa,0) as sofa,--
       COALESCE(gcs,15) as gcs,
       COALESCE(braden,23) as braden,
       COALESCE(sodium,141) as sodium,
       COALESCE(potassium,4.3) as potassium,
       COALESCE(chloride,102.5) as chloride,
       COALESCE(magnesium,1.9) as magnesium,
       COALESCE(calcium,9.45) as calcium,
       COALESCE(anion_gap,6) as anion_gap,
       COALESCE(bun,14) as bun,
       COALESCE(creatinine,
         case sex
           when 'Male' then 1
           when 'Female' then 0.85
           else -1
         end) as creatinine,
       COALESCE(glucose,80) as glucose,
       COALESCE(ph,7.4) as ph,
       COALESCE(be,0) as be,
       COALESCE(total_co2,26.5) as total_co2,
       COALESCE(total_o2,90) as total_o2,--
       COALESCE(pco2,40) as pco2,
       COALESCE(po2,87.5) as po2,
       COALESCE(wbc,7.25) as wbc,--
       COALESCE(rbc,4.8) as rbc,--
       COALESCE(hgb,14) as hgb,--
       COALESCE(hct,42) as hct,--
       COALESCE(mcv,90) as mcv,--
       COALESCE(mch,30) as mch,--
       COALESCE(mchc,33) as mchc,--
       COALESCE(rdw,13) as rdw,
       COALESCE(plates,250) as plates,--
       COALESCE(neuts,60) as neuts,--
       COALESCE(lymphs,30.5) as lymphs,
       COALESCE(monos,6.5) as monos,
       COALESCE(basos,0.375) as basos,--
       COALESCE(eos,2) as eos,
       COALESCE(pt,13) as pt,
       COALESCE(inr_pt,1) as inr_pt,
       COALESCE(ptt,32) as ptt,
       COALESCE(ckmb,5) as ckmb,--
       COALESCE(cpkmb,80) as cpkmb,--
       COALESCE(ldh,0) as ldh,--
       COALESCE(bnp,400) as bnp,
       COALESCE(tropi,0) as tropi,
       COALESCE(tropt,0) as tropt,
       COALESCE(total_bili,0.75) as total_bili,
       COALESCE(direct_bili,0.2) as direct_bili,
       COALESCE(indirect_bili,0.55) as indirect_bili,
       COALESCE(albumin,4.5) as albumin,
       COALESCE(tg,102) as tg--
from icd9
join patient on patient.subject_id = icd9.subject_id
left outer join demo on demo.subject_id = icd9.subject_id
left outer join temp on temp.subject_id = icd9.subject_id
left outer join heart_rate on heart_rate.subject_id = icd9.subject_id
left outer join resp_rate on resp_rate.subject_id = icd9.subject_id
left outer join bp on bp.subject_id = icd9.subject_id
left outer join spo2 on spo2.subject_id = icd9.subject_id
left outer join gcs on gcs.subject_id = icd9.subject_id
left outer join braden on braden.subject_id = icd9.subject_id
left outer join sodium on sodium.subject_id = icd9.subject_id
left outer join potassium on potassium.subject_id = icd9.subject_id
left outer join chloride on chloride.subject_id = icd9.subject_id
left outer join magnesium on magnesium.subject_id = icd9.subject_id
left outer join calcium on calcium.subject_id = icd9.subject_id
left outer join anion_gap on anion_gap.subject_id = icd9.subject_id
left outer join bun on bun.subject_id = icd9.subject_id
left outer join creatinine on creatinine.subject_id = icd9.subject_id
left outer join glucose on glucose.subject_id = icd9.subject_id
left outer join ph on ph.subject_id = icd9.subject_id
left outer join be on be.subject_id = icd9.subject_id
left outer join total_co2 on total_co2.subject_id = icd9.subject_id
left outer join total_o2 on total_o2.subject_id = icd9.subject_id
left outer join pco2 on pco2.subject_id = icd9.subject_id
left outer join po2 on po2.subject_id = icd9.subject_id
left outer join wbc on wbc.subject_id = icd9.subject_id
left outer join rbc on rbc.subject_id = icd9.subject_id
left outer join hgb on hgb.subject_id = icd9.subject_id
left outer join hct on hct.subject_id = icd9.subject_id
left outer join mcv on mcv.subject_id = icd9.subject_id
left outer join mch on mch.subject_id = icd9.subject_id
left outer join mchc on mchc.subject_id = icd9.subject_id
left outer join rdw on rdw.subject_id = icd9.subject_id
left outer join plates on plates.subject_id = icd9.subject_id
left outer join neuts on neuts.subject_id = icd9.subject_id
left outer join lymphs on lymphs.subject_id = icd9.subject_id
left outer join monos on monos.subject_id = icd9.subject_id
left outer join basos on basos.subject_id = icd9.subject_id
left outer join eos on eos.subject_id = icd9.subject_id
left outer join pt on pt.subject_id = icd9.subject_id
left outer join inr_pt on inr_pt.subject_id = icd9.subject_id
left outer join ptt on ptt.subject_id = icd9.subject_id
left outer join ckmb on ckmb.subject_id = icd9.subject_id
left outer join cpkmb on cpkmb.subject_id = icd9.subject_id
left outer join ldh on ldh.subject_id = icd9.subject_id
left outer join bnp on bnp.subject_id = icd9.subject_id
left outer join tropi on tropi.subject_id = icd9.subject_id
left outer join tropt on tropt.subject_id = icd9.subject_id
left outer join total_bili on total_bili.subject_id = icd9.subject_id
left outer join direct_bili on direct_bili.subject_id = icd9.subject_id
left outer join indirect_bili on indirect_bili.subject_id = icd9.subject_id
left outer join albumin on albumin.subject_id = icd9.subject_id
left outer join tg on tg.subject_id = icd9.subject_id
where (patient.i = 1) and
      (temp.i = 1 or temp.i is null) and
      (heart_rate.i = 1 or heart_rate.i is null) and
      (resp_rate.i = 1 or resp_rate.i is null) and
      (bp.i = 1 or bp.i is null) and
      (spo2.i = 1 or spo2.i is null) and
      (gcs.i = 1 or gcs.i is null) and
      (braden.i = 1 or braden.i is null) and
      (sodium.i = 1 or sodium.i is null) and
      (potassium.i = 1 or potassium.i is null) and
      (chloride.i = 1 or chloride.i is null) and
      (magnesium.i = 1 or magnesium.i is null) and
      (calcium.i = 1 or calcium.i is null) and
      (anion_gap.i = 1 or anion_gap.i is null) and
      (bun.i = 1 or bun.i is null) and
      (creatinine.i = 1 or creatinine.i is null) and
      (glucose.i = 1 or glucose.i is null) and
      (ph.i = 1 or ph.i is null) and
      (be.i = 1 or be.i is null) and
      (total_co2.i = 1 or total_co2.i is null) and
      (total_o2.i = 1 or total_o2.i is null) and
      (pco2.i = 1 or pco2.i is null) and
      (po2.i = 1 or po2.i is null) and
      (wbc.i = 1 or wbc.i is null) and
      (rbc.i = 1 or rbc.i is null) and
      (hgb.i = 1 or hgb.i is null) and
      (hct.i = 1 or hct.i is null) and
      (mcv.i = 1 or mcv.i is null) and
      (mch.i = 1 or mch.i is null) and
      (mchc.i = 1 or mchc.i is null) and
      (rdw.i = 1 or rdw.i is null) and
      (plates.i = 1 or plates.i is null) and
      (neuts.i = 1 or neuts.i is null) and
      (lymphs.i = 1 or lymphs.i is null) and
      (monos.i = 1 or monos.i is null) and
      (basos.i = 1 or basos.i is null) and
      (eos.i = 1 or eos.i is null) and
      (pt.i = 1 or pt.i is null) and
      (inr_pt.i = 1 or inr_pt.i is null) and
      (ptt.i = 1 or ptt.i is null) and
      (ckmb.i = 1 or ckmb.i is null) and
      (cpkmb.i = 1 or cpkmb.i is null) and
      (ldh.i = 1 or ldh.i is null) and
      (bnp.i = 1 or bnp.i is null) and
      (tropi.i = 1 or tropi.i is null) and
      (tropt.i = 1 or tropt.i is null) and
      (total_bili.i = 1 or total_bili.i is null) and
      (direct_bili.i = 1 or direct_bili.i is null) and
      (indirect_bili.i = 1 or indirect_bili.i is null) and
      (albumin.i = 1 or albumin.i is null) and
      (tg.i = 1 or tg.i is null)
) TO '/tmp/mimic2_out.csv' DELIMITER ',' CSV HEADER;
